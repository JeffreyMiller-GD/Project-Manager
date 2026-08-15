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

    auto tmp_func = [&](const std::vector<std::string>& args) -> int {
        boost::process::v1::ipstream output;
        boost::process::v1::child c(
            args[0],  
            std::vector<std::string>(args.begin() + 1, args.end()), 
            boost::process::v1::std_out > output
        );
        c.wait();
        std::string line{};
        while(std::getline(output, line)){
            std::cout << line << std::endl;
        }
        return c.exit_code();
    };

    std::vector<std::string> args ={
        con.cpack_ph.string(),
        "--config",
        x64.string(),
        "-B", packages.string()
    };

    tmp_func(args);
     
}

result run_pack(int argc, char *argv[], const configure& con){
    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("Usage:\n\n {} pack <project_root_path> [<build_type>];  default build_type=release", exe.string())};
    }

    std::filesystem::path pro_ph{};
    bool debug = false;

    for(int i = 2; i < argc; ++i){
        if(std::strcmp(argv[i], "--release") == 0 || std::strcmp(argv[i], "release") == 0){
            if(!debug){
                debug = false;
            }
            else {
                return {-1, "The --debug and --release parameters cannot be used together."};
            }
        }
        else if(std::strcmp(argv[i], "--debug") == 0 || std::strcmp(argv[i], "debug") == 0){
            
            debug = true;
            
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
    manager::cmd_pack(pro_ph, debug, con);
    return {0, ""};
}


};