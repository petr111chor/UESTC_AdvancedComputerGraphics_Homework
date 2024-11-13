#define _CRT_SECURE_NO_DEPRECATE
#include <GL/glut.h>
#include <stdio.h>  

// 光源和雾的初始状态
GLfloat fogDensity = 0.05f;
bool light0_state = true, light1_state = true;  // 光源状态

int windowWidth = 800, windowHeight = 600;  // 窗口宽度和高度


// 初始化光照
void initLighting() {
    glEnable(GL_LIGHTING);  // 启用光照
    glEnable(GL_LIGHT0);  // 启用光源0
    glEnable(GL_LIGHT1);  // 启用光源1

    GLfloat light0_position[] = { -5.0f, 0.0f, 0.0f, 0.0f };  // 光源0的位置
    GLfloat light1_position[] = { 5.0f, 5.0f, 0.0f, 0.0f };  // 光源1的位置

    GLfloat red_light[] = { 1.0f, 0.0f, 0.0f, 1.0f };    // 红色光
    GLfloat blue_light[] = { 0.0f, 0.0f, 1.0f, 1.0f };    // 蓝色光

    // 设置光源0
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, red_light);

    // 设置光源1
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, blue_light);
}

// 初始化雾气
void initFog() {
    glEnable(GL_FOG);  // 启用雾气效果
    GLfloat fogColor[] = { 0.5, 0.5, 0.5, 1.0 };  // 雾的颜色
    glFogfv(GL_FOG_COLOR, fogColor);  // 设置雾的颜色
    glFogi(GL_FOG_MODE, GL_EXP);  // 雾的模式（指数雾）
    glFogf(GL_FOG_DENSITY, fogDensity);  // 雾的初始密度
}

void renderText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // 清除颜色和深度缓冲
    glLoadIdentity();

    // 放置相机
    gluLookAt(5.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 更新光源状态
    if (light0_state) {
        glEnable(GL_LIGHT0);
    }
    else {
        glDisable(GL_LIGHT0);
    }

    if (light1_state) {
        glEnable(GL_LIGHT1);
    }
    else {
        glDisable(GL_LIGHT1);
    }

    // 绘制茶壶
    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, 0.0f);
    glutSolidTeapot(1.0);  // 茶壶
    glPopMatrix();

    // 绘制球体
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, -3.0f);
    glutSolidSphere(1.0, 20, 20);  // 球体
    glPopMatrix();

    // 绘制立方体 
    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 2.0f);
    glutSolidCube(1.0);  // 立方体
    glPopMatrix();

    // 禁用光照
    glDisable(GL_LIGHTING);
    // 设置正交投影，用于显示2D文字
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);  // 视口大小
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 设置文字颜色为白色
    glColor3f(1.0f, 1.0f, 1.0f);

    // 显示光源状态和雾气浓度
    char info[50];
    sprintf(info, "Light 0 (red): %s", light0_state ? "On" : "Off");
    renderText(windowWidth - 150, 50, info);  // 在右下角显示
    sprintf(info, "Light 1 (blue): %s", light1_state ? "On" : "Off");
    renderText(windowWidth - 150, 30, info);
    sprintf(info, "Fog Density: %.2f", fogDensity);
    renderText(windowWidth - 150, 70, info);

    // 恢复原始矩阵模式
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // 重新启用光照
    glEnable(GL_LIGHTING);

    glutSwapBuffers();  // 交换前后缓冲区，显示绘制内容
}

// 窗口大小改变回调函数
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    // 更新窗口宽度和高度
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);  // 设置视口
    glMatrixMode(GL_PROJECTION);      // 投影矩阵
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);  // 设置透视投影
    glMatrixMode(GL_MODELVIEW);       // 模型视图矩阵
}

// 键盘回调函数
void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        fogDensity += 0.01f;  // 增加雾的密度
        if (fogDensity > 1.0f) fogDensity = 1.0f;  // 密度最大为1.0
    }
    else if (key == GLUT_KEY_DOWN) {
        fogDensity -= 0.01f;  // 减少雾的密度
        if (fogDensity < 0.0f) fogDensity = 0.0f;  // 密度最小为0.0
    }
    glFogf(GL_FOG_DENSITY, fogDensity);  // 更新雾气密度
    glutPostRedisplay();  // 重新渲染场景
}

// 鼠标点击回调函数，控制光源的开关
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        light0_state = !light0_state;  // 切换光源0的开关状态
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        light1_state = !light1_state;  // 切换光源1的开关状态
    }
    glutPostRedisplay();  // 重新渲染场景
}

// 初始化OpenGL设置
void init() {
    glEnable(GL_DEPTH_TEST);  // 启用深度测试
    glShadeModel(GL_SMOOTH);  // 使用平滑着色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // 设置背景颜色
    initLighting();  // 初始化光照
    initFog();  // 初始化雾气
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // 使用双缓存、RGB颜色和深度缓存
    glutInitWindowSize(windowWidth, windowHeight);  // 窗口大小
    glutCreateWindow("3D场景中的灯光控制与雾气实现");  // 窗口标题

    init();  // 初始化OpenGL设置

    glutDisplayFunc(display);  // 注册显示回调函数
    glutSpecialFunc(keyboard);  // 注册键盘回调函数
    glutMouseFunc(mouse);  // 注册鼠标回调函数
    glutReshapeFunc(reshape);  // 注册窗口大小改变回调函数

    glutMainLoop();  // 进入主事件循环
    return 0;
}
