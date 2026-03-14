//ca sa nu se amestece codurile, le-am comentat pe toate si le-am delimitat prin + si decomentate pe rand pt a le rula.
/*
#include "glut.h"
#include <cmath>
#include <string>

// Constante pentru precizie maximă
const double PI = 3.14159265358979323846;

// Setăm nivelul inițial la 2 (pentru prima imagine din cerința ta)
// Apăsând tasta '+' vei ajunge la nivelul 5 (a doua imagine)
int nivelRecursivitate = 2; 
const double razaCerc = 0.8; 

class Turtle {
public:
    double x, y, unghi;
    Turtle(double startX, double startY) : x(startX), y(startY), unghi(0) {}

    void move(double distanta, bool deseneaza) {
        double xNou = x + distanta * cos(unghi * PI / 180.0);
        double yNou = y + distanta * sin(unghi * PI / 180.0);
        if (deseneaza) {
            glBegin(GL_LINES);
            glVertex2d(x, y);
            glVertex2d(xNou, yNou);
            glEnd();
        }
        x = xNou;
        y = yNou;
    }

    void turn(double grade) {
        unghi += grade;
    }
};

void deseneazaLinieKoch(Turtle &t, double lungime, int nivel) {
    if (nivel == 0) {
        t.move(lungime, true);
    } else {
        double l = lungime / 3.0;
        deseneazaLinieKoch(t, l, nivel - 1);
        t.turn(60);
        deseneazaLinieKoch(t, l, nivel - 1);
        t.turn(-120);
        deseneazaLinieKoch(t, l, nivel - 1);
        t.turn(60);
        deseneazaLinieKoch(t, l, nivel - 1);
    }
}

void deseneazaCerc(double r) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 200; i++) { // 200 de segmente pentru un cerc și mai fin
        double theta = 2.0 * PI * double(i) / 200.0;
        glVertex2d(r * cos(theta), r * sin(theta));
    }
    glEnd();
}

void afiseazaText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Curățăm matricea pentru a preveni deplasarea desenului
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 0.0, 0.0);

    // 1. Desenăm cercul circumscris
    deseneazaCerc(razaCerc);

    // 2. Calculăm latura L a triunghiului echilateral înscris în cerc
    double lungimeLatura = razaCerc * sqrt(3.0);
    
    // 3. Poziționăm țestoasa în vârful de sus al cercului (0, R)
    Turtle t(0.0, razaCerc);
    
    // CORELARE GEOMETRICĂ: Pentru a desena în sensul acelor de ceasornic
    // și a lăsa fractalul să crească spre EXTERIOR, trebuie să începem
    // coborând spre dreapta-jos (unghi de -60 grade).
    t.turn(-60); 

    // 4. Desenăm cele 3 laturi
    for (int i = 0; i < 3; i++) {
        deseneazaLinieKoch(t, lungimeLatura, nivelRecursivitate);
        t.turn(-120); // La fiecare colț, ne întoarcem spre interiorul triunghiului
    }

    // Afișăm textul informativ
    std::string info = "Recursion Level: " + std::to_string(nivelRecursivitate);
    afiseazaText(-0.95, -0.95, info);

    glutSwapBuffers();
}

void Reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Păstrăm proporția corectă (cerc perfect, nu oval)
    float aspect = (float)w / (float)h;
    if (w >= h)
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    else
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == '+' || key == '=') {
        if (nivelRecursivitate < 7) nivelRecursivitate++;
    } else if (key == '-' || key == '_') {
        if (nivelRecursivitate > 0) nivelRecursivitate--;
    }
    glutPostRedisplay(); // Cere desenarea noului cadru
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Koch Snowflake - Rezolvare Corecta");
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape); 
    glutKeyboardFunc(Keyboard);
    
    glutMainLoop();
    return 0;
}
*/ 

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*
#include "glut.h"
#include <cmath>
#include <string>

const double PI = 3.14159265358979323846;

