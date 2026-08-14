#include "../include/cmd_run.h"

namespace manager{
void cmd_run(std::filesystem::path ph,const configure& con, std::string bin , bool verbose, bool rele){
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
    

    if(verbose){
        if(rele){
            std::system(std::format("{} --build {}/out/build/x64-release --verbose",con.cmake_ph.string(), ph.string()).c_str());
            
            std::system(std::format("{}/out/build/x64-release/{}", ph.string(), bin).c_str());
        }
        else{
            std::system(std::format("{} --build {}/out/build/x64-debug --verbose",con.cmake_ph.string(), ph.string()).c_str());
            
            std::system(std::format("{}/out/build/x64-debug/{}", ph.string(), bin).c_str());
        }
    }else {
        if(rele){
            std::system(std::format("{} --build {}/out/build/x64-release",con.cmake_ph.string(), ph.string()).c_str());
           
            std::system(std::format("{}/out/build/x64-release/{}", ph.string(), bin).c_str());
        }
        else {
            std::system(std::format("{} --build {}/out/build/x64-debug",con.cmake_ph.string(), ph.string()).c_str());
            
            std::system(std::format("{}/out/build/x64-debug/{}", ph.string(), bin).c_str());
        }
    }
}

result run_run(int argc, char *argv[], const configure& con){
    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("Usage: {} run -pj <project_root_path> [<build_type>] -bin <executable_name>", exe.string())};
    }

    std::filesystem::path pro_ph{};
    bool release = false;
    bool verbose = false;
    std::string bin{};

    for(int i = 2; i < argc; ++i){
        if(std::strcmp(argv[i], "-pj") == 0){
            if(i+1 < argc){
                pro_ph = argv[++i];
            }
            else {
                return {-1, "The -pj option was used but no value was provided."};
            }
        }
        else if (std::strcmp(argv[i], "-bin") == 0){
            if(i+1 < argc){
                bin = argv[++i];
            }
            else {
                return {-1, "The -bin option was used but no value was provided."};
            }
        }
        else if(std::strcmp(argv[i], "--release") == 0 || std::strcmp(argv[i], "release") == 0){
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
        else {
            return {-1, std::format("{}: unknown cmd or option", argv[i])};
        }
    }
    if(pro_ph.empty()){
        throw std::runtime_error("The -pj option was empty.");
    }
    if(bin.empty()){
        throw std::runtime_error("The -bin option was empty.");
    }
    manager::cmd_run(pro_ph, con, bin, verbose, release);
    return {0, ""};
}
};