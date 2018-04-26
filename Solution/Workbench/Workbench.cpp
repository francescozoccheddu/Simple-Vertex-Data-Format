
#include <string>
#include <iostream>

class TestBase
{

public:


};

template<typename T>
class TestDerived : public TestBase
{

public:

	TestDerived (T t) : val (t) {}

	T val;

	T get () const
	{
		return val;
	}

};

int main ()
{

	TestBase * b = new TestDerived<float>{ 7356.7f };


	float a = static_cast<TestDerived<float>*>(b)->get();

	system ("pause");
	return 0;
}