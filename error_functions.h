/*
 * error_functions.h
 *
 *  Created on: Oct 11, 2017
 *      Author: YNG
 */

#ifndef ERROR_FUNCTIONS_H_
#define ERROR_FUNCTIONS_H_

#include <stdio.h>
#include <cstdarg>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

//The C style function as indicated in the book
/*
static inline void usageErrC(const char* format, ...)
{
	va_list argList;
	fflush(stdout);

	fprintf(stderr, "Usage: ");
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);

	fflush(stderr);
	exit(EXIT_FAILURE);
}
*/

//the C++11 style
template<typename... Args>
static inline void usageErr(const char* format, Args... args)
{
	std::cerr << std::flush;
	printf(format, args...);
	std::cerr << std::endl;
	std::cerr << std::flush;
	throw std::runtime_error("cmd line args mistaken.");
}






#endif /* ERROR_FUNCTIONS_H_ */