int nivelRecursivitateArbore = 2; 
const double razaCercArbore = 0.6; // Păstrăm variabila doar pentru a calcula lungimea tulpinii

class TurtleArbore {
public:
    double x, y, unghi;
    TurtleArbore(double startX, double startY) : x(startX), y(startY), unghi(0) {}

    void move(double distanta, bool deseneaza) {
        double xNou = x + distanta * cos(unghi * PI / 180.0);
        double yNou = y + distanta * sin(unghi * PI / 180.0);
        if (deseneaza) {
            glBegin(GL_LINES);
            glVertex2d(x, y);
            glVertex2d(xNou, yNou);
            glEnd();
        }
        x = xNou;
        y = yNou;
    }

    void turn(double grade) {
        unghi += grade;
    }
};

void deseneazaArboreFractal(TurtleArbore t, double lungime, int nivel, double unghiRamificare, double factorScalare) {
    if (nivel == 0) {
        t.move(lungime, true); 
    } else {
        t.move(lungime, true); 

        TurtleArbore tStateStanga = t;
        TurtleArbore tStateDreapta = t;

        tStateStanga.turn(unghiRamificare);
        deseneazaArboreFractal(tStateStanga, lungime * factorScalare, nivel - 1, unghiRamificare, factorScalare);

        tStateDreapta.turn(-unghiRamificare);
        deseneazaArboreFractal(tStateDreapta, lungime * factorScalare, nivel - 1, unghiRamificare, factorScalare);
    }
}

void afiseazaTextArbore(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void DisplayArbore() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 0.0, 0.0); 

    // --- CERCUL A FOST ELIMINAT COMPLET DE AICI ---

    // 1. Tulpina pleacă de jos, orientată în sus
    TurtleArbore t(0.0, -razaCercArbore);
    t.turn(90); 

    // Parametrii care permit fractalului să se extindă
    double lungimeTulpinainitiala = razaCercArbore; 
    double unghiRamificareArbore = 45.0;            
    double factorScalareArbore = 0.7;               

    // 2. Desenăm copacul
    deseneazaArboreFractal(t, lungimeTulpinainitiala, nivelRecursivitateArbore, unghiRamificareArbore, factorScalareArbore);

    std::string info = "Recursion Level: " + std::to_string(nivelRecursivitateArbore);
    afiseazaTextArbore(-0.95, -0.95, info);

    glutSwapBuffers();
}

void ReshapeArbore(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)w / (float)h;
    if (w >= h)
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    else
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
}

void KeyboardArbore(unsigned char key, int x, int y) {
    if (key == '+' || key == '=') {
        if (nivelRecursivitateArbore < 12) nivelRecursivitateArbore++; 
    } else if (key == '-' || key == '_') {
        if (nivelRecursivitateArbore > 0) nivelRecursivitateArbore--;
    }
    glutPostRedisplay(); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Arbore Fractal - Fara Cerc");
    
    glClearColor(1.0, 1.0, 1.0, 1.0); 
    
    glutDisplayFunc(DisplayArbore);
    glutReshapeFunc(ReshapeArbore); 
    glutKeyboardFunc(KeyboardArbore);
    
    glutMainLoop();
    return 0;
}
*/

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*
#include "glut.h"
#include <cmath>
#include <string>

const double PI = 3.14159265358979323846;

// Setăm nivelul de pornire la 2 pentru a coincide exact cu prima ta imagine
int nivelRecursivitate = 2; 

// --- Clasa Turtle Graphics (Singurul loc unde există glBegin) ---
class Turtle {
public:
    double x, y, unghi;
    Turtle(double startX, double startY) : x(startX), y(startY), unghi(0) {}

    void move(double distanta, bool deseneaza) {
        double xNou = x + distanta * cos(unghi * PI / 180.0);
        double yNou = y + distanta * sin(unghi * PI / 180.0);
        // Doar funcția move a țestoasei are voie să deseneze linii
        if (deseneaza) {
            glBegin(GL_LINES);
            glVertex2d(x, y);
            glVertex2d(xNou, yNou);
            glEnd();
        }
        x = xNou;
        y = yNou;
    }

