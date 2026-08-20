#include "../include/cmd_run.h"

namespace manager{
void cmd_run(std::filesystem::path ph,const configure& con, init &ini, std::filesystem::path target_b, bool verbose, bool rele){
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
    std::filesystem::path build_ph{};
    std::vector<std::string> run_para{};
    std::string v_type{};
    if(target_b.empty()){
        if(rele){
            build_ph = ph / ini.default_b_r;
            run_para.push_back(ph.string()+'/'+ini.default_b_r+'/'+ini.exe); 
        }
        else {
            build_ph = ph / ini.default_b_d;
            run_para.push_back(ph.string()+'/'+ini.default_b_d+'/'+ini.exe); 
        }
    }
    else {
        build_ph = target_b;
        run_para.push_back((build_ph / ini.exe).string()); 
    }
    if(verbose){
        v_type = "--verbose";
    }
    else {
        v_type = "";
    }

    std::vector<std::string> args ={
        con.cmake_ph.string(),
        "--build",
        build_ph.string()
    };

    
    int code = tmp_func(args);
    if(code == 0){
        std::cout << "\x1B[32;1m[Succeed]-[code=" << code << "]\x1B[0m" << std::endl;
        std::cout << "Using the path of executable file: " << run_para.front() << std::endl;
        tmp_func(run_para);
    }
    else {
        std::cerr << "\x1B[31;1m[Failed!]-[code=" << code << "]\x1b[0m" << std::endl;
    }
}

result run_run(int argc, char *argv[], const configure& con){
    
    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("Usage: {} run [-pj <project_root_path>] [-b <build_path>] [<build_type>] -bin <executable_name>", exe.string())};
    }

    std::filesystem::path pro_ph = std::filesystem::current_path();
    bool release = false;
    bool verbose = false;
    init ini{};
    
    std::filesystem::path target{};
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
                ini.exe = argv[++i];
                
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
        else if(std::strcmp(argv[i], "-b") == 0){
            std::cout << "The build type settings will follow the CMake cache, so the build settings you configure will be ineffective.\n";
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
    
    std::cout << ini.exe << ": the executable\n\n";

    manager::cmd_run(pro_ph, con, ini, target, verbose, release);
    return {0, ""};
}
};