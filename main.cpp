#include <GLUT/glut.h>
#include <math.h>

// Função utilitária para desenhar círculos (útil para olhos, bolhas, etc.)
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
    // Loop para desenhar várias árvores ao longo do eixo x
    for (float x = -0.9f; x <= 0.9f; x += 0.2f) {
        // Tronco da árvore (retângulo marrom)
        glColor3f(0.5f, 0.35f, 0.05f); // Cor marrom
        glBegin(GL_QUADS);
        glVertex2f(x - 0.01f, 0.3f);  // Base do tronco na linha da vegetação
        glVertex2f(x + 0.01f, 0.3f);
        glVertex2f(x + 0.01f, 0.35f); // Altura do tronco
        glVertex2f(x - 0.01f, 0.35f);
        glEnd();

        // Copa da árvore (círculo verde escuro)
        glColor3f(0.1f, 0.5f, 0.1f); // Verde mais escuro para a copa
        drawCircle(x, 0.38f, 0.04f); // Desenha a copa acima do tronco
    }
}

// Desenha o fundo com céu, montanhas e vegetação
void drawBackground() {
    // Céu
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.7f, 1.0f); // Céu azul claro
    glVertex2f(-1, 0.3);
    glVertex2f(1, 0.3);
    glColor3f(0.2f, 0.4f, 0.6f); // Céu azul mais escuro
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();

    // Montanhas
    glBegin(GL_TRIANGLES);
    glColor3f(0.1f, 0.3f, 0.3f);
    glVertex2f(-1.0f, 0.3f);
    glVertex2f(-0.5f, 0.6f);
    glVertex2f(0.0f, 0.3f);

    glVertex2f(0.0f, 0.3f);
    glVertex2f(0.5f, 0.65f);
    glVertex2f(1.0f, 0.3f);
    glEnd();

    // Vegetação
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
    // Água
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.6f, 0.7f);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 0.15);
    glVertex2f(-1, 0.15);
    glEnd();

    // Ondas
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
        glVertex2f(x, -0.3f + 0.05f * sin(x * 15));
    }
    glEnd();

    for (float x = -1.0f; x <= 1.0f; x += 0.2f) {
        drawCircle(x, -0.3f + 0.05f * sin(x * 15), 0.02f);
    }
}

// Cabeça do pescador
void drawFishermanHead() {
    const float deltaY = 0.33f; // Valor para manter o pescador abaixado
    glColor3f(0.3f, 0.2f, 0.15f); // Marrom escuro
    drawCircle(0.0f, 0.52f - deltaY, 0.09f);
}

// Corpo e braços do pescador
void drawFishermanBody() {
    const float deltaY = 0.33f; // Valor para manter o pescador abaixado
    // Camiseta
    glColor3f(0.9f, 0.7f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.2f, 0.08f - deltaY);
    glVertex2f(0.2f, 0.08f - deltaY);
    glVertex2f(0.15f, 0.43f - deltaY);
    glVertex2f(-0.15f, 0.43f - deltaY);
    glEnd();

    // Braço esquerdo
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-0.15f, 0.43f - deltaY);
    glVertex2f(-0.25f, 0.6f - deltaY);
    glVertex2f(-0.2f, 0.65f - deltaY);
    glVertex2f(-0.1f, 0.48f - deltaY);
    glEnd();

    // Braço direito
    glBegin(GL_QUADS);
    glVertex2f(0.15f, 0.43f - deltaY);
    glVertex2f(0.25f, 0.6f - deltaY);
    glVertex2f(0.2f, 0.65f - deltaY);
    glVertex2f(0.1f, 0.48f - deltaY);
    glEnd();
}

// Peixe com nadadeiras e olho
void drawFish() {
    // >>> INÍCIO DA MODIFICAÇÃO <<<
    // O valor de deltaY foi alterado de 0.33f para 0.13f APENAS PARA O PEIXE.
    // Isso faz com que ele suba e fique alinhado com as mãos do pescador.
    const float deltaY = 0.13f;
    // >>> FIM DA MODIFICAÇÃO <<<

    // Corpo do peixe
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.85f, 0.7f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180;
        float x = 0.0f + 0.15f * cos(angle);
        float y = (0.45f - deltaY) + 0.08f * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Cabeça do peixe
    glColor3f(1.0f, 0.95f, 0.85f);
    drawCircle(0.05f, 0.45f - deltaY, 0.05f);

    // Olho
    glColor3f(0, 0, 0);
    drawCircle(0.07f, 0.47f - deltaY, 0.01f);

    // Nadadeira superior
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.53f - deltaY);
    glVertex2f(-0.03f, 0.6f - deltaY);
    glVertex2f(0.03f, 0.6f - deltaY);
    glEnd();

    // Nadadeira inferior
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.37f - deltaY);
    glVertex2f(-0.03f, 0.3f - deltaY);
    glVertex2f(0.03f, 0.3f - deltaY);
    glEnd();

    // Cauda
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.12f, 0.45f - deltaY);
    glVertex2f(-0.18f, 0.5f - deltaY);
    glVertex2f(-0.18f, 0.4f - deltaY);
    glEnd();
}

// Exibição principal
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    drawTrees();
    drawWater();
    drawFishingNet();

    // A ordem de chamada aqui é importante:
    // 1. Desenha o pescador primeiro
    drawFishermanHead();
    drawFishermanBody();
    // 2. Desenha o peixe depois, para que ele apareça na frente
    drawFish();

    glutSwapBuffers();
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
    glutMainLoop();
    
    return 0;
}