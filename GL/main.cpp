#include <GLUT/glut.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION // Necessário para a biblioteca stb_image
#include "stb_image.h"
#include <iostream> // Para mensagens de erro

GLuint shirtTextureID; //// Variável global para guardar o ID da nossa textura blusa
GLuint waterTextureID; // textura agua
GLuint grassTextureID; // textura grama
GLuint woodTextureID; // textura madeira das arvores
GLuint leafTextureID; // textura folhas da arvore
GLuint buoyTextureID; // textura boias
GLuint mountainTextureID; // textura montamhas
GLuint fishStripeTextureID; // listras peixe

// Função para carregar uma textura de um arquivo
GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID); // Gera um ID para a textura

    int width, height, nrChannels;
    // Inverte a imagem no carregamento porque o OpenGL espera que o 0.0 no eixo Y esteja na parte inferior da imagem
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        // Define os parâmetros de "wrapping" e "filtering" da textura
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {
        std::cout << "Falha ao carregar a textura: " << filename << std::endl;
    }

    stbi_image_free(data); // Libera a memória da imagem

    return textureID;
}

// --- Variáveis Globais de Animação ---
float netAnimationPhase = 0.0f;
float fishAnimationTime = 0.0f;
float fishRotation = 0.0f;
float tailFlap = 0.0f;
float cyclePhase = 0.0f; // Controla o ciclo de dia e noite
 
float waterTextureOffset = 0.0f;

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

// Nova função para desenhar círculos COM textura
void drawTexturedCircle(float x, float y, float radius, int segments = 100) {
    glBegin(GL_TRIANGLE_FAN);
        // O centro do círculo corresponde ao centro da textura (0.5, 0.5)
        glTexCoord2f(0.5f, 0.5f);
        glVertex2f(x, y); // Vértice central

        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * 3.1415926f * i / segments;
            
            // Calcula a coordenada do vértice na borda do círculo
            float vx = x + cos(angle) * radius;
            float vy = y + sin(angle) * radius;

            // Calcula a coordenada da textura correspondente
            // Mapeia a borda do círculo para a borda da textura
            float tx = 0.5f + cos(angle) * 0.5f;
            float ty = 0.5f + sin(angle) * 0.5f;

            glTexCoord2f(tx, ty);
            glVertex2f(vx, vy);
        }
    glEnd();
}

// Função para desenhar as árvores
// Função para desenhar as árvores
void drawTrees() {
    // --- PARTE 1: Desenhar todos os TRONCOS com textura ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, woodTextureID);
    glColor3f(1.0f, 1.0f, 1.0f); 

    for (float x = -0.9f; x <= 0.9f; x += 0.2f) {
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(x - 0.01f, 0.3f);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(x + 0.01f, 0.3f);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(x + 0.01f, 0.35f);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(x - 0.01f, 0.35f);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);


    // --- PARTE 2: Desenhar todas as COPAS com textura de folhas ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, leafTextureID);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para a textura de folhas

    for (float x = -0.9f; x <= 0.9f; x += 0.2f) {
        // Usamos a nossa NOVA função para desenhar círculos com textura
        drawTexturedCircle(x, 0.38f, 0.04f);
    }
    glDisable(GL_TEXTURE_2D); // Desabilita a textura ao final
}

// Função que gerencia o ciclo de dia/noite e desenha o fundo
// Função que gerencia o ciclo de dia/noite e desenha o fundo
void drawBackground() {
    // --- PARTE 1: CÉU (sem textura) ---
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


    // --- PARTE 2: MONTANHAS (com textura) ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mountainTextureID);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não tingir a textura

    glBegin(GL_TRIANGLES);
        // Triângulo 1
        glTexCoord2f(0.0f, 0.0f);   glVertex2f(-1.0f, 0.3f);   // Base esquerda
        glTexCoord2f(0.25f, 0.85f); glVertex2f(-0.5f, 0.6f);    // Pico esquerdo
        glTexCoord2f(0.5f, 0.0f);   glVertex2f(0.0f, 0.3f);    // Base meio
        
        // Triângulo 2
        glTexCoord2f(0.5f, 0.0f);   glVertex2f(0.0f, 0.3f);    // Base meio
        glTexCoord2f(0.75f, 1.0f);  glVertex2f(0.5f, 0.65f);   // Pico direito (mais alto)
        glTexCoord2f(1.0f, 0.0f);   glVertex2f(1.0f, 0.3f);    // Base direita
    glEnd();
    glDisable(GL_TEXTURE_2D);


    // --- PARTE 3: GRAMA (com textura) ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, grassTextureID);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-1, 0.15);
        glTexCoord2f(5.0f, 0.0f); glVertex2f(1, 0.15);
        glTexCoord2f(5.0f, 1.0f); glVertex2f(1, 0.3);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-1, 0.3);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// Desenha a água
