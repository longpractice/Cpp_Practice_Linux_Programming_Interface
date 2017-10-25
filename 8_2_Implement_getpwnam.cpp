/*
 * 8_2_Implement_getpwnam.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: YNG
 */


#include "common.h"
#include <pwd.h>


struct passwd * getpwnam_YNG(const char* name)
{
	// a statically allowcated area
	static struct passwd *found_pwd = NULL;
	struct passwd * tmp_pwd = NULL;

	int len = strlen(name);

	while( (tmp_pwd = getpwent()) != NULL)
	{
		if(strcmp(tmp_pwd->pw_name, name) == 0)
		{
			//we have a match here, copy it to found
			found_pwd = (passwd *)malloc(sizeof(passwd));
			memcpy((void*)found_pwd, (void*)tmp_pwd, sizeof(passwd));
			break;
		}

	}

	return found_pwd;
}


