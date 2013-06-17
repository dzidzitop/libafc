#ifndef AFC_PLATFORM_H_
#define AFC_PLATFORM_H_

#ifdef __linux__
	#define AFC_UNIX
	#define AFC_LINUX
	#define AFC_HAS_EXECINFO
	#define AFC_PTHREADS
#elif defined __unix__
	#define AFC_UNIX
#elif defined __WIN32__
	#define AFC_WIN32
	#ifdef __GNUC__
		#ifdef __CYGWIN__
			#define AFC_WIN32_CYGWIN
			#define AFC_PTHREADS
		#elif defined __MINGW32__
			#define AFC_WIN32_MINGW
		#endif
	#endif
#elif defined _WIN32
	#ifdef _WIN64
		#define AFC_WIN64_MSVC
	#else
		#define AFC_WIN32_MSVC
	#endif
#else
	#error "unknown target OS"
#endif

#if defined __i386__ || defined _M_IX86
	#define AFC_X86
	#define AFC_LE
#elif defined __x86_64__
	#define AFC_AMD64
	#define AFC_LE
#else
	#error "unknown target processor architecture"
#endif

#endif /*AFC_PLATFORM_H_*/
