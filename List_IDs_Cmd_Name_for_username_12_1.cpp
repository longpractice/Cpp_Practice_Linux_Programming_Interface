/*
 * List_IDs_Cmd_Name_for_username.cpp
 *
 *  Created on: Nov 3, 2017
 *      Author: YNG
 */


#include "common.h"
#include <vector>
uid_t find_userIdByName(std::string username)
{
	struct passwd *pwd;
	uid_t j;
	char *endptr;

	if(username.size() == 0)
		return -1;

	pwd = getpwnam(username.c_str());

	if(pwd == nullptr)
		return -1;

	return pwd->pw_uid;
}

//this function splits strings into lines
static inline std::vector<std::string> SplitToLines(std::string str, bool keep_empty_lines = true)
{
	std::vector<std::string> res;
	std::stringstream ss(str);
	std::string to;

	while(std::getline(ss,to,'\n'))
	{
		if(keep_empty_lines || !to.empty())
			res.emplace_back(to);
	}

	return res;
}

template<typename Out>
void split(const std::string &s, char delim, Out result,  bool keep_empty = true) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
		if(keep_empty || !item.empty())
        *(result++) = item;
    }
}

static inline std::vector<std::string> split(const std::string &s, char delim, bool keep_empty = true) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems), keep_empty);
    return elems;
}

std::string Find_Matching_ID_And_Cmd_Name(std::string proc_dir, uid_t id)
{
	//we should open the status inside
	std::string file_full_dir = proc_dir + "/status";
	int fd;
	std::string file_content;
	std::string res;
	fd = open(file_full_dir.c_str(), O_RDONLY);

	/*
	 * Read all the contents out to one string
	 */
	char ch = 0;
	int readCnt = 0;
	while(readCnt = read(fd, (void*)&ch, 1))
	{
		if(readCnt == -1)
			return res; //we could not read anything, probably process terminated
		file_content.push_back(ch);
	} //the reading has been

	if(close(fd) == -1)
		return res; //could not close probably the proc is terminated.



	/*
	 * Handle the contents
	 */
	auto lines = split(file_content, '\n', false); //split without keeping empty
	//find the correct line

	auto uid_lines = Where(lines,
			[&](std::string _line){
				return Contains(_line, "Uid");
			}
	);

	auto uid_line = uid_lines[0];

	//get the uid
	auto uid_fields = split(uid_line, '\t', false);

	auto uid = std::stoi(uid_fields[1]);
	if(uid != id) //does the id match?
		return res;


	auto name_lines = Where(lines,
			[&](std::string _line){
				return Contains(_line, "Name");
			}
	);

	res = name_lines[0];

	return res;

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
			int process_id = std::stoi(dir_name);
			std::string file_full_dir = dir_location + dir_name;
			std::string found_name = Find_Matching_ID_And_Cmd_Name(file_full_dir, user_id);
			if(!found_name.empty())
			{
				//it is found inside
				std::string line = "PID:\t" + dir_name + "\t" + found_name  + "\n";
				wholeList += line;
			}
		}
	}

	std::cout << wholeList << std::endl;

}

