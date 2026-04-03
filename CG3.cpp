/*

#include "glut.h"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Constantele calculate simbolic (Fara aproximari manuale oarbe) ---
const float TARGET_ANGLE_X = 45.0f;
// Unghiul exact pentru a alinia (1, 0, sqrt(2)) la axa Z
const float TARGET_ANGLE_Y = asin(1.0f / sqrt(3.0f)) * 180.0f / M_PI; // ~35.2643...

// Variabile de stare pentru animatia secventiala (+0.5 pct)
float angle1_X = 0.0f; // Rotația 1: Aliniere plan XZ
float angle2_Y = 0.0f; // Rotația 2: Aliniere pe axa Z
float angle3_Z = 0.0f; // Rotația 3: Rotația propriu-zisă pe Z
float angle4_Y = 0.0f; // Rotația 4: Inversare Y
float angle5_X = 0.0f; // Rotația 5: Inversare X

int animState = 0; // Tine evidenta etapei de animatie (0 -> 6)

void updateAnimation(int value) {
    float speed = 1.0f; // Viteza animatiei

    switch (animState) {
        case 0: // Etapa 1: Aliniem axa cu planul XZ rotind in jurul lui X
            angle1_X += speed;
            if (angle1_X >= TARGET_ANGLE_X) { angle1_X = TARGET_ANGLE_X; animState = 1; }
            break;
        case 1: // Etapa 2: Aliniem axa la Z rotind in jurul lui Y
            angle2_Y -= speed;
            if (angle2_Y <= -TARGET_ANGLE_Y) { angle2_Y = -TARGET_ANGLE_Y; animState = 2; }
            break;
        case 2: // Etapa 3: Efectuam rotatia principala in jurul axei acum-aliniate (Z)
            angle3_Z += speed * 2.0f; // Se invarte putin mai repede
            if (angle3_Z >= 360.0f) { angle3_Z = 360.0f; animState = 3; }
            break;
        case 3: // Etapa 4: Inversam rotatia Y pentru a pune axa la loc
            angle4_Y += speed;
            if (angle4_Y >= TARGET_ANGLE_Y) { angle4_Y = TARGET_ANGLE_Y; animState = 4; }
            break;
        case 4: // Etapa 5: Inversam rotatia X pentru a pune axa la loc
            angle5_X -= speed;
            if (angle5_X <= -TARGET_ANGLE_X) { angle5_X = -TARGET_ANGLE_X; animState = 5; }
            break;
        case 5: // Pauza scurta si reset pentru a repeta demonstratia
            glutTimerFunc(1500, updateAnimation, 6);
            return;
        case 6:
            angle1_X = angle2_Y = angle3_Z = angle4_Y = angle5_X = 0.0f;
            animState = 0;
            break;
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, updateAnimation, 0); // ~60 fps
}

void drawAxesAndDiagonal() {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        // Axa X (Rosu)
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.0f);
        // Axa Y (Verde)
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 3.0f, 0.0f);
        // Axa Z (Albastru)
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 3.0f);
        
        // Prima diagonala (1, 1, 1) - Culoare Galbena
        glColor3f(1.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(2.0f, 2.0f, 2.0f);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Setam o camera care priveste originile dintr-un unghi bun
    gluLookAt(4.0, 3.0, 5.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);

    // Desenam axele fixe si diagonala referinta (inainte de a aplica rotatiile)
    drawAxesAndDiagonal();

    glPushMatrix();
        
        // Aceste comenzi implementeaza descompunerea simbolica. 
        // In OpenGL matricea se inmulteste la dreapta, deci transformarile 
        // se citesc in ordine inversa (de jos in sus). 
           glRotatef(angle5_X, 1.0f, 0.0f, 0.0f); // 5. Inversam Rotația X
           glRotatef(angle4_Y, 0.0f, 1.0f, 0.0f); // 4. Inversam Rotația Y
           glRotatef(angle3_Z, 0.0f, 0.0f, 1.0f); // 3. Rotația propriu-zisă în jurul lui Z
           glRotatef(angle2_Y, 0.0f, 1.0f, 0.0f); // 2. Aliniem (1, 0, sqrt(2)) la axa Z
           glRotatef(angle1_X, 1.0f, 0.0f, 0.0f); // 1. Rotim 45 de grade X pt a anula coordonata Y
           
           // Desenam un ceainic (Wireframe pentru a vedea usor rotatia 3D)
           glColor3f(0.4f, 0.8f, 1.0f);
           glutWireTeapot(1.2);
           
       glPopMatrix();
   
       glutSwapBuffers();
   }
   
   void reshape(int w, int h) {
       glViewport(0, 0, w, h);
       glMatrixMode(GL_PROJECTION);
       glLoadIdentity();
       gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
       glMatrixMode(GL_MODELVIEW);
   }
   
   void init() {
       glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Fundal inchis
       glEnable(GL_DEPTH_TEST);
       
       std::cout << "Animatia arata etapele descompunerii simbolice:" << std::endl;
       std::cout << "1. Rotatie X pentru a alinia pe planul XZ." << std::endl;
       std::cout << "2. Rotatie Y pentru a suprapune vectorul pe axa Z." << std::endl;
       std::cout << "3. Rotatia principala de 360 grade in jurul axei Z (care acum reprezinta diagonala)." << std::endl;
       std::cout << "4. Restore Y." << std::endl;
       std::cout << "5. Restore X." << std::endl;
   }
   
   int main(int argc, char** argv) {
       glutInit(&argc, argv);
       glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
       glutInitWindowSize(800, 600);
       glutCreateWindow("Descompunere Simbolica Rotatie (1, 1, 1)");
   
       init();
   
       glutDisplayFunc(display);
       glutReshapeFunc(reshape);
       glutTimerFunc(16, updateAnimation, 0);
   
       glutMainLoop();
       return 0;
   }

   */


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*

