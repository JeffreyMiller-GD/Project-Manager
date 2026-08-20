#pragma once
#ifndef CMD_INIT_H
#define CMD_INIT_H
#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <format>
#include <stdexcept>
#include "./config.h"
namespace manager{
void cmd_init(std::filesystem::path ph);

result run_init(int argc, char *argv[]);
};


#endif // cmd_init.h