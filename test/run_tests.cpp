#include <cpptest.h>
#include "cpu/Int32Test.h"

using namespace std;
using namespace afc;
using namespace Test;

int main() {
	TextOutput out(TextOutput::Verbose);
	Suite allTests;
	allTests.add(auto_ptr<Suite>(new Int32Test));
	allTests.run(out);
	return 0;
}
