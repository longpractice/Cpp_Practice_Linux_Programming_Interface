/*
 * atomic_append_5_3.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: YNG
 */


#include "common.h"

// this function mainly verifies the atomic importance of write.
// I will omit some error checking here.

//Then write try with
/*
   atomic_append f1 10000000 & atomic_append f1 10000000
   and
   atomic append f1 10000000 x & atomic_append f1 10000000 x

   verfify the file size. Indeed if not using append flag, the file size of much smaller
   sizes from ls -l are 20000000 vs 10219408.

*/
int atomic_append_5_3(int argc, char* argv[])
{

	std::string usage = "Specify the first parameter as the file path."
			"The second as the file length."
			"If the third arg exist, no append is used.";

	if(argc < 3 || argc > 4)
		throw EUsage(usage);

	bool if_append = (argc == 3);

	std::string file_path = argv[1];
	int file_length = atoi(argv[2]);

	std::cout << "Append: " << if_append << " Length: " << file_length << std::endl;

	int fd = 0;

	if(if_append)
		fd = open(file_path.c_str(), O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	else
		fd = open(file_path.c_str(), O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);


	while(file_length --> 0)
	{
		write(fd, "a", 1);
		if(!if_append)
			lseek(fd, 0, SEEK_END);
	}

	close(fd);
}

