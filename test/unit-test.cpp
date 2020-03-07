#include "pch.h"
#include "CppUnitTest.h"
#include "intersect.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	TEST_CLASS(unittest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			FileReader input("C:\\Users\\zhang\\Documents\\code\\software-dev-hw1\\test\\test1.txt");
			Intersect solution;
			Assert::AreEqual(solution.getIntersect(input), 7);
		}
	};
}
