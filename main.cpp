#include <GLUT/glut.h>
#include<math.h>

float angleX = 0.0f, angleY = 0.0f;  // Ângulos de rotação para movimentação da câmera
float distance = 8.0f;               // Distância da câmera ao centro da cena

void drawFish() {
    // Corpo principal
    glPushMatrix();
    glColor3f(0.2, 0.6, 0.8);  // Azul claro
    glScalef(1.8, 0.5, 0.4);   // Forma mais longa e fina
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();

    // Cauda (mais afilada)
    glPushMatrix();
    glTranslatef(-1.4, 0.0, 0.0);  // Mais para trás
    glColor3f(0.15, 0.6, 0.15);  // Verde
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.4, 0.25, 0.0);
    glVertex3f(-0.4, -0.25, 0.0);
    glVertex3f(0.2, 0.0, 0.0);
    glEnd();
    glPopMatrix();

    // Boca
    glPushMatrix();
    glTranslatef(0.9, 0.0, 0.15);  // Mais para frente
    glColor3f(0.8, 0.2, 0.0);
    glutSolidCone(0.1, 0.2, 10, 10);
    glPopMatrix();

    // Nadadeira superior (maior)
    glPushMatrix();
    glTranslatef(0.0, 0.35, 0.0);  // Mais alta
    glColor3f(0.1, 0.3, 0.1);
    glBegin(GL_TRIANGLES);
    glVertex3f(-0.25, 0.0, 0.0);
    glVertex3f(0.0, 0.4, 0.0);
    glVertex3f(0.25, 0.0, 0.0);
    glEnd();
    glPopMatrix();

    // Olho esquerdo
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.6, 0.1, 0.2);  // Levemente ajustado
    glutSolidSphere(0.05, 10, 10);
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.02, 0.0, 0.03);
    glutSolidSphere(0.02, 10, 10);
    glPopMatrix();

    // Olho direito
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(0.6, 0.1, -0.2);
    glutSolidSphere(0.05, 10, 10);
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(0.02, 0.0, -0.03);
    glutSolidSphere(0.02, 10, 10);
    glPopMatrix();
}

void drawMan() {
    // Corpo (camisa)
    glPushMatrix();
    glColor3f(0.2, 0.6, 0.2);
    glScalef(1.2, 2.5, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();

    // Cabeça com tom de pele mais escuro
    glPushMatrix();
    glTranslatef(0, 1.7, 0);
    glColor3f(0.5, 0.35, 0.2);  // Tom de pele mais escuro
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();

    // Braço esquerdo
    glPushMatrix();
    glTranslatef(-0.6, 2.2, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.3, 1.2, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    // Braço direito
    glPushMatrix();
    glTranslatef(0.6, 2.2, 0);
    glRotatef(-90, 1, 0, 0);
    glScalef(0.3, 1.2, 0.3);
    glutSolidCube(1.0);
    glPopMatrix();

    // Pernas
    glPushMatrix();
    glTranslatef(0, 0.5, 0);
    glColor3f(0.1, 0.2, 0.2);
    glScalef(0.6, 1.0, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();

    // Cabelo
    glPushMatrix();
    glTranslatef(0, 2.2, 0);
    glColor3f(0.2, 0.1, 0.1);
    glutSolidSphere(0.55, 20, 20);
    glPopMatrix();

    // Peixes
    glPushMatrix();
    glTranslatef(0.6, 2.8, 0);
    glScalef(0.6, 0.6, 0.6);
    drawFish();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.6, 2.8, 0);
    glScalef(0.6, 0.6, 0.6);
    drawFish();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Movimentação da câmera baseada nos ângulos
    gluLookAt(
        distance * sin(angleX) * cos(angleY), 
        distance * sin(angleY), 
        distance * cos(angleX) * cos(angleY), 
        0, 1, 0, 0, 1, 0
    );

    glPushMatrix();
    glTranslatef(0, -1, 0);
    drawMan();
    glPopMatrix();

    glutSwapBuffers();
}

void keyboard(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        angleX -= 0.1f;  // Girar a câmera para a esquerda
    } else if (key == GLUT_KEY_RIGHT) {
        angleX += 0.1f;  // Girar a câmera para a direita
    } else if (key == GLUT_KEY_UP) {
        angleY += 0.1f;  // Girar a câmera para cima
        if (angleY > 1.5f) angleY = 1.5f;  // Limite de rotação para não passar do topo
    } else if (key == GLUT_KEY_DOWN) {
        angleY -= 0.1f;  // Girar a câmera para baixo
        if (angleY < -1.5f) angleY = -1.5f;  // Limite de rotação para não passar da base
    }
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.3, 0.6, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Homem segurando peixe detalhado em 3D");
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);  // Função para capturar as teclas de seta
    glutMainLoop();
    return 0;
}