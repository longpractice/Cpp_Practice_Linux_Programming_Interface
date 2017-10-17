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
int setenv_YNG_6_3(const char *name, const char *value, int overwrite);
int unsetenv_YNG_6_3(const char *name);

void print_env()
{
	char **ep;
	for(ep = environ; *ep != NULL; ep++)
		printf(*ep);
	return;
}

int main(int argc, char *argv[])
{
	try
	{
		std::cout << "Old Envs" << std::endl;
		print_env();

		std::cout << "\n\n\n\nNew Envs" << std::endl;
		setenv_YNG_6_3("YNG", "TEST", 0);
		print_env();

		std::cout << "\n\n\nOverwrite YNG env" << std::endl;
		setenv_YNG_6_3("YNG", "TEST_OVERWRITE", 1);
		print_env();

		std::cout << "\n\n\nonOverwrite YNG env" << std::endl;
		setenv_YNG_6_3("YNG", "TEST_NONOVERWRITE", 0);
		print_env();

		std::cout << "\n\n\nUnset YNG env" << std::endl;
		unsetenv_YNG_6_3("YNG");
		print_env();

		return 0;

	}
	catch(std::runtime_error& e)
	{
		std::cerr << std::flush << e.what() << std::endl << std::flush;
		return(EXIT_FAILURE);
	}
}





