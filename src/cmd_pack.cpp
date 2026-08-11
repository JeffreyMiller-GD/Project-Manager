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
#include "../include/cmd_pack.h"

namespace manager{
void cmd_pack(std::filesystem::path ph, bool debug, const configure& con){
    if(!std::filesystem::exists(ph)){
        throw std::runtime_error(std::format("{} does not exists", ph.string()));
    }

    if(!std::filesystem::is_directory(ph)){
        throw std::runtime_error(std::format("{} is not a directory", ph.string()));
    }
    std::filesystem::path x64{};
    if(!debug){
        x64 = ph / "out/build/x64-release/CPackConfig.cmake";
    }
    else {
        x64 = ph / "out/build/x64-debug/CPackConfig.cmake";
    }
    std::filesystem::path packages = ph / "out/packages";

    std::system(std::format("{} --config {} -B {}",con.cpack_ph.string(), x64.string(), packages.string()).c_str());
    
}



};