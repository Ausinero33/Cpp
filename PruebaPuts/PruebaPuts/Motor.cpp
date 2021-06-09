//#include "Planeta.h"
#include<stdio.h>
#include <math.h>
#include <Windows.h>
#include <chrono>
#include <iostream>

#define WIDTHS 80
#define WIDTH 159
#define HEIGHT 49
#define PI 3.14159265359

void drawPoint(CHAR_INFO* c, int x, int y, int limX, int limY);
void inizialiceArr(CHAR_INFO* c, int limX, int limY);

void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}

class Shape {
    virtual void create(float angle) = 0;
    virtual void draw(CHAR_INFO* screen) = 0;
};

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    float elapsed() const {
        return std::chrono::duration_cast<second_>
            (clock_::now() - beg_).count();
    }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<float, std::ratio<1> > second_;
    std::chrono::time_point<clock_> beg_;
};

void gotoxy(short x, short y) {
    COORD coord = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

float dist(float x0, float y0, float x1, float y1) {
    return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}

class Vector2D {
public:
    float x;
    float y;

    Vector2D(float x, float y) {
        this->x = x;
        this->y = y;
    }
};

class Point{
public:
    float* x;
    float* y;

    Point(float* x, float* y) {
        this->x = x;
        this->y = y;
    }
};

class Line {
public:
    Point* p1;
    Point* p2;

    Line(Point* p1, Point* p2) {
        this->p1 = p1;
        this->p2 = p2;
    }
};

class Circle {
public:
    Point* center;
    float* rad;

    Circle(Point* center, float* rad) {
        this->center = center;
        this->rad = rad;
    }
};

class Elipse : Shape {
private:
    Point* center;
    Point* a;
    Point* b;
    Point* f1, * f2;
    float nX, nY, f1x, f2x;
public:
    float ejeHor, ejeVer;
    float c;
    CHAR_INFO* elipse;

    Elipse(Point* center, float* h, float* w) {
        this->center = center;
        this->ejeHor = *w;
        this->ejeVer = *h;
        nX = *center->x + ejeHor;
        nY = *center->y + ejeVer;
        elipse = new CHAR_INFO[(4 * ejeHor + 2) * (2 * ejeHor + 1)];
        inizialiceArr(elipse, 4 * ejeHor + 2, 2 * ejeHor + 1);

        this->a = new Point(&nX, center->y);
        this->b = new Point(center->x, &nY);

        this->c = sqrt(ejeHor * ejeHor - ejeVer * ejeVer);
        f1x = round(*center->x - c);
        f2x = round(*center->x + c);
        f1 = new Point(&f1x, center->y);
        f2 = new Point(&f2x, center->y);
    }

    void Shape::create(float angle) {
        int cRelX = (2 * ejeHor + 1) / 2;
        int cRelY = (2 * ejeHor + 1) / 2;
        int f1Xrel = round(cRelX - (cos(angle) * c));
        int f1Yrel = round(cRelX + (sin(angle) * c));
        int f2Xrel = round(cRelX + (cos(angle) * c));
        int f2Yrel = round(cRelX - (sin(angle) * c));
        inizialiceArr(elipse, 4 * ejeHor + 2, 2 * ejeHor + 1);

        for (int i = 0; i < (2 * ejeHor + 1); i++) {
            for (int j = 0; j < (2 * ejeHor + 1); j++) {
                if (dist(i, j, f1Xrel, f1Yrel) + dist(i, j, f2Xrel, f2Yrel) <= 2 * ejeHor) {
                    drawPoint(elipse, i, j, 4 * ejeHor + 2, 2 * ejeHor + 1);
                }
            }
        }
    }

    void Shape::draw(CHAR_INFO* screen) {
        int pivotX = (*center->x - ejeHor) * 2;
        int pivotX2 = (*center->x + ejeHor) * 2 + 1;
        int pivotY = *center->y - ejeHor;
        int k = 0;
        
        for (int j = pivotY, j2 = 0; j <= *center->y + ejeHor; j++, j2++) {
            for (int i = pivotX, i2 = 0; i <= pivotX2; i++, i2++) {
                if(elipse[k].Char.UnicodeChar != 0x0021)
                    screen[j * WIDTH + i] = elipse[k];
                k++;
            }
        }
    }

    void actualizarElipse() {
        nX = *center->x + ejeHor;
        nY = *center->y + ejeVer;
        f1x = *center->x - c;
        f2x = *center->x + c;
    }

    Point* getCenter() {
        return center;
    }

    Point* getA() {
        return a;
    }

    Point* getB() {
        return b;
    }

    Point* getF1() {
        return f1;
    }

    Point* getF2() {
        return f2;
    }
};

void inizialiceArr(CHAR_INFO* c, int limX, int limY) {
    for (int i = 0; i < limX; i++) {
        for (int j = 0; j < limY; j++) {
            c[j * limX + i].Char.UnicodeChar = 0x0021;
            c[j * limX + i].Attributes = 0x0004;
        }
    }
}

void inizialiceArr(CHAR_INFO* c) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            c[i * WIDTH + j].Char.UnicodeChar = 0x0020;
            c[i * WIDTH + j].Attributes = 0x0000;
        }
    }
}

