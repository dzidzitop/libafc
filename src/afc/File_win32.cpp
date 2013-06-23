#include "platform.h"

#ifdef AFC_WIN32

#include "File.h"
#include <windows.h>

unsigned long long afc::File::size()
{
	HANDLE file = CreateFile(m_path.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (file == 0) {
		// TODO throw proper exception;
		return 0;
	}
	DWORD high;
	DWORD low = GetFileSize(file, &high);
	CloseHandle(file);
	return (static_cast<unsigned long long>(high)<<32) + low;
}

void afc::File::setSize(const unsigned long long size)
{
	HANDLE file = CreateFile(m_path.c_str(), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (file == 0) {
		// TODO throw proper exception;
		return;
	}
	LARGE_INTEGER shift;
	shift.QuadPart = size;
	if (!SetFilePointerEx(file, shift, 0, FILE_BEGIN)) {
		// TODO throw proper exception;
		return;
	}
	if (!SetEndOfFile(file)) {
		// TODO throw proper exception;
		return;
	}
	CloseHandle(file);
}

#endif