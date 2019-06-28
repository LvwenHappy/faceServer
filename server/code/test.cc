#include <stdio.h>
#include <stddef.h>

class A
{
private:
	const int b;
public:
	A() : b(10)
	{

	}
	void fun()
	{
		printf("b:\t%d\n", b);
	}

};

int main()
{
	A a;
	a.fun();

	return 0;
}