void drawPoint(CHAR_INFO* c, Point* p) {
    c[((int)*p->y % HEIGHT) * WIDTH + 2 * ((int)*p->x % WIDTHS)].Char.UnicodeChar = 0x2588;
    c[((int)*p->y % HEIGHT) * WIDTH + 2 * ((int)*p->x % WIDTHS)].Attributes = 0x000F;
    c[((int)*p->y % HEIGHT) * WIDTH + 2 * (((int)*p->x % WIDTHS) + 1)].Char.UnicodeChar = 0x2588;
    c[((int)*p->y % HEIGHT) * WIDTH + 2 * (((int)*p->x % WIDTHS) + 1)].Attributes = 0x000F;
}

void drawPoint(CHAR_INFO* c, int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        c[y * WIDTH + 2 * x].Char.UnicodeChar = 0x2588;
        c[y * WIDTH + 2 * x].Attributes = 0x000F;
        c[y * WIDTH + 2 * x + 1].Char.UnicodeChar = 0x2588;
        c[y * WIDTH + 2 * x + 1].Attributes = 0x000F;
    }
}

void drawPoint(CHAR_INFO* c, int x, int y, int limX, int limY) {
    if (x >= 0 && x < limX && y >= 0 && y < limY) {
        c[y * limX + 2 * x].Char.UnicodeChar = 0x2588;
        c[y * limX + 2 * x].Attributes = 0x000F;
        c[y * limX + 2 * x + 1].Char.UnicodeChar = 0x2588;
        c[y * limX + 2 * x + 1].Attributes = 0x000F;
    }
}

void dwLH(CHAR_INFO* c, int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }

    int D = (2 * dx) - dy;
    int x = x0;

    for (int y = y0; y < y1; y++) {
        drawPoint(c, x, y);
        if (D > 0) {
            x += xi;
            D += (2 * (dx - dy));
        }
        else {
            D += 2 * dx;
        }
    }
}

void dwlL(CHAR_INFO* c, int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;

    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }

    int D = (2 * dy) - dx;
    int y = y0;

    for (int x = x0; x < x1; x++) {
        drawPoint(c, x, y);
        if (D > 0) {
            y += yi;
            D += (2 * (dy - dx));
        }
        else {
            D += 2 * dy;
        }
    }
}

void drawLine(CHAR_INFO* c, Line* line) {
    int dx = *line->p2->x - *line->p1->x;
    int dy = *line->p2->y - *line->p1->y;

    if (abs(dy) < abs(dx)) {
        if (*line->p1->x > *line->p2->x)
            dwlL(c, *line->p2->x, *line->p2->y, *line->p1->x, *line->p1->y);
        else
            dwlL(c, *line->p1->x, *line->p1->y, *line->p2->x, *line->p2->y);
    }
    else {
        if (*line->p1->y > *line->p2->y)
            dwLH(c, *line->p2->x, *line->p2->y, *line->p1->x, *line->p1->y);
        else
            dwLH(c, *line->p1->x, *line->p1->y, *line->p2->x, *line->p2->y);
    }
}

void drawCircle(CHAR_INFO* c, Circle* cir) {
    for (int i = *cir->center->x - *cir->rad; i <= *cir->center->x + *cir->rad; i++) {
        for (int j = *cir->center->y - *cir->rad; j <= *cir->center->y + *cir->rad; j++) {
            if (i > 0 && j > 0 && dist(i, j, *cir->center->x, *cir->center->y) <= *cir->rad) {
                drawPoint(c, i, j);
            }
        }
    }
}

void drawEllipse(CHAR_INFO* c, Elipse* el) {
    for (int i = *el->getCenter()->x - el->ejeHor; i <= *el->getCenter()->x + el->ejeHor; i++) {
        for (int j = *el->getCenter()->y - el->ejeVer; j <= *el->getCenter()->y + el->ejeVer; j++) {
            if (dist(i, j, *el->getF1()->x, *el->getF1()->y) + dist(i, j, *el->getF2()->x, *el->getF2()->y) <= 2 * el->ejeHor) {
                drawPoint(c, i, j);
            }
        }
    }
}

void cambiarColor(int k) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}

