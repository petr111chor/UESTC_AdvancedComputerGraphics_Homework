#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// ���崰�ڵĿ�Ⱥ͸߶�
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
// ������ת�Ƕȱ��������ڿ�����������ת
float angle = 0.0f;

// �����嶥�����ݣ�����ÿ�����6�����㣬�ܼ�36������
float vertices[] = {
    // ����
    -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,   1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    // ǰ��
    -1.0f, -1.0f,  1.0f,   1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,   1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,
    // ����
    -1.0f,  1.0f,  1.0f,  -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
    // ����
     1.0f,  1.0f,  1.0f,   1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,   1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,
     // ����
     -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,
     -1.0f, -1.0f, -1.0f,   1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
     // ����
     -1.0f,  1.0f, -1.0f,   1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
     -1.0f,  1.0f, -1.0f,   1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f
};

// ������ÿ�����Ӧ�������ļ�·��
const char* faces[6] = {
    "texture/right.jpg",
    "texture/left.jpg",
    "texture/top.jpg",
    "texture/bottom.jpg",
    "texture/back.jpg",
    "texture/front.jpg"
};

// �༭������ɫ��
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 rotation;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"out vec3 TexCoords;\n"
"void main()\n"
"{\n"
    // Ӧ����ת����ʵ����������ת
"   gl_Position = projection * view * rotation * vec4(aPos, 1.0);\n"  
"   TexCoords = aPos;\n"
"}\0";

// �༭ƬԪ��ɫ��
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 TexCoords;\n"
"out vec4 FragColor;\n"
"uniform samplerCube skybox;\n"
"void main()\n"
"{\n"
"   FragColor = texture(skybox, TexCoords);\n"
"}\n\0";

// ȫ�ֱ���������ɫ�����򡢶���������󡢶��㻺��������������ͼ����
GLuint shaderProgram, VAO, VBO, cubemapTexture;

// ���������벢������ɫ������
GLuint loadShaders(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // ����������ɫ������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // ���޸Ĺ��Ķ�����ɫ�����ӵ�������ɫ������
    glCompileShader(vertexShader); // ���붥����ɫ��

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // ����ƬԪ��ɫ������
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); // ���޸Ĺ���ƬԪ��ɫ�����ӵ�Ƭ����ɫ������
    glCompileShader(fragmentShader); // ����ƬԪ��ɫ��

    GLuint shaderProgram = glCreateProgram(); // ������ɫ���������
    glAttachShader(shaderProgram, vertexShader); // ���ѱ���Ķ�����ɫ����ƬԪ��ɫ�����ӵ����������
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // ������ɫ������

    // ɾ����ɫ������
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// ������������ͼ����
GLuint loadCubemapTexture(const char* faces[6]) {
    GLuint textureID;
    glGenTextures(1, &textureID); // �����������
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID); // ������������

    int width, height, nrChannels;
    // ���μ���ÿ���������
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
            printf("�������������ʧ�ܣ�ʧ��·���� %s\n", faces[i]);
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // ����������˲���
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // ���������������
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    return textureID;
}

// ��ʼ��OpenGL״̬�Ͷ���
void init() {
    glewInit();
    // ���ز�������ɫ��
    shaderProgram = loadShaders(vertexShaderSource, fragmentShaderSource);

    // ���ɲ�����VAO��VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ������������ͼ
    cubemapTexture = loadCubemapTexture(faces);
    // ������ɫ���е�����Ԫλ��
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);

    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    angle += 0.0001f;
    // ��ת����
    float rotationMatrix[] = {
        cos(angle), 0, sin(angle), 0,
        0, 1, 0, 0,
        -sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1
    };
    // ����ת���󴫸���ɫ��
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "rotation"), 1, GL_FALSE, rotationMatrix);

    // ��ͼ����
    float viewMatrix[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,  
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    // ����ͼ���󴫸���ɫ��
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, viewMatrix);

    // ͶӰ����
    float projectionMatrix[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, -1, -2,
        0, 0, -1, 0
    };
    // ��ͶӰ���󴫸���ɫ��
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projectionMatrix);

    // ��������Ԫ������������ͼ
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    // ����������
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glutSwapBuffers();
}

// ���лص�����
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
