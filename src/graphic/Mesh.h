#pragma once
#include "GraphicCommonHeader.h"
#include "Vertex.h"
#include "Material.h"

#include <assimp/mesh.h>
#include <vector>
GRAPHIC_NAMESPACE_BEGIN


class Mesh
{
public:
    Mesh(const std::vector<GLuint> &indices, const std::vector<Vertex> &vertices, Material *mat, const std::string &name = "");
    Mesh(aiMesh *mesh, Material *m = nullptr, const std::string &name = "");
    Mesh() = default;
    ~Mesh() { delete m_material; }

public:
    void loadFromAssimp(aiMesh *mesh);

public:
    void SetMaterial(Material *material);
    void SetMaterial(const std::string &name);
	void SetVertices(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices);

    void SetRenderingIndex(const unsigned int& newIndex);
    unsigned int GetRenderingIndex() const { return m_renderIndex; }
    bool IsActive() const { return m_enable; }
    void SetActive(bool v) { m_enable = v; }

    const GLuint& GetVAO() const;
    const std::vector<GLuint>& GetIndices() const;
    const std::vector<Vertex>& GetVertices() const;
    const Material* GetMaterial() const;

    void draw(Shader* shader = nullptr) const;
    void draw(Material* material, Shader* shader = nullptr) const;

    Mesh* Clone() const;

    template<typename T>
    void SetShaderParam(const std::string& name, const T& val);
private:
    void SetUpMesh();
private:
    std::vector<GLuint> m_indices;
    std::vector<Vertex> m_vertices;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::string m_name;

    Material* m_material;

    unsigned int m_renderIndex = 0;
    bool m_enable = true;
};

template<typename T>
void Mesh::SetShaderParam(const std::string& name, const T& val)
{
    m_material->SetShaderParam(name,val);
}

GRAPHIC_NAMESPACE_END