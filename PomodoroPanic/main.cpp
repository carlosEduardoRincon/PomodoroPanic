// by Carlos Eduardo Rincon Junior - m322015
// Game: Pomodoro Panic 
// 
// IMPORTS

#include <gl/glut.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>

// STRUCTS

struct Player 
{
    GLfloat xpos, ypos;
    GLfloat size;
    GLfloat r, g, b;
};

struct Tomato
{
    GLfloat xpos, ypos;
    GLfloat size;
    GLfloat r, g, b;
    bool left, right;
};

struct Deposit
{
    GLfloat xpos, ypos;
    GLfloat size;
    GLfloat r, g, b;
};

struct MenuButtom
{
    GLfloat xpos, ypos;
    GLfloat size;
    GLfloat r, g, b;
};

struct Ground
{
    GLfloat r, g, b;
};

struct Background
{
    GLfloat r, g, b;
};

struct Difficult
{
    int time, minimalScore;
};


// ENUMS

enum State
{ 
    InitialMenu, 
    Gameplay,
    WinGame,
    LoseGame,
    MenuHelp
};

enum GameColorMode
{
    BlackMode,
    OriginalMode,
    LightMode
};


// DECLARATIONS

State gameState;
Player player;
Deposit deposit;
Ground ground;
Background background;
MenuButtom menuStartButtom, menuHelpButtom, menuQuitButtom;
Tomato tomato;
Difficult difficult;
std::vector<Tomato> tomatos;
std::vector<Tomato> selectDifficutTomatos;
float posicaoY = -150.0f;
float jumpHeight = 100.0f;
bool jumpIsActive = false;
int tempo = 0;
int view_w;
int view_h;
int score = 0;
int currentTomato = 0;
float userSelectR = 0.0f;
float userSelectG = 0.0f;
float userSelectB = 1.0f;


// UTILS METHODS

template <typename Iterator>
Iterator collectTomato(Iterator tomato) {
    return tomatos.erase(tomato);
}

void renderBitmapString(float x, float y, void* font, const char* string) 
{
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        if (*c == '\n') {
            glTranslatef(0.0f, -10.0f, 0.0f);
        }
        glutBitmapCharacter(font, *c);
    }
}

void incrementTimer(int value)
{
    tempo++;
    glutTimerFunc(1000, incrementTimer, 0);
    
    glutPostRedisplay();
}