// Desenha a água
void drawWater() {
    // Habilita e vincula a textura da água
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, waterTextureID);
    
    // Cor branca para não tingir a textura
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        // Usamos o waterTextureOffset para deslocar a coordenada X da textura, criando a animação
        glTexCoord2f(0.0f + waterTextureOffset, 0.0f); glVertex2f(-1, -1);
        glTexCoord2f(2.0f + waterTextureOffset, 0.0f); glVertex2f(1, -1); // Usamos 2.0f para a textura repetir mais vezes
        glTexCoord2f(2.0f + waterTextureOffset, 1.0f); glVertex2f(1, 0.15);
        glTexCoord2f(0.0f + waterTextureOffset, 1.0f); glVertex2f(-1, 0.15);
    glEnd();

    // Desabilita a textura para desenhar as ondinhas brancas por cima
    glDisable(GL_TEXTURE_2D);

    // Desenha as ondinhas brancas (sem alteração)
    glColor3f(1, 1, 1);
    for (float x = -1.0f; x <= 1.0f; x += 0.2f) {
        drawCircle(x, 0.1 + 0.02f * sin(x * 10 + netAnimationPhase * 2.0), 0.01f); // Adicionei animação aqui também
    }
}

// Desenha a rede com boias
// Desenha a rede com boias
void drawFishingNet() {
    // --- PARTE 1: Desenhar a linha da rede (sem textura) ---
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para a linha
    glBegin(GL_LINE_STRIP);
    for (float x = -1.0f; x <= 1.0f; x += 0.05f) {
        glVertex2f(x, -0.3f + 0.05f * sin(x * 15 + netAnimationPhase));
    }
    glEnd();

    // --- PARTE 2: Desenhar as boias com textura ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, buoyTextureID);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não tingir a textura

    for (float x = -1.0f; x <= 1.0f; x += 0.2f) {
        // Usamos a função de círculo texturizado que já criamos!
        drawTexturedCircle(x, -0.3f + 0.05f * sin(x * 15 + netAnimationPhase), 0.04f);
    }
    
    glDisable(GL_TEXTURE_2D); // Desabilita a textura ao final
}

// Cabeça do pescador
void drawFishermanHead() {
    const float deltaY = 0.33f;
    glColor3f(0.3f, 0.2f, 0.15f);
    drawCircle(0.0f, 0.52f - deltaY, 0.09f);
}

