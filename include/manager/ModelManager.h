#pragma once
#include "model.h"
#include "tgaimage.h"
#include<string>
#include<cmath>
#include<vector>

class ModelManager
{
    private:
    struct ModelEntry
    {
        std::string name;
        std::string model_path;
        std::string texture_path;
        std::string normal_path;
        std::string specular_path;
        
        Model* model;
        TGAImage* diffuse;
        TGAImage* normal;
        TGAImage* specular;
        TGAImage* NormalMap;
        
        bool is_loaded;

        int vertex_count;
        int face_count;
        vec3 center;
        vec3 bounding_min;
        vec3 bounding_max;
        float radius;

    };

    std::vector<ModelEntry>models;
    size_t current_index;

    bool loadModelData(size_t index);

    void unloadModelData(size_t index);


    public:
    ModelManager();
    ~ModelManager();

    void AddModel(
        const std::string&name,
        const std::string&model_path,
        const std::string&texture_path,
        const std::string&normal_path="",
        const std::string&specular_path=""
    );

    bool switchTo(size_t index);

    Model* getCurrentModel();

    TGAImage* getCurrentTexture();

    bool isModelComplete(size_t index)const;

    size_t getModelCount()const;
    const std::string& getModelName(size_t index)const;
    size_t getCurrentIndex()const;

    bool hasModel()const;

    void removeModel(size_t index);
    void removeCurrentModel();


};