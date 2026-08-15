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
#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdint>
#include <format>
#include <stdexcept>
#include "./json.hpp"
#include <boost/process/v1.hpp>


namespace manager{
class configure {
public:

    std::filesystem::path cmake_ph;
    std::filesystem::path ninja_ph;
    std::filesystem::path cpack_ph;
    configure() : cmake_ph(), ninja_ph(), cpack_ph(){}
    configure(std::filesystem::path cmk, std::filesystem::path nin, std::filesystem::path cpk)
        : cmake_ph(cmk), ninja_ph(nin), cpack_ph(cpk){}
    ~configure() = default;

    std::ostream& operator<<(std::ostream& os){
        os << cmake_ph << "; " << ninja_ph << "; " << cpack_ph;
        return os;
    }

    void clear(){
        cmake_ph.clear();
        cpack_ph.clear();
        ninja_ph.clear();
    }
};

class result{
public:
    int code;
    std::string msg;
    result() : code(0), msg(""){}
    result(int cod, std::string ms) : code(code), msg(ms){}
    ~result() = default;
};

inline std::vector<std::string> cmake_get(
    std::string name,

 
    bool enable,
    
    int standard = 23)
{
    std::vector<std::string> cmk{

        "cmake_minimum_required(VERSION 3.25)\n",

        std::format(
            "project({} VERSION 0.1 LANGUAGES CXX C)\n\n",
            name
        ),

        std::format(
            "set(CMAKE_CXX_STANDARD {})\n",
            standard
        ),

        "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n",
        "set(CMAKE_CXX_EXTENSIONS OFF)\n\n",

        std::format(
            "set(CMAKE_C_STANDARD {})\n",
            standard
        ),

        "set(CMAKE_C_STANDARD_REQUIRED ON)\n",
        "set(CMAKE_C_EXTENSIONS OFF)\n\n",

        "include_directories(include)\n\n"
    };


    if(enable){

        cmk.push_back(
            "if(WIN32)\n"
        );

        cmk.push_back(
            "    enable_language(RC)\n"
        );

        cmk.push_back(
            "    set(APP_ICON_RESOURCE "
            "\"${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc\")\n"
        );

        cmk.push_back(
            "endif()\n\n"
        );

    }


    cmk.push_back(
        std::format(
            "add_executable({}\n",
            name
        )
    );

    cmk.push_back(
        "    src/main.cpp\n"
    );


    if(enable){

        cmk.push_back(
            "    ${APP_ICON_RESOURCE}\n"
        );

    }


    cmk.push_back(
        ")\n"
    );


    return cmk;
}

inline std::vector<std::string> get_cpp(){
    return {
        "#include <iostream>\n\n",

        "int main(int argc, char *argv[]) {\n\n",
        "   std::cout << \"Hello, World!\" << std::endl;\n",
        "   return 0;\n\n",
        "}\n",
    };
}

inline std::vector<std::string> get_rc(bool enable){
    std::vector<std::string> rc{
        "#include <windows.h>\n\n"
    };
    if(enable){
        rc.push_back("IDI_ICON1 ICON DISCARDABLE \"app.ico\"");
    }
    else {
        rc.push_back("/*IDI_ICON1 ICON DISCARDABLE \"app.ico\"*/");
    }

    rc.push_back("VS_VERSION_INFO VERSIONINFO\n");
    rc.push_back("    FILEVERSION    1,0,0,0\n");
    rc.push_back("    PRODUCTVERSION 1,0,0,0\n");
    rc.push_back("\n");
    rc.push_back("    FILEFLAGSMASK  VS_FFI_FILEFLAGSMASK\n");
    rc.push_back("    FILEFLAGS      0\n");
    rc.push_back("    FILEOS         VOS_NT_WINDOWS32\n");
    rc.push_back("    FILETYPE       VFT_APP\n");
    rc.push_back("    FILESUBTYPE    VFT2_UNKNOWN\n");
    rc.push_back("\n");
    rc.push_back("BEGIN\n");
    rc.push_back("    BLOCK \"StringFileInfo\"\n");
    rc.push_back("    BEGIN\n");
    rc.push_back("        BLOCK \"040904b0\"\n");
    rc.push_back("        BEGIN\n");
    rc.push_back("            VALUE \"FileDescription\",  \"Application Description\"\n");
    rc.push_back("            VALUE \"FileVersion\",      \"1.0.0.0\"\n");
    rc.push_back("            VALUE \"InternalName\",     \"ApplicationInternalName\"\n");
    rc.push_back("            VALUE \"LegalCopyright\",   \"Copyright (C) YYYY Author. Licensed under GPL-3.0-or-later.\"\n");
    rc.push_back("            VALUE \"ProductName\",      \"Application Name\"\n");
    rc.push_back("            VALUE \"ProductVersion\",   \"1.0.0\"\n");
    rc.push_back("            VALUE \"OriginalFilename\", \"application.exe\"\n");
    rc.push_back("            VALUE \"CompanyName\",      \"Company Name\"\n");
    rc.push_back("        END\n");
    rc.push_back("    END\n");
    rc.push_back("\n");
    rc.push_back("    BLOCK \"VarFileInfo\"\n");
    rc.push_back("    BEGIN\n");
    rc.push_back("        VALUE \"Translation\", 0x409, 1200\n");
    rc.push_back("    END\n");
    rc.push_back("END\n");

    return rc;
}



inline void to_json(nlohmann::json &j, const configure& con){
    j = nlohmann::json{
       
        {"cmake", con.cmake_ph},
        {"ninja", con.ninja_ph},
        {"cpack", con.cpack_ph},
    };
}

inline void from_json(const nlohmann::json &j, configure& con) {
    con.cmake_ph = j.value("cmake", "");
    con.ninja_ph = j.value("ninja", "");
    con.cpack_ph = j.value("cpack", "");
}

void write_config(const configure& con);
void read_config(configure& con);


};


#endif