void ChangeSize(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    view_w = w;
    view_h = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-250, 250, -250, 250, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void menuOptions(int item)
{
    switch (item)
    {
    case BlackMode:
    {
        ground.r = 0.15f;
        ground.g = 0.01f;
        ground.b = 0.11f;

        background.r = 0.17f;
        background.g = 0.22f;
        background.b = 0.22f;

        deposit.r = 0.43f;
        deposit.g = 0.59f;
        deposit.b = 0.53f;

        tomato.r = 0.87f;
        tomato.g = 0.89f;
        tomato.b = 0.8f;
    } break;

    case OriginalMode:
    {
        deposit.r = 0.25f;
        deposit.g = 0.23f;
        deposit.b = 0.2f;

        ground.r = 0.83f;
        ground.g = 0.39f;
        ground.b = 0.23f;

        background.r = 0.58f;
        background.g = 0.78f;
        background.b = 0.71f;

        tomato.r = 1.0f;
        tomato.g = 0.0f;
        tomato.b = 0.0f;
    } break;

    case LightMode:
    {
        ground.r = 0.9f;
        ground.g = 0.79f;
        ground.b = 0.55f;

        background.r = 0.90f;
        background.g = 0.70f;
        background.b = 0.60f;

        deposit.r = 0.345f;
        deposit.g = 0.42f;
        deposit.b = 0.40f;

        tomato.r = 0.88f;
        tomato.g = 0.47f;
        tomato.b = 0.52f;
    } break;
    }
    glutPostRedisplay();
}

void initDifficultTomatos(void)
{
    if (selectDifficutTomatos.size() <= 0) {
        Tomato newTomatoEasy;
        newTomatoEasy.xpos = -400.0f;
        newTomatoEasy.ypos = -50.0f;
        newTomatoEasy.size = 10.0f;
        newTomatoEasy.left = true;
        newTomatoEasy.right = false;
        newTomatoEasy.r = 0.0f;
        newTomatoEasy.g = 1.0f;
        newTomatoEasy.b = 0.0f;

        selectDifficutTomatos.push_back(newTomatoEasy);

        Tomato newTomatoHard;
        newTomatoHard.xpos = 400.0f;
        newTomatoHard.ypos = 00.0f;
        newTomatoHard.size = 10.0f;
        newTomatoHard.right = true;
        newTomatoHard.left = false;
        newTomatoHard.r = 0.5f;
        newTomatoHard.g = 0.0f;
        newTomatoHard.b = 0.5f;

        selectDifficutTomatos.push_back(newTomatoHard);
    }
}

void selectDifficult(void)
{
    for (std::vector<Tomato>::iterator tm = selectDifficutTomatos.begin(); tm != selectDifficutTomatos.end(); ) {
        if ((player.xpos + (player.size / 2)) >= tm->xpos/2 && (player.xpos + (player.size / 2)) <= tm->xpos/2 + 20) {
           
            if (tm->left) {
                player.r = tm->r;
                player.b = tm->b;
                player.g = tm->g;

                userSelectR = tm->r;
                userSelectG = tm->g;
                userSelectB = tm->b;

                difficult.minimalScore = 10;
                difficult.time = 60;
            }
            else if (tm->right) {
                player.r = tm->r;
                player.b = tm->b;
                player.g = tm->g;

                userSelectR = tm->r;
                userSelectG = tm->g;
                userSelectB = tm->b;

                difficult.minimalScore = 10;
                difficult.time = 30;
            }
            
            glutPostRedisplay();
            break;
        }
        ++tm;
    }
}


// DRAW METHODS

void drawTimer(void)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(210.0, 220.0);

    char buffer[60];
    snprintf(buffer, 60, "%d", tempo);

    renderBitmapString(200, 200, GLUT_BITMAP_HELVETICA_18, "Time: ");
    for (char* c = buffer; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void drawTomatos(void)
{
    for (std::vector<Tomato>::iterator tm = tomatos.begin(); tm != tomatos.end(); ) {
        glPushMatrix();
        glColor3f(tomato.r, tomato.g, tomato.b);
        glTranslatef(tm->xpos, -150.0f, 0.0f);
        glRectf(-6.0f, 0.0f, tm->size, tm->size);
        glutPostRedisplay();
        ++tm;
    } 
}

void drawDifficultTomatos(void)
{
    for (std::vector<Tomato>::iterator tm = selectDifficutTomatos.begin(); tm != selectDifficutTomatos.end(); ) {
        glPushMatrix();
        glColor3f(tm->r, tm->g, tm->b);
        glTranslatef(tm->xpos, tm->ypos, 0.0f);
        glRectf(-6.0f, 0.0f, tm->size, tm->size);
        glutPostRedisplay();
        ++tm;
    }
}

void drawDeposit(void)
{   
    glColor3f(deposit.r, deposit.g, deposit.b);
    glPushMatrix();
    glTranslatef(deposit.xpos -10, -110.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(deposit.size , 2 * deposit.size - 10);
    glVertex2f(2 * deposit.size, 0.0f);
    glEnd();
    glPopMatrix();

    glColor3f(deposit.r, deposit.g, deposit.b);
    glPushMatrix();
    glTranslatef(deposit.xpos, -150.0f, 0.0f);
    glRectf(0.0f, 0.0f, deposit.size, deposit.size * 2);
    glPopMatrix();
 
}

void drawGameButtons(void)
{
    MenuButtom menuRestartButtom;
    menuRestartButtom.xpos = 0.0f;
    menuRestartButtom.ypos = 200.0f;
    menuRestartButtom.size = 15.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(menuRestartButtom.xpos, menuRestartButtom.ypos, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuRestartButtom.size, -menuRestartButtom.size);
    glVertex2f(menuRestartButtom.size, -menuRestartButtom.size);
    glVertex2f(menuRestartButtom.size, menuRestartButtom.size);
    glVertex2f(-menuRestartButtom.size, menuRestartButtom.size);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-7 , -5, GLUT_BITMAP_TIMES_ROMAN_24, "<-");
}

void drawPlayer(void)
{
    glColor3f(player.r, player.g, player.b);
    glTranslatef(player.xpos, player.ypos, 0.0f);
    glRectf(-6.0f, 0.0f, player.size, player.size);
    
    for (std::vector<Tomato>::iterator tm = tomatos.begin(); tm != tomatos.end(); ) {
        if ((player.xpos + (player.size / 2)) >= tm->xpos && (player.xpos + (player.size / 2)) <= tm->xpos + 20
            && currentTomato == 0) {
            tm = tomatos.erase(tm);
            currentTomato = 1;
            player.r = tomato.r;
            player.b = tomato.g;
            player.g = tomato.g;
            glutPostRedisplay();
            break;
        }
        ++tm;
    }

    if ((player.xpos + (player.size / 2)) >= deposit.xpos && (player.xpos + (player.size / 2)) <= deposit.xpos + 20) {
        if (currentTomato == 1) {
            score++;
            currentTomato = 0;
            player.r = userSelectR;
            player.g = userSelectG;
            player.b = userSelectB;
            glutPostRedisplay();
        }

    }
}

void drawGround(void)
{
    glOrtho(-1, 2, -2, 2, -1, 1);
    glColor3f(ground.r , ground.g, ground.b);
    glBegin(GL_QUADS);
    glTexCoord2f(-520.0f, -300.0f); glVertex2f(-520.0f, -300.0f);
    glTexCoord2f(520.0f, -300.0f); glVertex2f(520.0f, -300.0f);
    glTexCoord2f(520.0f, -500.0f); glVertex2f(520.0f, -500.0f);
    glTexCoord2f(-520.0f, -500.0f); glVertex2f(-520.0f, -500.0f);
    glEnd();
    glFlush();
    glutPostRedisplay();
}

void drawDifficult(void)
{
    MenuButtom menuEasyDifficultButtom;
    menuEasyDifficultButtom.xpos = -200.0f;
    menuEasyDifficultButtom.ypos = -100.0f;
    menuEasyDifficultButtom.size = 20.0f;

    MenuButtom menuHardDifficultButtom;
    menuHardDifficultButtom.xpos = 400.0f;
    menuHardDifficultButtom.ypos = 0.0f;
    menuHardDifficultButtom.size = 20.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(menuEasyDifficultButtom.xpos, menuEasyDifficultButtom.ypos, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuEasyDifficultButtom.size, -menuEasyDifficultButtom.size / 2);
    glVertex2f(menuEasyDifficultButtom.size, -menuEasyDifficultButtom.size / 2);
    glVertex2f(menuEasyDifficultButtom.size, menuEasyDifficultButtom.size / 2);
    glVertex2f(-menuEasyDifficultButtom.size, menuEasyDifficultButtom.size / 2);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-10, -5, GLUT_BITMAP_TIMES_ROMAN_24, "Easy");

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(menuHardDifficultButtom.xpos, menuHardDifficultButtom.ypos, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuHardDifficultButtom.size, -menuHardDifficultButtom.size / 2);
    glVertex2f(menuHardDifficultButtom.size, -menuHardDifficultButtom.size / 2);
    glVertex2f(menuHardDifficultButtom.size, menuHardDifficultButtom.size / 2);
    glVertex2f(-menuHardDifficultButtom.size, menuHardDifficultButtom.size / 2);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-10, -5, GLUT_BITMAP_TIMES_ROMAN_24, "Hard");

}

