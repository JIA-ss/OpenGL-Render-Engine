#include "test.h"
#include "resource/resourceManager.h"
#include "resource/resourceBasic.h"
#include "resource/types/TextureResource.h"
#include "tools/ioUtil.h"
#include "pipline/Shader.h"
#include "pipline/Buffer.h"
#include "pipline/basicTypes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>


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

Pipline::Shader test::drawTwoTextureWithBuff(Window* window)
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

    window->AddPreUpdateCallback([buffer](){
        buffer.use();
    });

    window->AddUpdateCallback([ourShader](){
        ourShader.use();
        ourShader.setFloat("percent", 0.2);
        glm::mat4 transform(1.0f);
        ourShader.setMat4f("transform", glm::value_ptr(transform));
    });

    window->AddPostUpdateCallback([buffer](){
        buffer.draw();
    });
    return ourShader;
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

void test::transformTest(Window* window)
{
    Pipline::Shader shader = test::drawTwoTextureWithBuff(window);
    window->AddUpdateCallback([shader](){
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setMat4f("transform", glm::value_ptr(transform));
    });
}

void test::_3Dtest(Window* window)
{
    Shader shader("camera.vs", "camera.fs");
    Buffer buffer;
    Pipline::Texture2DInfo tex1("blockes.png");
    Pipline::Texture2DInfo tex2("smile.jpg");

    tex1.setDefaultAttrib();
    tex2.setDefaultAttrib();

    buffer.setVertexPos({
        {-0.5f, -0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f}
    });

    buffer.setTexCoord({
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f}
    });

    buffer.setIndices({
        0,1,2,
        1,3,2
    });

    buffer.addTexture(tex1);
    buffer.addTexture(tex2);

    buffer.prepare();
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    window->AddUpdateCallback([shader, buffer, window](){
        buffer.use();
        shader.use();

                // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        
        projection = glm::perspective(glm::radians(45.0f), (float)window->getWindowWidth() / (float)window->getWindowHeight(), 0.1f, 100.0f);
        
        shader.setMat4f("model", glm::value_ptr(model));
        shader.setMat4f("view", glm::value_ptr(view));
        shader.setMat4f("projection", glm::value_ptr(projection));

        buffer.draw();
    });
}

void test::_3DtestMuiltiCube(Window* window)
{
    Shader shader("3DMultiCube.vs", "3DMultiCube.fs");
    Buffer buffer;
    Pipline::Texture2DInfo tex1("blockes.png");
    Pipline::Texture2DInfo tex2("smile.jpg");

    tex1.setDefaultAttrib();
    tex2.setDefaultAttrib();

    buffer.setVertexPos({
        {-0.5f, -0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f},

        {-0.5f, -0.5f, -1.0f},
        {-0.5f, 0.5f, -1.0f},
        {0.5f, -0.5f, -1.0f},
        {0.5f, 0.5f, -1.0f},
    });

    buffer.setTexCoord({
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},

        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f}
    });

    buffer.setIndices({
        0,1,2,
        1,3,2,

        0,1,4,
        1,4,5,

        2,3,6,
        3,6,7,

        1,3,5,
        3,5,7,

        0,2,4,
        2,4,6,

        4,5,6,
        5,7,6
    });

    buffer.addTexture(tex1);
    buffer.addTexture(tex2);

    buffer.prepare();
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

            // create transformations
    glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 view          = glm::mat4(1.0f);
    glm::mat4 projection    = glm::mat4(1.0f);
    glm::mat4 transform     = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.f), (float)window->getWindowWidth() / (float)window->getWindowHeight(), 0.1f, 100.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
    //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    shader.setMat4f("model", glm::value_ptr(model));
    shader.setMat4f("view", glm::value_ptr(view));
    shader.setMat4f("projection", glm::value_ptr(projection));
    shader.setMat4f("transform", glm::value_ptr(transform));

    window->enableZTest(true);
    window->AddUpdateCallback([shader, buffer](){
        glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
        };
        buffer.use();
        shader.use();
        for (int i = 0; i < 5; i++)
        {   
            glm::mat4 transform     = glm::mat4(1.0f);
            transform = glm::translate(transform, cubePositions[i]);
            //transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
            shader.setMat4f("transform", glm::value_ptr(transform));
            buffer.draw();
        }
    });
}

