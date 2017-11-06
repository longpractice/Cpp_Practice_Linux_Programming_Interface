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

std::string FileLocToString(std::string file_loc)
{
	//we should open the status inside
	std::string file_full_dir = file_loc;
	int fd;
	std::string file_content;
	fd = open(file_full_dir.c_str(), O_RDONLY);

	/*
	 * Read all the contents out to one string
	 */
	char ch = 0;
	int readCnt = 0;
	while(readCnt = read(fd, (void*)&ch, 1))
	{
		if(readCnt == -1)
			return ""; //we could not read anything, probably process terminated
		file_content.push_back(ch);
	} //the reading has been

	if(close(fd) == -1)
		return ""; //could not close probably the proc is terminated.

	return file_content;

}

std::string GetLineByStatusKey(std::string file_content, std::string key)
{
	/*
	 * Handle the contents
	 */
	auto lines = split(file_content, '\n', false); //split without keeping empty
	//find the correct line

	auto val_lines = Where(lines,
			[&](std::string _line){
				return Contains(_line, key);
			}
	);
	if(val_lines.empty())
		return "";
	return val_lines[0];
}

int Find_UId_From_Status(std::string status)
{
	//we should open the status inside
	auto parent_id_line = GetLineByStatusKey(status, "Uid");
	auto fields = split(parent_id_line, '\t', false);
	return std::stoi(fields[1]);
}

std::string Find_Cmd_Name_From_Status(std::string status)
{
	//we should open the status inside
	auto name_line = GetLineByStatusKey(status, "Name");
	return name_line.substr(6, name_line.size() - 6);
}

int Find_Parent_Id_From_Status(std::string status)
{
	//we should open the status inside
	auto parent_id_line = GetLineByStatusKey(status, "PPid");
	auto fields = split(parent_id_line, '\t', false);
	return std::stoi(fields[1]);
}

std::string Find_Matching_ID_And_Cmd_Name(std::string proc_dir, uid_t id)
{
	//we should open the status inside
	std::string file_content = FileLocToString(proc_dir);
	auto uid = Find_UId_From_Status(file_content);

	if(uid != id) //does the id match?
		return "";

	auto name_line = GetLineByStatusKey(file_content, "Name");

	return name_line;
}


struct TProcInfo
{
	int pid;
	int ppid;
	std::string cmd;
};

std::vector<TProcInfo> Get_All_Proc_Infos()
{
	std::string dir_location = "/proc/";
    DIR *dp = nullptr;
	if(nullptr == (dp = opendir(dir_location.c_str())))
	{
		throw EWithNumber("Could not open the file %s", dir_location);
	}

	//the subdir with name of process ids
	dirent *dptr = nullptr;
	std::vector<TProcInfo> res;

	while(nullptr !=  (dptr = readdir(dp)))
	{
		std::string dir_name = dptr->d_name;
		//see if all dir name is number
		if(All(dir_name, [](char ch){return ch <= '9' && ch >= '0';}))
		{
			int process_id = std::stoi(dir_name);
			std::string file_full_dir = dir_location + dir_name + "/status";
			std::string file_content = FileLocToString(file_full_dir);
			int parent_id = Find_Parent_Id_From_Status(file_content);
			std::string cmd = Find_Cmd_Name_From_Status(file_content);
			TProcInfo proc_info;
			proc_info.cmd = cmd;
			proc_info.pid = process_id;
			proc_info.ppid = parent_id;
			res.emplace_back(proc_info);
		}
	}
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
			std::string file_full_dir = dir_location + dir_name + "/status";
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

	auto all = Get_All_Proc_Infos();

	return;
}

struct TProcTreeNode
{
	int pid;
	int ppid;
	std::string cmd;
	std::vector<std::unique_ptr<TProcTreeNode>> children;
};


void Find_Children(TProcTreeNode* root, std::vector<TProcInfo>& infos, int depth)
{
	for(auto info : infos)
	{
		if(info.ppid == root->pid)
		{
			auto pNode = new TProcTreeNode;
			pNode->cmd = info.cmd;
			pNode->pid = info.pid;
			pNode->ppid = info.ppid;
			root->children.emplace_back(pNode);
		}
	}

	if(root->children.empty())
		return;

	for(auto & child : root->children)
	{
		Find_Children(child.get(), infos, depth + 1);
	}
}

void Print_tree(TProcTreeNode* root, std::stringstream& output, int depth)
{
    //first print myself
    for (int i = 0; i < depth; i++)
    {
    	if(i < depth - 1)
    		output << "    ";
    	else
    		output << "|-------";
    }

    //strip the name part
    output << root->pid << ":" << root->cmd << "\n";
    for (auto &child : root->children)
    {
        Print_tree(child.get(), output, depth + 1);
    }
}

void Get_Procs_Tree_12_2()
{
	auto all = Get_All_Proc_Infos();
	auto root = Where(all, [](TProcInfo info){return info.pid == 1;})[0];

	TProcTreeNode root_node;
	root_node.cmd = root.cmd;
	root_node.pid = root.pid;
	root_node.ppid = root.ppid;

	Find_Children(&root_node, all, 0);

	//print the tree pretty

	std::stringstream ss;

	Print_tree(&root_node, ss, 0);

	std::cout << ss.str();

	return;
}

//for a /proc/PID/fd/ dir, scan the dir and see if the certain file with file_path as name is opened
bool If_Open_File(std::string dir_location, std::string file_path)
{
    DIR *dp = nullptr;
	if(nullptr == (dp = opendir(dir_location.c_str())))
	{
		throw EWithNumber("Could not open the file %s", dir_location);
	}

	//the subdir with name of process ids
	dirent *dptr = nullptr;
	std::string wholeList;

	while(nullptr != (dptr = readdir(dp)))
	{
		std::string link_name = dptr->d_name;
		if(All(link_name, [](char ch){return ch <= '9' && ch >= '0';}))
		{
			std::string link_full_dir = dir_location + "/" + link_name;
			char file_name_buf[500];
			int size = readlink(link_full_dir.c_str(), file_name_buf, 500);
			std::string linked_file_name;
			std::copy(file_name_buf, file_name_buf + size, std::back_inserter(linked_file_name));
			//see what represents the link
			//std::cout << linked_file_name << std::endl;
			if(linked_file_name == file_path)
				return true;
		}
	}

	return false;

}

std::vector<int> Get_Pids_opening_file_12_3(std::string file_path)
{
	std::string dir_location = "/proc/";

    DIR *dp = nullptr;
	if(nullptr == (dp = opendir(dir_location.c_str())))
	{
		throw EWithNumber("Could not open the file %s", dir_location);
	}

	//the subdir with name of process ids
	dirent *dptr = nullptr;

	std::vector<int> res;
	while(nullptr !=  (dptr = readdir(dp)))
	{
		std::string dir_name = dptr->d_name;
		//see if all dir name is number
		if(All(dir_name, [](char ch){return ch <= '9' && ch >= '0';}))
		{
			int process_id = std::stoi(dir_name);
			std::string fd_folder_full_dir = dir_location + dir_name + "/fd/";
			if(If_Open_File(fd_folder_full_dir, file_path))
			{
				res.emplace_back(process_id);
			}
		}

	}
	return res;

}






