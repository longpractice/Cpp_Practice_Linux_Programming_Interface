/*
 * setenv_YNG_6_3.cpp
 *
 *  Created on: Oct 17, 2017
 *      Author: YNG
 */


#include "common.h"

int setenv_YNG_6_3(const char *name, const char *value, int overwrite)
{
	char *old_env = getenv(name);

	//according to MAN, return a success on no overwrite and already existing condition
	if(old_env && !overwrite)
		return 0;

	int new_len = strlen(name) + strlen(value) + 1 + 1; //equal sign and terminating char
	char* newChar = (char*)malloc(new_len); //allowcate the new space
	strcpy(newChar, name); //copy the key
	newChar[strlen(name)] = '='; //copy the equals sign
	strcpy(newChar + strlen(name) + 1, value); //copy the value
	if(putenv(newChar) != 0)
		return -1;
	else
		return 0;


	//if it already exists, we have to overwrite it
}


int unsetenv_YNG_6_3(const char *name)
{
	char **ep;
	for(ep = environ; *ep != NULL;)
	{
		//see if we have a name that equals to this
		if(strncmp(*ep, name, strlen(name)) == 0 && (*ep)[strlen(name)] == '=' )
		{
			//shift to the next
			char **nextp;
			for(nextp = ep; *nextp != NULL; ++nextp)
				*nextp = *(nextp + 1);
		}
		else
			++ep;
	}
	return 0;
}
