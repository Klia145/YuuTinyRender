#include"model.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include<algorithm>

Model::Model(const std::string&filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr<<"Failed to open file"<<filename<<std::endl;
        return ;
    }
    std::string line;
    while(std::getline(file,line)){
        std::istringstream iss(line);
        std::string prefix;
        iss>>prefix;

        if(prefix=="v"){
            vec3 v;
            iss>>v.x>>v.y>>v.z;
            vertices_.push_back(v);
        }
        else if(prefix=="vt"){
            vec2 uv;
            iss>>uv.u>>uv.v;
            uvs_.push_back(uv);
        }
        else if(prefix=="vn"){
            vec3 n;
            iss>>n.x>>n.y>>n.z;
            normals_.push_back(n);
        }
        else if(prefix=="f"){
            std::vector<int>face;
            std::string vertex_str;

            std::vector<int>vertex_indices;
            std::vector<int>uv_indices;
            std::vector<int>normal_indices;
            while(iss>>vertex_str){
                std::replace(vertex_str.begin(),vertex_str.end(),'/',' ');
                std::istringstream viss(vertex_str);

                int v_idx,vt_idx,vn_idx;
                viss>>v_idx;
                vertex_indices.push_back(v_idx-1);
                if(viss>>vt_idx){
                    uv_indices.push_back(vt_idx-1);
                }
                if(viss>>vn_idx){
                    normal_indices.push_back(vn_idx-1);
                }
            }
            if(vertex_indices.size()==3){
                faces_.push_back(vertex_indices);
                if(uv_indices.size()==3){
                    uv_indices_.push_back(uv_indices);   
                }
                else{
                    uv_indices_.push_back({0,0,0});
                }
                if(normal_indices.size()==3){
                    normal_indices_.push_back(normal_indices);
                }
                else{
                    normal_indices_.push_back({0,0,0});
                }
            }
        }

    }
    file.close();
    std::cout<<"模型加载完成:"<<std::endl;
    std::cout<<"顶点数:"<<vertices_.size()<<std::endl;
    std::cout<<"UV:"<<uvs_.size()<<std::endl;
    std::cout<<"法线:"<<normals_.size()<<std::endl;
    std::cout<<"面:"<<faces_.size()<<std::endl;

}

    /*    
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr<<"Failed to open file"<<filename<<std::endl;
        return ;
    }
    std::string line;
    while(std::getline(file,line)){
        std::istringstream iss(line);
        std::string prefix;

        iss>>prefix;

        if(prefix=="v"){
            vec3 v;
            iss>>v.x>>v.y>>v.z;
            vertices_.push_back(v);
        }
        else if (prefix=="vt"){
            vec2 uv;
            iss>>uv.u>>uv.v;
            uvs_.push_back(uv);
        }
        else if(prefix=="f"){
            std::vector<int>face;
            std::string vertex_str;

            std::vector<int>vertex_indices;
            std::vector<int>uv_indices;
            while(iss>>vertex_str){
                std::replace(vertex_str.begin(),vertex_str.end(),'/',' ');
                std::istringstream viss(vertex_str);
                int v_idx,vt_idx,vn_idx;
                viss>>v_idx;
                vertex_indices.push_back(v_idx-1);
                if(viss>>vt_idx){
                    uv_indices.push_back(vt_idx-1);
                }

            
                std::string::size_type slash_pos = vertex_str.find('/');
                std::string index_str;
                if (slash_pos != std::string::npos) {
                    index_str = vertex_str.substr(0, slash_pos);
                } else {
                    index_str = vertex_str;
                }
                int idx = std::stoi(index_str);
                face.push_back(idx - 1);  // OBJ索引从1开始，转为从0开始
                */
                

int Model::nverts()const  {
        return vertices_.size();
    }
int Model::nfaces() const{
    return faces_.size();
}
vec3 Model::vert(int i)const{
    return vertices_[i];
}
std::vector<int>Model::face(int i)const{
    return faces_[i];
}
vec3 Model::vert(int iface,int nthvert) const{
    return vertices_[faces_[iface][nthvert]];
}
vec3 Model::normal(int iface,int nthvert)const{
    if(normal_indices_.empty()||normals_.empty()){
        return vec3(0,0,1);

    }
    int normal_idx=normal_indices_[iface][nthvert];
    return normals_[normal_idx];
}
vec2 Model::uv(int iface,int nthvert)const{
    if(uv_indices_.empty()||uvs_.empty()){
        return vec2(0,0);
    }
    int uv_idx=uv_indices_[iface][nthvert];
    return uvs_[uv_idx];
}
/*
Model::Model(const std::string&filename){
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cerr<<"Failed to open file"<<filename<<std::endl;
        return ;
    }
    std::string line;
    while(std::getline(file,line)){
        std::istringstream iss(line);
        std::string prefix;
        iss>>prefix;

        if(prefix=="v")
        {
            vec3 v;
            iss>>v.x>>v.y>>v.z;
            vertices_.push_back(v);
        }
        else if(prefix=="f")
        {
            std::vector<int>face;
            std::string vertex_str;

            while(iss>>vertex_str){

                int slash_pos=vertex_str.find('/');
                std::string index_str;

                if (slash_pos != std::string::npos) {
                    index_str = vertex_str.substr(0, slash_pos);
                } else {
                    index_str = vertex_str;
                }
                
                int idx = std::stoi(index_str);
                face.push_back(idx - 1);  // OBJ索引从1开始，转为从0开始
            }
            if(face.size()==3){
                faces_.push_back(face);
            }
            else{
                std::cerr<<"Error: face with more than 3 vertices"<<std::endl;
            }
        }
        std::cout<<"加载结束："<<nverts()<<"个顶点，"<<nfaces()<<"个面"<<std::endl;
    }
}
int Model::nverts()const  {
        return vertices_.size();
    }
int Model::nfaces() const{
    return faces_.size();
}
vec3 Model::vert(int i)const{
    return vertices_[i];
}
std::vector<int>Model::face(int i)const{
    return faces_[i];
}
*/