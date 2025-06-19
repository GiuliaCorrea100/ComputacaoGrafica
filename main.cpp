#include <GLUT/glut.h>
#include <math.h>
#include <cmath>

// --- Variáveis Globais de Animação ---
float netAnimationPhase = 0.0f; // Para a rede

// <<< NOVAS variáveis para a animação do peixe >>>
float fishAnimationTime = 0.0f; // Um "relógio" para a animação do peixe
float fishRotation = 0.0f;      // Ângulo de rotação do corpo do peixe
float tailFlap = 0.0f;          // Deslocamento da cauda do peixe


// Função utilitária para desenhar círculos
void drawCircle(float x, float y, float radius, int segments = 100) {
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.1415926f * i / segments;
        glVertex2f(x + cos(angle) * radius, y + sin(angle) * radius);
    }
    glEnd();
}

// Função para desenhar as árvores
void drawTrees() {
    for (float x = -0.9f; x <= 0.9f; x += 0.2f) {
        glColor3f(0.5f, 0.35f, 0.05f);
        glBegin(GL_QUADS);
        glVertex2f(x - 0.01f, 0.3f);
        glVertex2f(x + 0.01f, 0.3f);
        glVertex2f(x + 0.01f, 0.35f);
        glVertex2f(x - 0.01f, 0.35f);
        glEnd();
        glColor3f(0.1f, 0.5f, 0.1f);
        drawCircle(x, 0.38f, 0.04f);
    }
}

// Desenha o fundo
void drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.7f, 1.0f);
    glVertex2f(-1, 0.3);
    glVertex2f(1, 0.3);
    glColor3f(0.2f, 0.4f, 0.6f);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(0.1f, 0.3f, 0.3f);
    glVertex2f(-1.0f, 0.3f);
    glVertex2f(-0.5f, 0.6f);
    glVertex2f(0.0f, 0.3f);
    glVertex2f(0.0f, 0.3f);
    glVertex2f(0.5f, 0.65f);
    glVertex2f(1.0f, 0.3f);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.2f);
    glVertex2f(-1, 0.15);
    glVertex2f(1, 0.15);
    glVertex2f(1, 0.3);
    glVertex2f(-1, 0.3);
    glEnd();
}

// Desenha a água
void drawWater() {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.7f);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 0.15);
    glVertex2f(-1, 0.15);
    glEnd();
    glColor3f(1, 1, 1);
    for (float x = -1.0f; x <= 1.0f; x += 0.2f) {
        drawCircle(x, 0.1 + 0.02f * sin(x * 10), 0.01f);
    }
}

// Desenha a rede com boias
void drawFishingNet() {
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_STRIP);
    for (float x = -1.0f; x <= 1.0f; x += 0.05f) {
        glVertex2f(x, -0.3f + 0.05f * sin(x * 15 + netAnimationPhase));
    }
    glEnd();
    for (float x = -1.0f; x <= 1.0f; x += 0.2f) {
        drawCircle(x, -0.3f + 0.05f * sin(x * 15 + netAnimationPhase), 0.02f);
    }
}

// Cabeça do pescador
void drawFishermanHead() {
    const float deltaY = 0.33f;
    glColor3f(0.3f, 0.2f, 0.15f);
    drawCircle(0.0f, 0.52f - deltaY, 0.09f);
}

// Corpo e braços do pescador
void drawFishermanBody() {
    const float deltaY = 0.33f;
    glColor3f(0.9f, 0.7f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.2f, 0.08f - deltaY);
    glVertex2f(0.2f, 0.08f - deltaY);
    glVertex2f(0.15f, 0.43f - deltaY);
    glVertex2f(-0.15f, 0.43f - deltaY);
    glEnd();
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, 0.43f - deltaY);
    glVertex2f(-0.25f, 0.6f - deltaY);
    glVertex2f(-0.2f, 0.65f - deltaY);
    glVertex2f(-0.1f, 0.48f - deltaY);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(0.15f, 0.43f - deltaY);
    glVertex2f(0.25f, 0.6f - deltaY);
    glVertex2f(0.2f, 0.65f - deltaY);
    glVertex2f(0.1f, 0.48f - deltaY);
    glEnd();
}

