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
project(project_manager VERSION 1.03 LANGUAGES CXX)

# ============================================================================
# Boost Configuration
# ============================================================================
# NOTE: If you are building on Linux or using a different Boost installation,
#       change the paths below to point to your Boost root directory.
#
# Example for Linux (system-wide Boost):
#   set(BOOST_ROOT "/usr")
#   set(BOOST_INCLUDEDIR "/usr/include/boost")
#   set(BOOST_LIBRARYDIR "/usr/lib/x86_64-linux-gnu")
#
# Example for custom Boost path on Linux:
#   set(BOOST_ROOT "/home/user/libs/boost_1_91_0")
#   set(BOOST_INCLUDEDIR "/home/user/libs/boost_1_91_0")
#   set(BOOST_LIBRARYDIR "/home/user/libs/boost_1_91_0/stage/lib")
# ============================================================================
set(BOOST_ROOT "D:/Libraries/boost_1_91_0")
set(BOOST_INCLUDEDIR "D:/Libraries/boost_1_91_0")
set(BOOST_LIBRARYDIR "D:/Libraries/boost_1_91_0/stage/lib")

# Force Boost.Process to use v1 API
add_definitions(-DBOOST_PROCESS_USE_V1)

# ============================================================================
# C++ Standard
# ============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# Compiler Settings (Optional)
# ============================================================================
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Uncomment and adjust if you need a specific compiler version
    # set(CMAKE_C_COMPILER /usr/bin/gcc-15)
    # set(CMAKE_CXX_COMPILER /usr/bin/g++-15)
endif()

# ============================================================================
# Windows Resource File
# ============================================================================
if(WIN32)
    enable_language(RC)
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/resources/app.rc")
endif()

# ============================================================================
# Executable
# ============================================================================
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

# ============================================================================
# Include Directories
# ============================================================================
target_include_directories(prom PRIVATE ${BOOST_INCLUDEDIR})

# ============================================================================
# Link Libraries
# ============================================================================
# NOTE: The Boost.Filesystem library name may differ on your system.
#       Check your ${BOOST_LIBRARYDIR} for the actual filename and adjust below.
#
# Windows (MinGW) example:  libboost_filesystem-mgw16-mt-d-x64-1_91.a
# Linux (GCC) example:      libboost_filesystem.a  or  libboost_filesystem.so
# ============================================================================
target_link_libraries(prom PRIVATE
    ${BOOST_LIBRARYDIR}/libboost_filesystem-mgw16-mt-d-x64-1_91.a
)

# ============================================================================
# Platform-Specific Dependencies
# ============================================================================
# Windows: ws2_32 is required by Boost.Asio (used by Boost.Process)
# Linux:   ws2_32 is NOT available. Use pthread instead if needed.
#
# If building on Linux, comment out the WIN32 block below and uncomment
# the Linux block.
# ============================================================================
if(WIN32)
    # Windows Socket Library (required by Boost.Asio)
    target_link_libraries(prom PRIVATE ws2_32)
else()
    # Linux: pthread is required for Boost libraries
    # Uncomment the line below if you encounter undefined references to pthread
    # target_link_libraries(prom PRIVATE pthread)
endif()

# ============================================================================
# Static Linking (Optional)
# ============================================================================
# Uncomment to link statically (not recommended for distribution)
# target_link_options(prom PRIVATE -static)

# ============================================================================
# Installation
# ============================================================================
install(
    TARGETS prom
    RUNTIME DESTINATION bin
)

# Install runtime dependencies on Windows (MinGW)
if(WIN32)
    # NOTE: Update these paths to match your MinGW installation
    install(
        FILES
            "D:/a_cxx/mingw64/bin/libgcc_s_seh-1.dll"
            "D:/a_cxx/mingw64/bin/libstdc++-6.dll"
        DESTINATION bin
    )
endif()

# ============================================================================
# CPack Packaging
# ============================================================================
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
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_MODIFY_PATH ON)
elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")
elseif(APPLE)
    set(CPACK_GENERATOR "TGZ")
    set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
endif()

include(CPack)