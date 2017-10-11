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
#include <string>
#include <memory>
#include <errno.h>
#include <sstream>

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
//see https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template<typename ... Args>
std::string string_format( const char* format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format, args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format, args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


static inline std::string errorStrWithNo()
{
	std::stringstream ss;
	ss << "Error number: " << errno << ". " << strerror(errno);
	return ss.str();
}

class EUsage: public std::runtime_error
{
public:
	template<typename... Args>
	EUsage(const char* format, Args... args):std::runtime_error(string_format(format, args...))
	{}

	EUsage(std::string msg):std::runtime_error(msg){}
};

class EWithNumber:public std::runtime_error
{
public:
	template<typename... Args>
	EWithNumber(const char* format, Args... args):std::runtime_error(string_format(format, args...) + errorStrWithNo())
	{

	}

	EWithNumber(std::string msg):std::runtime_error(msg + errorStrWithNo())
	{

	}
};






#endif /* ERROR_FUNCTIONS_H_ */
