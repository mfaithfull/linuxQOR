// Copyright Querysoft Limited 2008 - 2025
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "../src/configuration/configuration.h"
#include "../src/qor/test/test.h"

using namespace qor;
using namespace qor::test;

qor_pp_test_case(Check_true)
{
	qor_pp_test_check(true); //does not throw
}


qor_pp_test_case(Check_qor_pp_test_check_FailThrowsFailureWithContainedCheckExpression)
{
	try
	{
		qor_pp_test_check(1 == 0);
	}
	catch (const failure& f)
	{
		std::string msg = f.what();

		if (msg.find("1 == 0") <= 0)
		{
			throw;
		}
	}
}


struct MyType
{
	bool operator==(const MyType& other) const { return true; }
	bool operator!=(const MyType& other) const { return false; }
};


std::ostream& operator<<(std::ostream& out, const MyType&)
{
	return out << "xxx";
}


struct TWithoutStream
{
	bool operator==(const TWithoutStream& other) const { return true; }
	bool operator!=(const TWithoutStream& other) const { return false; }
};


qor_pp_test_case(Check_qor_pp_test_equalWithNoCoutStreamingSupportStillCompilesAndExecutes)
{
	qor_pp_test_check((detail::has_cout_support<std::ostream, MyType>::value));

	qor_pp_test_check((detail::has_cout_support<std::ostream, TWithoutStream>::value) == false);

	TWithoutStream t1;
	TWithoutStream t2;

	qor_pp_test_equal(t1, t2);  //should compile, should not fail

	MyType m1; //type supports operator <<
	MyType m2;

	qor_pp_test_equal(m1, m2);//should compile, should not fail
}


qor_pp_test_case(Check_qor_pp_test_equalReportsActualValueIfSupported)
{
	try
	{
		qor_pp_test_equal(1, 0);
	}
	catch (const failure& f)
	{
		std::string msg = f.what();
		size_t result = msg.find("expected: (int) 1 != actual: (int) 0");
		if (result <= 0)
		{
			throw;
		}
	}
}


qor_pp_test_case(Check_EqualEpsilon)
{
	double d = 0;
	d += 0.1;
	d += 0.1;
	d += 0.1;

	if (d == 0.3) //isn't equal because of rounding
	{
		qor_pp_test_fail("roundingerror");
	}
	qor_pp_test_equal(d, 0.3); //equal distinguishes rounding errors.

}


qor_pp_test_case(Check_qor_pp_test_unequal)
{
	qor_pp_test_unequal(1, 0);
}


qor_pp_test_case(Check_qor_pp_test_fail)
{
	try
	{
		qor_pp_test_fail("fails here");
	}
	catch (const failure& f)
	{
		if (std::string(f.what()).find("fails_here") > 0)
		{
			return; //all fine
		}
		throw; //missing error text
	}
	throw failure("qor_pp_test_fail does not raise exception", qor_pp_compiler_at);
}


void raise_exception()
{
	throw std::logic_error("throws here");
}


qor_pp_test_case(Check_qor_pp_test_assert_throw)
{
	qor_pp_test_assert_throw(raise_exception(), std::logic_error);
	qor_pp_test_assert_throw(raise_exception(), std::exception);
}


void no_throw() 
{
}


qor_pp_test_case(Check_qor_pp_test_assert_throw_CatchesNoThrowCase)
{
	try
	{
		qor_pp_test_assert_throw(no_throw(), std::exception);
	}
	catch (const failure&)
	{
		return;
	}
	qor_pp_test_fail("qor_pp_test_assert_throw failed");
}


struct SuiteExample
{
	double d;

	SuiteExample() : d(0.123)
	{
		//setup
	}

	~SuiteExample()
	{
		//teardown
	}

	bool Invariant() const
	{
		return true;
	}

	MyType MakeSut()
	{
		return MyType();
	}
};


qor_pp_test_suite_case(SuiteExample, Case1)
{
	equal(0.123, d, qor_pp_compiler_at);
}


qor_pp_test_suite_case(SuiteExample, Case2)
{
	check(Invariant(), qor_pp_compiler_at);
}


qor_pp_test_suite_case(SuiteExample, Case3)
{
	unequal("hello", "world", qor_pp_compiler_at);
}


qor_pp_test_suite_case(SuiteExample, Case4)
{
	MyType t = MakeSut();
	qor_pp_test_check(t == t);
}


qor_pp_test_suite_case(SuiteExample, Case5)
{
	qor_pp_test_check(true);
}


class MyClass
{
public:
	
	MyClass(const char*) 
	{
	}
	
	void doSomethng() 
	{
	}
	
	void doSomethingElse() 
	{
	}
};


struct FixtureTestSuite
{
	float someNum;
	std::string str;
	MyClass someObject;
	
	FixtureTestSuite() : someNum(2.0f) , str("Hello") , someObject("somename")
	{
		someObject.doSomethng();
	}

	~FixtureTestSuite()
	{
		someObject.doSomethingElse();
	}
};


qor_pp_test_suite_case(FixtureTestSuite, Test1)
{
	qor_pp_test_equal(someNum, 2.0f);
}


qor_pp_test_suite_case(FixtureTestSuite, Test3)
{
	qor_pp_test_equal(str, "Hello");
}


struct SpecificSuiteCtors
{
	int i;
	std::string s;

	SpecificSuiteCtors() : i(0), s("default value")
	{
	}
	
	SpecificSuiteCtors(int i_) : i(i_), s("default value")
	{
	}
	
	SpecificSuiteCtors(int i_, const std::string& s_) : i(i_), s(s_)
	{
	}
};


qor_pp_test_suite_case(SpecificSuiteCtors, CaseA) : SpecificSuiteCtors()
{
	qor_pp_test_equal(0, i);
	qor_pp_test_equal("default value", s);
}


qor_pp_test_suite_case(SpecificSuiteCtors, CaseB) : SpecificSuiteCtors(123)
{
	qor_pp_test_equal(123, i);
}


qor_pp_test_suite_case(SpecificSuiteCtors, CaseC) : SpecificSuiteCtors(123, "blah")
{
	qor_pp_test_equal("blah", s);
}
