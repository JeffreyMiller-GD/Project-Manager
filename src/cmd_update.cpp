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

result run_update(int argc, char *argv[], const configure& con){
/*
result run_new(int argc, char *argv[], const configure& con){


    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("Usage: {} new <project_path> [<build mode>] [rc]\n\n--build mode: debug, release, both;  default: debug", exe.string())};
    }

    std::filesystem::path pro_ph{};

    bool release = false;

    bool both = false;
    bool rc = false;

    for(int i = 2; i < argc; ++i){
        if (std::strcmp(argv[i], "--release") == 0 || std::strcmp(argv[i], "release") == 0){
            if(!both){
                release = true;
            }
            else {
                return {-1, "The --both, --debug and --release parameters cannot be used together."};
            }
        }
        else if (std::strcmp(argv[i], "--both") == 0 || std::strcmp(argv[i], "both") == 0){
            if(!release){
                both = true;
            }
            else {
                return {-1, "The --both, --debug and --release parameters cannot be used together."};
            }
        }
        else if (std::strcmp(argv[i], "--debug") == 0 || std::strcmp(argv[i], "debug") == 0){
            if(!both || !release){
                release = false;
            }
            else {
                return {-1, "The --both, --debug and --release parameters cannot be used together."};
            }
        }
        else if (std::strcmp(argv[i], "--rc") == 0 || std::strcmp(argv[i], "rc") == 0){
            rc = true;
        }
        
        else {
            pro_ph = argv[i];
        }
    }
    manager::cmd_new(pro_ph, release, con, both, 23, rc);
    return {0, ""};
}



       if(argc < 3){
                std::cerr << std::format("Usage:\n\n {} update <project_root_path> [<build_type>]", exe.string()) << std::endl;
                return -1;
            }
            std::filesystem::path ph = argv[2];
            if(argc >= 4){
                std::string typ ={};
                typ = argv[3];
                if(typ == "release"){
                    manager::cmd_update(ph, true, con);
                }
                else if(typ == "debug"){
                    manager::cmd_update(ph, false, con);
                }
                else {
                    std::cerr << "Unknown build mode: " << typ << std::endl;
                    return -1;
                }
            }
            else {
                manager::cmd_update(ph, false, con);
            }
*/

    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("Usage:\n\n {} update <project_root_path> [<build_type>]", exe.string())};
    }

    std::filesystem::path pro_ph{};
    bool release = false;

    for(int i = 2; i < argc; ++i){
        
        if(std::strcmp(argv[i], "--release") == 0 || std::strcmp(argv[i], "release") == 0){
            release = true;
          
        }
        else if(std::strcmp(argv[i], "--debug") == 0 || std::strcmp(argv[i], "debug") == 0){
            if(!release){
                release = false;
            }
            else {
                return {-1, "The --debug and --release parameters cannot be used together."};
            }
        }
        else if(std::strcmp(argv[i], "-pj") == 0){
            if(i+1 < argc){
                pro_ph = argv[++i];
            }
            else {
                return {-1, "The -pj option was used but no value was provided."};
            }
        }
        else {
            return {-1, std::format("{}: unknown cmd or option", argv[i])};
        }
    }
    if(pro_ph.empty()){
        throw std::runtime_error("The -pj option was empty.");
    }
    manager::cmd_update(pro_ph, release, con);
    return {0, ""};
}
};