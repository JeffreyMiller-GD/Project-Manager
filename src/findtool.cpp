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
#include "../include/findTools.h"

namespace manager{
void auto_get_tool(configure& con, std::array<bool, 3>& have,const std::filesystem::path& output){
    std::ofstream out(output, std::ios::out);
    if(!out.is_open()){
        
        throw std::runtime_error("cannot read config");

    }
    out.close();
    std::ifstream in(output, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("cannot read config");
    }
#ifdef _WIN32

    std::system(std::format("where.exe cmake > {}", output.string()).c_str());
    in.close();
    in.open(output, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("cannot read config");
    }
    in >> con.cmake_ph;
    if(!con.cmake_ph.empty()){
        have[0] = true;
    }
    std::system(std::format("where.exe ninja > {}", output.string()).c_str());
    in.close();
    in.open(output, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("cannot read config");
    }
    in >> con.ninja_ph;
    if(!con.ninja_ph.empty()){
        have[1] = true;
    }
    std::system(std::format("where.exe cpack > {}", output.string()).c_str());
    in.close();
    in.open(output, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("cannot read config");
    }
    in >> con.cpack_ph;
    if(!con.cpack_ph.empty()){
        have[2] = true;
    }
#else

    std::system(std::format("which cmake > {}", output.string()).c_str());
    in.close();
    in.open(output, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("cannot read config");
    }
    in >> con.cmake_ph;
    if(!con.cmake_ph.empty()){
        have[0] = true;
    }
    std::system(std::format("which ninja > {}", output.string()).c_str());
    in.close();
    in.open(output, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("cannot read config");
    }
    in >> con.ninja_ph;
    if(!con.ninja_ph.empty()){
        have[1] = true;
    }
    std::system(std::format("which cpack > {}", output.string()).c_str());
    in.close();
    in.open(output, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("cannot read config");
    }
    in >> con.cpack_ph;
    if(!con.cpack_ph.empty()){
        have[2] = true;
    }
#endif

    if(!std::filesystem::exists(con.cmake_ph)){
        have[0] = false;
    }
    if(!std::filesystem::exists(con.ninja_ph)){
        have[1] = false;
    }
    if(!std::filesystem::exists(con.cpack_ph)){
        have[2] = false;
    }

}
};