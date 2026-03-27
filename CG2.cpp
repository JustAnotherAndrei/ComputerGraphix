/*

#include "glut.h"
#include <cmath>
#include <string>
 
// Constante                                                         

static const int   DISC_SEGMENTS = 48;    // segmente pentru disc umplut    
static const int   CIRCLE_SEGS   = 16;    // 16 segmente pentru un cerc non-perfect (poligonal), ca în imaginea de referință 
static const int   BRUSH_RADIUS  = 1;     // raza brush in celule           
static const float DISC_RATIO    = 0.42f; // disc = 42% din celula          
static const float PIf           = 3.14159265358979f;


// Stare interna                                                     


static int   gGridW    = 30;
static int   gGridH    = 30;
static float gCellSize = 20.f;  // dimensiunea celulei, in pixeli ecran 
static float gOffX     = 0.f;   // offset stanga-jos al grilei          
static float gOffY     = 0.f;

// ================================================================== 
// Functii interne                                                   
// ================================================================== 

// Vertexul (gx, gy) in coordonate ecran (pixeli) 
static void gridToScreen(int gx, int gy, float &sx, float &sy)
{
    // Am eliminat "+ 0.5f" pt a pozitiona centrul discului pe liniile grilei, nu în centrul celulei.
    sx = gOffX + (float)gx * gCellSize;
    sy = gOffY + (float)gy * gCellSize;
}

//
// Deseneaza un disc plin la (sx, sy) cu raza r in pixeli.
// SINGURUL loc din biblioteca unde apare glBegin() pentru discuri.
//
static void drawDisc(float sx, float sy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(sx, sy);
        for (int i = 0; i <= DISC_SEGMENTS; ++i) {
            float a = 2.f * PIf * i / DISC_SEGMENTS;
            glVertex2f(sx + r * cosf(a), sy + r * sinf(a));
        }
    glEnd();
}


// raster_init  —  Seteaza dimensiunile grilei (numar de celule).

void raster_init(int gridW, int gridH)
{
    gGridW = gridW;
    gGridH = gridH;
}


// raster_reshape  —  Garantează celule pătrate și centrate.

void raster_reshape(int windowW, int windowH)
{
    glViewport(0, 0, windowW, windowH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)windowW, 0.0, (double)windowH, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float cellW = (float)windowW / (float)gGridW;
    float cellH = (float)windowH / (float)gGridH;
    gCellSize   = (cellW < cellH) ? cellW : cellH;

    gOffX = ((float)windowW - gCellSize * (float)gGridW) * 0.5f;
    gOffY = ((float)windowH - gCellSize * (float)gGridH) * 0.5f;
}

//
// raster_drawGrid  —  Deseneaza liniile grilei.
//
void raster_drawGrid(void)
{
    glColor3f(0.75f, 0.75f, 0.75f);
    glLineWidth(1.0f);

    float left   = gOffX;
    float right  = gOffX + gGridW * gCellSize;
    float bottom = gOffY;
    float top    = gOffY + gGridH * gCellSize;

    glBegin(GL_LINES);
        for (int i = 0; i <= gGridW; ++i) {
            float x = gOffX + i * gCellSize;
            glVertex2f(x, bottom);
            glVertex2f(x, top);
        }
        for (int j = 0; j <= gGridH; ++j) {
            float y = gOffY + j * gCellSize;
            glVertex2f(left,  y);
            glVertex2f(right, y);
        }
    glEnd();
}


// raster_setPixel  —  Deseneaza un disc negru la vertexul (x, y)

void raster_setPixel(int x, int y)
{
    // vertexurile sunt de la 0 la GRID_SIZE inclusiv
    if (x < 0 || x > gGridW || y < 0 || y > gGridH) return;
    float sx, sy;
    gridToScreen(x, y, sx, sy);
    glColor3f(0.05f, 0.05f, 0.05f);
    drawDisc(sx, sy, gCellSize * DISC_RATIO);
}


// raster_drawLineSegment  —  Rasterizeazaun segment de dreapta


void raster_drawLineSegment(int x1, int y1, int x2, int y2)
{
    // 1. Primitiva geometrica albastra 
    {
        float sx1, sy1, sx2, sy2;
        gridToScreen(x1, y1, sx1, sy1);
        gridToScreen(x2, y2, sx2, sy2);
        glColor3f(0.15f, 0.15f, 0.95f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
            glVertex2f(sx1, sy1);
            glVertex2f(sx2, sy2);
        glEnd();
    }

    // 2. Rasterizare Bresenham + brush 
    int dx =  abs(x2 - x1), stepX = (x1 < x2) ? 1 : -1;
    int dy = -abs(y2 - y1), stepY = (y1 < y2) ? 1 : -1;
    int err = dx + dy;
    int cx = x1, cy = y1;

    for (;;) {
        for (int bx = -BRUSH_RADIUS; bx <= BRUSH_RADIUS; ++bx)
            for (int by = -BRUSH_RADIUS; by <= BRUSH_RADIUS; ++by)
                if (bx*bx + by*by <= BRUSH_RADIUS*BRUSH_RADIUS + BRUSH_RADIUS)
                    raster_setPixel(cx + bx, cy + by);

        if (cx == x2 && cy == y2) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; cx += stepX; }
        if (e2 <= dx) { err += dx; cy += stepY; }
    }
}

//
// raster_drawCircle  —  Rasterizează un cerc.
//
void raster_drawCircle(int cx, int cy, int r)
{
    // 1. Primitiva geometrica albastra: cerc non-perfect (poligonal) 
    {
        float scx, scy;
        gridToScreen(cx, cy, scx, scy);

        float sr = (float)r * gCellSize;

        glColor3f(0.15f, 0.15f, 0.95f);
        glLineWidth(1.5f);
        glBegin(GL_LINE_LOOP);
            // Acum CIRCLE_SEGS este 16, deci cercul albastru va fi un poligon cu 16 laturi
            for (int i = 0; i < CIRCLE_SEGS; ++i) {
                float angle = 2.f * PIf * (float)i / (float)CIRCLE_SEGS;
                glVertex2f(scx + sr * cosf(angle),
                           scy + sr * sinf(angle));
            }
        glEnd();
    }

    // 2. Rasterizare Midpoint Circle + brush 
    auto plotPoints = [&](int px, int py) {
        int pts[8][2] = {
            { cx+px, cy+py }, { cx-px, cy+py },
            { cx+px, cy-py }, { cx-px, cy-py },
            { cx+py, cy+px }, { cx-py, cy+px },
            { cx+py, cy-px }, { cx-py, cy-px }
        };
        for (auto &p : pts)
            for (int bx = -BRUSH_RADIUS; bx <= BRUSH_RADIUS; ++bx)
                for (int by = -BRUSH_RADIUS; by <= BRUSH_RADIUS; ++by)
                    if (bx*bx + by*by <= BRUSH_RADIUS*BRUSH_RADIUS + BRUSH_RADIUS)
                        raster_setPixel(p[0]+bx, p[1]+by);
    };

    int px = 0, py = r;
    int d  = 3 - 2 * r;
    plotPoints(px, py);
    while (px <= py) {
        if (d < 0) {
            d += 4*px + 6;
        } else {
            d += 4*(px - py) + 10;
            --py;
        }
        ++px;
        plotPoints(px, py);
    }
}

// ================================================================== 
// Demo / main                                                        
// ================================================================== 

static const int GRID = 40;

void display(void)
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    raster_drawGrid();

    // Poziționăm centrul pe vertexul din mijloc
    int cx = GRID / 2;
    int cy = GRID / 2;
    int r  = GRID / 3;
    raster_drawCircle(cx, cy, r);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    raster_reshape(w, h);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Raster Library");

    raster_init(GRID, GRID);
    raster_reshape(700, 700);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

*/


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*

