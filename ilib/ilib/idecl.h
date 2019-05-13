#pragma once

#include <string>


namespace ilib
{
	using std::string;

	/*
	*turn a declaration string into a string that explains it
	*currently don't support 
	*	func args
	*	keywords const, static ...
	*
	*examples of usage and result string
	*	idecltype(int **ptr)						pointer to pointer to int
	*	idecltype(char *argv[])					array of pointer to char
	*	idecltype(void *cmp())					function returning pointer to void
	*	idecltype(int(*func)())					pointer to function returning int
	*	idecltype(int (*func)()[13])			pointer to function returning array of int
	*	idecltype(char (*(*x[3])())[5])		array of pointer to function returning pointer to array of char
	*/
	const string _idecltype(string decl);

#define idecltype(expr) _idecltype(#expr)

}



