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
#include "../include/cmd_update.h"

namespace manager{
void cmd_update(std::filesystem::path ph, bool rele, const configure& con){
    if(!std::filesystem::exists(ph)){
        throw std::runtime_error(std::format("{} does not exist", ph.string()));
    }
    if(!std::filesystem::is_directory(ph)){
        throw std::runtime_error(std::format("{} is not a directory", ph.string()));
    }

    std::filesystem::path x64_debug = ph / "out/build/x64-debug";
    std::filesystem::path x64_release = ph / "out/build/x64-release";

    if(rele){
        std::system(std::format("{} -S {} -B {} -G Ninja -DCMAKE_MAKE_PROGRAM={} -DCMAKE_BUILD_TYPE=Release",con.cmake_ph.string(), ph.string(), x64_release.string(), con.ninja_ph.generic_string()).c_str());
    }
    else {
        std::system(std::format("{} -S {} -B {} -G Ninja -DCMAKE_MAKE_PROGRAM={} -DCMAKE_BUILD_TYPE=Debug",con.cmake_ph.string(), ph.string(), x64_debug.string(), con.ninja_ph.generic_string()).c_str());
    }
}
};