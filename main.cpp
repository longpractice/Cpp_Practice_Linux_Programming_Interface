/*
 * main.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: YNG
 */

#include "common.h"
#include "Tee.h"
#include "large_file.h"
#include "write_with_append_5_2.h"

int main(int argc, char *argv[])
{
	try
	{
		write_with_append_5_2();
	}
	catch(std::runtime_error& e)
	{
		std::cerr << std::flush << e.what() << std::endl << std::flush;
		return(EXIT_FAILURE);
	}
}





