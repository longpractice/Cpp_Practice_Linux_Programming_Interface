/*
 * dup_dup2_with_fcntl_5_4.cpp
 *
 *  Created on: Oct 13, 2017
 *      Author: YNG
 */
#include "common.h"

int dup_YNG(int oldfd)
{
	int newfd = fcntl(oldfd, F_DUPFD, 0);
	return newfd;
}

int dup2_YNG(int oldfd, int newfd)
{
	if(oldfd == newfd) //in this case we have to make sure that the current fd is valid
	{
		//if the old is invalid
		if(fcntl(oldfd, F_GETFL) == -1)
		{
			errno = EBADF;
			return -1;
		}
		//otherwise do nothing
		return 0;
	}

	//we have to create a new one, first check if the new is already open
	if(fcntl(newfd, F_GETFL) != -1)
		close(newfd);

	return fcntl(oldfd, F_DUPFD, newfd);
}

