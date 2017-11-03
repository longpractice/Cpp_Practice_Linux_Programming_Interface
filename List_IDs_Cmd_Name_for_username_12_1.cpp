/*
 * List_IDs_Cmd_Name_for_username.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: YNG
 */


#include "common.h"

uid_t find_userIdByName(std::string username)
{
	struct passwd *pwd;
	uid_t j;
	char *endptr;

	if(username.size() == 0)
		return -1;

	pwd = getpwnam(username.c_str());

	if(pwd = nullptr)
		return -1;

	return pwd->pw_uid;
}

std::string Find_Matching_ID_And_Cmd_Name(std::string proc_dir, uid_t id)
{
	//we should open the status inside
	std::string file_full_dir = proc_dir + "/status";
	int fd;
	std::string file_content;

	fd = open(file_full_dir.c_str(), O_RDONLY);

	char ch = 0;
	int readCnt = 0;
	while(readCnt = read(fd, (void*)&ch, 1))
	{
		if(readCnt == -1)
			throw EWithNumber("Could not read file using read(): %s", file_full_dir.c_str());
		file_content.push_back(ch);

	} //the reading has been

	file_content += "\n\n\n\n";

	std::cout << file_content << std::endl;

}

void List_Process_IDs_And_Cmd_Name_for_Username_12_1(std::string username)
{
	std::string dir_location = "/proc/";
	uid_t user_id = find_userIdByName(username);

    DIR *dp = nullptr;
	if(nullptr == (dp = opendir(dir_location.c_str())))
	{
		throw EWithNumber("Could not open the file %s", dir_location);
	}

	//the subdir with name of process ids
	dirent *dptr = nullptr;
	std::string wholeList;

	while(nullptr !=  (dptr = readdir(dp)))
	{
		std::string dir_name = dptr->d_name;
		//see if all dir name is number
		if(All(dir_name, [](char ch){return ch <= '9' && ch >= '0';}))
		{
			//we have found one dir with number as a
			std::string file_full_dir = dir_location + dir_name;
			wholeList += Find_Matching_ID_And_Cmd_Name(file_full_dir, user_id);
		}
	}

}

