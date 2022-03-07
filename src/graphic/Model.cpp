#include "Model.h"

GRAPHIC_NAMESPACE_USING

Model::Model(std::vector<Mesh> &&meshes)
{
    m_meshes = std::move(meshes);
}

Model::Model(const std::vector<Mesh> &meshes)
{
    m_meshes = meshes;
}

Model::Model(const std::string &path) : m_id(path)
{
    loadModel(path);
}

Model *Model::loadModel(const std::string &path)
{
    Assimp::Importer importer;
    m_id = path;
    std::string _path_ = std::string(_RESOURCE_PATH_) + "/models/" + m_id;
    const aiScene *scene = importer.ReadFile(_path_, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    processNode(scene->mRootNode, scene);

    return this;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	auto m_directory = m_id.substr(0, m_id.find_last_of('/'));

    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        
        //process textures from aiMaterial
        std::vector<Texture *> textures;
        aiString name;
        
        for (size_t aiType = aiTextureType_DIFFUSE; aiType < aiTextureType_UNKNOWN; aiType++)
        {
            int maxTextureCount = material->GetTextureCount((aiTextureType)aiType);
            for (size_t i = 0; i < maxTextureCount; i++)
            {
                material->GetTexture((aiTextureType)aiType, i, &name);
                auto path = m_directory + "/" + name.C_Str();
                Texture *texture = Texture::Add(path, path, (TextureType)aiType);
                textures.push_back(texture);
            }
        }

		std::string matName = name.C_Str();
		auto pos = matName.find_last_of("_");
		if (pos == matName.npos)
			pos = matName.find_last_of("-");
		if (pos == matName.npos)
			pos = matName.find_last_of(" ");
		if (pos == matName.npos) pos = matName.size();
		matName = matName.substr(0, pos);

		auto mat = Material::Add(m_id + "-" + matName , "Model", textures);
        mat->SetName(m_id + "-" + matName);

        m_meshes.emplace_back(mesh, mat, std::to_string(mesh->mMaterialIndex));
    }
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}