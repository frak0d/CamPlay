#include <cstddef>
#include <cstdlib>
#include <map>
#include <chrono>
#include <thread>
#include <cstdio>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "colorify.hpp"
#include "opencv2/core/mat.hpp"
#include "predictor.hpp"

using namespace std::literals;
namespace fs = std::filesystem;

bool send_input(const std::string& program, const std::string& xdo_args)
{
	std::system(("xdotool "s+xdo_args).c_str());
	return true;
}

int choose_from_vector(const std::vector<std::string>& vec, const std::string& message)
{
	int i = 1; int choice;
	std::cout << message << std::endl;

	for (const auto& s : vec)
	{
		std::cout << " " << i << " --> " << s << std::endl;
		++i;
	}
	
	std::cin >> choice; --choice;

	if (choice < 0 || choice >= vec.size())
	{
		std::cout << "Invalid Choice, Try again...\n"_red << std::endl;
        return choose_from_vector(vec, message);
	}
	return choice;
}

inline void strip_string(std::string& str)
{
	str.erase(str.find_last_not_of(' ') + 1); //rtrim
	str.erase(0, str.find_first_not_of(' ')); //ltrim
}

auto get_actions(const std::string& config_file)
{
	size_t id;
	std::string line;
	std::string name, cmd;

	std::ifstream file(config_file);
	std::map<size_t, std::pair<std::string,std::string>> actions;
	
	while (std::getline(file, line))
	{
		std::stringstream linestream(line);
		linestream >> id;
		std::getline(linestream, name, ':'); strip_string(name);
		std::getline(linestream, cmd, '\0'); strip_string(cmd);
		actions[id] = {name, cmd};
	}
	return actions;
}

int main(int argc, char* argv[])
{
	if (argc != 2) {
		std::puts("Usage :-\n./CamPlay <delay_ms>");
		std::exit(-1);
	}
try
{
	std::vector<std::string> config_folders;
	for (const auto& entry : fs::directory_iterator("configs"))
	{
		if (entry.is_directory())
		{
			config_folders.push_back(entry.path().string());
		}
	}

	if (config_folders.empty())
	{
		std::cout << "No configs found. Please put your config in 'configs' folder"_red << std::endl;
		return -1;
	}

	auto config_folder = config_folders.at(choose_from_vector(config_folders, "Choose a Program Config :-"_bld));
	
	auto actions = get_actions(config_folder + "/actions.txt");
	auto program = fs::path(config_folder).stem().string();
	
	Predictor predictor(config_folder+"/model.json");
	
	int err_cnt = 0;
	while (true)
	{
		const auto& [Class, Confidence] = predictor.predictImage();

		if (actions.contains(Class))
		{
			std::clog << Class << " : " << actions[Class].first << " -> " << Confidence << std::endl;
			if (!send_input(program, actions[Class].second))
			{
				std::cerr << "Unable to Send Input..."_red << std::endl;
				++err_cnt;
			}
		}
		else // to catch errors if pose_model and pose_action do not match
		{
			std::cerr << "Model Prediction "_red << Class << " not found in 'actions.txt' file, Ignoring..."_red << std::endl;
		}

		if (err_cnt > 50)
		{
			std::cerr << "Too many errors, exiting..."_bldred << std::endl;
			return -3;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(std::stoul(argv[1])));
	}
}
catch (const std::exception& e)
{
	std::cout << e.what() << std::endl;
	std::cout << "Exiting due to Fatal Error..."_bldred << std::endl;
}

}
