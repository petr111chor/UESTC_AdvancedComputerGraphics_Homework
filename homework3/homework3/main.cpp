#define _CRT_SECURE_NO_DEPRECATE
#include <GL/glut.h>
#include <stdio.h>  

// ��Դ����ĳ�ʼ״̬
GLfloat fogDensity = 0.05f;
bool light0_state = true, light1_state = true;  // ��Դ״̬

int windowWidth = 800, windowHeight = 600;  // ���ڿ�Ⱥ͸߶�


// ��ʼ������
void initLighting() {
    glEnable(GL_LIGHTING);  // ���ù���
    glEnable(GL_LIGHT0);  // ���ù�Դ0
    glEnable(GL_LIGHT1);  // ���ù�Դ1

    GLfloat light0_position[] = { -5.0f, 0.0f, 0.0f, 0.0f };  // ��Դ0��λ��
    GLfloat light1_position[] = { 5.0f, 5.0f, 0.0f, 0.0f };  // ��Դ1��λ��

    GLfloat red_light[] = { 1.0f, 0.0f, 0.0f, 1.0f };    // ��ɫ��
    GLfloat blue_light[] = { 0.0f, 0.0f, 1.0f, 1.0f };    // ��ɫ��

    // ���ù�Դ0
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, red_light);

    // ���ù�Դ1
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, blue_light);
}

// ��ʼ������
void initFog() {
    glEnable(GL_FOG);  // ��������Ч��
    GLfloat fogColor[] = { 0.5, 0.5, 0.5, 1.0 };  // �����ɫ
    glFogfv(GL_FOG_COLOR, fogColor);  // ���������ɫ
    glFogi(GL_FOG_MODE, GL_EXP);  // ���ģʽ��ָ����
    glFogf(GL_FOG_DENSITY, fogDensity);  // ��ĳ�ʼ�ܶ�
}

void renderText(float x, float y, const char* text) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

// ��ʾ�ص�����
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // �����ɫ����Ȼ���
    glLoadIdentity();

    // �������
    gluLookAt(5.0, 3.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // ���¹�Դ״̬
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

    // ���Ʋ��
    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, 0.0f);
    glutSolidTeapot(1.0);  // ���
    glPopMatrix();

    // ��������
    glPushMatrix();
    glTranslatef(-1.0f, 0.0f, -3.0f);
    glutSolidSphere(1.0, 20, 20);  // ����
    glPopMatrix();

    // ���������� 
    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 2.0f);
    glutSolidCube(1.0);  // ������
    glPopMatrix();

    // ���ù���
    glDisable(GL_LIGHTING);
    // ��������ͶӰ��������ʾ2D����
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);  // �ӿڴ�С
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // ����������ɫΪ��ɫ
    glColor3f(1.0f, 1.0f, 1.0f);

    // ��ʾ��Դ״̬������Ũ��
    char info[50];
    sprintf(info, "Light 0 (red): %s", light0_state ? "On" : "Off");
    renderText(windowWidth - 150, 50, info);  // �����½���ʾ
    sprintf(info, "Light 1 (blue): %s", light1_state ? "On" : "Off");
    renderText(windowWidth - 150, 30, info);
    sprintf(info, "Fog Density: %.2f", fogDensity);
    renderText(windowWidth - 150, 70, info);

    // �ָ�ԭʼ����ģʽ
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // �������ù���
    glEnable(GL_LIGHTING);

    glutSwapBuffers();  // ����ǰ�󻺳�������ʾ��������
}

// ���ڴ�С�ı�ص�����
void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    // ���´��ڿ�Ⱥ͸߶�
    windowWidth = width;
    windowHeight = height;

    glViewport(0, 0, width, height);  // �����ӿ�
    glMatrixMode(GL_PROJECTION);      // ͶӰ����
    glLoadIdentity();
    gluPerspective(45.0, aspect, 1.0, 100.0);  // ����͸��ͶӰ
    glMatrixMode(GL_MODELVIEW);       // ģ����ͼ����
}

// ���̻ص�����
void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_UP) {
        fogDensity += 0.01f;  // ��������ܶ�
        if (fogDensity > 1.0f) fogDensity = 1.0f;  // �ܶ����Ϊ1.0
    }
    else if (key == GLUT_KEY_DOWN) {
        fogDensity -= 0.01f;  // ��������ܶ�
        if (fogDensity < 0.0f) fogDensity = 0.0f;  // �ܶ���СΪ0.0
    }
    glFogf(GL_FOG_DENSITY, fogDensity);  // ���������ܶ�
    glutPostRedisplay();  // ������Ⱦ����
}

// ������ص����������ƹ�Դ�Ŀ���
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        light0_state = !light0_state;  // �л���Դ0�Ŀ���״̬
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        light1_state = !light1_state;  // �л���Դ1�Ŀ���״̬
    }
    glutPostRedisplay();  // ������Ⱦ����
}

// ��ʼ��OpenGL����
void init() {
    glEnable(GL_DEPTH_TEST);  // ������Ȳ���
    glShadeModel(GL_SMOOTH);  // ʹ��ƽ����ɫ
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // ���ñ�����ɫ
    initLighting();  // ��ʼ������
    initFog();  // ��ʼ������
}

// ������
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // ʹ��˫���桢RGB��ɫ����Ȼ���
    glutInitWindowSize(windowWidth, windowHeight);  // ���ڴ�С
    glutCreateWindow("3D�����еĵƹ����������ʵ��");  // ���ڱ���

    init();  // ��ʼ��OpenGL����

    glutDisplayFunc(display);  // ע����ʾ�ص�����
    glutSpecialFunc(keyboard);  // ע����̻ص�����
    glutMouseFunc(mouse);  // ע�����ص�����
    glutReshapeFunc(reshape);  // ע�ᴰ�ڴ�С�ı�ص�����

    glutMainLoop();  // �������¼�ѭ��
    return 0;
}
