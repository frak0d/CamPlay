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

namespace fs = std::filesystem;

bool send_input(const std::string& program, const std::vector<std::string>& keys)
{
	// send keyboard input to program using platform dependent stuff
	return false;
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

auto get_actions(const std::string& config_file)
{
	std::map<std::string, std::vector<std::string>> actions;
	std::ifstream file(config_file); std::string line;

	while (std::getline(file, line))
	{
		std::stringstream linestream(line);
		std::string action, key;
		linestream >> action;
		while (linestream >> key)
		{
			actions[action].push_back(key);
			key.clear();
		}
	}
	return actions;
}

int main()
{
try
{
	std::vector<std::string> config_folders;
	for (const auto& entry : fs::directory_iterator("app_configs"))
	{
		if (entry.is_directory())
		{
			config_folders.push_back(entry.path().string());
		}
	}

	if (config_folders.empty())
	{
		std::cout << "No config folders found. Please put your configs in 'app_configs' folder"_red << std::endl;
		return -1;
	}

	auto config_folder = config_folders.at(choose_from_vector(config_folders, "Choose a Program Config :-"_bld));
	
	auto actions = get_actions(config_folder + "/pose_action");
	auto program = fs::path(config_folder).stem().string();

	int err_cnt = 0;
	while (true)
	{
		std::string prediction; // = tflite->predict(image);

		if (actions.contains(prediction))
		{
			if (!send_input(program, actions[prediction]))
			{
				std::cerr << "Unable to Send Input..."_red << std::endl;
				++err_cnt;
			}
		}
		else // to catch errors if pose_model and pose_action do not match
		{
			std::cout << "Model Prediction "_red << prediction << " not found in 'pose_action' file, Ignoring..."_red << std::endl;
		}

		if (err_cnt > 20)
		{
			std::cout << "Too many errors, exiting..."_bldred << std::endl;
			return -3;
		}
		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
catch (const std::exception& e)
{
	std::cout << e.what() << std::endl;
	std::cout << "Exiting due to Fatal Error..."_bldred << std::endl;
}
}
