/*
 * many_writes.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: YNG
 */


#include "common.h"
int many_writes_5_6()
{
	char file[] = "somefile";

	int fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	int fd2 = dup(fd1);
	int fd3 = open(file, O_RDWR);
	write(fd1, "Hello,", 6);
	//content "Hello,"
	write(fd2, "world", 6);
	//content "Hello,world"
	// appended since they have the same offset and open file description
	lseek(fd2, 0, SEEK_SET);
	write(fd1, "HELLO,", 6);
	//content "HELLO,world"
	//write from the start since fd2 changed the offset which is shared by fd1
	write(fd3, "Gidday", 6);
	//content "Giddayworld"
	//fd3 does not share the open file description, instead it holds a different
	// open file description but they all eventually point to the same inode

	int i = close(fd1);
	int j = close(fd2);
	// j == 0, so closing a duplicated fd has no problem
	int k = close(fd3);

	return 0;



}
