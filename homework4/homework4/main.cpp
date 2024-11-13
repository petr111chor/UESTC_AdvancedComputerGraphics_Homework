#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// 定义窗口的宽度和高度
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
// 定义旋转角度变量，用于控制立方体旋转
float angle = 0.0f;

// 立方体顶点数据，包含每个面的6个顶点，总计36个顶点
float vertices[] = {
    // 后面
    -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    // 前面
    -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
    // 左面
    -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
    // 右面
     1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,   1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,
     // 底面
     -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,
     -1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
     // 顶面
     -1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
     -1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f
};

// 立方体每个面对应的纹理文件路径
const char* faces[6] = {
    "texture/right.jpg",
    "texture/left.jpg",
    "texture/top.jpg",
    "texture/bottom.jpg",
    "texture/back.jpg",
    "texture/front.jpg"
};

// 编辑顶点着色器
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 rotation;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"out vec3 TexCoords;\n"
"void main()\n"
"{\n"
    // 应用旋转矩阵，实现立方体旋转
"   gl_Position = projection * view * rotation * vec4(aPos, 1.0);\n"  
"   TexCoords = aPos;\n"
"}\0";

// 编辑片元着色器
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 TexCoords;\n"
"out vec4 FragColor;\n"
"uniform samplerCube skybox;\n"
"void main()\n"
"{\n"
"   FragColor = texture(skybox, TexCoords);\n"
"}\n\0";

// 全局变量定义着色器程序、顶点数组对象、顶点缓冲对象和立方体贴图纹理
GLuint shaderProgram, VAO, VBO, cubemapTexture;

// 创建、编译并链接着色器程序
GLuint loadShaders(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建顶点着色器对象
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 将修改过的顶点着色器附加到顶点着色器对象
    glCompileShader(vertexShader); // 编译顶点着色器

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // 创建片元着色器对象
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // 将修改过的片元着色器附加到片段着色器对象
    glCompileShader(fragmentShader); // 编译片元着色器

    GLuint shaderProgram = glCreateProgram(); // 创建着色器程序对象
    glAttachShader(shaderProgram, vertexShader); // 将已编译的顶点着色器和片元着色器附加到程序对象中
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // 链接着色器程序

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// 加载立方体贴图纹理
GLuint loadCubemapTexture(const char* faces[6]) {
    GLuint textureID;
    glGenTextures(1, &textureID); // 创建纹理对象
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); // 绑定立方体纹理

    int width, height, nrChannels;
    // 依次加载每个面的纹理
    for (unsigned int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else {
            printf("立方体纹理加载失败！失败路径： %s\n", faces[i]);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 设置纹理过滤参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // 设置纹理包裹参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

// 初始化OpenGL状态和对象
void init() {
    glewInit();
    // 加载并编译着色器
    shaderProgram = loadShaders(vertexShaderSource, fragmentShaderSource);

    // 生成并配置VAO和VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 加载立方体贴图
    cubemapTexture = loadCubemapTexture(faces);
    // 设置着色器中的纹理单元位置
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    angle += 0.0001f;
    // 旋转矩阵
    float rotationMatrix[] = {
        cos(angle), 0, sin(angle), 0,
        0, 1, 0, 0,
        -sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1
    };
    // 将旋转矩阵传给着色器
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotation"), 1, GL_FALSE, rotationMatrix);

    // 视图矩阵
    float viewMatrix[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,  
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    // 将视图矩阵传给着色器
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, viewMatrix);

    // 投影矩阵
    float projectionMatrix[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -1, -2,
        0, 0, -1, 0
    };
    // 将投影矩阵传给着色器
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projectionMatrix);

    // 激活纹理单元并绑定立方体贴图
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    // 绘制立方体
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glutSwapBuffers();
}

// 空闲回调函数
void idle() {
    glutPostRedisplay();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("homework4 rotate cube");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
