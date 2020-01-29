#include "config/config.h"

std::string ConfigFilePath = R"(C:\????\GitHub\TLib\TLib\config.json)";

int main()
{
    tlib::Config config(ConfigFilePath);
	return 0;
}