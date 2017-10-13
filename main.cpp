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


//The result is that the first group with write both word to the file

// The second group will write not only both writes but also what is in std::out
int main(int argc, char *argv[])
{
	try
	{
		/*First write*/
		char str[] = "write to fd = 1";
		write(1, str, sizeof(str));
		int fd;
		fd = open("f_dup", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR );
		int fd_dup = dup_YNG(fd);
		write(fd, "first",5);
		write(fd_dup, "second", 6);
		close(fd);


		/*Second Write*/
		int fd2;
		fd2 = open("f_dup2", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
		int fd2_dup2 = dup2_YNG(fd2, 1);
		write(fd2, "2first",6);
		std::cout << "fd2_dup2 is " << fd2_dup2 << std::endl;
		write(fd2_dup2, "2second", 7);
		close(fd2);


	}
	catch(std::runtime_error& e)
	{
		std::cerr << std::flush << e.what() << std::endl << std::flush;
		return(EXIT_FAILURE);
	}
}





