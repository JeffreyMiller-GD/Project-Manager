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
#include "../include/cmd_build.h"

namespace manager{

void cmd_build(std::filesystem::path ph,const configure& con, bool verbose, bool rele){
    if(!std::filesystem::exists(ph)){
        throw std::runtime_error(std::format("{} does not exists", ph.string()));
    }

    if(!std::filesystem::is_directory(ph)){
        throw std::runtime_error(std::format("{} is not a directory", ph.string()));
    }
    
    std::string temp = ph.string();
#ifdef _WIN32
    if(temp.back() == '/' || temp.back() == '\\'){
        temp.pop_back();
    }
    ph = temp;
#else
    if(temp.back() == '/'){
        temp.pop_back();
    }
    ph = temp;
#endif
    auto tmp_func = [&](std::string cmd) -> int {
        boost::process::v1::ipstream output;
        boost::process::v1::child c(cmd, boost::process::v1::std_out > output);
        c.wait();
        std::string line{};
        while(std::getline(output, line)){
            std::cout << line << std::endl;
        }
        return c.exit_code();
    };
    if(verbose){
        if(rele){
            tmp_func(std::format("{} --build {}/out/build/x64-release --verbose",con.cmake_ph.string(), ph.string()).c_str());
        }
        else{
            tmp_func(std::format("{} --build {}/out/build/x64-debug --verbose",con.cmake_ph.string(), ph.string()).c_str());

        }
    }else {
        if(rele){
            tmp_func(std::format("{} --build {}/out/build/x64-release",con.cmake_ph.string(), ph.string()).c_str());
        }
        else {
            tmp_func(std::format("{} --build {}/out/build/x64-debug",con.cmake_ph.string(), ph.string()).c_str());
        }
    }
    
}

result run_build(int argc, char *argv[], const configure& con){

    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("Usage:\n\n {} build <project_root_path> [<build_type>] [--verbose]", exe.string())};
    }

    std::filesystem::path pro_ph{};
    bool release = false;
    bool verbose = false;

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
        else if(std::strcmp(argv[i], "--verbose") == 0 || std::strcmp(argv[i], "verbose") == 0){
            verbose = true;
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
    manager::cmd_build(pro_ph, con, verbose, release);
    return {0, ""};
}
    

};