#include "glut.h"

// Variabile globale pentru a identifica ferestrele
int window1, window2;

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

// Aceeași funcție de desenare a scenei (nu se schimbă nimic)
void drawScene() {
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(4.0f, 0.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 3.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 3.0f);
    glEnd();

    float d1 = -0.3f;
    float z2 = -1.2f;
    float d2 = -1.5f;

    // CUTIA 1
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, d1);
        glVertex3f(1.0f, 1.0f, d1); glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();

    // CUTIA 2
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(1.0f, 0.0f, z2); glVertex3f(2.0f, 0.0f, z2);
        glVertex3f(2.0f, 1.0f, z2); glVertex3f(1.0f, 1.0f, z2);
    glEnd();
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(2.0f, 0.0f, z2); glVertex3f(2.0f, 0.0f, d2);
        glVertex3f(2.0f, 1.0f, d2); glVertex3f(2.0f, 1.0f, z2);
    glEnd();
}

// Funcția de afișare pentru FEREASTRA 1 (Front View)
void displayWindow1() {
    // Înainte de a desena, ne asigurăm că setăm contextul pe fereastra corectă
    glutSetWindow(window1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawScene(); // Fără rotație

    glutSwapBuffers();
}

// Funcția de afișare pentru FEREASTRA 2 (Rotated View)
void displayWindow2() {
    // Setăm contextul pe fereastra a doua
    glutSetWindow(window2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(-10.0f, 0.0f, 1.0f, 0.0f); // Cu rotație
    drawScene();

    glutSwapBuffers();
}

// Funcția de reshape care se aplică pentru ambele ferestre
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5, 3.0, -0.5, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // --- CREARE FEREASTRA 1 ---
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100); // Poziționează pe ecran
    window1 = glutCreateWindow("Task 2: Imaginea 1 (Vedere din fata)");
    init(); // Inițializează starea OpenGL pt fereastra 1
    glutDisplayFunc(displayWindow1);
    glutReshapeFunc(reshape);

    // --- CREARE FEREASTRA 2 ---
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(650, 100); // Poziționează mai la dreapta
    window2 = glutCreateWindow("Task 2: Imaginea 2 (Rotatie -10 grade)");
    init(); // Inițializează starea OpenGL pt fereastra 2
    glutDisplayFunc(displayWindow2);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

*/


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/*
#include "glut.h"

// Identificatori pentru cele doua ferestre
int window1, window2;

void init() {
    // Fundal alb
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // Activați testul de adancime pentru ascunderea fetelor posterioare
    glEnable(GL_DEPTH_TEST);
}

// Functia de desenare a scenei 
void drawScene() {
    // 1. Desenam Axele (Rosu=X, Verde=Y, Albastru=Z)
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        // Axa X
        glColor3f(1.0f, 0.0f, 0.0f); 
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(4.0f, 0.0f, 0.0f);
        // Axa Y
        glColor3f(0.0f, 1.0f, 0.0f); 
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 3.0f, 0.0f);
        // Axa Z
        glColor3f(0.0f, 0.0f, 1.0f); 
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 3.0f);
    glEnd();

    // Parametrii cutiilor
    float d1 = -0.3f; // Grosimea primei cutii
    float z2 = -1.2f; // Pozitia pe Z a celei de-a doua cutii (mai in spate)
    float d2 = -1.5f; // Spatele celei de-a doua cutii
    
    // Inaltimile (DIFERENTA FATA DE TASK-UL PRECEDENT)
    float h1 = 1.0f;  // Cutia din fata este mai inalta
    float h2 = 0.9f;  // Cutia din spate este mai scunda

    // 2. CUTIA 1 (Stanga - in fata)
    // Fata frontala (Cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, h1, 0.0f);
        glVertex3f(0.0f, h1, 0.0f);
    glEnd();
    // Fata laterala dreapta (Magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, d1);
        glVertex3f(1.0f, h1, d1);
        glVertex3f(1.0f, h1, 0.0f);
    glEnd();

    // 3. CUTIA 2 (Dreapta - impinsa in spate pe Z)
    // Fata frontala (Cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(1.0f, 0.0f, z2);
        glVertex3f(2.0f, 0.0f, z2);
        glVertex3f(2.0f, h2, z2);
        glVertex3f(1.0f, h2, z2);
    glEnd();
    // Fata laterala dreapta (Magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(2.0f, 0.0f, z2);
        glVertex3f(2.0f, 0.0f, d2);
        glVertex3f(2.0f, h2, d2);
        glVertex3f(2.0f, h2, z2);
    glEnd();
}

// Functia de afisare pentru FEREASTRA 1 (Fara rotatie)
void displayWindow1() {
    glutSetWindow(window1); // Setam contextul
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    drawScene(); // Desenam din fata

    glutSwapBuffers();
}

// Functia de afisare pentru FEREASTRA 2 (Cu rotatie de -10 grade)
void displayWindow2() {
    glutSetWindow(window2); // Setam contextul
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Aplicam diferenta ceruta in document
    glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
    drawScene();

    glutSwapBuffers();
}

// Functia de reshape pentru mentinerea proportiilor in ambele ferestre
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Setam vizualizarea ortografica 2D
    glOrtho(-0.5, 3.0, -0.5, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    // --- CREARE FEREASTRA 1 ---
    glutInitWindowSize(450, 450);
    glutInitWindowPosition(100, 100); 
    window1 = glutCreateWindow("Task 3: Imaginea 1 (Vedere din fata)");
    init(); 
    glutDisplayFunc(displayWindow1);
    glutReshapeFunc(reshape);

    // --- CREARE FEREASTRA 2 ---
    glutInitWindowSize(450, 450);
    glutInitWindowPosition(580, 100); 
    window2 = glutCreateWindow("Task 3: Imaginea 2 (Rotatie pe Oy)");
    init(); 
    glutDisplayFunc(displayWindow2);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}


*/

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*

#include "glut.h"

void init() {
    // Fundal alb
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // Activați testul de adâncime pentru ca fețele cubului să acopere corect axele
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

       // SECRETUL TASK-ULUI 4: Matricea de Proiecție Oblică (Shear)
       // Asta face ca axa Z să fie proiectată oblic pe ecran (în jos și la stânga)
       // fără a distorsiona deloc coordonatele X și Y ale feței frontale.
       // x_nou = x - 0.5 * z
       // y_nou = y - 0.5 * z
       
    GLfloat obliqueMatrix[16] = {
        1.0f,  0.0f,  0.0f, 0.0f,
        0.0f,  1.0f,  0.0f, 0.0f,
       -0.5f, -0.5f,  1.0f, 0.0f,
        0.0f,  0.0f,  0.0f, 1.0f
    };
    glMultMatrixf(obliqueMatrix);

    // 1. Desenăm Axele (pornesc din originea 0,0,0 care acum e in spatele cubului)
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        // Axa X (Roșu)
        glColor3f(1.0f, 0.0f, 0.0f); 
        glVertex3f(0.0f, 0.0f, 0.0f); 
        glVertex3f(3.0f, 0.0f, 0.0f);
        
        // Axa Y (Verde)
        glColor3f(0.0f, 1.0f, 0.0f); 
        glVertex3f(0.0f, 0.0f, 0.0f); 
        glVertex3f(0.0f, 3.0f, 0.0f);
    glEnd();

    // 2. Desenăm Cubul 1x1x1 (de la Z=0 la Z=1)
    
    // Fața Frontală (Cyan) - Aflată la Z = 1 (mai în față)
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 1.0f);
    glEnd();

    // Fața de Sus (Galben) - Aflată la Y = 1
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();

    // Fața din Dreapta (Magenta) - Aflată la X = 1
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Încadrăm scena ca să arate exact proporțiile din PDF
    glOrtho(-1.5, 3.0, -1.5, 3.0, -10.0, 10.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Task 4: Oblique Projection Illusion");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutMainLoop();
    return 0;
}
*/

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include "glut.h"

void init() {
    // Fundal alb
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // Activați testul de adâncime pentru ocluziunea corectă a fețelor și axelor
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* =========================================================================
       MATRICEA DE FORFECARE (SHEAR) MODIFICATĂ
       Folosim +0.5 pentru a proiecta axa Z în sus și la dreapta, 
       ceea ce va dezvălui fața din stânga și cea de jos.
       x_nou = x + 0.5 * z
       y_nou = y + 0.5 * z
       ========================================================================= */
    GLfloat obliqueMatrix[16] = {
        1.0f,  0.0f,  0.0f, 0.0f,
        0.0f,  1.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  1.0f, 0.0f, // Diferența este aici: +0.5 și +0.5
        0.0f,  0.0f,  0.0f, 1.0f
    };
    glMultMatrixf(obliqueMatrix);

    // 1. Desenăm Axele din origine (0,0,0)
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        // Axa X (Roșu)
        glColor3f(1.0f, 0.0f, 0.0f); 
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(3.0f, 0.0f, 0.0f);
        
        // Axa Y (Verde)
        glColor3f(0.0f, 1.0f, 0.0f); 
        glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 3.0f, 0.0f);
    glEnd();

    // 2. Desenăm Cubul 1x1x1

    // Fața Frontală (Cyan) - Aflată la Z = 1
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 1.0f);
    glEnd();

    // Fața din Stânga (Verde) - Aflată la X = 0
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();

    // Fața de Jos (Albastră) - Aflată la Y = 0
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Centram corect scena la fel ca în imaginea PDF
    glOrtho(-1.5, 3.5, -1.5, 3.5, -10.0, 10.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Task 4: Close Solution (-0.5 points)");
    
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    glutMainLoop();
    return 0;
}