    void turn(double grade) {
        unghi += grade;
    }
};

// --- Funcție Ajutătoare Respectând Restricția ---
// În loc să folosim glBegin(GL_QUADS), forțăm o țestoasă nouă să traseze un pătrat
void deseneazaPatratCuTestoasa(double cx, double cy, double latura) {
    // Calculăm colțul din stânga-jos al pătratului
    double startX = cx - latura / 2.0;
    double startY = cy - latura / 2.0;
    
    Turtle t(startX, startY);
    t.unghi = 0; // Orientată spre dreapta
    
    // Țestoasa trasează cele 4 laturi
    for (int i = 0; i < 4; i++) {
        t.move(latura, true);
        t.turn(90);
    }
}

// --- LOGICA RECURSIVĂ: Găurile din Covorul Sierpiński ---
void deseneazaCovorSierpinski(double cx, double cy, double dimensiune, int nivel) {
    if (nivel == 0) return;

    // 1. Pătratul central are întotdeauna o treime din dimensiunea regiunii curente
    double dimensiuneGaura = dimensiune / 3.0;
    
    // Desenăm "gaura" din centru folosind țestoasa
    deseneazaPatratCuTestoasa(cx, cy, dimensiuneGaura);

    // 2. Apelăm recursiv pentru cele 8 sub-regiuni din jurul centrului
    if (nivel > 1) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; // Sărim peste centrul deja desenat
                
                double centruNouX = cx + dx * dimensiuneGaura;
                double centruNouY = cy + dy * dimensiuneGaura;
                
                deseneazaCovorSierpinski(centruNouX, centruNouY, dimensiuneGaura, nivel - 1);
            }
        }
    }
}

void afiseazaText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 0.0, 0.0); // Roșu

    // Dimensiunea pătratului exterior maxim
    double laturaMaxima = 1.8;

    // 1. Desenăm marginea exterioară (Nivelul 0 de bază)
    deseneazaPatratCuTestoasa(0.0, 0.0, laturaMaxima);

    // 2. Pornim recursivitatea care decupează "găurile" în interior
    deseneazaCovorSierpinski(0.0, 0.0, laturaMaxima, nivelRecursivitate);

    // 3. Afișăm textul în colțul stânga-jos
    std::string info = "Recursion Level: " + std::to_string(nivelRecursivitate);
    afiseazaText(-0.95, -0.95, info);

    glutSwapBuffers();
}

void Reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)w / (float)h;
    if (w >= h)
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    else
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
}

void Keyboard(unsigned char key, int x, int y) {
    if (key == '+' || key == '=') {
        // Limităm la nivelul 5. Dincolo de nivelul 5 (aprox. 32.000 de pătrate) 
        // programul poate îngheța în funcție de procesor.
        if (nivelRecursivitate < 5) nivelRecursivitate++; 
    } else if (key == '-' || key == '_') {
        if (nivelRecursivitate > 0) nivelRecursivitate--;
    }
    glutPostRedisplay(); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Sierpiński Carpet - Turtle Graphics");
    
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fundal Alb
    
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape); 
    glutKeyboardFunc(Keyboard);
    
    glutMainLoop();
    return 0;
}
*/

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// mai bine de atat nu pot


