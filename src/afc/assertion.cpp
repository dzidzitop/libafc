#include "assertion.h"
#include "StackTrace.h"
#include <iostream>
#include <exception>

using std::cerr;
using std::endl;
using std::terminate;

#ifndef NDEBUG
	void afc::assertion(const bool assertion, const char * const message)
	{
		if (assertion) {
			return;
		}
		cerr << "Assertion failure";
		if (message != 0) {
			cerr << " (" << message << ')';
		}
		cerr << " at:\n";
		StackTrace(1).print(cerr);
		terminate();
	}
#endif
