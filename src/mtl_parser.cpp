#include"mtl_parser.h"  
#include <fstream>  // 添加这一行
std::map<std::string, Material> MTLParser::parse(const std::string& filepath) {
    std::map<std::string, Material> materials;
    std::ifstream in(filepath);
    
    if (!in.is_open()) {
        std::cerr << "无法打开 MTL 文件: " << filepath << std::endl;
        return materials;
    }
    
    Material current_mat;
    std::string base_path = filepath.substr(0, filepath.find_last_of("/\\") + 1);
    
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "newmtl") {
            if (!current_mat.name.empty()) {
                materials[current_mat.name] = current_mat;
            }
            iss >> current_mat.name;
            current_mat = Material();  
            current_mat.name = materials.size() > 0 ? 
                               current_mat.name : current_mat.name;
        }
        else if (prefix == "map_Kd") { 
            std::string texname;
            iss >> texname;
            current_mat.diffuse_map = base_path + texname;
        }
        else if (prefix == "map_Bump" || prefix == "bump") { 
            std::string texname;
            iss >> texname;
            current_mat.normal_map = base_path + texname;
        }
        else if (prefix == "map_Ks") { 
            std::string texname;
            iss >> texname;
            current_mat.specular_map = base_path + texname;
        }
        else if (prefix == "Ka") {  
            iss >> current_mat.Ka.x >> current_mat.Ka.y >> current_mat.Ka.z;
        }
        else if (prefix == "Kd") {  
            iss >> current_mat.Kd.x >> current_mat.Kd.y >> current_mat.Kd.z;
        }
        else if (prefix == "Ks") {  
            iss >> current_mat.Ks.x >> current_mat.Ks.y >> current_mat.Ks.z;
        }
        else if (prefix == "Ns") {  
            iss >> current_mat.Ns;
        }
    }
    
    if (!current_mat.name.empty()) {
        materials[current_mat.name] = current_mat;
    }
    
    std::cout << "✓ 加载了 " << materials.size() << " 个材质" << std::endl;
    return materials;
}