/*
#include "glut.h" 
#include <cmath>
#include <string>

// Constanta PI pentru calculul unghiurilor
const double PI = 3.14159265358979323846;

// Setăm startul la Nivelul 2 
int nivelRecursivitateArrowhead = 2; 

// --- Clasa Turtle Graphics ---
class TurtleArrowhead {
public:
    double x, y, unghi;
    
    TurtleArrowhead(double startX, double startY) : x(startX), y(startY), unghi(0) {}

    void move(double distanta, bool deseneaza) {
        double xNou = x + distanta * cos(unghi * PI / 180.0);
        double yNou = y + distanta * sin(unghi * PI / 180.0);
        if (deseneaza) {
            glBegin(GL_LINES);
            glVertex2d(x, y);
            glVertex2d(xNou, yNou);
            glEnd();
        }
        x = xNou;
        y = yNou;
    }

    void turn(double grade) {
        unghi += grade;
    }
};

// --- LOGICA RECURSIVĂ: Curba Sierpinski Arrowhead ---
void deseneazaArrowheadCurve(TurtleArrowhead &t, double lungime, int nivel, double turn_angle) {
    if (nivel == 0) {
        t.move(lungime, true); 
    } else {
        // CORECȚIA 1: Lungimea se împarte la 2, nu la 3!
        double l = lungime / 2.0;

        deseneazaArrowheadCurve(t, l, nivel - 1, -turn_angle); 
        t.turn(turn_angle);                                   
        deseneazaArrowheadCurve(t, l, nivel - 1, turn_angle);  
        t.turn(turn_angle);                                   
        deseneazaArrowheadCurve(t, l, nivel - 1, -turn_angle); 
    }
}

void afiseazaTextArrowhead(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void DisplayArrowhead() {
    glClear(GL_COLOR_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 0.0, 0.0); 

    // CORECȚIA 2: Mutăm startul complet în stânga-jos (-0.9, -0.9)
    // pentru a avea loc să deseneze de la stânga la dreapta
    TurtleArrowhead t(-0.9, -0.9);
    double lungimeTotala = 1.8;

    // CORECȚIA 3: Regula de Paritate (din algoritmul original de pe Wikipedia)
    // Dacă nivelul este impar, trebuie să rotim țestoasa inițial pentru ca
    // triunghiul să crească în sus, nu în jos.
    if (nivelRecursivitateArrowhead % 2 == 0) {
        // Nivel par (ex: 0, 2, 4, 6)
        deseneazaArrowheadCurve(t, lungimeTotala, nivelRecursivitateArrowhead, 60.0);
    } else {
        // Nivel impar (ex: 1, 3, 5)
        t.turn(60.0);
        deseneazaArrowheadCurve(t, lungimeTotala, nivelRecursivitateArrowhead, -60.0);
    }

    std::string info = "Recursion Level: " + std::to_string(nivelRecursivitateArrowhead);
    afiseazaTextArrowhead(-0.95, -0.95, info);

    glutSwapBuffers(); 
}

void ReshapeArrowhead(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    float aspect = (float)w / (float)h;
    if (w >= h)
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    else
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
}

void KeyboardArrowhead(unsigned char key, int x, int y) {
    if (key == '+' || key == '=') {
        if (nivelRecursivitateArrowhead < 8) nivelRecursivitateArrowhead++; 
    } else if (key == '-' || key == '_') {
        if (nivelRecursivitateArrowhead > 0) nivelRecursivitateArrowhead--;
    }
    glutPostRedisplay(); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800); 
    glutCreateWindow("Sierpinski Arrowhead Curve Perfect");
    
    glClearColor(1.0, 1.0, 1.0, 1.0); 
    
    glutDisplayFunc(DisplayArrowhead);
    glutReshapeFunc(ReshapeArrowhead); 
    glutKeyboardFunc(KeyboardArrowhead);
    
    glutMainLoop(); 
    return 0;
}

*/

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// julia-fatou a iesit prea...straveziu 

