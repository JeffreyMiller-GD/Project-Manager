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
#include "../include/config.h"
#include "../include/exePath.h"
namespace manager{
void write_config(const configure& con){
    nlohmann::json j;
    to_json(j, con);

    std::filesystem::path exe_ph = get_executable_path();
    std::ofstream out(exe_ph / "config.json", std::ios::out);
    if(!out.is_open()){
        throw std::runtime_error("cannot make config.json");
    }
    out << std::setw(4) << j << std::endl;
    out.close();
}

void read_config(configure& con){
    std::filesystem::path exe_ph = get_executable_path();
    std::ifstream in(exe_ph / "config.json", std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("cannot open config.json");
    }
    nlohmann::json j;
    in >> j;
    in.close();
    from_json(j, con);
}
};