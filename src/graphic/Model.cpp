#include "Model.h"
#include "resource/resourceManager.h"
#include "resource/types/ShaderResource.h"
GRAPHIC_NAMESPACE_USING



Model::Model(std::vector<Mesh*> &&meshes)
{
    m_meshes = std::move(meshes);
}

Model::Model(const std::vector<Mesh*> &meshes)
{
    m_meshes = meshes;
}

Model::Model(const std::string &path, const std::string& shader) : m_id(path)
{
    m_shaderPath = shader;
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
		auto pos = matName.find_first_of("_");
		if (pos == matName.npos) 
            pos = matName.size();
		matName = matName.substr(0, pos);

        std::string shaderPath = m_shaderPath;
        if (shaderPath.empty())
        {
            shaderPath = m_directory + "/" + matName;
            Resource::ShaderRef vRef = Resource::ResourceManager::Instance()->GetResource((shaderPath + ".vs").c_str(), Resource::shader);
            if (vRef.isNull())
                shaderPath = "DefaultModel";
        }
		auto mat = Material::Add(m_id + "-" + matName , shaderPath, textures);
        mat->SetName(m_id + "-" + matName);
        Mesh* _mesh_ = new Mesh(mesh, mat, std::to_string(mesh->mMaterialIndex));
        m_meshes.emplace_back(_mesh_);
    }
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::vector<Mesh*>& Model::GetMeshes()
{
    return m_meshes;
}

void Model::SetActive(bool v)
{
    m_enable = v;
    for (auto& mesh : m_meshes)
    {
        mesh->SetActive(v);
    }
}

void Model::draw(Shader* shader) const
{
    for (auto& mesh : m_meshes)
    {
        mesh->draw(shader);
    }
}

Model* Model::Clone() const
{
    std::vector<Mesh*> meshes;
    for (auto& mesh : m_meshes)
    {
        meshes.push_back(mesh->Clone());
    }
    return new Model(meshes);
}

Model::~Model()
{
    for (auto& mesh : m_meshes)
    {
        delete mesh;
    }
}