// <<< MODIFICADO: A função inteira foi alterada para aplicar rotação e movimento da cauda >>>
// Peixe com nadadeiras e olho
void drawFish() {
    const float deltaY = 0.13f;
    const float scale = 1.5f;
    float fishCenterX = 0.0f; // Peixe está centralizado no eixo X
    float fishCenterY = 0.45f - deltaY;

    // Salva a matriz de transformação atual (como um "checkpoint")
    glPushMatrix();

    // Aplica as transformações de rotação para o corpo
    glTranslatef(fishCenterX, fishCenterY, 0.0f);        // 1. Move para o centro do peixe
    glRotatef(fishRotation, 0.0f, 0.0f, 1.0f); // 2. Rotaciona em torno do eixo Z
    glTranslatef(-fishCenterX, -fishCenterY, 0.0f);       // 3. Move de volta para a posição original

    // --- Desenha o corpo, cabeça e nadadeiras (que agora serão rotacionados) ---

    // Corpo do peixe
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.85f, 0.7f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180;
        float x = fishCenterX + (0.15f * scale) * cos(angle);
        float y = fishCenterY + (0.08f * scale) * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Cabeça do peixe
    glColor3f(1.0f, 0.95f, 0.85f);
    drawCircle(fishCenterX + (0.05f * scale), fishCenterY, 0.05f * scale);

    // Olho
    glColor3f(0, 0, 0);
    float eyeOffsetX = 0.07f * scale;
    float eyeOffsetY = (0.47f - 0.45f) * scale;
    drawCircle(fishCenterX + eyeOffsetX, fishCenterY + eyeOffsetY, 0.01f * scale);

    // Nadadeira superior e inferior
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_TRIANGLES);
    glVertex2f(fishCenterX, fishCenterY + (0.08f * scale));
    glVertex2f(fishCenterX - (0.03f * scale), fishCenterY + (0.15f * scale));
    glVertex2f(fishCenterX + (0.03f * scale), fishCenterY + (0.15f * scale));
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(fishCenterX, fishCenterY - (0.08f * scale));
    glVertex2f(fishCenterX - (0.03f * scale), fishCenterY - (0.15f * scale));
    glVertex2f(fishCenterX + (0.03f * scale), fishCenterY - (0.15f * scale));
    glEnd();

    // --- Desenha a cauda com animação de "flap" ---
    // A cauda não será rotacionada junto com o corpo, mas terá seu próprio movimento
    glBegin(GL_TRIANGLES);
    // Base da cauda (conectada ao corpo)
    glVertex2f(fishCenterX - (0.12f * scale), fishCenterY);
    // Pontas da cauda (o 'tailFlap' adiciona movimento no eixo X)
    glVertex2f(fishCenterX - (0.18f * scale) + tailFlap, fishCenterY + (0.05f * scale));
    glVertex2f(fishCenterX - (0.18f * scale) + tailFlap, fishCenterY - (0.05f * scale));
    glEnd();

    // Restaura a matriz de transformação (volta ao "checkpoint")
    glPopMatrix();
}


// Exibição principal
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();
    drawTrees();
    drawWater();
    drawFishingNet();
    drawFishermanHead();
    drawFishermanBody();
    drawFish();
    glutSwapBuffers();
}

// <<< MODIFICADO: Função de animação agora também calcula o movimento do peixe >>>
void animate(int value) {
    // --- Animação da Rede ---
    netAnimationPhase += 0.05f;
    netAnimationPhase = fmodf(netAnimationPhase, 2.0f * 3.1415926f);

    // --- Animação do Peixe ---
    fishAnimationTime += 0.1f; // Incrementa o "relógio" do peixe
    // Corpo rotaciona levemente (entre -5 e +5 graus)
    fishRotation = sin(fishAnimationTime) * 5.0f;
    // Cauda se move mais (deslocamento de -0.04 a +0.04)
    tailFlap = sin(fishAnimationTime * 1.5f) * 0.04f; // Multiplicar por 1.5 faz a cauda mover um pouco mais rápido que o corpo

    // Pede ao GLUT para redesenhar a cena
    glutPostRedisplay();

    // Agenda a próxima chamada desta função
    glutTimerFunc(16, animate, 0);
}

// Setup
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Pescador com Peixe - OpenGL");
    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glutDisplayFunc(display);

    // Inicia o loop de animação
    glutTimerFunc(0, animate, 0);

    glutMainLoop();
    return 0;
}