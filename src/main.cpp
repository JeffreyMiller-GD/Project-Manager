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
#define PROJECT_MANAGER_VERSION "1.00"

#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <format>

#include "../include/main.h"
int main(int argc, char *argv[]){

    std::filesystem::path exep = get_executable_path();
    std::filesystem::path first = exep / "config.json";
    std::filesystem::path output = exep / "output.txt";
    manager::configure con{};
  
    manager::prepare_configuration(con, first, output);

    std::filesystem::remove(output);
    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc >= 2){
        std::string_view cmd = argv[1];
        if(cmd == "new"){
            std::filesystem::path pro_path{};
            if(argc < 3){
                std::cerr << std::format("Usage: {} new <project_path> [<build mode>] [rc]\n\n--build mode: debug, release, both;  default: debug", exe.string()) << std::endl;
                return -1;
            }
            pro_path = argv[2];
            std::string mode{};
            if(argc >= 4){
                mode = argv[3];
                bool rc = false;

                if(argc >= 5){
                    if(std::strcmp(argv[4], "rc") == 0){
                        rc = true;
                    }
                    else {
                        std::cerr << "Unknown build option: " << argv[4] << std::endl;
                    }
                }

                if(mode == "debug")
                    manager::cmd_new(pro_path, false, con, false, 23, rc);
                else if (mode == "release")
                    manager::cmd_new(pro_path, true, con, false, 23, rc);
                else if (mode == "both"){
                    manager::cmd_new(pro_path, true, con, true, 23, rc);
                }
                else {
                    std::cerr << "Unknown build mode: " << mode << std::endl;
                    return -1;
                }
            }
            else {
                manager::cmd_new(pro_path, false, con, false, 23, false);
            }
        }
        else if(cmd == "help"){
            std::cerr << "Usage:\n\n";
            std::cerr << std::format("{} new <project_path> [<build mode>] [rc]\n\n--build mode: debug, release, both;  default: debug", exe.string()) << std::endl;
            std::cerr << std::format("\n{} build <project_root_path> [<build_type>] [--verbose]", exe.string()) << std::endl;;
            std::cerr << std::format("\n{} update <project_root_path> [<build_type>]", exe.string()) << std::endl;
            std::cerr << std::format("\n{} pack <project_root_path> [<build_type>];  default build_type=release", exe.string()) << std::endl;
            std::cerr << std::format("\n{} --version ; show version", exe.string()) << std::endl;
            std::cerr << std::format("\n{} show w ; show warranty message\n{} show c ; show conditions message", exe.string(), exe.string()) << std::endl;
        }
        else if (cmd == "pack"){
            if(argc < 3){
                std::cerr << std::format("Usage:\n\n {} pack <project_root_path> [<build_type>];  default build_type=release", exe.string()) << std::endl;
                return -1;
            }
            std::filesystem::path ph = argv[2];
            if(argc >= 4){
                std::string typ{};
                typ = argv[3];
                if(typ == "release"){
                    manager::cmd_pack(ph, false, con);
                }
                else if(typ == "debug"){
                    manager::cmd_pack(ph, true, con);
                }
                else {
                    std::cerr << "Unknown build mode: " << typ << std::endl;
                    return -1;
                }
            }
            else {
                manager::cmd_pack(ph, false, con);
            }
        }
        else if(cmd == "build"){
            if(argc < 3){
                std::cerr << std::format("Usage:\n\n {} build <project_root_path> [<build_type>] [--verbose]", exe.string()) << std::endl;
                return -1;
            }
            std::filesystem::path ph;
            ph = argv[2];
            std::string typ{};
            bool verbose = false;
            if(argc >= 4){
                typ = argv[3];
                if(argc >= 5){
                    if(std::strcmp(argv[4], "verbose") == 0){
                        verbose = true;
                    }
                    else {
                        std::cerr << "Unknown parameter: " << argv[4] << std::endl;
                        return -1;
                    }
                }
                if(typ == "release"){
                    manager::cmd_build(ph, con, verbose, true);
                }
                else if(typ == "debug"){
                    manager::cmd_build(ph, con, verbose, false);
                }
                else {
                    std::cerr << "Unknown build mode: " << typ << std::endl;
                    return -1;
                }
            }
            else {
                manager::cmd_build(ph, con, verbose, false);
            }
        }
        else if (cmd == "update"){
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
        }
        else if(cmd == "show"){
            if(argc < 3){
                std::cerr << "show w -- show warranty message\nshow c -- show conditions message\n";
                return -1;
            }
            if(std::strcmp(argv[2], "w") == 0){
                show_warranty_full();
            }
            else if(std::strcmp(argv[2], "c") == 0){
                show_conditions();
            }
            else {
                std::cerr << "show w -- show warranty message\nshow c -- show conditions message\n";
                return -1;
            }
        }
        else if(cmd == "--version" || cmd == "-v" || cmd == "--v"){
            std::cout << PROJECT_MANAGER_VERSION << std::endl;
            return 0;
        }
        else {
            std::cerr << "Unknown command: " << argv[1] << std::endl;
            return -1;
        }
    }
    else {
        std::cerr << std::format("Usage:\n\n {} <command> ...\nrun \"{} help\" to get help.", exe.string(), exe.string());
        return -1;
    }
    return 0;
}


