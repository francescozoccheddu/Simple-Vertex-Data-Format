
#include <string>
#include <iostream>
#include "../Library/ListName.hpp"

class B;

class A
{

public:

	A (int a) : a (a) {}

	int a;

	virtual B operator+(const A& oth) const;

	virtual void print () const
	{
		std::cout << a << std::endl;
	}

};

class B : public A
{
public:
	int b;

	B (int b, int a) : A (a), b (b) {}

	virtual void print () const
	{
		std::cout << a << " " << b << std::endl;
	}

};

B A::operator+ (const A& oth) const
{
	return B (a + oth.a, 724);
}

int main ()
{
	SVDF::ListName name{ "Dioc" };
	SVDF::ListName name2{ "Dioc" };

	A a1 (1);
	A a2 (2);
	A b = a1 + a2;
	b.print ();

	std::cout << (name == name2) << std::endl;
	
	system ("pause");
	return 0;
}