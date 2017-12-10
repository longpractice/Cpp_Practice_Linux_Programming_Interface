/*
 * params.h
 *
 *  Created on: Dec 10, 2017
 *      Author: yng
 */

#ifndef PARAMS_H_
#define PARAMS_H_

#include <string>
#include <vector>
#include "linqcpp.h"
#include <tuple>
class Params
{
public:
	Params(int _minArgs, int argc, char *argv[])
	:minArgs{_minArgs}
	{
		fileName = argv[0];
		for (int i = 1; i < argc; i++)
			args.emplace_back(argv[i]);
	}



	virtual ~Params() = default;

	std::string fileName;
	std::vector<std::string> args;
	int minArgs{0};

	bool IfHelp(bool must_explicit = false)
	{
		bool has_help_option = args.size() > 0 && (args[0] == "--help" || args[0] == "-h");
		if(!must_explicit)
		{
			if(args.size() < minArgs || has_help_option)
				return true;
		}
		else
		{
			if(has_help_option)
				return true;
		}
		return false;
	}


	bool IfOption(std::string _option)
	{
		return Any(args, [&](std::string val){return val == _option;});
	}


	//
	std::tuple<bool, std::string> FindStrWithOption(std::string _option)
	{
		auto option_iter = std::find(args.begin(), args.end(), _option);
		if(option_iter == args.end())
			return {false, ""};
		else if(std::next(option_iter) == args.end())
			return {true, ""};
		else
			return {true, *std::next(option_iter)};
	}

	std::tuple<bool, bool, int> FindIntWithOption(std::string _option)
	{
		auto option_iter = std::find(args.begin(), args.end(), _option);
		if(option_iter == args.end())
			return {false, false, 0};
		else if(std::next(option_iter) == args.end())
			return {true, false, 0};
		else
		{
			const char* cstr = (*++option_iter).c_str();
			return {true, true, atoi(cstr)};
		}

	}
};



#endif /* PARAMS_H_ */
