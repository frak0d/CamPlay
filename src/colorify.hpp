#include <string>


std::string operator ""_bld (const char* str, size_t)
{
	return "\033[1m" + std::string(str) + "\033[0m";
}


std::string operator ""_red (const char* str, size_t)
{
	return "\033[0;31m" + std::string(str) + "\033[0m";
}

std::string operator ""_bldred (const char* str, size_t)
{
	return "\033[1;31m" + std::string(str) + "\033[0m";
}


std::string operator ""_grn (const char* str, size_t)
{
	return "\033[0;32m" + std::string(str) + "\033[0m";
}

std::string operator ""_bldgrn (const char* str, size_t)
{
	return "\033[1;32m" + std::string(str) + "\033[0m";
}


std::string operator ""_cyn (const char* str, size_t)
{
	return "\033[0;36m" + std::string(str) + "\033[0m";
}

std::string operator ""_bldcyn (const char* str, size_t)
{
	return "\033[1;36m" + std::string(str) + "\033[0m";
}