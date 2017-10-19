/*
 * 7_1_malloc_and_sbrk.cpp
 *
 *  Created on: Oct 19, 2017
 *      Author: YNG
 */


#include "common.h"

static const int c_max_allocs = 1e6;
//test with argument 100, 8192, 1
// it appears that the brk() increases a large amount(0x21000) and then could be used by several malloc.
// the free only changes the brk after the last one is freed, meaning that the brk have to be the upper limit of all malloced blockes
int free_and_sbrk_7_1(int argc, char* argv[])
{

	std::string usage = "Specify the first two parameters specify the number and size"
			"of blocks for allocation.\r\n"
			"The third argument specifies the loop step unit to be used when freeing memory blocks. Default to be 1.\r\n"
			"The fourth and fifth arguments specify the range of blocks to free. When omitted, free all.";

	if(argc < 3 || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")
		throw EUsage(usage);

	int nBlocks = atoi(argv[1]);
	if(nBlocks > c_max_allocs)
		throw EUsage("You are not allowed to allocate more than %d blocks. You have allocated %d blocks.", c_max_allocs, nBlocks);
	int blockSize = atoi(argv[2]);

	int freeStep = argc > 3? atoi(argv[3]) : 1;
	int freeMin = argc > 4? atoi(argv[4]) : 1;
	int freeMax = argc > 5? atoi(argv[5]) : nBlocks;

	if(freeMin > freeMax || freeMax > nBlocks)
		throw EUsage("You have specified min bigger than max. Or max bigger than amount of total blocks.");

	printf("Initial program break:        %10p\n", sbrk(0));

	printf("Allocating %d * %d bytes.\n", nBlocks, blockSize);

	char *ptr[c_max_allocs];
	for(int j = 0; j < nBlocks; j++)
	{
		ptr[j] = (char*)malloc(blockSize);
		if(ptr[j] == nullptr)
			throw EWithNumber("malloc failed.");
		printf("Current program break:    %10p. Ptr is at    %10p.\n", sbrk(0), ptr[j]);
	}

	printf("All allocations done, current program break is:    %10p\n. Now freeing.\n", sbrk(0));

	for(int j = freeMin - 1; j < freeMax; j += freeStep)
	{
		free(ptr[j]);
		printf("Current program break:    %10p. Free Ptr is at    %10p.\n", sbrk(0), ptr[j]);
	}

	printf("After all freeing, the program break is:    %10p\n", sbrk(0));

	return 0;

}


