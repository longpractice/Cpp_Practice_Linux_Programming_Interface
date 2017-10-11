/*
 * main.cpp
 *
 *  Created on: Oct 11, 2017
 *      Author: YNG
 */

#include "Tee.h"
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


int main(int argc, char *argv[])
{
	try
	{
		return Tee(argc, argv);
	}
	catch(...)
	{
		return(EXIT_FAILURE);
	}
}





