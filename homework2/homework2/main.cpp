#include <GL/glut.h>
#include <cmath>

int perspective = 1; // 投影模式，为1透视投影，为0正交投影
int isRotate = 0;     // 旋转标志，为0不旋转，为1旋转
float angle = 0.0f;      // 物体旋转角度

// 初始化OpenGL
void init() {
    // 设置背景颜色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    // 启用深度测试
    glEnable(GL_DEPTH_TEST); 
    
}

// 绘制坐标轴
void drawAxes() {
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // X轴
        glVertex3f(-5.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 0.0f);

        glColor3f(1.0f, 0.0f, 0.0f); // Y轴
        glVertex3f(0.0f, -5.0f, 0.0f);
        glVertex3f(0.0f, 5.0f, 0.0f);

        glColor3f(1.0f, 0.0f, 0.0f); // Z轴
        glVertex3f(0.0f, 0.0f, -5.0f);
        glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();
}

// 绘制立方体
void drawCube() {
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f); // 蓝色
        glTranslatef(-1.0f, 0.0f, -3.0f); 
        glutSolidCube(1.0);// 调用库函数进行立方体绘制
    glPopMatrix();

}

// 绘制球体
void drawSphere() {
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); // 红色
        glTranslatef(1.0f, 0.0f, -5.0f); 
        glutSolidSphere(1.0, 20, 20);// 调用库函数进行球体绘制
    glPopMatrix();
}

// 绘制犹他壶
void drawTeapot() {
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); // 红色
        glTranslatef(0.0f, 1.0f, -7.0f); 
        glutSolidTeapot(1.0);// 调用库函数进行犹他壶绘制
    glPopMatrix();
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 根据perspective的值切换投影模式
    if (perspective == 1) {
        gluPerspective(45.0f, 1.0f, 0.1f, 100.0f); // 透视投影
        gluLookAt(-5.0, 5.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    else {
        glOrtho(-7.5, 7.5, -7.5, 7.5, -7.5, 7.5); // 正交投影
    }

    // 旋转物体
    if (isRotate == 1) {
        angle += 0.5f;
    }
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    drawAxes(); // 绘制坐标轴
    drawCube(); // 绘制立方体
    drawSphere(); // 绘制球体
    drawTeapot(); // 绘制犹他壶

    glutSwapBuffers();
}


// 定时器函数
void timer(int value) {
    glutPostRedisplay(); // 重新绘制
    glutTimerFunc(15, timer, 0); // 每15毫秒重绘一次，实现旋转
}

// 菜单回调函数
void menu(int value) {
    if (value == 0) {
        perspective = 1; // 透视投影
    }
    else if (value == 1) {
        perspective = 0; // 正交投影
    }
}

// 鼠标点击事件
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        glutCreateMenu(menu);
        glutAddMenuEntry("Perspective", 0);
        glutAddMenuEntry("Orthu", 1);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
}

// 键盘事件处理
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'r':
        isRotate = 1; // 开始旋转
        break;
    case 's':
        isRotate = 0; // 停止旋转
        break;
    }
}

// 主函数
int main(int argc, char** argv) {
    // glut库初始化
    glutInit(&argc, argv);
    // 设定窗口显示模式
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // 指定窗口大小、位置，创建窗口
    glutInitWindowSize(800, 600);
    glutCreateWindow("投影模式切换");

    init();

    // 注册事件处理函数：注册回调函数
    glutDisplayFunc(display);

    // 注册键盘事件处理函数、鼠标点击事件处理函数
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    // 启用定时器
    timer(0);

    // 循环检查事件队列，执行相应回调函数
    glutMainLoop();
    return 0;
}