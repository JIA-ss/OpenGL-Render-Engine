#include "Mesh.h"
#include "Triangle.h"
#include "system/ResourceSystem.h"
GRAPHIC_NAMESPACE_USING
GRAPHIC_IMPLEMENT(Mesh)
Mesh::Mesh(const std::vector<GLuint> &indices, const std::vector<Vertex> &vertices, Material *mat, const std::string &name) : m_indices(indices), m_vertices(vertices), m_name(name)
{
    EBO = 0;
    VAO = 0;
    VBO = 0;
    m_material = mat;
    SetUpMesh();
}

Mesh::Mesh(const VertexStream& vertex, const std::string& shader, const std::vector<std::string>& texture, TextureType type, const std::string& name) : m_name(name)
{
    m_indices = vertex.indices;
    m_vertices = vertex.vertices;
    EBO = 0;
    VAO = 0;
    VBO = 0;
    m_material = ResourceSystem::LoadGraphicResource<Material>(m_name + "- Material", shader, texture, type).GetGraphic();
    SetUpMesh();
}

Mesh::Mesh(const VertexStream& vertex, const std::string& shader, const std::vector<Texture*>& textures, const std::string& name) : m_name(name)
{
    m_indices = vertex.indices;
    m_vertices = vertex.vertices;
    EBO = 0;
    VAO = 0;
    VBO = 0;
    m_material = ResourceSystem::LoadGraphicResource<Material>(m_name + "- Material", shader, textures).GetGraphic();
    SetUpMesh();
}



Mesh::Mesh(aiMesh *mesh, Material *m, const std::string &name) : m_material(m), m_name(name)
{
    EBO = 0;
    VAO = 0;
    VBO = 0;
    m_material = m;
    loadFromAssimp(mesh);
    SetUpMesh();
}

void RecordTriangles(const std::vector<Vertex>& vertex, const std::vector<GLuint>& indices)
{
    for(int i = 0; i < indices.size(); i+=3)
    {
        Triangle::collections.emplace_back(
            Triangle(
                vertex[indices[i]].position, 
                vertex[indices[i+1]].position, 
                vertex[indices[i+2]].position
                )
            );
    }

    if (indices.empty())
    {
        for (int i = 0; i <vertex.size(); i+=3)
        {
                    Triangle::collections.emplace_back(
            Triangle(
                vertex[i].position, 
                vertex[i+1].position, 
                vertex[i+2].position
                )
            );
        }
    }
}

void Mesh::SetUpMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

    if (!m_indices.empty())
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_DYNAMIC_DRAW);
    }
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

void Mesh::Free()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (!m_indices.empty())
        glDeleteBuffers(1, &EBO);
}

void Mesh::loadFromAssimp(aiMesh *mesh)
{
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vec3;
        vec3.x = mesh->mVertices[i].x;
        vec3.y = mesh->mVertices[i].y;
        vec3.z = mesh->mVertices[i].z;
        vertex.position = vec3;

        vec3.x = mesh->mNormals[i].x;
        vec3.y = mesh->mNormals[i].y;
        vec3.z = mesh->mNormals[i].z;
        vertex.normal = vec3;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec2;
            vec2.x = mesh->mTextureCoords[0][i].x;
            vec2.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vec2;
        }
        else
        {
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
        }

        if (mesh->mTangents)
        {
            // tangent
            vec3.x = mesh->mTangents[i].x;
            vec3.y = mesh->mTangents[i].y;
            vec3.z = mesh->mTangents[i].z;
            vertex.tangent = vec3;
        }

        if (mesh->mBitangents)
        {
            // bitangent
            vec3.x = mesh->mBitangents[i].x;
            vec3.y = mesh->mBitangents[i].y;
            vec3.z = mesh->mBitangents[i].z;
            vertex.bitangent = vec3;
        }

        m_vertices.push_back(vertex);
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            m_indices.push_back(face.mIndices[j]);
        }
    }
}

void Mesh::SetMaterial(Material *material)
{
    m_material = material;
}

void Mesh::SetMaterial(const std::string &name)
{
    SetMaterial(Material::Get(name));
}

void Mesh::SetVertices(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (!m_indices.empty())
        glDeleteBuffers(1, &EBO);
    m_indices = std::move(indices);
    m_vertices = std::move(vertices);
    SetUpMesh();
}

void Mesh::SetRenderingIndex(const unsigned int& newIndex)
{
    m_renderIndex = newIndex;
}

void Mesh::draw(Shader* shader) const
{
    m_material->UseMaterial(shader);
    glBindVertexArray(VAO);
    if (m_indices.empty())
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    else
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::draw(Material* material, Shader* shader) const
{
    material->UseMaterial(shader);
    glBindVertexArray(VAO);
    if (m_indices.empty())
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    else
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh* Mesh::Clone() const
{
    auto meshRes = ResourceSystem::LoadGraphicResource<Mesh>(m_name + "_copy");
    Mesh* mesh = meshRes.GetGraphic();
    mesh->m_name = meshRes.GetName();
    mesh->VAO = VAO;
    mesh->VBO = VBO;
    mesh->EBO = EBO;
    
    mesh->m_indices = m_indices;
    mesh->m_vertices = m_vertices;

    mesh->m_name = m_name;
    mesh->m_material = m_material->Clone();

    mesh->m_renderIndex = m_renderIndex;
    mesh->m_enable = m_enable;
    return mesh;
}