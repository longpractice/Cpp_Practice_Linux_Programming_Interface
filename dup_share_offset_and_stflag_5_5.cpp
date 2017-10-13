/*
 * dup_share_offset_and_stflag_5_5.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: YNG
 */

#include "common.h"
int verify_fd_dup()
{
	int fd;
	fd = open("somefile", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);

	write(fd, "first", 5);


	int fd2 = dup(fd);
	write(fd, "second", 6);

	bool ok = lseek(fd,0, SEEK_CUR) == 11 && lseek(fd, 0, SEEK_CUR) == 11;
	if(!ok)
		throw std::runtime_error("They do not share same offset");

	int flag = fcntl(fd, F_GETFL);
	int flag2 = fcntl(fd2, F_GETFL);

	if(flag != flag2)
		throw std::runtime_error("They do not share same status flag");




}