void test::_3DMultiSmile(Window* window)
{
    Shader shader("3D.vs", "3D.fs");
    Buffer buffer;
    Texture2DInfo tex1("blockes.png");
    Texture2DInfo tex2("smile.jpg");
    tex1.setDefaultAttrib();
    tex2.setDefaultAttrib();
    
    buffer.addTexture(tex1);
    buffer.addTexture(tex2);

    float pos[] = {
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f  
    };

    float uv[] = 
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    buffer.setVertexPos(pos, sizeof(pos));
    buffer.setTexCoord(uv, sizeof(uv));

    buffer.prepare();
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 projection    = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)window->getWindowWidth() / (float)window->getWindowHeight(), 0.1f, 100.0f);
    view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // pass transformation matrices to the shader
    shader.setMat4f("projection", glm::value_ptr(projection)); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader.setMat4f("view", glm::value_ptr(view));


    window->enableZTest(true);

    window->AddUpdateCallback([shader, buffer, window, cubePositions](){
        buffer.use();
        shader.use();

        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        float radius = 10.0f;
        float camX = static_cast<float>(sin(glfwGetTime()) * radius);
        float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4f("view", glm::value_ptr(view));

        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4f("model", glm::value_ptr(model));
            buffer.draw();
        }
    });
}

void test::camera(Window* window)
{
    Shader shader("3D.vs", "3D.fs");
    Buffer buffer;
    Texture2DInfo tex1("blockes.png");
    Texture2DInfo tex2("smile.jpg");
    tex1.setDefaultAttrib();
    tex2.setDefaultAttrib();
    
    buffer.addTexture(tex1);
    buffer.addTexture(tex2);

    float pos[] = {
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f  
    };

    float uv[] = 
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    buffer.setVertexPos(pos, sizeof(pos));
    buffer.setTexCoord(uv, sizeof(uv));

    buffer.prepare();
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    Camera& cam = window->getCamera();
    cam.setFov(45.f);
    cam.setCameraPos(Vector3{0.0f, 0.0f, -1.0f});
    // pass transformation matrices to the shader
    shader.setMat4f("projection", glm::value_ptr(cam.getProjectionMat4())); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader.setMat4f("view", glm::value_ptr(cam.getViewMat4()));

    cam.setSensitive(0.2f);

    window->enableZTest(true);

    window->AddPreUpdateCallback([window]()
    {
        Camera& cam = window->getCamera();
        glm::vec3 cameraPos = cam.getCameraPos();
        glm::vec3 cameraFront = cam.getCameraFront();
        glm::vec3 cameraUp = DEFAULT_CAMERA_UP;
        float cameraSpeed = cam.getSensitive();
        if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_Q) == GLFW_PRESS)
            cam.addYaw(1.f);
        if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_E) == GLFW_PRESS)
            cam.addYaw(-1.f);
        if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
            cam.addPitch(1.f);
        if (glfwGetKey(window->getGLFWwindow(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            cam.addPitch(-1.f);
        
        
        glm::vec3 oriPos = cam.getCameraPos();
        if (oriPos != cameraPos)
            cam.setCameraPos(cameraPos);
    });

    window->AddUpdateCallback([shader, buffer, window, cubePositions](){
        buffer.use();
        shader.use();

        Camera& cam = window->getCamera();

        // pass transformation matrices to the shader
        shader.setMat4f("projection", glm::value_ptr(cam.getProjectionMat4())); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shader.setMat4f("view", glm::value_ptr(cam.getViewMat4()));


        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4f("model", glm::value_ptr(model));
            buffer.draw();
        }
    });
}

