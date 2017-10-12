/*
 * write_with_append_5_2.cpp
 *
 *  Created on: Oct 12, 2017
 *      Author: YNG
 */

#include "common.h"

int write_with_append_5_2()
{
	int fd = 0;

	std::string randFile = "RandomFile_for_OAPPEND";




	/*==========Write some chars in the file first ============*/
	fd = open(randFile.c_str(), O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1)
		throw EWithNumber("Could not open file using open(): %s", randFile.c_str());

	if(write(fd, "aaaa", 4) == -1) //write some dummy string there
		throw EWithNumber("Could not write to file: %s", randFile.c_str());

	if(close(fd) == -1)
		throw EWithNumber("Could not close file: %s", randFile.c_str());




	/*========Attempt to open use append flag, seek to end, write and close==========*/
	int fd_append = 0;

	//open the file first
	fd_append = open(randFile.c_str(), O_RDWR|O_CREAT|O_APPEND, S_IRUSR | S_IWUSR);
	if(fd_append == -1)
		throw EWithNumber("2nd phase Could not open file using open(): %s", randFile.c_str());

	if(lseek(fd_append, 0, SEEK_END) == -1) //write some dummy string there
		throw EWithNumber("2nd phase Could not seek end to file: %s", randFile.c_str());

	if(write(fd_append, "xx", 4) == -1) //write some dummy string there
		throw EWithNumber("2nd phase Could not write to file: %s", randFile.c_str());

	if(close(fd_append) == -1)
		throw EWithNumber("2nd phase Could not close file: %s", randFile.c_str());



	/*=============Verify the content of the file==================*/
	int fd_verify = 0;

	//open the file first
	fd_verify = open(randFile.c_str(), O_RDONLY);
	if(fd_verify == -1)
		throw EWithNumber("3rd phase Could not open file using open(): %s", randFile.c_str());

	std::string file_content;
	char ch = 0;
	int readCnt = 0;
	while(readCnt = read(fd_verify, (void*)&ch, 1))
	{
		if(readCnt == -1)
			throw EWithNumber("3rd phase Could not read file using read(): %s", randFile.c_str());
		file_content.push_back(ch);

	} //the reading has been

	std::cout << "The file content is: " << file_content << std::endl;

	if(close(fd_verify) == -1)
		throw EWithNumber("3nd phase Could not close file: %s", randFile.c_str());

	if(file_content == "aaaaxx")
		return 0;
	else
		throw std::runtime_error("The file content is not what is expected.");
}