/*
#include "glut.h"
#include <cmath>
#include <iostream>

// Rezoluția ferestrei - păstrăm 800x800 pentru aspect ratio 1:1
const int WIDTH = 800;
const int HEIGHT = 800;

// Constantele exacte cerute (a = -0.81, b = -0.2)
const double a = -0.81;
const double b = -0.2;

// Parametrii de iterație
const int MAX_ITER = 256;
const double RAZA_EVADARE_SQ = 4.0; // r=2.0 -> r^2=4.0

// --- FUNCȚIA DE COLORARE CALIBRATĂ ---
// Aceasta generează gradientul: Albastru -> Verde -> Galben -> Roșu
void set_color(int iteratii) {
    if (iteratii == MAX_ITER) {
        // Interiorul fractalului este NEGRU
        glColor3f(0.0f, 0.0f, 0.0f);
    } else {
        // Factor de scalare pentru densitatea culorilor
        // Ajustăm 't' pentru a trece prin tot spectrul vizibil
        float t = (float)iteratii / 15.0f; 

        // Algoritm de colorare sinusoidal defazat pentru paleta "Rainbow"
        // Red: maxim la iterații mari (lângă fractal)
        // Green: maxim la iterații medii
        // Blue: maxim la iterații mici (fundal)
        float r = 0.5f + 0.5f * std::sin(t + 3.1415f); // Defazaj 180 grade
        float g = 0.5f + 0.5f * std::sin(t + 1.5707f); // Defazaj 90 grade
        float bl = 0.5f + 0.5f * std::sin(t);          // Fără defazaj

        glColor3f(r, g, bl);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    // Zoom-ul imaginii: [-1.3, 1.3] oferă o încadrare similară cu poza ta
    // Folosim o scară simetrică pentru a evita deformarea
    double view_scale = 1.3;

    for (int px = 0; px < WIDTH; px++) {
        for (int py = 0; py < HEIGHT; py++) {
            
            // 1. Mapăm pixelul (px, py) în planul complex
            double x = -view_scale + (2.0 * view_scale * px) / WIDTH;
            double y = -view_scale + (2.0 * view_scale * py) / HEIGHT;

            // Julia: z0 este coordonata pixelului
            double zx = x;
            double zy = y;
            int iter = 0;

            // 2. Bucla fractalului: z = z^2 + c
            // Optimizat: calculăm pătratele o singură dată pe iterație
            double zx2 = zx * zx;
            double zy2 = zy * zy;

            while (zx2 + zy2 < RAZA_EVADARE_SQ && iter < MAX_ITER) {
                zy = 2.0 * zx * zy + b;
                zx = zx2 - zy2 + a;
                
                zx2 = zx * zx;
                zy2 = zy * zy;
                iter++;
            }

            // 3. Aplicăm culoarea și desenăm punctul
            set_color(iter);
            glVertex2d(x, y);
        }
    }

    glEnd();
    glutSwapBuffers();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Fundalul ferestrei
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Coordonatele vederii (match cu view_scale)
    gluOrtho2D(-1.3, 1.3, -1.3, 1.3);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Julia-Fatou - Full Gradient Palette");

    init();
    glutDisplayFunc(display);
    
    std::cout << "Se genereaza fractalul... ai rabdare oleaca." << std::endl;
    
    glutMainLoop();
    return 0;
}

*/


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/* mandelbrot stil dexter

#include "glut.h"
#include <cmath>
#include <iostream>

// Rezoluția ferestrei
const int WIDTH = 800;
const int HEIGHT = 800;

// Parametrii de iterație pentru detalii fine
const int MAX_ITER = 500;
const double ESCAPE_RADIUS_SQ = 4.0; 

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    // Mapăm vizualizarea pentru a cuprinde întreg setul Mandelbrot
    // Planul complex: x intre [-2.0, 1.0], y intre [-1.5, 1.5]
    double x_min = -2.1, x_max = 0.6;
    double y_min = -1.35, y_max = 1.35;

    for (int px = 0; px < WIDTH; px++) {
        for (int py = 0; py < HEIGHT; py++) {
            
            // 1. Mapăm pixelul (px, py) la coordonata complexă c = x0 + y0 * i
            double x0 = x_min + (x_max - x_min) * px / WIDTH;
            double y0 = y_min + (y_max - y_min) * py / HEIGHT;

            // Mandelbrot: z0 = 0
            double zx = 0.0;
            double zy = 0.0;
            int iter = 0;

            // 2. Aplicăm regula recursivă: z_{n+1} = z_n^2 + c
            while (zx * zx + zy * zy < ESCAPE_RADIUS_SQ && iter < MAX_ITER) {
                double xtemp = zx * zx - zy * zy + x0;
                zy = 2.0 * zx * zy + y0;
                zx = xtemp;
                iter++;
            }

            // 3. Logica de colorare (Cerința: Fractal Roșu, Fundal Alb)
            if (iter == MAX_ITER) {
                // Punctul aparține setului (nu a evadat) -> ROȘU
                glColor3f(1.0f, 0.0f, 0.0f);
            } else {
                // Punctul a evadat -> ALB
                glColor3f(1.0f, 1.0f, 1.0f);
            }

            glVertex2d(x0, y0);
        }
    }

    glEnd();
    glutSwapBuffers();
}

void init() {
    // Fundal alb implicit
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Setăm proiecția să se potrivească cu intervalul mapat mai sus
    gluOrtho2D(-2.1, 0.6, -1.35, 1.35);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mandelbrot Set - Red on White");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
*/

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// alt fractal straveziu

