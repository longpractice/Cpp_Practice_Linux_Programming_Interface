/*
 * 8_1_duplicated_printing_problem.cpp
 *
 *  Created on: Oct 25, 2017
 *      Author: YNG
 */



#include "common.h"
#include <pwd.h>

//according to practice 8.1, the following should give duplicated numbers.
// But it did not give duplicated numbers indeed.

//The MAN page says that getpwnam MAY point to a static area. So the author of the book probably refer to this effect
// and the static area may be overwritten by the second call of getpwnam.

// In my case, there is no duplication of the number, probably it is pointed to staic area.
void same_number_twice_8_1()
{
	printf("passwords: %ld %ld\n", (long)(getpwnam("games")->pw_uid),
			(long)(getpwnam("root")->pw_uid));
}
