#pragma once
#include "GraphicCommonHeader.h"
#include "Mesh.h"
#include "GraphicBase.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
GRAPHIC_NAMESPACE_BEGIN

class Model : public GraphicBase
{
    GRAPHIC_DECLARE
public:
    Model() = default;
    ~Model();
    Model(const std::string &path, const std::string& shader = "");
    Model(std::vector<Mesh*>&& meshes);
	Model(const std::vector<Mesh*>& meshes);

    std::vector<Mesh*>& GetMeshes();
    
    void draw(Shader* shader = nullptr) const;

    Model* Clone() const;
    bool IsActive() const { return m_enable; }
    void SetActive(bool v);
    void Free() override;
    template<typename T>
    void SetShaderParam(const std::string& name, const T& val);
private:
    std::vector<Mesh*> m_meshes;
    bool m_gammaCorrection;
    std::string m_id;
    std::string m_shaderPath;
    bool m_enable = true;

    Model *loadModel(const std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
public:
    static Model* Get(const std::string& id);
    template<typename ...Args>
    static Model* Add(const std::string& id, Args&& ... args);
    static void Remove(const std::string& id);
private:
    static std::unordered_map<std::string, Model> collection;
};

template<typename T>
void Model::SetShaderParam(const std::string& name, const T& val)
{
    for (int i = 0; i < m_meshes.size(); ++i)
    {
        m_meshes[i]->SetShaderParam(name, val);
    }
}

GRAPHIC_NAMESPACE_END