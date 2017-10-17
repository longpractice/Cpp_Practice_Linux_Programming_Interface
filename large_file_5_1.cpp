/*
 * large_file.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: YNG
 */

#include "common.h"

//we will test this using command x 10111222333
#define _FILE_OFFSET_BITS 64

int large_file(int argc, char *argv[])
{
	int fd;
	off_t off;

	if(argc != 3 || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")
		throw EUsage("First the file and then the size. ");

	fd = open(argv[1], O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);

	if(fd == -1)
		throw EWithNumber("Could not open file using open(): %s", argv[1]);

	off = atoll(argv[2]);

	if(lseek(fd, off, SEEK_SET) == -1)
		throw EWithNumber("Could not seek to location %ll.", off);

	if(write(fd, "test", 4) == -1)
		throw EWithNumber("Could not write to file.");

	return 0;
}