// Corpo e braços do pescador
// Corpo e braços do pescador
void drawFishermanBody() {
    const float deltaY = 0.33f;

    // Habilita e vincula a textura da blusa
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, shirtTextureID);

    // Definir a cor para branco (1,1,1) fará com que a textura apareça em suas cores originais.
    glColor3f(1.0f, 1.0f, 1.0f); 

    glBegin(GL_POLYGON);
        // Canto inferior esquerdo do polígono -> Canto inferior esquerdo da textura
        glTexCoord2f(0.0f, 0.0f); 
        glVertex2f(-0.2f, 0.08f - deltaY);

        // Canto inferior direito do polígono -> Canto inferior direito da textura
        glTexCoord2f(1.0f, 0.0f); 
        glVertex2f(0.2f, 0.08f - deltaY);

        // Canto superior direito do polígono -> Canto superior direito da textura
        glTexCoord2f(1.0f, 1.0f); 
        glVertex2f(0.15f, 0.43f - deltaY);

        // Canto superior esquerdo do polígono -> Canto superior esquerdo da textura
        glTexCoord2f(0.0f, 1.0f); 
        glVertex2f(-0.15f, 0.43f - deltaY);
    glEnd();

    // Desabilita as texturas para não afetar os braços
    glDisable(GL_TEXTURE_2D);

    // O código dos braços continua aqui, pois eles não têm textura
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
// Peixe com nadadeiras, olho e listras texturizadas
void drawFish() {
    const float deltaY = 0.13f;
    const float scale = 1.5f;
    float fishCenterX = 0.0f;
    float fishCenterY = 0.45f - deltaY;

    glPushMatrix();
    glTranslatef(fishCenterX, fishCenterY, 0.0f);
    glRotatef(fishRotation, 0.0f, 0.0f, 1.0f);
    glTranslatef(-fishCenterX, -fishCenterY, 0.0f);

    // --- PARTE 1: Desenhar partes SEM textura ---

    // Corpo principal do peixe
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.85f, 0.7f);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180;
        float x = fishCenterX + (0.15f * scale) * cos(angle);
        float y = fishCenterY + (0.08f * scale) * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
    
    // Cabeça e olho
    glColor3f(1.0f, 0.95f, 0.85f);
    drawCircle(fishCenterX + (0.05f * scale), fishCenterY, 0.05f * scale);
    glColor3f(0, 0, 0);
    float eyeOffsetX = 0.07f * scale;
    float eyeOffsetY = (0.47f - 0.45f) * scale;
    drawCircle(fishCenterX + eyeOffsetX, fishCenterY + eyeOffsetY, 0.01f * scale);
    
    // Nadadeiras e cauda
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


    // --- PARTE 2: Desenhar LISTRAS COM textura ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fishStripeTextureID);
    glColor3f(1.0f, 1.0f, 1.0f); // Cor branca para não tingir a textura

    // Listra 1
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(fishCenterX - 0.11f * scale, fishCenterY - 0.05f * scale);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(fishCenterX - 0.08f * scale, fishCenterY - 0.065f * scale);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(fishCenterX - 0.08f * scale, fishCenterY + 0.065f * scale);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(fishCenterX - 0.11f * scale, fishCenterY + 0.05f * scale);
    glEnd();
    
    // Listra 2
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(fishCenterX - 0.05f * scale, fishCenterY - 0.075f * scale);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(fishCenterX - 0.02f * scale, fishCenterY - 0.078f * scale);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(fishCenterX - 0.02f * scale, fishCenterY + 0.078f * scale);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(fishCenterX - 0.05f * scale, fishCenterY + 0.075f * scale);
    glEnd();
    
    // Listra 3
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(fishCenterX + 0.01f * scale, fishCenterY - 0.078f * scale);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(fishCenterX + 0.04f * scale, fishCenterY - 0.07f * scale);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(fishCenterX + 0.04f * scale, fishCenterY + 0.07f * scale);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(fishCenterX + 0.01f * scale, fishCenterY + 0.078f * scale);
    glEnd();

    glDisable(GL_TEXTURE_2D);

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
    netAnimationPhase += 0.02f;
    netAnimationPhase = fmodf(netAnimationPhase, 2.0f * 3.1415926f);
    fishAnimationTime += 0.1f;
    fishRotation = sin(fishAnimationTime) * 5.0f;
    tailFlap = sin(fishAnimationTime * 1.5f) * 0.04f;

    cyclePhase += 0.001f; // Velocidade 8x maior que a original
    cyclePhase = fmodf(cyclePhase, 1.0f);

    waterTextureOffset += 0.0005f;

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

    // Carrega a textura da blusa
    shirtTextureID = loadTexture("texturas/texturaBlusaAmarela.jpg");
    waterTextureID = loadTexture("texturas/texturaAguaAzul.jpg");
    grassTextureID = loadTexture("texturas/texturaGrama.jpg");
    woodTextureID = loadTexture("texturas/texturaMadeira.jpg"); 
    leafTextureID = loadTexture("texturas/texturaFolhas.jpg");
    buoyTextureID = loadTexture("texturas/texturaBoia.jpg");
    mountainTextureID = loadTexture("texturas/texturaMontanhas.jpg");
    fishStripeTextureID = loadTexture("texturas/texturaListrasPeixe.jpg");

    generateStars();

    // Executa o comando 'afplay' do macOS para tocar o MP3 em segundo plano
    system("afplay sons/sonsIndígenasDaAmazonia.mp3 &");
    // para a musica parar de tocar - killall afplay

    glutDisplayFunc(display);
    glutTimerFunc(0, animate, 0);
    glutMainLoop();
    return 0;
}