#include "glut.h"
#include <cmath>
#include <string>
#include <algorithm> // Pentru std::swap

// ================================================================== 
// Constante                                                         
// ================================================================== 

static const int   DISC_SEGMENTS = 48;    // segmente pentru disc umplut    
static const int   CIRCLE_SEGS   = 9;    // n laturi pentru poligon
static const int   BRUSH_RADIUS  = 1;     // raza brush in celule           
static const float DISC_RATIO    = 0.42f; // disc = 42% din celula          
static const float PIf           = 3.14159265358979f;

// ================================================================== 
// Stare interna                                                     
// ================================================================== 

static int   gGridW    = 30;
static int   gGridH    = 30;
static float gCellSize = 20.f;  
static float gOffX     = 0.f;   
static float gOffY     = 0.f;

// ================================================================== 
// Functii interne                                                   
// ================================================================== 

// Vertexul (gx, gy) in coordonate ecran (pixeli) 
static void gridToScreen(int gx, int gy, float &sx, float &sy)
{
    sx = gOffX + (float)gx * gCellSize;
    sy = gOffY + (float)gy * gCellSize;
}

//
// Deseneaza un disc plin la (sx, sy) cu raza r in pixeli.
//
static void drawDisc(float sx, float sy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(sx, sy);
        for (int i = 0; i <= DISC_SEGMENTS; ++i) {
            float a = 2.f * PIf * i / DISC_SEGMENTS;
            glVertex2f(sx + r * cosf(a), sy + r * sinf(a));
        }
    glEnd();
}