/*
#include "glut.h"
#include <cmath>
#include <iostream>

// Rezoluția ferestrei
const int WIDTH = 800;
const int HEIGHT = 800;

// Parametrii de iterație pentru detalii fine
const int MAX_ITER = 256;
const double ESCAPE_RADIUS_SQ = 4.0; 

// --- FUNCȚIA DE COLORARE CALIBRATĂ ---
// Generează gradientul: Albastru (exterior) -> Cyan -> Verde -> Galben -> Roșu (margine)
void set_mandelbrot_color(int iter) {
    if (iter == MAX_ITER) {
        // Interiorul setului este NEGRU
        glColor3f(0.0f, 0.0f, 0.0f);
    } else {
        // Factor de scalare pentru densitatea culorilor (frecvența benzilor)
        float t = (float)iter / 15.0f; 

        // Algoritm sinusoidal pentru paleta de culori "Rainbow"
        // Calibrat pentru a începe cu Albastru la iterații mici
        float r = 0.5f + 0.5f * std::sin(t + 3.1415f); // Roșu (maxim la marginea setului)
        float g = 0.5f + 0.5f * std::sin(t + 1.5707f); // Verde
        float b = 0.5f + 0.5f * std::sin(t + 0.0000f); // Albastru (maxim în exterior)

        glColor3f(r, g, b);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    // Mapăm vizualizarea pentru a cuprinde întreg setul Mandelbrot fără deformare
    // Interval standard: x în [-2.1, 0.6], y în [-1.35, 1.35]
    double x_min = -2.1, x_max = 0.6;
    double y_min = -1.35, y_max = 1.35;

    for (int px = 0; px < WIDTH; px++) {
        for (int py = 0; py < HEIGHT; py++) {
            
            // 1. Mapăm pixelul (px, py) la coordonata complexă c = x0 + y0 * i
            double x0 = x_min + (x_max - x_min) * px / WIDTH;
            double y0 = y_min + (y_max - y_min) * py / HEIGHT;

            // Mandelbrot: punctul de start z0 este mereu (0,0)
            double zx = 0.0;
            double zy = 0.0;
            int iter = 0;

            // Optimizare: calculăm pătratele o singură dată pe iterație
            double zx2 = 0.0;
            double zy2 = 0.0;

            // 2. Aplicăm regula recursivă: z_{n+1} = z_n^2 + c
            while (zx2 + zy2 < ESCAPE_RADIUS_SQ && iter < MAX_ITER) {
                zy = 2.0 * zx * zy + y0;
                zx = zx2 - zy2 + x0;
                
                zx2 = zx * zx;
                zy2 = zy * zy;
                iter++;
            }

            // 3. Aplicăm culoarea bazată pe numărul de iterații
            set_mandelbrot_color(iter);
            glVertex2d(x0, y0);
        }
    }

    glEnd();
    glutSwapBuffers();
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Fundal negru până la randare
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Setăm proiecția ortografică pentru a se potrivi cu planul complex mapat
    gluOrtho2D(-2.1, 0.6, -1.35, 1.35);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mandelbrot Set - Full Color Gradient");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
*/