int main() {
    const float fps = 60.0f;
    const float dt = 1.0f / fps;
    Timer tmr;
    float frameStart = tmr.elapsed();
    float frameTime = dt;
    float acc = 0;

    CHAR_INFO* m_bufScreen = new CHAR_INFO[WIDTH * HEIGHT];
    SMALL_RECT m_rectWindow = { 0, 0, (short)WIDTH - 1, (short)HEIGHT - 1 };
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    float x1 = 40, x2 = 4;
    float y1 = 20, y2 = 15;
    float h = 10, w = 20;
    float angle = 0;
    Point* p1 = new Point(&x1, &y1);
    Point* p2 = new Point(&x2, &y2);
    Line* l1 = new Line(p1, p2);
    Circle* cir = new Circle(p1, &x2);
    Elipse* el = new Elipse(p1, &h, &w);

    Vector2D* speed = new Vector2D(1, 0);

    ShowConsoleCursor(false);

    while (true) {
        inizialiceArr(m_bufScreen);
        gotoxy(0, 0);

        frameStart = tmr.elapsed();

        /*
        if (acc >= 1) {
            x1 += speed->x;// *dt;
            y1 += speed->y;// *dt;
            acc = 0;
        }
        acc += dt;

        if ((int)(*cir->center->x + *cir->rad) >= WIDTHS - 2 || (int)(*cir->center->x - *cir->rad) <= 1) {
            speed->x = -speed->x;
            
        }
        if ((int)(*cir->center->y + *cir->rad) >= HEIGHT - 1 || (int)(*cir->center->y - *cir->rad) <= 1) {
            speed->y = -speed->y;
        }

        */
        //el->actualizarElipse();
        //drawEllipse(m_bufScreen, el);
        //drawCircle(m_bufScreen, cir);
        
        el->create(angle);
        el->draw(m_bufScreen);

        angle += dt / 20;

        //drawLine(m_bufScreen, l1);

        //c[HEIGHT * WIDTH - 1] = '\0';
        WriteConsoleOutput(hConsole, m_bufScreen, {(short)WIDTH, (short)HEIGHT}, {0,0}, &m_rectWindow);

        frameTime = tmr.elapsed() - frameStart;

        if (dt > frameTime) {
            Sleep(dt - frameTime);
        }
    }
}
/*
int main() {
    const float fps = 60.0f;
    const float dt = 1.0f / fps;
    float accumulator = 0.0f;
    Timer tmr;
    float frameStart = tmr.elapsed();

    char c[HEIGHT * WIDTH];

    float x1 = 10, x2 = 4;
    float y1 = 10, y2 = 8;
    int h = 7, w = 15;
    Point* p1 = new Point(&x1, &y1);
    Point* p2 = new Point(&x2, &y2);
    Line* l1 = new Line(p1, p2);
    Circle* cir = new Circle(p1, &x2);
    Elipse el(p1, h, w);
    Vector2D* speed = new Vector2D(2, 2);

    ShowConsoleCursor(false);

    while (true) {

        inizialiceArr(c);
        gotoxy(0, 0);
        float currentTime = tmr.elapsed();
        accumulator += currentTime - frameStart;

        frameStart = currentTime;


        if (accumulator >= 1.0f / 30.0f)
            accumulator = 1.0f / 30.0f;

        while (accumulator > dt) {
            //x1 += speed->x * 4 * dt;
            //y1 += speed->y * 4 * dt;
            accumulator -= dt;
        }

        //x1 = floor(x1);
        //y1 = floor(y1);

        if ((int) (*cir->center->x + *cir->rad) >= WIDTHS - 1 || (int) (*cir->center->x - *cir->rad) <= 1)
            speed->x = -speed->x;
        if ((int) (*cir->center->y + *cir->rad) >= HEIGHT - 1 || (int) (*cir->center->y - *cir->rad) <= 1)
            speed->y = -speed->y;


        c[HEIGHT * WIDTH - 1] = '\0';
        drawEllipse(c, &el);
        //drawLine(c, l1);
        puts(c);
    }
}
*/
/*
int man() {
    char c[HEIGHT * WIDTH];

    float rad = 15;
    float w = 15, h = 7;
    float x1 = 18, x2 = 52,
        y1 = 20, y2 = 20;

    inizialiceArr(c);
    Point* p1 = new Point(&x1, &y1);
    Point* p2 = new Point(&x2, &y2);
    //drawPoint(c, 10, 10);
    //drawPoint(c, new Point(80, 10));
    //Line* linea = new Line(p1, p2);
    //drawLine(c, linea);
    //drawPoint(c, p1);
    //drawPoint(c, p2);
    //drawCircle(c, new Circle(p1, &rad));
    Elipse* el = new Elipse(p1, &h, &w);
    drawEllipse(c, el);
    drawCircle(c, new Circle(p2, &rad));

    c[HEIGHT * WIDTH - 1] = '\0';

    puts(c);
    gotoxy(0, 0);

    while (1);
}
*/