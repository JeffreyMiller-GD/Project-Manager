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
void cmd_update(std::filesystem::path ph, bool rele, const configure& con, init &ini, std::filesystem::path target_b){
    if(!std::filesystem::exists(ph)){
        throw std::runtime_error(std::format("{} does not exist", ph.string()));
    }
    if(!std::filesystem::is_directory(ph)){
        throw std::runtime_error(std::format("{} is not a directory", ph.string()));
    }

    

    std::filesystem::path build_ph{};
    std::string type{};
    if(target_b.empty()){
        if(rele){
            build_ph = ph / ini.default_b_r;
        }
        else {
            build_ph = ph / ini.default_b_d;
        }
    }
    else {
        build_ph = target_b;
    }
    if(rele){
        type = "Release";
    }
    else {
        type = "Debug";
    }
    auto tmp_func = [&](const std::vector<std::string>& args) -> int {
        boost::process::v1::ipstream output;
        boost::process::v1::ipstream e_output;
        boost::process::v1::child c(
            args[0],  
            std::vector<std::string>(args.begin() + 1, args.end()), 
            boost::process::v1::std_out > output,
            boost::process::v1::std_err > e_output
        );
        std::thread out_th([&]()->void{
            std::string line{};
            while(std::getline(output, line)){
                std::cout << line << std::endl;
            }
        });

        std::thread err_th([&]()->void{
            std::string line{};
            while(std::getline(e_output, line)){
                std::cerr << line << std::endl;
            }
        });
        c.wait();
        err_th.join();
        out_th.join();

        return c.exit_code();
    };
    std::vector<std::string> args = {
        con.cmake_ph.string(),
        "-S", ph.string(),
        "-B", build_ph.string(),
        "-G", "Ninja",
        "-DCMAKE_MAKE_PROGRAM=" + con.ninja_ph.generic_string(),
        "-DCMAKE_BUILD_TYPE="+type,
    };

    int code = tmp_func(args);
    if(code == 0){
        std::cout << "\x1B[32;1m[Succeed]-[code=" << code << "]\x1B[0m" << std::endl;
    }
    else {
        std::cerr << "\x1B[31m[Failed!]-[code=" << code << "]\x1b[0m" << std::endl;
    }
}

result run_update(int argc, char *argv[], const configure& con){


    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("Usage:\n\n {} update [-pj <project_root_path>] [-b <build_path>] [<build_type>]", exe.string())};
    }

    std::filesystem::path pro_ph = std::filesystem::current_path();
    bool release = false;
    std::filesystem::path target{};
    init ini{};
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
        else if(std::strcmp(argv[i], "-b") == 0){
            if(i+1 < argc){
                target = argv[++i];
            }
            else {
                return {-1, "The -b option was used but no value was provided."};
            }
        }
        else {
            return {-1, std::format("{}: unknown cmd or option", argv[i])};
        }
    }
    init_exists(pro_ph);
    check_init(pro_ph);
    
    std::filesystem::path conf = pro_ph / "configuration.json";
    std::ifstream in(conf, std::ios::in);
    if(!in.is_open()){
        throw std::runtime_error("Cannot read configuration.json");
    }
    else {
        nlohmann::json j;
        in >> j;
        
        from_json(j, ini);
    }
    manager::cmd_update(pro_ph, release, con, ini, target);
    return {0, ""};
}
};