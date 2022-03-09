#pragma once
#include "GraphicCommonHeader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
GRAPHIC_NAMESPACE_BEGIN

class Model
{
public:
    Model(const std::string &path);
    Model(std::vector<Mesh>&& meshes);
	Model(const std::vector<Mesh>& meshes);
private:
    std::vector<Mesh> m_meshes;
    bool m_gammaCorrection;
    std::string m_id;

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

GRAPHIC_NAMESPACE_END