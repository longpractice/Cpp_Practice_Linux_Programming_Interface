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

int atomic_append_5_3(int argc, char* argv[]);
int dup_YNG(int);
int dup2_YNG(int, int);
int verify_fd_dup();
int many_writes_5_6();

//The verification here indicates that they do share status and offset
int main(int argc, char *argv[])
{
	try
	{
		many_writes_5_6();

	}
	catch(std::runtime_error& e)
	{
		std::cerr << std::flush << e.what() << std::endl << std::flush;
		return(EXIT_FAILURE);
	}
}





