#include <cpptest.h>
#include "CompileTimeMathTest.h"
#include "EncodeBase64Test.hpp"
#include "FastDivisionTest.h"
#include "MathUtilsTest.h"
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
	allTests.add(auto_ptr<Suite>(new CompileTimeMathTest));
	allTests.add(auto_ptr<Suite>(new EncodeBase64Test));
	allTests.add(auto_ptr<Suite>(new FastDivisionTest));
	allTests.add(auto_ptr<Suite>(new MathUtilsTest));
	allTests.add(auto_ptr<Suite>(new NumberTest));
	allTests.add(auto_ptr<Suite>(new RepositoryTest));
	allTests.add(auto_ptr<Suite>(new StringUtilTest));
	allTests.add(auto_ptr<Suite>(new TokeniserTest));
	allTests.add(auto_ptr<Suite>(new UTF16LEToStringTest));

	allTests.add(auto_ptr<Suite>(new Int32Test));
	allTests.run(out);
	return 0;
}