// ================================================================== 
// API PUBLIC                                                        
// ================================================================== 

void raster_init(int gridW, int gridH)
{
    gGridW = gridW;
    gGridH = gridH;
}

void raster_reshape(int windowW, int windowH)
{
    glViewport(0, 0, windowW, windowH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)windowW, 0.0, (double)windowH, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float cellW = (float)windowW / (float)gGridW;
    float cellH = (float)windowH / (float)gGridH;
    gCellSize   = (cellW < cellH) ? cellW : cellH;

    gOffX = ((float)windowW - gCellSize * (float)gGridW) * 0.5f;
    gOffY = ((float)windowH - gCellSize * (float)gGridH) * 0.5f;
}

void raster_drawGrid(void)
{
    glColor3f(0.75f, 0.75f, 0.75f);
    glLineWidth(1.0f);

    float left   = gOffX;
    float right  = gOffX + gGridW * gCellSize;
    float bottom = gOffY;
    float top    = gOffY + gGridH * gCellSize;

    glBegin(GL_LINES);
        for (int i = 0; i <= gGridW; ++i) {
            float x = gOffX + i * gCellSize;
            glVertex2f(x, bottom);
            glVertex2f(x, top);
        }
        for (int j = 0; j <= gGridH; ++j) {
            float y = gOffY + j * gCellSize;
            glVertex2f(left,  y);
            glVertex2f(right, y);
        }
    glEnd();
}

void raster_setPixel(int x, int y)
{
    if (x < 0 || x > gGridW || y < 0 || y > gGridH) return;
    float sx, sy;
    gridToScreen(x, y, sx, sy);
    glColor3f(0.05f, 0.05f, 0.05f);
    drawDisc(sx, sy, gCellSize * DISC_RATIO);
}

