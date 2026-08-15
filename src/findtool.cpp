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
#define CMAKE 0
#define NINJA 1
#define CPACK 2
namespace manager{
void auto_get_tool(configure& con, std::array<bool, 3>& have){
    

    auto tmp_func = [&](std::string cmd, int target) -> int {
        boost::process::v1::ipstream output;
        boost::process::v1::child c(cmd, boost::process::v1::std_out > output);
        c.wait();
        std::string line{};
        std::getline(output, line);
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        switch(target){
            case 0:{
                con.cmake_ph = line;
                
                break;
            }
            case 1:{
                con.ninja_ph = line;
                break;
            }
            case 2:{
                con.cpack_ph = line;
                break;
            }
        }
        
        
        int code = c.exit_code();
        
        switch(target){
            case 0:{
                if(!con.cmake_ph.empty()){
                    have[target] = true;
                }
                break;
            }
            case 1:{
                if(!con.ninja_ph.empty()){
                    have[target] = true;
                }
                break;
            }
            case 2:{
                if(!con.cpack_ph.empty()){
                    have[target] = true;
                }
                break;
            }
        }

        return code;
    };
#ifdef _WIN32

    tmp_func("where.exe cmake", CMAKE);
    tmp_func("where.exe ninja", NINJA);
    tmp_func("where.exe cpack", CPACK);
    
#else
    tmp_func("which cmake", CMAKE);
    tmp_func("which ninja", NINJA);
    tmp_func("which cpack", CPACK);
#endif


    auto file_exists = [](const std::filesystem::path& p) -> bool {
        std::ifstream f(p);
        return f.good();
    };

    
    if(!file_exists(con.cmake_ph)){
       
        have[0] = false;
    }
    if(!file_exists(con.ninja_ph)){
      
        have[1] = false;
    }
    if(!file_exists(con.cpack_ph)){
        
        have[2] = false;
    }

}
};