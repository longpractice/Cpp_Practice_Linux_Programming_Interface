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
#include <pwd.h>
#include <vector>
int atomic_append_5_3(int argc, char* argv[]);
int dup_YNG(int);
int dup2_YNG(int, int);
int verify_fd_dup();
int many_writes_5_6();
int Verify_bss_no_space_allowcated_6_1(int argc, char *argv[]);
int setenv_YNG_6_3(const char *name, const char *value, int overwrite);
int unsetenv_YNG_6_3(const char *name);
int free_and_sbrk_7_1(int argc, char* argv[]);
int same_number_twice_8_1();
struct passwd * getpwnam_YNG(const char* name);
void time_overflow_10_1();
void List_Process_IDs_And_Cmd_Name_for_Username_12_1(std::string name);
void Get_Procs_Tree_12_2();
std::vector<int> Get_Pids_opening_file_12_3(std::string file_name);
void time_stats_13_1(int argc, char *argv[]);
void write_bytes_13_2(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	try
	{
		write_bytes_13_2(argc, argv);
	}
	catch(std::runtime_error& e)
	{
		std::cerr << std::flush << e.what() << std::endl << std::flush;
		return(EXIT_FAILURE);
	}
}





