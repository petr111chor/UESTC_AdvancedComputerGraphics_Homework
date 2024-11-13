#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

// �������ģʽ
#define LINE 1
#define RECTANGLE 2
#define CIRCLE 3

#define PI 3.1415926535f

#define MAX_SHAPES 100  // ���洢ͼ����

typedef struct {
    int shapeType;  // ��¼ͼ�����ͣ�LINE, RECTANGLE, CIRCLE��
    int x_start, y_start, x_end, y_end;  // ��ʼ�����ֹ��
} Shape;

Shape shapes[MAX_SHAPES];  // �洢���Ƶ�ͼ��
int shapeCount = 0;  // ��ǰͼ������

int drawing_mode = 0; // ��ǰ����ģʽ
int is_drawing = 0; // �Ƿ��ڻ��ƹ�����
int x_start, y_start, x_end, y_end; // ��¼��ʼ�ͽ��������λ��

// ��ʼ��OpenGL
void init() {
    // ���ñ�����ɫ
    glClearColor(0.0, 0.0, 0.0, 0.0);
    // ���������ɫ
    glColor3f(1.0, 1.0, 1.0);
    // ����ͶӰ����
    glMatrixMode(GL_PROJECTION);
    // ���ö�ά�����ü�����
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
}

// ����Ļ����ת��Ϊ OpenGL ����
void convertCoordinates(int x, int y, int* x_out, int* y_out) {
    *x_out = x;
    *y_out = glutGet(GLUT_WINDOW_HEIGHT) - y;
}

// ����ֱ��
void drawLine() {
    glBegin(GL_LINES);
        glVertex2i(x_start, y_start);
        glVertex2i(x_end, y_end);
    glEnd();
}

// ���ƾ���
void drawRectangle() {
    glBegin(GL_LINE_LOOP);
        glVertex2i(x_start, y_start);
        glVertex2i(x_end, y_start);
        glVertex2i(x_end, y_end);
        glVertex2i(x_start, y_end);
    glEnd();
}

// ����Բ
void drawCircle() {
    // ʹ���������빫ʽ����Բ�İ뾶
    int radius = sqrt((x_end - x_start) * (x_end - x_start) + (y_end - y_start) * (y_end - y_start));
    // ʹ��������ν���Բ���������εı���
    int num_segments = 200;
    // ÿ���߶ζ�Ӧ�ĽǶ�
    float theta;

    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < num_segments; i++) {
            // ����ÿ���߶ε�ƫ����
            theta = 2.0f * PI * i / num_segments;
            float dx = radius * cosf(theta);
            float dy = radius * sinf(theta);
            glVertex2f(x_start + dx, y_start + dy);
        }
    glEnd();
}

// ��ʾ�ص�����
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // ���������ѱ����ͼ�β�����
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

// ������¼�����
void mouseEvent(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // ��ʼ����
        convertCoordinates(x, y, &x_start, &y_start);
        is_drawing = 1;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // ��������
        convertCoordinates(x, y, &x_end, &y_end);
        is_drawing = 0;

        // ����ǰ���Ƶ�ͼ�δ洢�� shapes ������
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

// ����ƶ��¼�����
void motionEvent(int x, int y) {
    if (is_drawing) {
        convertCoordinates(x, y, &x_end, &y_end);
        glutPostRedisplay();// ÿ���ƶ���궼���ػ棬��ʵ����Ƥ��Ч��
    }
}

// �˵��ص�����
void menu(int value) {
    drawing_mode = value;
}


// ������
int main(int argc, char** argv) {
    // glut���ʼ�� 
    glutInit(&argc, argv);
    // �趨������ʾģʽ
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    // ָ�����ڴ�С��λ�ã���������
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(200, 100);
    glutCreateWindow("��ͼdemo");

    init();

    // �����˵�
    glutCreateMenu(menu);
    glutAddMenuEntry("Line", LINE);
    glutAddMenuEntry("Rectangle", RECTANGLE);
    glutAddMenuEntry("Circle", CIRCLE);
    // �������Ҽ�ʱ��ʾ�˵�
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    // ע���¼���������ע��ص�����
    glutDisplayFunc(display);

    // ע��������¼�������������ƶ��¼�������
    glutMouseFunc(mouseEvent);
    glutMotionFunc(motionEvent);

    // ѭ������¼����У�ִ����Ӧ�ص�����
    glutMainLoop();
    return 0;
}
