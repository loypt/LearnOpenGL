//
//  IndexedDraw.cpp
//  OGLDEV
//
//  Created by Loyio Hex on 3/5/23.
//

#define STB_IMAGE_IMPLEMENTATION

#include "Common.h"
#include "Shader.h"

#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unistd.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 屏幕信息
constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

GLFWwindow* window;

unsigned int VAO;
unsigned int VBO;
unsigned int IBO;

unsigned int gWorldLocation;

static void RenderSceneCB()
{
    processInput(window);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    static float Scale = 0.0f;
    
    glm::mat4 World = glm::mat4(1.0f);
    World[0] = glm::vec4{cosf(Scale), -sinf(Scale), 0.0f, 0.0f};
    World[1] = glm::vec4{sinf(Scale), cosf(Scale), 0.0f, 0.0f};
    World[2] = glm::vec4{0.0f, 0.0f, 1.0f, 0.0f};
    World[3] = glm::vec4{0.0f, 0.0f, 0.0f, 1.0f};
    
    //ourShader.use();
    glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, glm::value_ptr(World));
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    
    glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);
    
    
    glfwSwapBuffers(window);
    glfwPollEvents();
}


struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    
    Vertex() {}
    
    Vertex(float x, float y)
    {
        pos = glm::vec3(x, y, 0.0f);
        
        float red = (float)rand() / (float)RAND_MAX;
        float green = (float)rand() / (float)RAND_MAX;
        float blue = (float)rand() / (float)RAND_MAX;
        color = glm::vec3(red, green, blue);
    }
};

static void CreateVertexBuffer()
{
    Vertex Vertices[19];
    
    // Center
    Vertices[0] = Vertex(0.0f, 0.0);

    // Top row
    Vertices[1] = Vertex(-1.0f,  1.0f);
    Vertices[2] = Vertex(-0.75f, 1.0f);
    Vertices[3] = Vertex(-0.50f, 1.0f);
    Vertices[4] = Vertex(-0.25f, 1.0f);
    Vertices[5] = Vertex(-0.0f,  1.0f);
    Vertices[6] = Vertex(0.25f,  1.0f);
    Vertices[7] = Vertex(0.50f,  1.0f);
    Vertices[8] = Vertex(0.75f,  1.0f);
    Vertices[9] = Vertex(1.0f,   1.0f);

    // Bottom row
    Vertices[10] = Vertex(-1.0f,  -1.0f);
    Vertices[11] = Vertex(-0.75f, -1.0f);
    Vertices[12] = Vertex(-0.50f, -1.0f);
    Vertices[13] = Vertex(-0.25f, -1.0f);
    Vertices[14] = Vertex(-0.0f,  -1.0f);
    Vertices[15] = Vertex(0.25f,  -1.0f);
    Vertices[16] = Vertex(0.50f,  -1.0f);
    Vertices[17] = Vertex(0.75f,  -1.0f);
    Vertices[18] = Vertex(1.0f,   -1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
}

static void CreateIndexBuffer()
{
    unsigned int Indices[] = {
        // Top triangles
        0, 2, 1,
        0, 3, 2,
        0, 4, 3,
        0, 5, 4,
        0, 6, 5,
        0, 7, 6,
        0, 8, 7,
        0, 9, 8,

        // Bottom triangles
        0, 10, 11,
        0, 11, 12,
        0, 12, 13,
        0, 13, 14,
        0, 14, 15,
        0, 15, 16,
        0, 16, 17,
        0, 17, 18,

        // Left triangle
        0, 1, 10,

        // Right triangle
        0, 18, 9
    };


    glGenBuffers(1, &IBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}


int main()
{
    Initialize();
    
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL - Loyio", NULL, NULL);
    
#ifdef __APPLE__
    glfwMakeContextCurrent(window);
#endif
    
    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // 注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // 用线框模式绘制三角形
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    srand(getpid());
    
    CreateVertexBuffer();
    CreateIndexBuffer();
    
    
    // 导入编译Shader
    Shader ourShader("OGLDEV/IndexedDraw/shader.vs", "OGLDEV/IndexedDraw/shader.fs");
    
    gWorldLocation = glGetUniformLocation(ourShader.ID, "gWorld");
    
    ourShader.use();
    
    
    
    while(!glfwWindowShouldClose(window))
    {
        RenderSceneCB();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