void drawMenuButtoms(void)
{   
    menuStartButtom.xpos = 0.0f;
    menuStartButtom.ypos = 120.0f;
    menuStartButtom.size = 60.0f;

    menuHelpButtom.xpos = 0.0f;
    menuHelpButtom.ypos = -70.0f;
    menuHelpButtom.size = 60.0f;

    menuQuitButtom.xpos = 0.0f;
    menuQuitButtom.ypos = -70.0f;
    menuQuitButtom.size = 60.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(menuStartButtom.xpos, menuStartButtom.ypos, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuStartButtom.size, -menuStartButtom.size/2);
    glVertex2f(menuStartButtom.size, -menuStartButtom.size/2);
    glVertex2f(menuStartButtom.size, menuStartButtom.size/2);
    glVertex2f(-menuStartButtom.size, menuStartButtom.size/2);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-10, -5, GLUT_BITMAP_TIMES_ROMAN_24, "Start");

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(menuHelpButtom.xpos, menuHelpButtom.ypos, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuHelpButtom.size, -menuHelpButtom.size / 2);
    glVertex2f(menuHelpButtom.size, -menuHelpButtom.size / 2);
    glVertex2f(menuHelpButtom.size, menuHelpButtom.size / 2);
    glVertex2f(-menuHelpButtom.size, menuHelpButtom.size / 2);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-10, -5, GLUT_BITMAP_TIMES_ROMAN_24, "Help");

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(menuQuitButtom.xpos, menuQuitButtom.ypos, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuQuitButtom.size, -menuQuitButtom.size / 2);
    glVertex2f(menuQuitButtom.size, -menuQuitButtom.size / 2);
    glVertex2f(menuQuitButtom.size, menuQuitButtom.size / 2);
    glVertex2f(-menuQuitButtom.size, menuQuitButtom.size / 2);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-10, -5, GLUT_BITMAP_TIMES_ROMAN_24, "Quit");

}

