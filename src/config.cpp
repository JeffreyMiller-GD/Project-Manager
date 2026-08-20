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

void init_exists(const std::filesystem::path ph){
    if(!std::filesystem::exists(ph / "configuration.json")){
        throw std::runtime_error(std::format("{} does not exists.\n\tTry run 'init -pj <project_root_path>' command.", ph.string()));
    }
}

void check_init(const std::filesystem::path ph){
    auto i_ph = ph / "configuration.json";
    std::ifstream in(i_ph, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("Cannot read configuration.json");
    }
    nlohmann::json j{};
    in >> j;
    in.close();
    init ini{};
    from_json(j, ini);
    if(ini.exe.empty()){
        throw std::runtime_error("the key 'exe' is empty or does not exists\n\tTry run 'init -pj <project_root_path>' command.");
    }
    if(ini.default_b_d.empty()){
        throw std::runtime_error("the key 'default_debug' is empty or does not exists\n\tTry run 'init -pj <project_root_path>' command.");
    }
    if(ini.default_b_r.empty()){
        throw std::runtime_error("the key 'default_release' is empty or does not exists\n\tTry run 'init -pj <project_root_path>' command.");
    }
    if(ini.default_p.empty()){
        throw std::runtime_error("the key 'default_pkg' is empty or does not exists\n\tTry run 'init -pj <project_root_path>' command."); 
    }
}

};