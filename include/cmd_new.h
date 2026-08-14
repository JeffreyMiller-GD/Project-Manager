/*
 * prom(Project Manager) - A simple cxx project manager application
 * Copyright (C) 2026  Yvhang Cai(Jeffrey Miller)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#ifndef CMD_NEW_H
#define CMD_NEW_H
#define CMD_NEW_VERSION "1.00"
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

void cmd_new(std::filesystem::path ph, bool rele,const configure& con, bool both = false, int standard = 23, bool enable = false);

result run_new(int argc, char *argv[], const configure& con);

};


#endif