void drawMenu(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(background.r, background.g, background.b, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawMenuButtoms();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawGround();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawPlayer();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawDifficult();
    initDifficultTomatos();
    drawDifficultTomatos();
    selectDifficult();

    glutSwapBuffers();
}

void drawMenuLoseGame(void)
{   
    glClearColor(background.r, background.g, background.b, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0, 20, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-50, -50);
    glVertex2f(50, -50);
    glVertex2f(50, 25);
    glVertex2f(-50, 25);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-20, -20, GLUT_BITMAP_TIMES_ROMAN_24, "You lose!");
    glPopMatrix();

    MenuButtom menuRestartButtom;
    menuRestartButtom.xpos = 0.0f;
    menuRestartButtom.ypos = 200.0f;
    menuRestartButtom.size = 15.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(menuRestartButtom.xpos, menuRestartButtom.ypos, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuRestartButtom.size, -menuRestartButtom.size);
    glVertex2f(menuRestartButtom.size, -menuRestartButtom.size);
    glVertex2f(menuRestartButtom.size, menuRestartButtom.size);
    glVertex2f(-menuRestartButtom.size, menuRestartButtom.size);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-7, -5, GLUT_BITMAP_TIMES_ROMAN_24, "<-");

    glutSwapBuffers();
}

void drawMenuWinGame()
{
    glClearColor(background.r, background.g, background.b, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0, 20, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-50, -50);
    glVertex2f(50, -50);
    glVertex2f(50, 25);
    glVertex2f(-50, 25);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-20, -20, GLUT_BITMAP_TIMES_ROMAN_24, "You win!");
    glPopMatrix();

    MenuButtom menuRestartButtom;
    menuRestartButtom.xpos = 0.0f;
    menuRestartButtom.ypos = 200.0f;
    menuRestartButtom.size = 15.0f;

    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(menuRestartButtom.xpos, menuRestartButtom.ypos, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuRestartButtom.size, -menuRestartButtom.size);
    glVertex2f(menuRestartButtom.size, -menuRestartButtom.size);
    glVertex2f(menuRestartButtom.size, menuRestartButtom.size);
    glVertex2f(-menuRestartButtom.size, menuRestartButtom.size);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-7, -5, GLUT_BITMAP_TIMES_ROMAN_24, "<-");

    glutSwapBuffers();

}

void drawScore(void)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-210.0, 220.0);

    char buffer[60];
    snprintf(buffer, 60, "%d", score);

    renderBitmapString(-240, 200, GLUT_BITMAP_HELVETICA_18, "Score: ");
    for (char* c = buffer; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glutPostRedisplay();
}

