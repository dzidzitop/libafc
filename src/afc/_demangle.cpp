/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2013 Dźmitry Laŭčuk

libafc is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#include "_demangle.h"

#ifdef __GNUC__
#include <cxxabi.h>

char *afc::demangle(const char * const name) throw()
{
	return abi::__cxa_demangle(name, 0, 0, 0);
}

#ifdef AFC_USE_STACK_TRACE
/*
 * Derived from addr2line.c and associated binutils files, version 2.18.
 */
#include "utils.h"
namespace tmp { // 'basename' functions are declared in both bfd and lib c libraries. Importing only some macro definitions
	#include <demangle.h>
}
#include <bfd.h>
#include <cstdio>
#include <cstring>
#include <memory>

using std::string;
using std::sprintf;
using std::vector;
using std::strlen;
using std::shared_ptr;

namespace
{
	struct Status
	{
		bool success;
		const char *message;
	};

	typedef struct _libtrace_data {
		bfd_boolean unwind_inlines;  /* Unwind inlined functions. */
		bfd_boolean with_functions;  /* Show function names.  */
		bfd_boolean do_demangle;     /* Demangle names.  */
		bfd_boolean base_names;      /* Strip directory names.  */
		asymbol **syms;              /* Symbol table.  */

		bfd *abfd;
		asection *section;
	} libtrace_data;

	/* These variables are used to pass information between
	   translate_addresses and find_address_in_section.  */
	typedef struct _sym_info {
		bfd_vma pc;
		const char *fileName;
		const char *functionName;
		unsigned int line;
		bfd_boolean found;
		asymbol **syms;
	} sym_info;

	shared_ptr<string> clone(const char * const src)
	{
		if (src == nullptr) {
			return shared_ptr<string>(nullptr);
		}
		return shared_ptr<string>(new string(src));
	}

	static Status slurp_symtab (bfd * const, asymbol **&);
	static void find_address_in_section (bfd *, asection *, void *);

	/* Read in the symbol table. */
	static Status slurp_symtab(bfd * const abfd, asymbol **&syms)
	{
		long symCount;
		unsigned int size;

		if ((bfd_get_file_flags(abfd) & HAS_SYMS) == 0) {
			return {.success = false};
		}

		symCount = bfd_read_minisymbols(abfd, FALSE, reinterpret_cast<void **>(&syms), &size);
		if (symCount == 0) {
			symCount = bfd_read_minisymbols(abfd, TRUE /* dynamic */, reinterpret_cast<void **>(&syms), &size);
		}

		if (symCount < 0) {
			return {.success = false};
		}
		return {.success = true};
	}


	/* Look for an address in a section.  This is called via
	   bfd_map_over_sections.  */

	static void
	find_address_in_section(bfd *abfd, asection *section, void *data)
	{
		bfd_vma vma;
		bfd_size_type size;
		sym_info *psi = (sym_info*)data;

		if (psi->found)
			return;

		if ((bfd_get_section_flags(abfd, section) & SEC_ALLOC) == 0)
			return;

		vma = bfd_get_section_vma(abfd, section);
		if (psi->pc < vma)
			return;

		size = bfd_get_section_size(section);
		if (psi->pc >= vma + size)
			return;

		psi->found = bfd_find_nearest_line(abfd, section, psi->syms, psi->pc - vma,
				&psi->fileName, &psi->functionName, &psi->line);
	}

	Status libtrace_init(const string &fileName, bfd *&abfd, asymbol **&syms) throw()
	{
		bfd_init();

		abfd = bfd_openr(fileName.c_str(), "default");
		if (abfd == NULL) {
			return {.success = false, .message = "unable to open file"};
		}

		if (bfd_check_format(abfd, bfd_archive)) {
			return {.success = false, .message = "cannot get addresses from archive"};
		}

		if (!bfd_check_format_matches(abfd, bfd_object, 0)) {
			return {.success = false, .message = "cannot determine file type unambiguously"};
		}

		if (!slurp_symtab(abfd, syms).success) {
			return {.success = false};
		}

		return {.success = true};
	}

	// TODO handle errors
	Status libtrace_close(bfd *&abfd, asymbol **&syms)
	{
		if (syms != 0) {
			free(syms);
			syms = 0;
		}
		bfd_close(abfd);
		return {.success = true};
	}

	afc::AddrStatus libtrace_resolve(void *xaddr, bfd * const abfd, asymbol ** const syms)
	{
		#define ADDR_BUF_LEN ((CHAR_BIT/4)*(sizeof(void*))+1)
		char addr[ADDR_BUF_LEN+1] = {0};
		sym_info si = {0};

		sprintf(addr, "%p", xaddr);
		si.pc = bfd_scan_vma (addr, NULL, 16);
		si.syms = syms;

		si.found = FALSE;
		bfd_map_over_sections(abfd, find_address_in_section, &si);

		if (!si.found) {
			return {.success = false};
		}

		do {
			const char *name;
			char *alloc = NULL;

			name = si.functionName;
			if (name == NULL || *name == '\0')
				name = "??";
			else {
				alloc = bfd_demangle(abfd, name, DMGL_ANSI | DMGL_PARAMS);
				if (alloc != 0) {
					name = alloc;
				}
			}

			string functionName;
			if (alloc != NULL) {
				functionName = alloc;
				free(alloc);
			} else {
				if (name != 0) {
					functionName = name;
				}
			}

			return {.success = true, .fileName = clone(si.fileName), .functionName = functionName, .line = si.line};
			// TODO support inlines
			/*
			if (!m_libtrace_data.unwind_inlines)
				si.found = FALSE;
			else
				si.found = bfd_find_inliner_info(abfd,
							&si.filename,
							&si.functionname,
							&si.line);
			*/
		} while (si.found);

		return {.success = false};
	}
}

namespace afc
{
	bool backtraceSymbols(void ** const addresses, size_t size, vector<AddrStatus> &dest) throw()
	{
		// TODO support shared libraries
		const string fileName = getExecPath();

		bfd *abfd = 0;
		asymbol **syms = 0;

		// TODO init data only once per binary (or per thread)
		if (libtrace_init(fileName.c_str(), abfd, syms).success) {
			for (size_t i = 0; i < size; ++i) {
				dest.push_back(libtrace_resolve(addresses[i], abfd, syms));
			}

			// TODO add error handling here
			libtrace_close(abfd, syms);

			// TODO return result
			return true;
		}

		return false;
	}
}
#endif // AFC_USE_STACK_TRACE

#endif // __GNUC__
