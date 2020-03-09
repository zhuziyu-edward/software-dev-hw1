#include "pch.h"
#include "CppUnitTest.h"
#include "intersect.h"
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <numeric>
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

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

		TEST_METHOD(TestMethod2)
		{
			int totalPoints = 1000;
			const int kBound = 300;
			ofstream  file("C:\\Users\\zhang\\Documents\\code\\software-dev-hw1\\test\\test2.txt");
			file << totalPoints << endl;
			srand(time(NULL));
			random_device dev;
			mt19937 rng(dev());
			uniform_int_distribution<mt19937::result_type> dist(0, kBound * 2);
			while (totalPoints--) {
				int x1 = dist(rng) - kBound, y1 = dist(rng) - kBound;
				int x2 = dist(rng) - kBound, y2 = dist(rng) - kBound;
				file << 'L' << ' '<< x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
				file << 'L' << ' ' << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
			}
			file.close();
			FileReader input("C:\\Users\\zhang\\Documents\\code\\software-dev-hw1\\test\\test2.txt");
			Intersect solution;
			time_t t1 = time(0);
			solution.getIntersect(input);
			time_t t2 = time(0);
			if ((t2 - t1) > 60)
				Assert::AreEqual(0, 1);
			cout << t2 - t1 << endl;
		}

		TEST_METHOD(TestMethod3)
		{
			FileReader input("C:\\Users\\zhang\\Documents\\code\\software-dev-hw1\\test\\test3.txt");
			Intersect solution;
			Assert::AreEqual(solution.getIntersect(input), 0);
		}
	};
}
