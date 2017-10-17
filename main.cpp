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

int Verify_bss_no_space_allowcated_6_1(int argc, char *argv[]);

//This program when compiled as debug is 50656kb which is smaller than the uninitialized segment.
// the code below will not affect this behavior since some will be in .bss.
int some[10000000] = {};

//the code below will make the program put some in .data making the exe much larger, 40050704 Bytes!
//even though we have initialized only one element, all will be in .data
int some[10000000] = {1};

int main(int argc, char *argv[])
{
	try
	{
		Verify_bss_no_space_allowcated_6_1(argc, argv);

	}
	catch(std::runtime_error& e)
	{
		std::cerr << std::flush << e.what() << std::endl << std::flush;
		return(EXIT_FAILURE);
	}
}