//
// raster_drawLineSegment — TASK 2: BRESENHAM MID-POINT LINE 
// Contine exact 4 sub-algoritmi, fara inmultiri in interiorul buclelor.
//
void raster_drawLineSegment(int x1, int y1, int x2, int y2)
{
    // 1. Primitiva geometrica albastra (se traseaza direct intre punctele originale)
    {
        float sx1, sy1, sx2, sy2;
        gridToScreen(x1, y1, sx1, sy1);
        gridToScreen(x2, y2, sx2, sy2);
        glColor3f(0.15f, 0.15f, 0.95f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
            glVertex2f(sx1, sy1);
            glVertex2f(sx2, sy2);
        glEnd();
    }

    // Functie lambda pentru a pastra grosimea ceruta de laborator
    auto plotBrush = [&](int cx, int cy) {
        for (int bx = -BRUSH_RADIUS; bx <= BRUSH_RADIUS; ++bx)
            for (int by = -BRUSH_RADIUS; by <= BRUSH_RADIUS; ++by)
                if (bx*bx + by*by <= BRUSH_RADIUS*BRUSH_RADIUS + BRUSH_RADIUS)
                    raster_setPixel(cx + bx, cy + by);
    };

    // 2. Pentru a reduce cele 8 octante la doar 4 sub-algoritmi (Task 2.3),
    // ne asiguram ca linia se traseaza intotdeauna de la stanga la dreapta (x1 <= x2).
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = y2 - y1;

    // Aici incep cei 4 SUB-ALGORITMI (Task 2.3 si 2.4).
    // Inmultirile se fac DOAR in afara buclelor (Task 2.2).
    if (dy >= 0) {
        if (dx >= dy) {
            // --- SUB-ALGORITM 1: Panta intre 0 si 1 (Octant 1) ---
            int d = 2 * dy - dx;
            int incE = 2 * dy;
            int incNE = 2 * (dy - dx);
            int x = x1, y = y1;
            plotBrush(x, y);
            while (x < x2) {
                if (d <= 0) {
                    d += incE;  // DOAR adunari in interior
                    x++;
                } else {
                    d += incNE; // DOAR adunari in interior
                    x++;
                    y++;
                }
                plotBrush(x, y);
            }
        } else {
            // --- SUB-ALGORITM 2: Panta mai mare ca 1 (Octant 2) ---
            int d = 2 * dx - dy;
            int incN = 2 * dx;
            int incNE = 2 * (dx - dy);
            int x = x1, y = y1;
            plotBrush(x, y);
            while (y < y2) {
                if (d <= 0) {
                    d += incN;
                    y++;
                } else {
                    d += incNE;
                    x++;
                    y++;
                }
                plotBrush(x, y);
            }
        }
    } else {
        int abs_dy = -dy; // Lucram cu valoarea absoluta pentru simetrie
        if (dx >= abs_dy) {
            // --- SUB-ALGORITM 3: Panta intre -1 si 0 (Octant 8) ---
            int d = 2 * abs_dy - dx;
            int incE = 2 * abs_dy;
            int incSE = 2 * (abs_dy - dx);
            int x = x1, y = y1;
            plotBrush(x, y);
            while (x < x2) {
                if (d <= 0) {
                    d += incE;
                    x++;
                } else {
                    d += incSE;
                    x++;
                    y--;
                }
                plotBrush(x, y);
            }
        } else {
            // --- SUB-ALGORITM 4: Panta mai mica ca -1 (Octant 7) ---
            int d = 2 * dx - abs_dy;
            int incS = 2 * dx;
            int incSE = 2 * (dx - abs_dy);
            int x = x1, y = y1;
            plotBrush(x, y);
            while (y > y2) { // Aici 'y' scade
                if (d <= 0) {
                    d += incS;
                    y--;
                } else {
                    d += incSE;
                    x++;
                    y--;
                }
                plotBrush(x, y);
            }
        }
    }
}

//
// raster_drawPolygon — Creează poligonul care demonstrează toate octantele
//
void raster_drawPolygon(int cx, int cy, int r)
{
    int vertices[CIRCLE_SEGS][2];
    
    for (int i = 0; i < CIRCLE_SEGS; ++i) {
        // Pentru a indeplini conditia de a NU avea linii verticale/orizontale/prim diagonale,
        // adaugam un mic offset (+ 0.5f) la unghi. Astfel formam un poligon usor inclinat.
        float angle = 2.f * PIf * ((float)i + 0.123f) / (float)CIRCLE_SEGS;
        vertices[i][0] = cx + (int)round((float)r * cosf(angle));
        vertices[i][1] = cy + (int)round((float)r * sinf(angle));
    }

    // Rasterizam poligonul trecand prin fiecare varf
    for (int i = 0; i < CIRCLE_SEGS; ++i) {
        int next = (i + 1) % CIRCLE_SEGS;
        raster_drawLineSegment(vertices[i][0], vertices[i][1], vertices[next][0], vertices[next][1]);
    }
}

// ================================================================== 
// Demo / main                                                        
// ================================================================== 

static const int GRID = 40;

void display(void)
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    raster_drawGrid();

    // Poziționăm centrul pe vertexul din mijloc
    int cx = GRID / 2;
    int cy = GRID / 2;
    int r  = GRID / 3;
    
    // Inlocuim desenarea cercului cu desenarea poligonului cerut de task
    raster_drawPolygon(cx, cy, r);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    raster_reshape(w, h);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Raster Library - Bresenham Mid-Point Line");

    raster_init(GRID, GRID);
    raster_reshape(700, 700);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

*/


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/*

#include "glut.h"
#include <cmath>
#include <string>

// Constante                                                        

static const int   DISC_SEGMENTS = 48;    // segmente pentru disc umplut    
static const int   CIRCLE_SEGS   = 720;   // 720 pentru cercul albastru perfect
static const float DISC_RATIO    = 0.42f; // disc = 42% din celula          
static const float PIf           = 3.14159265358979f;


// Stare interna                                                     


static int   gGridW    = 40; // Marit un pic pentru a incapea cercul mare
static int   gGridH    = 40;
static float gCellSize = 20.f;  
static float gOffX     = 0.f;   
static float gOffY     = 0.f;


// Functii interne                                                   


// Vertexul (gx, gy) in coordonate ecran (pixeli) 
static void gridToScreen(int gx, int gy, float &sx, float &sy)
{
    // Coordonate pe intersectii (vertexuri)
    sx = gOffX + (float)gx * gCellSize;
    sy = gOffY + (float)gy * gCellSize;
}

// Deseneaza discul negru de rasterizare
static void drawDisc(float sx, float sy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(sx, sy);
        for (int i = 0; i <= DISC_SEGMENTS; ++i) {
            float a = 2.f * PIf * i / DISC_SEGMENTS;
            glVertex2f(sx + r * cosf(a), sy + r * sinf(a));
        }
    glEnd();
}


// API PUBLIC                                                        


void raster_init(int gridW, int gridH)
{
    gGridW = gridW;
    gGridH = gridH;
}

void raster_reshape(int windowW, int windowH)
{
    glViewport(0, 0, windowW, windowH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (double)windowW, 0.0, (double)windowH, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float cellW = (float)windowW / (float)gGridW;
    float cellH = (float)windowH / (float)gGridH;
    gCellSize   = (cellW < cellH) ? cellW : cellH;

    gOffX = ((float)windowW - gCellSize * (float)gGridW) * 0.5f;
    gOffY = ((float)windowH - gCellSize * (float)gGridH) * 0.5f;
}

void raster_drawGrid(void)
{
    glColor3f(0.75f, 0.75f, 0.75f);
    glLineWidth(1.0f);

    float left   = gOffX;
    float right  = gOffX + gGridW * gCellSize;
    float bottom = gOffY;
    float top    = gOffY + gGridH * gCellSize;

    glBegin(GL_LINES);
        for (int i = 0; i <= gGridW; ++i) {
            float x = gOffX + i * gCellSize;
            glVertex2f(x, bottom);
            glVertex2f(x, top);
        }
        for (int j = 0; j <= gGridH; ++j) {
            float y = gOffY + j * gCellSize;
            glVertex2f(left,  y);
            glVertex2f(right, y);
        }
    glEnd();
}

void raster_setPixel(int x, int y)
{
    if (x < 0 || x > gGridW || y < 0 || y > gGridH) return;
    float sx, sy;
    gridToScreen(x, y, sx, sy);
    glColor3f(0.05f, 0.05f, 0.05f);
    drawDisc(sx, sy, gCellSize * DISC_RATIO);
}


// Cele 4 functii distincte pentru desenarea si umplerea cercului (3.3, 3.4, 3.5)
// Desenand o linie orizontala intre capete, trasam simultan 
// si marginile (octantele), dar umplem si interiorul.

// Functia 1: Deseneaza octantele 2 si 3 (partea de sus, extrema)
void drawOctants2_and_3(int cx, int cy, int x, int y) {
    for (int i = cx - x; i <= cx + x; i++) {
        raster_setPixel(i, cy + y);
    }
}

// Functia 2: Deseneaza octantele 1 si 4 (partea de sus, interior)
void drawOctants1_and_4(int cx, int cy, int x, int y) {
    for (int i = cx - y; i <= cx + y; i++) {
        raster_setPixel(i, cy + x);
    }
}

// Functia 3: Deseneaza octantele 6 si 7 (partea de jos, extrema)
void drawOctants6_and_7(int cx, int cy, int x, int y) {
    for (int i = cx - x; i <= cx + x; i++) {
        raster_setPixel(i, cy - y);
    }
}

// Functia 4: Deseneaza octantele 5 si 8 (partea de jos, interior)
void drawOctants5_and_8(int cx, int cy, int x, int y) {
    for (int i = cx - y; i <= cx + y; i++) {
        raster_setPixel(i, cy - x);
    }
}


// TASK 3: Mid-point circle rasterisation algorithm 

void raster_drawFilledMidpointCircle(int cx, int cy, int r)
{
    // 1. Primitiva geometrica albastra ideala
    {
        float scx, scy;
        gridToScreen(cx, cy, scx, scy);
        float sr = (float)r * gCellSize;

        glColor3f(0.15f, 0.15f, 0.95f);
        glLineWidth(1.5f);
        glBegin(GL_LINE_LOOP);
            for (int i = 0; i < CIRCLE_SEGS; ++i) {
                float angle = 2.f * PIf * (float)i / (float)CIRCLE_SEGS;
                glVertex2f(scx + sr * cosf(angle), scy + sr * sinf(angle));
            }
        glEnd();
    }

    // 2. Algoritmul Mid-Point 
    // Task 3.2 (Doar int) si Task 3.1 (INMULTIRILE SUNT PERMISE DOAR AICI, INAINTEA BUCLEI!)
    int x = 0;
    int y = r;
    int d = 1 - r;               // Parametrul de decizie initial
    int incE = 3;                // Increment pentru cand alegem Est
    int incSE = 5 - 2 * r;       // Increment pentru cand alegem Sud-Est

    while (x <= y) {
        // Task 3.3 si 3.4: 4 functii distince
        // Acestea se ocupa si de Task 3.5 (umplerea interiorului)
        drawOctants2_and_3(cx, cy, x, y);
        drawOctants1_and_4(cx, cy, x, y);
        drawOctants6_and_7(cx, cy, x, y);
        drawOctants5_and_8(cx, cy, x, y);

        // Task 3.1
        if (d < 0) {
            d += incE;
            incE += 2;
            incSE += 2;
        } else {
            d += incSE;
            incE += 2;
            incSE += 4;
            y--;
        }
        x++;
    }
}


// Demo / main                                                        

void display(void)
{
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    raster_drawGrid();

    // Plasare centrala fix pe vertex
    int cx = gGridW / 2;
    int cy = gGridH / 2;
    int r  = 16; // O raza decenta pentru a reproduce a doua poza
    
    raster_drawFilledMidpointCircle(cx, cy, r);

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    raster_reshape(w, h);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutCreateWindow("Raster Library — Filled Mid-Point Circle");

    raster_init(40, 40);
    raster_reshape(700, 700);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}

*/