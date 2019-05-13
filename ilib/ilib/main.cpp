#include "idecl.h"
#include <iostream>

using namespace ilib;

int main()
{
	std::cout << idecltype(int **ptr) << std::endl;
	std::cout << idecltype(char *argv[]) << std::endl;
	std::cout << idecltype(void *cmp()) << std::endl;
	std::cout << idecltype(int(*func)()) << std::endl;
	std::cout << idecltype(int (*func)()[13]) << std::endl;
	std::cout << idecltype(char (*(*x[3])())[5]) << std::endl;




	system("pause");
	return 0;
}