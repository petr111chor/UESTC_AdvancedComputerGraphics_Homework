#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// 定义绘制模式
#define LINE 1
#define RECTANGLE 2
#define CIRCLE 3

#define PI 3.1415926535f

#define MAX_SHAPES 100  // 最大存储图形数

typedef struct {
    int shapeType;  // 记录图形类型（LINE, RECTANGLE, CIRCLE）
    int x_start, y_start, x_end, y_end;  // 起始点和终止点
} Shape;

Shape shapes[MAX_SHAPES];  // 存储绘制的图形
int shapeCount = 0;  // 当前图形数量

int drawing_mode = 0; // 当前绘制模式
int is_drawing = 0; // 是否在绘制过程中
int x_start, y_start, x_end, y_end; // 记录起始和结束的鼠标位置

// 初始化OpenGL
void init() {
    // 设置背景颜色
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // 设置填充颜色
    glColor3f(1.0, 1.0, 1.0);
    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    // 设置二维正交裁剪区域
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
}

// 将屏幕坐标转换为 OpenGL 坐标
void convertCoordinates(int x, int y, int* x_out, int* y_out) {
    *x_out = x;
    *y_out = glutGet(GLUT_WINDOW_HEIGHT) - y;
}

// 绘制直线
void drawLine() {
    glBegin(GL_LINES);
        glVertex2i(x_start, y_start);
        glVertex2i(x_end, y_end);
    glEnd();
}

// 绘制矩形
void drawRectangle() {
    glBegin(GL_LINE_LOOP);
        glVertex2i(x_start, y_start);
        glVertex2i(x_end, y_start);
        glVertex2i(x_end, y_end);
        glVertex2i(x_start, y_end);
    glEnd();
}

// 绘制圆
void drawCircle() {
    // 使用两点间距离公式计算圆的半径
    int radius = sqrt((x_end - x_start) * (x_end - x_start) + (y_end - y_start) * (y_end - y_start));
    // 使用正多边形近似圆，定义多边形的边数
    int num_segments = 200;
    // 每条线段对应的角度
    float theta;

    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < num_segments; i++) {
            // 计算每条线段的偏移量
            theta = 2.0f * PI * i / num_segments;
            float dx = radius * cosf(theta);
            float dy = radius * sinf(theta);
            glVertex2f(x_start + dx, y_start + dy);
        }
    glEnd();
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 遍历所有已保存的图形并绘制
    for (int i = 0; i < shapeCount; i++) {
        switch (shapes[i].shapeType) {
        case LINE:
            glBegin(GL_LINES);
            glVertex2i(shapes[i].x_start, shapes[i].y_start);
            glVertex2i(shapes[i].x_end, shapes[i].y_end);
            glEnd();
            break;
        case RECTANGLE:
            glBegin(GL_LINE_LOOP);
            glVertex2i(shapes[i].x_start, shapes[i].y_start);
            glVertex2i(shapes[i].x_end, shapes[i].y_start);
            glVertex2i(shapes[i].x_end, shapes[i].y_end);
            glVertex2i(shapes[i].x_start, shapes[i].y_end);
            glEnd();
            break;
        case CIRCLE:
        {
            int radius = sqrt((shapes[i].x_end - shapes[i].x_start) * (shapes[i].x_end - shapes[i].x_start) +
                (shapes[i].y_end - shapes[i].y_start) * (shapes[i].y_end - shapes[i].y_start));
            int num_segments = 100;
            float theta;
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < num_segments; j++) {
                theta = 2.0f * PI * j / num_segments;
                float dx = radius * cosf(theta);
                float dy = radius * sinf(theta);
                glVertex2f(shapes[i].x_start + dx, shapes[i].y_start + dy);
            }
            glEnd();
        }
        break;
        }
    }

    if (is_drawing) {
        switch (drawing_mode) {
        case LINE:
            drawLine();
            break;
        case RECTANGLE:
            drawRectangle();
            break;
        case CIRCLE:
            drawCircle();
            break;
        }
    }
    glFlush();
}

// 鼠标点击事件处理
void mouseEvent(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // 开始绘制
        convertCoordinates(x, y, &x_start, &y_start);
        is_drawing = 1;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // 结束绘制
        convertCoordinates(x, y, &x_end, &y_end);
        is_drawing = 0;

        // 将当前绘制的图形存储到 shapes 数组中
        if (shapeCount < MAX_SHAPES) {
            shapes[shapeCount].shapeType = drawing_mode;
            shapes[shapeCount].x_start = x_start;
            shapes[shapeCount].y_start = y_start;
            shapes[shapeCount].x_end = x_end;
            shapes[shapeCount].y_end = y_end;
            shapeCount++;
        }
        glutPostRedisplay();
    }
}

// 鼠标移动事件处理
void motionEvent(int x, int y) {
    if (is_drawing) {
        convertCoordinates(x, y, &x_end, &y_end);
        glutPostRedisplay();// 每次移动鼠标都会重绘，以实现橡皮条效果
    }
}

// 菜单回调函数
void menu(int value) {
    drawing_mode = value;
}


// 主函数
int main(int argc, char** argv) {
    // glut库初始化 
    glutInit(&argc, argv);
    // 设定窗口显示模式
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // 指定窗口大小、位置，创建窗口
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("绘图demo");

    init();

    // 构建菜单
    glutCreateMenu(menu);
    glutAddMenuEntry("Line", LINE);
    glutAddMenuEntry("Rectangle", RECTANGLE);
    glutAddMenuEntry("Circle", CIRCLE);
    // 点击鼠标右键时显示菜单
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // 注册事件处理函数：注册回调函数
    glutDisplayFunc(display);

    // 注册鼠标点击事件处理函数、鼠标移动事件处理函数
    glutMouseFunc(mouseEvent);
    glutMotionFunc(motionEvent);

    // 循环检查事件队列，执行相应回调函数
    glutMainLoop();
    return 0;
}