namespace manager{
void prepare_configuration(configure& con, const std::filesystem::path& first,
                            const std::filesystem::path& output){
    if(!std::filesystem::exists(first))
        
    {
        std::array<bool, 3> have = {};
        manager::auto_get_tool(con, have, output);
        while(!have[0] || !have[1] || !have[2] ){
            std::string c_com{};

   
            if(!have[0]){
                std::cout << "enter cmake absolute path: ";
                std::getline(std::cin, c_com);
                con.cmake_ph = c_com;
                if(!std::filesystem::exists(con.cmake_ph)){
                    have[0] = false;
                    std::cerr << "Invalid cmake path\n";
                }else {
                    have[0] = true;
                }
            }
            if(!have[1]){
                std::cout << "enter ninja absolute path: ";
                std::getline(std::cin, c_com);
                con.ninja_ph = c_com;
                if(!std::filesystem::exists(con.ninja_ph)){
                    have[1] = false;
                    std::cerr << "Invalid ninja path\n";
                }else {
                    have[1] = true;
                }
            }
            if(!have[2]){
                std::cout << "enter cpack absolute path: ";
                std::getline(std::cin, c_com);
                con.cpack_ph = c_com;
                if(!std::filesystem::exists(con.cpack_ph)){
                    have[2] = false;
                    std::cerr << "Invalid cpack path\n";
                }else {
                have[2] = true;}
            }
        }
        

  
        manager::write_config(con);
        
    }
    else {

        bool failen = false;
        try{
            manager::read_config(con);
        }
        catch (std::exception &e){
            std::cout << "Read JSON failed! Error: " << e.what() << "; Automatic repairing...\n\n";
            con.clear();
            failen = true;
        }
        std::array<bool, 3> have{};
        
        if(!failen){
            if(!con.cmake_ph.empty())
                if(std::filesystem::exists(con.cmake_ph))
                    have[0] = true;
            if(!con.ninja_ph.empty())
                if(std::filesystem::exists(con.ninja_ph))
                    have[1] = true;
            if(!con.cpack_ph.empty())
                if(std::filesystem::exists(con.cpack_ph))
                    have[2] = true;
        }
        if(!have[0] || !have[1] || !have[2]){
            manager::auto_get_tool(con, have, output);
            if(!have[0] || !have[1] || !have[2]){
                std::cout << "\n\nAutomatic repair failed!, you should enter the path of cmake, ninja or cpack.\n\n";
            }
            else {
                std::cout << "\n\nAutomatic repair successfully.\n\n";
            }
            while(!have[0] || !have[1] || !have[2] ){
                std::string c_com{};

    
                if(!have[0]){
                    std::cout << "enter cmake absolute path: ";
                    std::getline(std::cin, c_com);
                    con.cmake_ph = c_com;
                    if(!std::filesystem::exists(con.cmake_ph)){
                        have[0] = false;
                        std::cerr << "Invalid cmake path\n";
                    }else {
                        have[0] = true;
                    }
                }
                if(!have[1]){
                    std::cout << "enter ninja absolute path: ";
                    std::getline(std::cin, c_com);
                    con.ninja_ph = c_com;
                    if(!std::filesystem::exists(con.ninja_ph)){
                        have[1] = false;
                        std::cerr << "Invalid ninja path\n";
                    }else {
                        have[1] = true;
                    }
                }
                if(!have[2]){
                    std::cout << "enter cpack absolute path: ";
                    std::getline(std::cin, c_com);
                    con.cpack_ph = c_com;
                    if(!std::filesystem::exists(con.cpack_ph)){
                        have[2] = false;
                        std::cerr << "Invalid cpack path\n";
                    }else {
                    have[2] = true;}
                }
            }
            manager::write_config(con);
        }
    }
}
};

void show_warranty_full() {
    printf("\n");
    printf("NO WARRANTY\n");
    printf("-----------\n");
    printf("THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n");
    printf("APPLICABLE LAW. EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\n");
    printf("HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\n");
    printf("OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\n");
    printf("THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n");
    printf("PURPOSE. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\n");
    printf("IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n");
    printf("ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n");
    printf("\n");
    printf("Type 'show c' for redistribution conditions.\n");
    printf("\n");
}
void show_conditions() {
    printf("\n");
    printf("COPYRIGHT AND DISTRIBUTION CONDITIONS\n");
    printf("-------------------------------------\n");
    printf("Copyright (C) 2026  Yvhang Cai(Jeffrey Miller)\n"); 
    printf("\n");
    printf("This program is free software: you can redistribute it and/or modify\n");
    printf("it under the terms of the GNU General Public License as published by\n");
    printf("the Free Software Foundation, either version 3 of the License, or\n");
    printf("(at your option) any later version.\n");
    printf("\n");
    printf("This program is distributed in the hope that it will be useful,\n");
    printf("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    printf("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
    printf("GNU General Public License for more details.\n");
    printf("\n");
    printf("You should have received a copy of the GNU General Public License\n");
    printf("along with this program.  If not, see <https://www.gnu.org/licenses/>.\n");
    printf("\n");
}