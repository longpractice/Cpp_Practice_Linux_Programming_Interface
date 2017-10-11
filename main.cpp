/*
 * main.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: YNG
 */

#include "common.h"
#include "Tee.h"
#include "large_file.h"


int main(int argc, char *argv[])
{
	try
	{
		return large_file(argc, argv);
	}
	catch(std::runtime_error& e)
	{
		std::cerr << std::flush << e.what() << std::endl << std::flush;
		return(EXIT_FAILURE);
	}
}





