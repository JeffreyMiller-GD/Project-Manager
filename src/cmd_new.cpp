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
#include "../include/cmd_new.h"

namespace manager{
void cmd_new(std::filesystem::path ph, bool rele,const configure& con, bool both, int standard, bool enable){
    if(std::filesystem::exists(ph)){
        throw std::runtime_error(std::format("{} exists", ph.string()));
    }
    
    auto abs = std::filesystem::absolute(ph);
    std::string temp = ph.string();
    if(temp.back() == '/' || temp.back() == '\\'){
        temp.pop_back();
    }
    
    ph = temp;
  
    std::cout << "\n\n---------------------\n" << abs << "\n---------------------\n\n";


    std::cout << "The program will create an RC file, with the code specifying the use of an ICO file as the application icon commented out by default;\n"
                 "The program will comment out the code in the CMakeLists.txt file that enables the use of the RC file.\n"
                 "If you need to enable the use of an ICO file as the application icon, please ensure that the ICO file is located in the same directory as `app.rc` and its filename matches the one specified in the RC file, or verify that the ICO file path you provided is valid.\n\n";

    std::filesystem::create_directory(ph);

    std::filesystem::path src = ph / "src";
    std::filesystem::path include = ph / "include";
    std::filesystem::path resources = ph / "resources";
    std::filesystem::path out = ph / "out";

    std::filesystem::path build = out / "build";
    std::filesystem::path packages = out / "packages";

    std::filesystem::path x64_debug = build / "x64-debug";
    std::filesystem::path x64_release = build / "x64-release";

    
    std::filesystem::create_directory(src);
    std::filesystem::create_directory(include);
    std::filesystem::create_directory(resources);
    std::filesystem::create_directory(out);
    std::filesystem::create_directory(build);
    std::filesystem::create_directory(packages);
    std::filesystem::create_directory(x64_debug);
    std::filesystem::create_directory(x64_release);

    std::filesystem::path cmake = ph / "CMakeLists.txt";
    std::ofstream out_file(cmake, std::ios::out);
    if(!out_file.is_open()){
        throw std::runtime_error("Can not make CMakeLists.txt");
    }
    std::vector<std::string> cm{};
    if(enable){
        cm = cmake_get(ph.filename().string(), true, standard);
    }
    else {
        cm = cmake_get(ph.filename().string(), false, standard);
    }
    for(const std::string &line : cm){
        out_file << line;
    }

    out_file.close();


    std::filesystem::path rcph = resources / "app.rc";
    out_file.open(rcph, std::ios::out);
    if(!out_file.is_open()){
        throw std::runtime_error("Can not make app.rc");
    }
    auto rcs = get_rc(false);
    for(const std::string& line : rcs){
        out_file << line;
    }

    out_file.close();


    std::filesystem::path path_main = src / "main.cpp";

    out_file.open(path_main, std::ios::out);

    if(!out_file.is_open()){
        throw std::runtime_error("Can not make main.cpp");
    }

    auto cpp = get_cpp();

    for(const std::string& line : cpp){
        out_file << line;
    }

    out_file.close();

    
    if(!both){
        if(rele){
            std::system(std::format("{} -S {} -B {} -G Ninja -DCMAKE_MAKE_PROGRAM={} -DCMAKE_BUILD_TYPE=Release",con.cmake_ph.string(), ph.string(), x64_release.string(), con.ninja_ph.generic_string()).c_str());
        }
        else {
            
            std::system(std::format("{} -S {} -B {} -G Ninja -DCMAKE_MAKE_PROGRAM={} -DCMAKE_BUILD_TYPE=Debug",con.cmake_ph.string(), ph.string(), x64_debug.string(), con.ninja_ph.generic_string()).c_str());
        }
    }
    else {
        std::system(std::format("{} -S {} -B {} -G Ninja -DCMAKE_MAKE_PROGRAM={} -DCMAKE_BUILD_TYPE=Release",con.cmake_ph.string(), ph.string(), x64_release.string(), con.ninja_ph.generic_string()).c_str());
        std::system(std::format("{} -S {} -B {} -G Ninja -DCMAKE_MAKE_PROGRAM={} -DCMAKE_BUILD_TYPE=Debug",con.cmake_ph.string(), ph.string(), x64_debug.string(), con.ninja_ph.generic_string()).c_str());

    }
}

result run_new(int argc, char *argv[], const configure& con){


    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("{} new -pj <project_path> [<build mode>] [rc]\n\n--build mode: debug, release, both;  default: debug", exe.string())};
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
    manager::cmd_new(pro_ph, release, con, both, 23, rc);
    return {0, ""};
}
};