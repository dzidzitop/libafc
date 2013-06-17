#ifndef AFC_ASSERT_H_
#define AFC_ASSERT_H_

#ifdef NDEBUG
	#define AFC_ASSERT(assertion, message) ((void) 0)
#else
	#define AFC_ASSERT(assertion, message) ((assertion) ? ((void) 0): afc::assertion(#assertion, #message))
#endif

namespace afc
{
#ifndef NDEBUG
	void assertion(const bool assertion, const char * const message = 0);
#else
	inline void assertion(const bool assertion, const char * const message = 0) {/*nothing to do*/}
#endif
}

#endif /*AFC_ASSERT_H_*/
