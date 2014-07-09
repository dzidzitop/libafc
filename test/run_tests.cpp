#include <cpptest.h>
#include "NumberTest.h"
#include "RepositoryTest.h"
#include "TokeniserTest.h"
#include "UTF16LEToStringTest.h"
#include "StringUtilTest.hpp"
#include "cpu/Int32Test.h"

using namespace std;
using namespace afc;
using namespace Test;

int main() {
	TextOutput out(TextOutput::Verbose);
	Suite allTests;
	allTests.add(auto_ptr<Suite>(new NumberTest));
	allTests.add(auto_ptr<Suite>(new RepositoryTest));
	allTests.add(auto_ptr<Suite>(new StringUtilTest));
	allTests.add(auto_ptr<Suite>(new TokeniserTest));
	allTests.add(auto_ptr<Suite>(new UTF16LEToStringTest));

	allTests.add(auto_ptr<Suite>(new Int32Test));
	allTests.run(out);
	return 0;
}