void drawMenuHelp(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(background.r, background.g, background.b , 0.0);
    glClearColor(background.r, background.g, background.b , 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawGround();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawPlayer();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawDifficult();
    initDifficultTomatos();
    drawDifficultTomatos();
    selectDifficult();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(0, 20, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-100, -100);
    glVertex2f(100, -100);
    glVertex2f(100, 200);
    glVertex2f(-100, 200);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-80, 150, GLUT_BITMAP_TIMES_ROMAN_24, "About: ");
    renderBitmapString(-80, 120, GLUT_BITMAP_HELVETICA_12, "The objective of the game is to collect as many points");
    renderBitmapString(-80, 110, GLUT_BITMAP_HELVETICA_12, "as possible and deliver them to the collection point before");
    renderBitmapString(-80, 100, GLUT_BITMAP_HELVETICA_12, "time runs out. The collection point is the house in");
    renderBitmapString(-80, 90, GLUT_BITMAP_HELVETICA_12, "the left corner of the screen. To earn a point, it's necessary");
    renderBitmapString(-80, 80, GLUT_BITMAP_HELVETICA_12, "to pass over a  \"tomato\", and the character will automatically");
    renderBitmapString(-80, 70, GLUT_BITMAP_HELVETICA_12, "change color, symbolizing that it has captured the item.");
    renderBitmapString(-80, 50, GLUT_BITMAP_HELVETICA_12, "The game has three difficulties:");
    renderBitmapString(-80, 40, GLUT_BITMAP_HELVETICA_12, "- Easy: collect 10 points in 60 seconds;");
    renderBitmapString(-80, 30, GLUT_BITMAP_HELVETICA_12, "- Normal: collect 10 points in 45 seconds;");
    renderBitmapString(-80, 20, GLUT_BITMAP_HELVETICA_12, "- Hard: collect 10 points in 30 seconds;");
    renderBitmapString(-80, 10, GLUT_BITMAP_HELVETICA_12, "You can check your points in the upper left corner,");
    renderBitmapString(-80, 0, GLUT_BITMAP_HELVETICA_12, "while the time will be displayed in the upper right corner.");
    renderBitmapString(-80, -30, GLUT_BITMAP_HELVETICA_12, "Have fun playing! ");
    renderBitmapString(-80, -40, GLUT_BITMAP_HELVETICA_12, "by Carlos Eduardo Rincon Junior -m322015");
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glTranslatef(-200, 200, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-menuQuitButtom.size/2, -menuQuitButtom.size / 4);
    glVertex2f(menuQuitButtom.size/2, -menuQuitButtom.size / 4);
    glVertex2f(menuQuitButtom.size/2, menuQuitButtom.size / 4);
    glVertex2f(-menuQuitButtom.size/2, menuQuitButtom.size / 4);
    glEnd();
    glColor3f(0.0f, 0.0f, 0.0f);
    renderBitmapString(-10, -5, GLUT_BITMAP_TIMES_ROMAN_24, "Back");
    glPopMatrix();

    glutSwapBuffers();
}


// INIT METHODS

void initDifficult(void) 
{
    difficult.minimalScore = 10;
    difficult.time = 45;

}

void initTomatos(void)
{
    float numAleatorio = rand() / 100;

    if (numAleatorio > 220.0f) {
        numAleatorio = -numAleatorio / 3;
    }

    if (tomatos.size() <= 0 && numAleatorio != 0) {
        Tomato newTomato;
        newTomato.xpos = numAleatorio;
        newTomato.ypos = 0;
        newTomato.size = 10.0f;
        tomatos.push_back(newTomato);
    }
}

void initDeposit(void)
{
    deposit.xpos = -235.0f;
    deposit.ypos = 0;
    deposit.size = 20;
    deposit.r = 0.25f;
    deposit.g = 0.23f;
    deposit.b = 0.2f;
}

void initPlayer(void)
{
    player.xpos = 0;
    player.ypos = -150.0f;
    player.size = 15;
    player.r = 0;
    player.g = 0;
    player.b = 1;
}

void initGround(void)
{
    ground.r = 0.83f;
    ground.g = 0.39f;
    ground.b = 0.23f;
}

