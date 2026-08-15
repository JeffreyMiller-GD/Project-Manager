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
    std::vector<std::string> args_r_v ={
        con.cmake_ph.string(),
        "--build",
        ph.string()+"/out/build/x64-release",
        "--verbose"
    };
    std::vector<std::string> args_d_v ={
        con.cmake_ph.string(),
        "--build",
        ph.string()+"/out/build/x64-debug",
        "--verbose"
    };
    std::vector<std::string> args_r ={
        con.cmake_ph.string(),
        "--build",
        ph.string()+"/out/build/x64-release"
    };
    std::vector<std::string> args_d ={
        con.cmake_ph.string(),
        "--build",
        ph.string()+"/out/build/x64-debug"
    };

    std::vector<std::string> run_para_r ={
        ph.string()+"/out/build/x64-release/"+bin
    };
    std::vector<std::string> run_para_d ={
        ph.string()+"/out/build/x64-debug/"+bin
    };
    if(verbose){
        if(rele){
            tmp_func(args_r_v);
            
            tmp_func(run_para_r);
        }
        else{
            tmp_func(args_d_v);
            
            tmp_func(run_para_d);
        }
    }else {
        if(rele){
            tmp_func(args_r);
           
            tmp_func(run_para_r);
        }
        else {
            tmp_func(args_d);
            
            tmp_func(run_para_d);
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