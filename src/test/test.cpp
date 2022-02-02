#include "test.h"
#include "resource/resourceManager.h"
#include "resource/resourceBasic.h"
#include "resource/types/TextureResource.h"
#include "tools/ioUtil.h"
#include "pipline/Shader.h"
#include "pipline/Buffer.h"
#include "pipline/basicTypes.h"

void test::drawTriangle(Window* window)
{
    float vertex[] = 
    {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    unsigned int indices[] = 
    {
        0,1,2,
        1,2,3,
        0,2,3,
        0,1,3
    };
    Pipline::Shader shader = Pipline::Shader("shader1.vs", "shader2.fs");

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*index：第几个属性，从0开始取，0，1，2，顺序自己定义，例如顶点位置，纹理，法线
    这里只有顶点位置，也只能讨论顶点位置，所以为0
    size：一个顶点所有数据的个数，这里每个顶点又两个浮点数属性值，所以是2
    type：顶点描述数据的类型，这里position数组中的数据全部为float，所以是GL_FLOAT
    normalized：是否需要显卡帮忙把数据归一化到-1到+1区间，这里不需要，所以设置GL_FALSE
    stride：一个顶点占有的总的字节数，这里为两个float，所以是sizeof(float)*2
    pointer：当前指针指向的vertex内部的偏离字节数，可以唯一的标识顶点某个属性的偏移量
    */
    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //颜色属性
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
    window->AddPreUpdateCallback([shader, vao, indices]()
    {
        shader.use();
        static float colorValue = 0.0f;
        float timeValue = glfwGetTime();
        shader.setFloat("xOffSet", (colorValue + sin(timeValue) / 2.0f + 0.5f)/5.f);
        
        colorValue = sin(timeValue) / 2.0f + 0.5f;
        shader.setVec4("myColor", 0.0f,colorValue, 0.0f, 1.0f);
    
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    });
}

void test::drawTextureWithBuff(Window* window)
{
    std::vector<Vector3> pos = 
    {
        {-0.5f, -0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f}
    };

    std::vector<Color> color = 
    {
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f},
        {0.5f, 0.5f, 0.5f}
    };

    std::vector<Vector2> uv = 
    {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f}
    };

    std::vector<unsigned int> indices = 
    {
        0,1,2,
        1,3,2
    };

    Pipline::Buffer buffer;
    buffer.setVertexData(pos, color, uv);
    buffer.setIndices(indices);

    Pipline::Shader shader = Pipline::Shader("simpleTexture.vs", "simpleTexture.fs");

    Pipline::Texture2DInfo tex("blockes.png");
    tex.addAttrib(GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex.addAttrib(GL_TEXTURE_WRAP_T, GL_REPEAT);
    tex.addAttrib(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    tex.addAttrib(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    buffer.addTexture(tex);

    buffer.prepare();
    window->AddUpdateCallback([shader, buffer](){
        buffer.use();
        shader.use();
        buffer.draw();
    });
}

void test::drawTexture(Window* window)
{
    float vertex[] = 
    {
        // 位置              // 颜色             // uv
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,   // 左下
        -0.5f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // 左上
        0.5f,  -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,   // 右下
        0.5f,  0.5f, 0.0f, 0.5f, 0.5f, 0.5f,    1.0f, 1.0f   // 右上
    };

    unsigned int indices[] = 
    {
        0,1,2,
        1,3,2
    };
    Pipline::Shader shader = Pipline::Shader("simpleTexture.vs", "simpleTexture.fs");

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    /*index：第几个属性，从0开始取，0，1，2，顺序自己定义，例如顶点位置，纹理，法线
    这里只有顶点位置，也只能讨论顶点位置，所以为0
    size：一个顶点所有数据的个数，这里每个顶点又两个浮点数属性值，所以是2
    type：顶点描述数据的类型，这里position数组中的数据全部为float，所以是GL_FLOAT
    normalized：是否需要显卡帮忙把数据归一化到-1到+1区间，这里不需要，所以设置GL_FALSE
    stride：一个顶点占有的总的字节数，这里为两个float，所以是sizeof(float)*2
    pointer：当前指针指向的vertex内部的偏离字节数，可以唯一的标识顶点某个属性的偏移量
    */
    //位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //颜色属性
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //uv属性
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Resource::TextureRef texRef = Resource::ResourceManager::Instance()->GetResource("blockes.png",Resource::texture);
    GLint imgType = texRef->getChannels() == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, imgType, texRef->getWidth(), texRef->getHeight(), 0, imgType, GL_UNSIGNED_BYTE, texRef->getTextureData());
    glGenerateMipmap(GL_TEXTURE_2D);

    window->AddPreUpdateCallback([shader, vao, indices, texture]()
    {
        glBindTexture(GL_TEXTURE_2D,texture);

        shader.use();
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    });
}

void test::drawTwoTextureWithBuff(Window* window)
{
    Pipline::Shader ourShader("texture.vs", "texture.fs");
    Pipline::Buffer buffer;
    Pipline::Texture2DInfo tex1("blockes.png");
    Pipline::Texture2DInfo tex2("smile.jpg");

    tex1.setDefaultAttrib();
    tex2.setDefaultAttrib();

    std::vector<Vector3> pos = 
    {
        {-0.5f, -0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f}
    };

    std::vector<Color> color = 
    {
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, 0.0f},
        {0.5f, 0.5f, 0.5f}
    };

    std::vector<Vector2> uv = 
    {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f}
    };

    std::vector<unsigned int> indices = 
    {
        0,1,2,
        1,3,2
    };

    buffer.setVertexData(pos, color, uv);
    buffer.setIndices(indices);
    buffer.addTexture(tex1);
    buffer.addTexture(tex2);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    buffer.prepare();

    window->AddUpdateCallback([ourShader, buffer](){
        buffer.use();
        ourShader.use();
        ourShader.setFloat("percent", 0.2);
        buffer.draw();
    });

}

void test::drawTwoTexture(Window* window)
{
    Pipline::Shader ourShader("texture.vs", "texture.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    Resource::TextureRef texRef = Resource::ResourceManager::Instance()->GetResource("blockes.png", Resource::texture);
    const ubyte* data = texRef->getTextureData();
   if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, texRef->getFormat(), texRef->getWidth(), texRef->getHeight(), 0, texRef->getFormat(), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    Resource::TextureRef texRef2 = Resource::ResourceManager::Instance()->GetResource("smile.jpg", Resource::texture);
    data = texRef2->getTextureData();
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, texRef2->getFormat(), texRef2->getWidth(), texRef2->getHeight(), 0, texRef2->getFormat(), GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    ourShader.setInt("texture1", 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);

    window->AddPreUpdateCallback([ourShader, VAO, indices, texture1, texture2]()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // render container
        ourShader.use();
        ourShader.setFloat("percent", 0.2);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    });
}