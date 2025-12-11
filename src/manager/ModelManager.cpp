#include"manager/ModelManager.h"
#include"constants.h"

ModelManager::ModelManager(){
    current_index=static_cast<size_t>(-1);
    AddModel("Cube","obj/cube.obj","obj/cube_diffuse.tga");
    AddModel("AfricanHead","African_Head",African_Head_Diffuse);
    /**
     *@brief AddModel
     *可能还需要再添加几个例子模型。但目前就先添加两个
    */

    switchTo(0);
}
void ModelManager::AddModel
(
    const std::string&name,
    const std::string&model_path,
    const std::string&texture_path,
    const std::string&normal_path,
    const std::string&specular_path
){
    ModelEntry entry;
    entry.name=name;
    entry.model_path=model_path;
    entry.texture_path=texture_path;
    entry.normal_path=normal_path;
    entry.specular_path=specular_path;

    entry.model=nullptr;
    entry.diffuse=nullptr;
    entry.normal=nullptr;
    entry.specular=nullptr;
    entry.is_loaded=false;

    models.push_back(entry);
    std::cout<<"添加模型"<<name<<std::endl;
}
bool ModelManager::loadModelData(size_t index){
    if(index>=models.size())return false;
    if(models[index].is_loaded)return true;

    try{
        models[index].model=new Model(models[index].model_path.c_str());
    }
    catch(std::exception& e){

        std::cout<<"加载模型失败"<<e.what()<<std::endl;
        return false;
    }
    models[index].diffuse=new TGAImage();
    if(!models[index].diffuse->read_tga_file(models[index].texture_path.c_str())){
        std::cout<<"加载纹理失败"<<std::endl;
        delete models[index].model;
        delete models[index].diffuse;
        models[index].model=nullptr;
        models[index].diffuse=nullptr;
        return false;
    }

    if(!models[index].normal_path.empty()){
        models[index].normal=new TGAImage();
        if (!models[index].normal->read_tga_file(models[index].normal_path.c_str())) {
            std::cout << "法线贴图加载失败，继续..." << std::endl;
            delete models[index].normal;
            models[index].normal = nullptr;
        }
        
    }
    else{
        models[index].normal=nullptr;
    }
    if(!models[index].specular_path.empty()){
        models[index].specular=new TGAImage();
        if (!models[index].specular->read_tga_file(models[index].specular_path.c_str())) {
            std::cout << "高光贴图加载失败，继续..." << std::endl;
            delete models[index].specular;
            models[index].specular = nullptr;
        }
    }
    else{
        models[index].specular=nullptr;
    }

    vec3 bounding_min(FLT_MAX,FLT_MAX,FLT_MAX);
    vec3 bounding_max(-FLT_MAX,-FLT_MAX,-FLT_MAX);
    for(int i=0;i<models[index].model->nverts();i++){
        vec3 v=models[index].model->vert(i);
        bounding_min.x=std::min(bounding_min.x,v.x);
        bounding_min.y=std::min(bounding_min.y,v.y);
        bounding_min.z=std::min(bounding_min.z,v.z);

        bounding_max.x=std::max(bounding_max.x,v.x);
        bounding_max.y=std::max(bounding_max.y,v.y);
        bounding_max.z=std::max(bounding_max.z,v.z);
    }
    models[index].center=(bounding_min+bounding_max)*0.5f;
    models[index].radius=(bounding_max-bounding_min).length()*0.5f;
    if(isModelComplete(index)){
        return true;
    }
    else{
        return false;
    }
    
}
bool ModelManager::isModelComplete(size_t index)const{
    if(index>=models.size()){
        return false;
    }
    const auto&model=models[index];
    if(!model.model)return false;
    if(!model.diffuse)return false;
    if(model.model_path.empty())return false;
    if(model.texture_path.empty())return false;
    if(model.model->nverts()<=0)return false;
    if(model.radius<=0.0f)return false;
    if(std::isnan(model.center.x) ||std::isnan(model.center.y) ||std::isnan(model.center.z)) {
        return false;
    }
    return true;
}
bool ModelManager::switchTo(size_t index){
    if(!loadModelData(index)){
        return false;
    }
    current_index=index;
    std::cout<<"切换模型到:"<<models[index].name<<std::endl;
    return true;
}

ModelManager::~ModelManager(){
    for(auto&entry:models){
        if(entry.is_loaded){
            if(entry.model){
                delete entry.model;
                entry.model=nullptr;
            }
        }
        if(entry.diffuse){
            delete entry.diffuse;
            entry.diffuse=nullptr;
        }
    }
}
TGAImage* ModelManager::getCurrentTexture() {
    if (current_index >= models.size())
        return nullptr;
    return models[current_index].diffuse;
}

size_t ModelManager::getModelCount() const {
    return models.size();
}

const std::string& ModelManager::getModelName(size_t index) const {
    try{
    if(isModelComplete(index))return models[index].name;
    }
    catch(std::exception& e){
        std::cout<<"获取模型名称失败"<<e.what()<<std::endl;

    }
    static const std::string empty_string="";
    return empty_string;
}

size_t ModelManager::getCurrentIndex() const {
    return current_index;
}

bool ModelManager::hasModel() const {
    return current_index < models.size() && 
           models[current_index].is_loaded;
}