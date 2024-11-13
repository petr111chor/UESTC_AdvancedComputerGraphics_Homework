#include <GL/glut.h>
#include <cmath>

int perspective = 1; // ͶӰģʽ��Ϊ1͸��ͶӰ��Ϊ0����ͶӰ
int isRotate = 0;     // ��ת��־��Ϊ0����ת��Ϊ1��ת
float angle = 0.0f;      // ������ת�Ƕ�

// ��ʼ��OpenGL
void init() {
    // ���ñ�����ɫ
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    // ������Ȳ���
    glEnable(GL_DEPTH_TEST); 
    
}

// ����������
void drawAxes() {
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); // X��
        glVertex3f(-5.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 0.0f);

        glColor3f(1.0f, 0.0f, 0.0f); // Y��
        glVertex3f(0.0f, -5.0f, 0.0f);
        glVertex3f(0.0f, 5.0f, 0.0f);

        glColor3f(1.0f, 0.0f, 0.0f); // Z��
        glVertex3f(0.0f, 0.0f, -5.0f);
        glVertex3f(0.0f, 0.0f, 5.0f);
    glEnd();
}

// ����������
void drawCube() {
    glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ
        glTranslatef(-1.0f, 0.0f, -3.0f); 
        glutSolidCube(1.0);// ���ÿ⺯���������������
    glPopMatrix();

}

// ��������
void drawSphere() {
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
        glTranslatef(1.0f, 0.0f, -5.0f); 
        glutSolidSphere(1.0, 20, 20);// ���ÿ⺯�������������
    glPopMatrix();
}

// ����������
void drawTeapot() {
    glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
        glTranslatef(0.0f, 1.0f, -7.0f); 
        glutSolidTeapot(1.0);// ���ÿ⺯����������������
    glPopMatrix();
}

// ��ʾ�ص�����
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ����perspective��ֵ�л�ͶӰģʽ
    if (perspective == 1) {
        gluPerspective(45.0f, 1.0f, 0.1f, 100.0f); // ͸��ͶӰ
        gluLookAt(-5.0, 5.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    else {
        glOrtho(-7.5, 7.5, -7.5, 7.5, -7.5, 7.5); // ����ͶӰ
    }

    // ��ת����
    if (isRotate == 1) {
        angle += 0.5f;
    }
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    drawAxes(); // ����������
    drawCube(); // ����������
    drawSphere(); // ��������
    drawTeapot(); // ����������

    glutSwapBuffers();
}


// ��ʱ������
void timer(int value) {
    glutPostRedisplay(); // ���»���
    glutTimerFunc(15, timer, 0); // ÿ15�����ػ�һ�Σ�ʵ����ת
}

// �˵��ص�����
void menu(int value) {
    if (value == 0) {
        perspective = 1; // ͸��ͶӰ
    }
    else if (value == 1) {
        perspective = 0; // ����ͶӰ
    }
}

// ������¼�
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        glutCreateMenu(menu);
        glutAddMenuEntry("Perspective", 0);
        glutAddMenuEntry("Orthu", 1);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
}

// �����¼�����
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'r':
        isRotate = 1; // ��ʼ��ת
        break;
    case 's':
        isRotate = 0; // ֹͣ��ת
        break;
    }
}

// ������
int main(int argc, char** argv) {
    // glut���ʼ��
    glutInit(&argc, argv);
    // �趨������ʾģʽ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // ָ�����ڴ�С��λ�ã���������
    glutInitWindowSize(800, 600);
    glutCreateWindow("ͶӰģʽ�л�");

    init();

    // ע���¼���������ע��ص�����
    glutDisplayFunc(display);

    // ע������¼���������������¼�������
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    // ���ö�ʱ��
    timer(0);

    // ѭ������¼����У�ִ����Ӧ�ص�����
    glutMainLoop();
    return 0;
}