void initMenu(void)
{
    glutCreateMenu(menuOptions);
    glutAddMenuEntry("Black mode", BlackMode);
    glutAddMenuEntry("Original mode", OriginalMode);
    glutAddMenuEntry("Light mode", LightMode);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void initBackground(void)
{
    background.r = 0.58f;
    background.g = 0.78f;
    background.b = 0.71f;
}

void initInicialTomato(void)
{
    tomato.r = 1.0f;
    tomato.g = 0.0f;
    tomato.b = 0.0f;
}


// RULES

void verifyGameRule(void)
{
    if (tempo < difficult.time && score == difficult.minimalScore) {
        gameState = WinGame;
    }

    if (tempo == difficult.time) {
        gameState = LoseGame;
    }
}


// PRINCIPAL METHODS

void drawGame(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(background.r, background.g, background.b, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawGround();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawDeposit();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawPlayer();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawTimer();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawScore();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    initTomatos();
    drawTomatos();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawGameButtons();

    verifyGameRule();

    glutSwapBuffers();
}

void initGame(void)
{
    switch (gameState)
    {
    case InitialMenu:
        drawMenu();
        tempo = 0;
        score = 0;
        currentTomato = 1;
        break;
    case MenuHelp:
        drawMenuHelp();
        tempo = 0;
        break;
    case Gameplay:
        drawGame();
        break;
    case WinGame:
        drawMenuWinGame();
        break;
    case LoseGame:
        drawMenuLoseGame();
        break;
    default:
        break;
    }
}


// CONTROLS/HANDLERS

void HandleMouse(int key, int state, int x, int y)
{
    int centerX = view_w / 2;
    int centerY = view_h / 2;
    x = x - centerX;
    y = centerY - y;

    if (gameState == InitialMenu) {
        if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (x >= -130 && x <= 130 &&
                150 <= y && y <= 240) {
                currentTomato = 0;
                gameState = Gameplay;
            }
            else if (x >= -130 && x <= 130 &&
                50 <= y && y <= 135) {
                gameState = MenuHelp;
            } 
            else if (x >= -130 && x <= 130 &&
                -55 <= y && y <= 30) {
                exit(0);
            }
        }
    }
    else if (gameState == Gameplay) {
        if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (x >= -30 && x <= 32 &&
                300 <= y && y <= 332) {
                gameState = InitialMenu;
                player.r = 0.0f;
                player.g = 0.0f;
                player.b = 1.0f;
                difficult.time = 45;
            }
        }
    }
    else if (gameState == WinGame) {
        if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (x >= -30 && x <= 32 &&
                300 <= y && y <= 332) {
                gameState = InitialMenu;
                player.r = 0.0f;
                player.g = 0.0f;
                player.b = 1.0f;
                difficult.time = 45;
            }
        }
    }
    else if (gameState == LoseGame) {
        if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (x >= -30 && x <= 32 &&
                300 <= y && y <= 332) {
                gameState = InitialMenu;
                player.r = 0.0f;
                player.g = 0.0f;
                player.b = 1.0f;
                difficult.time = 45;
            }
        }
    }
    else if (gameState == MenuHelp) {
        if (key == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (x >= -500 && x <= -370 &&
                290 <= y && y <= 340) {
                gameState = InitialMenu;
            }
        }
    }
    
}

void PlayerJump(void)
{
    jumpIsActive = true;
    float jumpVelocity = 0.5f;
    float initialPosition = player.ypos;

    while (player.ypos < initialPosition + jumpHeight) {
        player.ypos += jumpVelocity;
        initGame();
        glutPostRedisplay();
    }
    while (player.ypos > initialPosition) {
        player.ypos -= jumpVelocity;
        initGame();
        glutPostRedisplay();
    }
    jumpIsActive = false;
}

void HandleKeyBoard(unsigned char key, int x, int y)
{
    float moveXSize = 10.0f;
    switch (key) {
    case 'a':
    case 'A':
        if (!((player.size / 2) + player.xpos == -240)) {
            player.xpos -= moveXSize;
            glutPostRedisplay();
        }
        break;
    case 'd':
    case 'D':
        if (!((player.size / 2) + player.xpos == 240)) {
            player.xpos += moveXSize;
            glutPostRedisplay();
        }
        break;
    case 'w':
    case 'W':
        if (!jumpIsActive) {
            PlayerJump();
            glutPostRedisplay();
        }
        break;
    case 'q':
    case 'Q':
        exit(0);
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }

}

void HandleSpecialKeys(int key, int x, int y)
{
    float moveXSize = 10.0f;
    switch (key) {
    case GLUT_KEY_LEFT:

        if (!((player.size / 2) + player.xpos == -240)) {
            player.xpos -= moveXSize;
            glutPostRedisplay();
        }
        break;
    case GLUT_KEY_RIGHT:
        if (!((player.size / 2) + player.xpos == 240)) {
            player.xpos += moveXSize;
            glutPostRedisplay();
        }
        break;
    case GLUT_KEY_UP:
        if (!jumpIsActive) {
            PlayerJump();
            glutPostRedisplay();
            jumpIsActive = true;
        }
        break;
    default:
        break;
    }
}


// MAIN

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1080, 780);
    glutCreateWindow("Pomodoro Panic");
    glutDisplayFunc(initGame);
    glutKeyboardFunc(HandleKeyBoard);
    glutMouseFunc(HandleMouse);
    glutSpecialFunc(HandleSpecialKeys);
    glutReshapeFunc(ChangeSize);
    glutTimerFunc(1000, incrementTimer, 0);

    initDifficult();
    initPlayer();
    initDeposit();
    initGround();
    initBackground();
    initInicialTomato();
    initMenu();
    gameState = InitialMenu;
   
    glutMainLoop();
}
