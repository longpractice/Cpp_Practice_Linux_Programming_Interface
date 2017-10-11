//============================================================================
// Name        : Tee.cpp
// Author      : YNG
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include "error_functions.h"

using namespace std;


//the tee will output to stdout and the file,
// as an argument as well as offering an option of -a file to append to file
int Tee(int argc, char *argv[])
{
	int opt = 0;
	std::string file_path;
	std::string usage = "Put in the file name as the argument in order to create a new or replace the old file.\r\n"
			"Use -a plus file name for appending to the file.";
	auto if_append = false;
	while((opt = getopt(argc, argv, ":a:")) != -1)
	{
		if(opt == '?' || opt == ':') // we have an unrecognizable argument
			usageErr(usage.c_str());
		file_path = optarg;
		if_append = true;
	}

	//if it is not appending, then we should simply create one
	if(!if_append)
	{
		if(argc > optind) //we indeed have an extra file path there for std::out
			file_path = argv[optind];
		else
			usageErr(usage.c_str());
	}

	//open a file
	int fd; //the file descriptor
	if(!if_append) // we will have to create or clean the file content
		fd = open(file_path.c_str(), O_RDWR|O_CREAT|O_TRUNC, S_IRUSR| S_IWUSR );
	else
		fd = open(file_path.c_str(), O_RDWR|O_APPEND, S_IRUSR|S_IWUSR); //we will append to end of the file

	if(fd < 0)
	{
		perror("open() failed.");
		return 1;
	}
	char ch;
	while(std::cin >> ch)
	{
		ssize_t written = write(fd, (void*)&ch, 1);
		if(written != 1)
		{
			perror("Write() failed.");
			std::cout << "Written: " << written << std::endl;
			return 1;
		}

		std::cout << ch;
	}
	std::cout << std::endl;

	int close_res = close(fd);
	if(close_res != 0)
	{
		perror("close() failed.");
		return 1;
	}

	return 0;
}
