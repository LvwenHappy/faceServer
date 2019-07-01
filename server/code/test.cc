#include <stdio.h>
#include <stddef.h>
#include "CLenet.h"

class A
{
private:
	static CLenet* m_ptrLenet;


public:
	void fun();

};

CLenet* A::m_ptrLenet = new CLenet();

CLenet* pLenet = new CLenet();

void A::fun()
{
	printf("lvwen\n");
	printf("%d\n", m_ptrLenet->testA);
	printf("%lf\n", m_ptrLenet->m_bias1[0]);
	printf("%lf\n", A::m_ptrLenet->m_conv1[0][0][0][0]);
	printf("%lf\n", m_ptrLenet->m_conv1[0][0][0][0]);
}

int main()
{
	printf("%lf\n", pLenet->m_bias1[0]);

	A* pA = new A();
	pA->fun();

	return 0;
}