#ifdef AFC_USE_STACK_TRACE

#include "StackTrace.h"
#include "platform.h"

#ifdef __GNUC__
	#include <execinfo.h>

	// should not be inlined
	int afc::StackTrace::backtrace(void ** const addresses, const size_t maxCount, const unsigned topFramesToSkip) throw()
	{
		// TODO remove magic number. '2' indicates that afc::StackTrace::backtrace and ::backtrace calls are to be filtered out
		int actualFramesToSkip = topFramesToSkip + 2;
		int actualSize = ::backtrace(addresses, maxCount);
		if (actualFramesToSkip == 0) {
			return actualSize;
		}
		// TODO support topFramesToSkip to do not affect maxCount;
		// if there are frames to skip then need to replace them with the needed ones
		for (int i = 0, n = actualSize - actualFramesToSkip; i < n; ++i) {
			addresses[i] = addresses[i + actualFramesToSkip];
		}
		return actualSize - actualFramesToSkip;
	}
#elif defined AFC_WIN32
	#include <windows.h>
	// TODO use StackWalk64 from dbghlp.h
	#include <imagehlp.h>

	using std::size_t;

	#define LOAD_THREAD_CONTEXT(stackFrame)\
		asm("call label\n\t"\
			"label: pop %%eax\n\t"\
			"movl %%eax, %0\n\t"\
			"movl %%esp, %1\n\t"\
			"movl %%ebp, %2"\
			:"=r"(stackFrame.AddrPC.Offset),"=r"(stackFrame.AddrStack.Offset),"=r"(stackFrame.AddrFrame.Offset));

	namespace
	{
	#ifdef AFC_X86
		const DWORD CPU_TYPE = IMAGE_FILE_MACHINE_I386;
	#elif defined AFC_AMD64
		const DWORD CPU_TYPE = IMAGE_FILE_MACHINE_AMD64;
	#else
		#error "unsupported processor architecture"
	#endif
	}

	// should not be inlined
	int afc::StackTrace::backtrace(void ** const addresses, const size_t maxCount, const unsigned topFramesToSkip) throw()
	{
		STACKFRAME stackFrame = {0};
		stackFrame.AddrFrame.Mode = AddrModeFlat;
		stackFrame.AddrPC.Mode = AddrModeFlat;
		stackFrame.AddrStack.Mode = AddrModeFlat;
		/*
	     * AddrStack should always be set to the stack pointer value for all platforms. You can certainly publish that AddrStack
	     * should be set. You're also welcome to say that new releases of dbghelp are now requiring this.
	     * Given a current dbghelp, your code should:
	         1. Always use StackWalk64
	         2. Always set AddrPC to the current instruction pointer (Eip on x86, Rip on x64 and StIIP on IA64)
	         3. Always set AddrStack to the current stack pointer (Esp on x86, Rsp on x64 and IntSp on IA64)
	         4. Set AddrFrame to the current frame pointer when meaningful. On x86 this is Ebp, on x64 you can use Rbp (but is not
	         	used by VC2005B2; instead it uses Rdi!) and on IA64 you can use RsBSP. StackWalk64 will ignore the value when it
	         	isn't needed for unwinding.
	         5. Set AddrBStore to RsBSP for IA64.
	    */
		LOAD_THREAD_CONTEXT(stackFrame);

		const HANDLE processHandle = GetCurrentProcess();
		const HANDLE threadHandle = GetCurrentThread();

		size_t i = 0;
		// this loop skips the requested number of top frames + the frame related to the backtrace function itself.
		unsigned j = topFramesToSkip;
		do {
			// TODO for the sake of code re-use it could be worth no to pass in 0 as an address of the thread context. This does not work for IA64 and AMD64.
			if (!StackWalk(CPU_TYPE, processHandle, threadHandle, &stackFrame, 0, 0, SymFunctionTableAccess, SymGetModuleBase, 0)) {
				goto finalise;
			}
		} while (j-- > 0);

		for (; i < maxCount; ++i) {
			// TODO for the sake of code re-use it could be worth no to pass in 0 as an address of the thread context. This does not work for IA64 and AMD64.
			if (!StackWalk(CPU_TYPE, processHandle, threadHandle, &stackFrame, 0,
					0, SymFunctionTableAccess, SymGetModuleBase, 0)) {
				//cout << "Break!" << endl;
				break;
			}
			if (stackFrame.AddrPC.Offset == 0) {
				// the end of the call stack
				break;
			}
			addresses[i] = reinterpret_cast<void*>(stackFrame.AddrPC.Offset);
		}
finalise:
		CloseHandle(processHandle);
		CloseHandle(threadHandle);
		return i;
	}
#else
	#error "unsupported platform"
#endif

#endif
