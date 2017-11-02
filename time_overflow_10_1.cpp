/*
 * time_overflow_10_1.cpp
 *
 *  Created on: Nov 2, 2017
 *      Author: YNG
 */

#include "common.h"
#include <limits.h>
void time_overflow_10_1()
{
	double ticks_per_second = sysconf(_SC_CLK_TCK);
	double max_int = INT_MAX;
	std::cout << "Ticks per second from _SC_CLK_TCK is: " << ticks_per_second << std::endl;
	std::cout << "Seconds it takes to overflow times() is " << max_int/ticks_per_second << std::endl;


	std::cout << "Clocks per second from CLOCKS_PER_SEC is: " << CLOCKS_PER_SEC << std::endl;
	std::cout << "Seconds it takes to overflow clocks() is " << max_int/CLOCKS_PER_SEC << std::endl;

	std::cout << "This is assuming that the clock type is int32, but indeed in this machine, it is int64" << std::endl;
}


