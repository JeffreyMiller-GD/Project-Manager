#pragma once
#ifndef CMD_RUN_H
#define CMD_RUN_H

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

void cmd_run(std::filesystem::path ph,const configure& con, std::string bin , bool verbose = false, bool rele = false);

result run_run(int argc, char *argv[], const configure& con);


};


#endif