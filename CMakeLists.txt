# prom(Project Manager) - A simple cxx project manager application
# Copyright (C) 2026  Yvhang Cai(Jeffrey Miller)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
cmake_minimum_required(VERSION 3.25)
project(project_manager VERSION 1.01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_C_COMPILER /usr/bin/gcc-15)
    set(CMAKE_CXX_COMPILER /usr/bin/g++-15)
endif()

if(WIN32)
    enable_language(RC)
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
endif()

add_executable(prom
    src/main.cpp
    src/cmd_new.cpp
    src/cmd_build.cpp
    src/cmd_update.cpp
    src/cmd_pack.cpp
    src/cmd_run.cpp
    src/config.cpp
    src/findtool.cpp
    ${APP_ICON_RESOURCE}
)

#target_link_options(prom PRIVATE -static)
#include(InstallRequiredSystemLibraries)
install(
    TARGETS prom
    RUNTIME DESTINATION bin
    #RUNTIME_DEPENDENCIES
)
if(WIN32)
    install(
        FILES
            "D:/a_cxx/mingw64/bin/libgcc_s_seh-1.dll"
            "D:/a_cxx/mingw64/bin/libstdc++-6.dll"
        DESTINATION bin
    )
endif()

if(UNIX AND NOT APPLE)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.29), libstdc++6 (>= 9)")
endif()


set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")

set(CPACK_PACKAGE_NAME "project_manager")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VENDOR "Yvhang Cai(Jeffrey Miller)")

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "A simple cxx project manager application"
)

set(CPACK_PACKAGE_CONTACT
    "Yvhang Cai(Jeffrey Miller)"
)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "project_manager-${PROJECT_VERSION}")

if(WIN32)

    # Windows installer
    set(CPACK_GENERATOR "NSIS")

    # Ask user whether to add install/bin to PATH
    set(CPACK_NSIS_MODIFY_PATH ON)


elseif(UNIX AND NOT APPLE)

    # Linux package
    set(CPACK_GENERATOR "DEB")

    # install to /usr/bin
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")


elseif(APPLE)

    # macOS
    set(CPACK_GENERATOR "TGZ")

    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")

endif()


include(CPack)