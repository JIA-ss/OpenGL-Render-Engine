#include "Sprite.h"
#include "Vertex.h"
#include "graphic/GraphicCommonHeader.h"

GRAPHIC_NAMESPACE_USING

void Sprite::SetUpSprite()
{
    ZoneScopedN("Mesh::SetUpMesh");
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertex2D::quad2D.size() * sizeof(Vertex2D), &Vertex2D::quad2D[0], GL_DYNAMIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Vertex2D::quad2DElement.size() * sizeof(GLuint), &Vertex2D::quad2DElement[0], GL_DYNAMIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void *)offsetof(Vertex2D, texCoord));

    glBindVertexArray(0);
}

void Sprite::draw(Material* material, Shader* shader) const
{
    ZoneScopedN("Sprite::draw");
    material->UseMaterial(shader);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, Vertex2D::quad2DElement.size());
    glBindVertexArray(0);
}
