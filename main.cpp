#include <GLUT/glut.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <cstdlib>

// --- Variáveis Globais de Animação ---
float netAnimationPhase = 0.0f;
float fishAnimationTime = 0.0f;
float fishRotation = 0.0f;
float tailFlap = 0.0f;
float cyclePhase = 0.0f; // Controla o ciclo de dia e noite

// Estrutura para as estrelas
struct Star {
    float x, y;
};
std::vector<Star> stars;
bool starsGenerated = false;


// Função auxiliar para interpolação linear
float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Função para gerar as estrelas uma única vez
void generateStars() {
    if (!starsGenerated) {
        srand(0); // Semente fixa para estrelas consistentes
        for (int i = 0; i < 200; ++i) {
            Star s;
            s.x = (rand() / (float)RAND_MAX) * 2.0f - 1.0f;
            s.y = (rand() / (float)RAND_MAX) * 0.7f + 0.3f;
            stars.push_back(s);
        }
        starsGenerated = true;
    }
}

// Função para desenhar as estrelas
void drawStars() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (const auto& s : stars) {
        glVertex2f(s.x, s.y);
    }
    glEnd();
}

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

// Função que gerencia o ciclo de dia/noite e desenha o fundo
void drawBackground() {
    const float c_day_t[] = {0.2f, 0.4f, 0.6f}; const float c_day_b[] = {0.4f, 0.7f, 1.0f};
    const float c_set_t[] = {0.8f, 0.2f, 0.2f}; const float c_set_b[] = {1.0f, 0.7f, 0.3f};
    const float c_night_t[]={0.0f, 0.0f, 0.1f}; const float c_night_b[]={0.1f, 0.1f, 0.3f};
    float topR, topG, topB, botR, botG, botB;
    float t = 0;

    if (cyclePhase < 0.4f) { // Dia
        topR = c_day_t[0]; topG = c_day_t[1]; topB = c_day_t[2];
        botR = c_day_b[0]; botG = c_day_b[1]; botB = c_day_b[2];
    } else if (cyclePhase < 0.5f) { // Pôr do Sol
        t = (cyclePhase - 0.4f) / 0.1f;
        topR = lerp(c_day_t[0], c_set_t[0], t); topG = lerp(c_day_t[1], c_set_t[1], t); topB = lerp(c_day_t[2], c_set_t[2], t);
        botR = lerp(c_day_b[0], c_set_b[0], t); botG = lerp(c_day_b[1], c_set_b[1], t); botB = lerp(c_day_b[2], c_set_b[2], t);
    } else if (cyclePhase < 0.9f) { // Noite
        t = (cyclePhase - 0.5f) / 0.4f;
        topR = lerp(c_set_t[0], c_night_t[0], t); topG = lerp(c_set_t[1], c_night_t[1], t); topB = lerp(c_set_t[2], c_night_t[2], t);
        botR = lerp(c_set_b[0], c_night_b[0], t); botG = lerp(c_set_b[1], c_night_b[1], t); botB = lerp(c_set_b[2], c_night_b[2], t);
        drawStars();
    } else { // Nascer do Sol
        t = (cyclePhase - 0.9f) / 0.1f;
        topR = lerp(c_night_t[0], c_day_t[0], t); topG = lerp(c_night_t[1], c_day_t[1], t); topB = lerp(c_night_t[2], c_day_t[2], t);
        botR = lerp(c_night_b[0], c_day_b[0], t); botG = lerp(c_night_b[1], c_day_b[1], t); botB = lerp(c_night_b[2], c_day_b[2], t);
    }

    glBegin(GL_QUADS);
    glColor3f(botR, botG, botB);
    glVertex2f(-1, 0.3);
    glVertex2f(1, 0.3);
    glColor3f(topR, topG, topB);
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

// Peixe com nadadeiras, olho e listras
void drawFish() {
    const float deltaY = 0.13f;
    const float scale = 1.5f;
    float fishCenterX = 0.0f;
    float fishCenterY = 0.45f - deltaY;

    glPushMatrix();
    glTranslatef(fishCenterX, fishCenterY, 0.0f);
    glRotatef(fishRotation, 0.0f, 0.0f, 1.0f);
    glTranslatef(-fishCenterX, -fishCenterY, 0.0f);

    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.85f, 0.7f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180;
        float x = fishCenterX + (0.15f * scale) * cos(angle);
        float y = fishCenterY + (0.08f * scale) * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    glColor3f(0.7f, 0.65f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(fishCenterX - 0.11f * scale, fishCenterY + 0.05f * scale);
    glVertex2f(fishCenterX - 0.08f * scale, fishCenterY + 0.065f * scale);
    glVertex2f(fishCenterX - 0.08f * scale, fishCenterY - 0.065f * scale);
    glVertex2f(fishCenterX - 0.11f * scale, fishCenterY - 0.05f * scale);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(fishCenterX - 0.05f * scale, fishCenterY + 0.075f * scale);
    glVertex2f(fishCenterX - 0.02f * scale, fishCenterY + 0.078f * scale);
    glVertex2f(fishCenterX - 0.02f * scale, fishCenterY - 0.078f * scale);
    glVertex2f(fishCenterX - 0.05f * scale, fishCenterY - 0.075f * scale);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(fishCenterX + 0.01f * scale, fishCenterY + 0.078f * scale);
    glVertex2f(fishCenterX + 0.04f * scale, fishCenterY + 0.07f * scale);
    glVertex2f(fishCenterX + 0.04f * scale, fishCenterY - 0.07f * scale);
    glVertex2f(fishCenterX + 0.01f * scale, fishCenterY - 0.078f * scale);
    glEnd();

    glColor3f(1.0f, 0.95f, 0.85f);
    drawCircle(fishCenterX + (0.05f * scale), fishCenterY, 0.05f * scale);
    glColor3f(0, 0, 0);
    float eyeOffsetX = 0.07f * scale;
    float eyeOffsetY = (0.47f - 0.45f) * scale;
    drawCircle(fishCenterX + eyeOffsetX, fishCenterY + eyeOffsetY, 0.01f * scale);
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

    glBegin(GL_TRIANGLES);
    glVertex2f(fishCenterX - (0.12f * scale), fishCenterY);
    glVertex2f(fishCenterX - (0.18f * scale) + tailFlap, fishCenterY + (0.05f * scale));
    glVertex2f(fishCenterX - (0.18f * scale) + tailFlap, fishCenterY - (0.05f * scale));
    glEnd();
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

// Função de animação com velocidade acelerada
void animate(int value) {
    netAnimationPhase += 0.05f;
    netAnimationPhase = fmodf(netAnimationPhase, 2.0f * 3.1415926f);
    fishAnimationTime += 0.1f;
    fishRotation = sin(fishAnimationTime) * 5.0f;
    tailFlap = sin(fishAnimationTime * 1.5f) * 0.04f;

    cyclePhase += 0.001f; // Velocidade 8x maior que a original
    cyclePhase = fmodf(cyclePhase, 1.0f);

    glutPostRedisplay();
    glutTimerFunc(16, animate, 0);
}

// Setup
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Pescador - Ciclo Dia e Noite com Peixe Detalhado");
    glClearColor(0.0f, 0.5f, 0.8f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    generateStars();

    glutDisplayFunc(display);
    glutTimerFunc(0, animate, 0);
    glutMainLoop();
    return 0;
}