void test::cameraControl(Window* window)
{
    Shader shader("3D.vs", "3D.fs");
    Buffer buffer;
    Texture2DInfo tex1("blockes.png");
    Texture2DInfo tex2("smile.jpg");
    tex1.setDefaultAttrib();
    tex2.setDefaultAttrib();
    
    buffer.addTexture(tex1);
    buffer.addTexture(tex2);

    float pos[] = {
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f  
    };

    float uv[] = 
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    buffer.setVertexPos(pos, sizeof(pos));
    buffer.setTexCoord(uv, sizeof(uv));

    buffer.prepare();
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    Camera& cam = window->getCamera();
    cam.setFov(45.f);
    cam.setCameraPos(Vector3{0.0f, 0.0f, -1.0f});
    // pass transformation matrices to the shader
    shader.setMat4f("projection", glm::value_ptr(cam.getProjectionMat4())); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader.setMat4f("view", glm::value_ptr(cam.getViewMat4()));

    cam.setSensitive(0.2f);

    window->enableZTest(true);
    cam.enableControl(true);

    window->AddUpdateCallback([shader, buffer, window, cubePositions](){
        buffer.use();
        shader.use();

        Camera& cam = window->getCamera();

        // pass transformation matrices to the shader
        shader.setMat4f("projection", glm::value_ptr(cam.getProjectionMat4())); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shader.setMat4f("view", glm::value_ptr(cam.getViewMat4()));


        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4f("model", glm::value_ptr(model));
            buffer.draw();
        }
    });
}

void test::color(Window* window)
{
    float pos[] = {
        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };
    Buffer cubeBuffer, lightBuffer;
    Shader cubeShader("color.vs", "color.fs");
    Shader lightShader("color.vs", "color_light.fs");
    cubeBuffer.setVertexPos(pos, sizeof(pos));
    lightBuffer.setVertexPos(pos, sizeof(pos));
    cubeShader.use();
    cubeShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    cubeShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);

    window->enableZTest(true);
    Camera& cam = window->getCamera();
    cam.enableControl(true);
    cam.setSensitive(0.5f);
    //cam.setCameraFront(Vector3{0,0,-5});
    cam.setCameraPos(Vector3{1.2f, 1.0f, 2.f });

    lightBuffer.prepare();
    cubeBuffer.prepare();

    window->AddUpdateCallback([cubeBuffer, lightBuffer, cubeShader, lightShader, window]()
    {
        Camera& cam = window->getCamera();
        glm::vec3 lightPos(1.2f, 1.0f, -2.0f);

        //cam.setCameraFront(lightPos - cam.getCameraPos());
        static glm::vec3 camPos = glm::vec3(0,0,0);
        if (camPos != cam.getCameraPos())
        {
            camPos = cam.getCameraPos();
            glm::vec3 ft = cam.getCameraFront();
            std::cout << "camPos: " << camPos.x << '\t' << camPos.y << '\t' << camPos.z << '\t';
            std::cout << "camFront: " << ft.x << '\t' << ft.y << '\t' << ft.z << std::endl;
        }

        lightBuffer.use();
        lightShader.use();
        lightShader.setMat4f("projection", glm::value_ptr(cam.getProjectionMat4()));
        lightShader.setMat4f("view", glm::value_ptr(cam.getViewMat4()));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightShader.setMat4f("model", glm::value_ptr(model));

        lightBuffer.draw();

        cubeBuffer.use();
        cubeShader.use();

        cubeShader.setMat4f("projection", glm::value_ptr(cam.getProjectionMat4()));
        cubeShader.setMat4f("view", glm::value_ptr(cam.getViewMat4()));
        model = glm::mat4(1.0f);
        lightShader.setMat4f("model", glm::value_ptr(model));
        cubeBuffer.draw();
    });

}