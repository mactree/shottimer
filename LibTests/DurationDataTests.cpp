#include "pch.h"
#include "CppUnitTest.h"
#include "..\\shottimer-basic\DurationData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(DurationDataTests)
	{
	public:
		
		TEST_METHOD(input_0s_is_0_with_no_leading_zeros_and_no_point)
		{
			DurationData d(0);
			Assert::AreEqual(0x7f, (int)d.d0);
			Assert::AreEqual(0x7f, (int)d.d1);
			Assert::AreEqual(0x7f, (int)d.d2);
			Assert::AreEqual(0, (int)d.d3);
			Assert::IsFalse(d.hasPoint);
		}

		TEST_METHOD(input_9s_is_9_with_no_leading_zeros_and_no_point)
		{
			DurationData d(9);
			Assert::AreEqual(0x7f, (int)d.d0);
			Assert::AreEqual(0x7f, (int)d.d1);
			Assert::AreEqual(0x7f, (int)d.d2);
			Assert::AreEqual(9, (int)d.d3);
			Assert::IsFalse(d.hasPoint);
		}

		TEST_METHOD(input_59s_is_5_9_with_no_leading_zeros_and_no_point)
		{
			DurationData d(59);
			Assert::AreEqual(0x7f, (int)d.d0);
			Assert::AreEqual(0x7f, (int)d.d1);
			Assert::AreEqual(5, (int)d.d2);
			Assert::AreEqual(9, (int)d.d3);
			Assert::IsFalse(d.hasPoint);
		}

		TEST_METHOD(input_60s_is_1_0_0_with_no_leading_zeros_and_point)
		{
			DurationData d(60);
			Assert::AreEqual(0x7f, (int)d.d0);
			Assert::AreEqual(1, (int)d.d1);
			Assert::AreEqual(0, (int)d.d2);
			Assert::AreEqual(0, (int)d.d3);
			Assert::IsTrue(d.hasPoint);
		}

		TEST_METHOD(input_59m_is_5_9_0_0_with_point)
		{
			DurationData d(3540);
			Assert::AreEqual(5, (int)d.d0);
			Assert::AreEqual(9, (int)d.d1);
			Assert::AreEqual(0, (int)d.d2);
			Assert::AreEqual(0, (int)d.d3);
			Assert::IsTrue(d.hasPoint);
		}

		TEST_METHOD(input_60m_is__1_0_0_with_no_leading_zero_and_point)
		{
			DurationData d(3600);
			Assert::AreEqual(0x7f, (int)d.d0);
			Assert::AreEqual(1, (int)d.d1);
			Assert::AreEqual(0, (int)d.d2);
			Assert::AreEqual(0, (int)d.d3);
			Assert::IsTrue(d.hasPoint);
		}

		TEST_METHOD(input_60m1s_is__1_0_0_with_no_leading_zero_and_point)
		{
			DurationData d(3601);
			Assert::AreEqual(0x7f, (int)d.d0, L"d0");
			Assert::AreEqual(1, (int)d.d1, L"d1");
			Assert::AreEqual(0, (int)d.d2, L"d2");
			Assert::AreEqual(0, (int)d.d3, L"d3");
			Assert::IsTrue(d.hasPoint);
		}

		TEST_METHOD(input_61m_is__1_1_0_with_no_leading_zero_and_point)
		{
			DurationData d(3660);
			Assert::AreEqual(0x7f, (int)d.d0, L"d0");
			Assert::AreEqual(1, (int)d.d1, L"d1");
			Assert::AreEqual(0, (int)d.d2, L"d2");
			Assert::AreEqual(1, (int)d.d3, L"d3");
			Assert::IsTrue(d.hasPoint);
		}

		TEST_METHOD(input_10h_is_1_0_0_0_with_no_leading_zero_and_point)
		{
			DurationData d(36000);
			Assert::AreEqual(1, (int)d.d0);
			Assert::AreEqual(0, (int)d.d1);
			Assert::AreEqual(0, (int)d.d2);
			Assert::AreEqual(0, (int)d.d3);
			Assert::IsTrue(d.hasPoint);
		}

		TEST_METHOD(input_99h_is_9_9_0_0_with_no_leading_zero_and_point)
		{
			DurationData d(356400);
			Assert::AreEqual(9, (int)d.d0);
			Assert::AreEqual(9, (int)d.d1);
			Assert::AreEqual(0, (int)d.d2);
			Assert::AreEqual(0, (int)d.d3);
			Assert::IsTrue(d.hasPoint);
		}

		TEST_METHOD(input_100h_is_9_9_9_9_with_no_leading_zero_and_point)
		{
			DurationData d(360000);
			Assert::AreEqual(9, (int)d.d0);
			Assert::AreEqual(9, (int)d.d1);
			Assert::AreEqual(9, (int)d.d2);
			Assert::AreEqual(9, (int)d.d3);
			Assert::IsTrue(d.hasPoint);
		}
	};
}
