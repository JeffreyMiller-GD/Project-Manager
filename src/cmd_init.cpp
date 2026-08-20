#include "../include/cmd_init.h"

namespace manager {
void cmd_init(std::filesystem::path ph){
    auto abs = std::filesystem::absolute(ph);
    std::filesystem::path configuration = abs / "configuration.json";
    
    init ini{};
    ini.exe = abs.filename().string();
    ini.default_b_d = "out/build/x64-debug";
    ini.default_b_r = "out/build/x64-release";
    ini.default_p = "out/packages";
    nlohmann::json j;
    to_json(j,ini);

    std::ofstream out_file{};
    out_file.open(configuration, std::ios::out);
    if(!out_file.is_open()){
        throw std::runtime_error("Can not make configuration.json");
    }
    out_file << std::setw(4) << j;
    out_file.close();
    return;
}

result run_init(int argc, char *argv[]){
    std::filesystem::path exe = argv[0];
    exe = exe.filename();
    if(argc < 3){
        return {-1, std::format("Usage:\n\n {} init -pj <project_root_path>", exe.string())};
    }

    std::filesystem::path pro_ph{};

    for(int i = 2; i < argc; ++i){
        if(std::strcmp(argv[i], "-pj") == 0){
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

    manager::cmd_init(pro_ph);
    return {0, ""};
}
};