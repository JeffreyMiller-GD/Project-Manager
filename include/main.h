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
#ifndef MAIN_H
#define MAIN_H
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
#include "../include/cmd_new.h"
#include "../include/cmd_build.h"
#include "../include/cmd_update.h"
#include "../include/cmd_pack.h"
#include "../include/config.h"
#include "../include/exePath.h"
#include "../include/findTools.h"
#include "../include/cmd_run.h"
namespace manager{
void prepare_configuration(configure& con, const std::filesystem::path& first,
                            const std::filesystem::path& output);
int main_run(int argc, char *argv[], const configure& con);
};

void show_warranty_full();
void show_conditions();
#endif