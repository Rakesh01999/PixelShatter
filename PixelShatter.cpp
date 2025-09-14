#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>
#include <string>
#include <ctime>

const int WIDTH = 1200, HEIGHT = 800;

// Game state
enum State { MENU, PLAYING, PAUSED, GAME_OVER, CREDITS } currentState = MENU;

// Ball
float ballX = 0.0f, ballY = -0.4f, ballDX = 0.008f, ballDY = 0.012f, ballRadius = 0.03f;
bool ballLaunched = false;

// Paddle
float paddleX = 0.0f, paddleWidth = 0.25f, paddleHeight = 0.08f;

// Bricks
const int ROWS = 6, COLS = 12;
bool bricks[ROWS][COLS];
float brickWidth = 0.14f, brickHeight = 0.06f;

// Game vars
int score = 0, lives = 3, level = 1, ballColor = 0, paddleColor = 2, brickColor = 1;
float menuAnim = 0;

// Colors
float colors[][3] = {{0.94f,0.35f,0.39f},{0.20f,0.68f,0.89f},{0.35f,0.89f,0.50f},{0.68f,0.51f,0.89f},
                     {0.98f,0.77f,0.27f},{0.27f,0.94f,0.89f},{0.95f,0.95f,0.95f},{1.00f,0.49f,0.31f}};

void resetBricks() {
    for(int i=0; i<ROWS; i++) for(int j=0; j<COLS; j++) bricks[i][j] = (rand()%3 != 0);
}

void resetGame() {
    ballX = paddleX; ballY = -0.4f; ballDX = 0.008f + level*0.002f; ballDY = 0.012f + level*0.003f;
    ballLaunched = false; paddleX = 0.0f; resetBricks(); currentState = PLAYING;
}

void startNewGame() { score = 0; lives = 3; level = 1; resetGame(); }

void drawText(float x, float y, const std::string& text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glRasterPos2f(x, y); for(char c : text) glutBitmapCharacter(font, c);
}

void drawButton(float x, float y, float w, float h, const std::string& text) {
    glColor4f(0.2f,0.2f,0.4f,0.8f);
    glBegin(GL_QUADS);
    glVertex2f(x-w/2,y-h/2); glVertex2f(x+w/2,y-h/2); glVertex2f(x+w/2,y+h/2); glVertex2f(x-w/2,y+h/2);
    glEnd();
    glColor3f(0.6f,0.8f,1.0f); glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x-w/2,y-h/2); glVertex2f(x+w/2,y-h/2); glVertex2f(x+w/2,y+h/2); glVertex2f(x-w/2,y+h/2);
    glEnd();
    glColor3f(1,1,1); drawText(x-text.length()*0.015f, y-0.02f, text);
}

void drawBall() {
    glColor3fv(colors[ballColor]);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ballX, ballY);
    for(int i=0; i<=32; i++) {
        float angle = i*2*M_PI/32;
        glVertex2f(ballX + cos(angle)*ballRadius, ballY + sin(angle)*ballRadius);
    }
    glEnd();
}

void drawPaddle() {
    glColor3fv(colors[paddleColor]);
    glBegin(GL_QUADS);
    glVertex2f(paddleX-paddleWidth/2, -0.9f); glVertex2f(paddleX+paddleWidth/2, -0.9f);
    glVertex2f(paddleX+paddleWidth/2, -0.9f+paddleHeight); glVertex2f(paddleX-paddleWidth/2, -0.9f+paddleHeight);
    glEnd();
}

void drawBricks() {
    for(int i=0; i<ROWS; i++) {
        for(int j=0; j<COLS; j++) {
            if(bricks[i][j]) {
                float x = -1.0f + j*(brickWidth+0.02f) + 0.1f;
                float y = 0.7f - i*(brickHeight+0.02f);
                int color = (brickColor + i) % 8;
                glColor3fv(colors[color]);
                glBegin(GL_QUADS);
                glVertex2f(x,y); glVertex2f(x+brickWidth,y); glVertex2f(x+brickWidth,y+brickHeight); glVertex2f(x,y+brickHeight);
                glEnd();
            }
        }
    }
}

void drawBackground() {
    float t = menuAnim*0.01f;
    glBegin(GL_QUADS);
    glColor3f(0.05f+0.02f*sin(t), 0.05f+0.02f*cos(t*1.3f), 0.15f+0.03f*sin(t*0.7f));
    glVertex2f(-1,-1); glVertex2f(1,-1);
    glColor3f(0.15f+0.03f*sin(t*1.1f), 0.1f+0.02f*cos(t), 0.25f+0.04f*sin(t*1.5f));
    glVertex2f(1,1); glVertex2f(-1,1);
    glEnd();
}

void drawMenu() {
    drawBackground();

    // --- Title (centered) ---
    const char* title = "PIXELSHATTER";
    int titleWidth = 0;
    for (const char* c = title; *c; c++) {
        titleWidth += glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
    float ndcX = -((float)titleWidth / WIDTH);  // shift so it's centered

    glColor3f(0.4f+0.3f*sin(menuAnim*0.05f),
              0.8f+0.2f*cos(menuAnim*0.03f), 1.0f);
    drawText(ndcX, 0.6f + 0.1f*sin(menuAnim*0.02f),
             title, GLUT_BITMAP_TIMES_ROMAN_24);

    // --- Buttons ---
    drawButton(0, 0.2f, 0.4f, 0.15f, "NEW GAME");
    drawButton(0, -0.1f, 0.4f, 0.15f, "Info");
    drawButton(0, -0.4f, 0.6f, 0.15f, "Press Q to QUIT");

    // --- Controls text (multi-line, centered) ---
    const char* controls[] = {
        "Keyboard Shortcuts:",
        "N = New Game, C = Credits, Q = Quit",
        "Mouse = Move Paddle, Space = Launch Ball, P = Pause",
        "Left Click = Launch Ball, Right Click = Color Menu"
    };
    int numLines = sizeof(controls) / sizeof(controls[0]);

    glColor3f(0.8f,0.8f,0.8f);
    for (int i = 0; i < numLines; i++) {
        int lineWidth = 0;
        for (const char* c = controls[i]; *c; c++) {
            lineWidth += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, *c);
        }
        float ndcXc = -((float)lineWidth / WIDTH);
        float yPos = -0.6f - i * 0.08f;   // adjust vertical spacing between lines
        drawText(ndcXc, yPos, controls[i], GLUT_BITMAP_HELVETICA_18);
    }

    menuAnim++;
}

void drawGame() {
    drawBackground();
    if(!ballLaunched) ballX = paddleX;
    drawBall(); drawPaddle(); drawBricks();
    glColor3f(1,1,1);
    drawText(-0.95f, 0.9f, "Score: " + std::to_string(score));
    drawText(-0.95f, 0.8f, "Lives: " + std::to_string(lives));
    drawText(-0.95f, 0.7f, "Level: " + std::to_string(level));
    if(!ballLaunched) { glColor3f(0.8f,0.8f,0.2f); drawText(-0.2f, 0.0f, "Press SPACE to launch!"); }
    if(currentState == PAUSED) {
        glColor4f(0,0,0,0.5f); glBegin(GL_QUADS); glVertex2f(-1,-1); glVertex2f(1,-1); glVertex2f(1,1); glVertex2f(-1,1); glEnd();
        glColor3f(1,1,0); drawText(-0.15f, 0.1f, "PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(-0.2f, -0.1f, "Press P to continue");
    }
}

void drawGameOver() {
    drawBackground();
    glColor3f(1,0.2f,0.2f); drawText(-0.2f, 0.2f, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(1,1,1); drawText(-0.25f, 0.0f, "Final Score: " + std::to_string(score));
    drawButton(0, -0.3f, 0.4f, 0.12f, "RESTART");
    drawButton(0, -0.5f, 0.4f, 0.12f, "MENU");
}

void drawCredits() {
    drawBackground();
    glColor3f(0.8f,0.9f,1.0f); drawText(-0.15f, 0.5f, "DEVELOPED BY", GLUT_BITMAP_TIMES_ROMAN_24);
    drawText(-0.1f, 0.3f, "Rakesh Biswas"); drawText(-0.1f, 0.2f, "Nafis Ahamed");
    drawButton(0, -0.4f, 0.4f, 0.12f, "BACK: Press B");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    switch(currentState) {
        case MENU: drawMenu(); break;
        case PLAYING: case PAUSED: drawGame(); break;
        case GAME_OVER: drawGameOver(); break;
        case CREDITS: drawCredits(); break;
    }
    glutSwapBuffers();
}

void update(int value) {
    if(currentState == PLAYING && ballLaunched) {
        ballX += ballDX; ballY += ballDY;
        if(ballX+ballRadius > 1.0f || ballX-ballRadius < -1.0f) ballDX = -ballDX;
        if(ballY+ballRadius > 1.0f) ballDY = -ballDY;
        
        // Paddle collision
        if(ballY-ballRadius < -0.9f+paddleHeight && ballX > paddleX-paddleWidth/2 && ballX < paddleX+paddleWidth/2) {
            ballDY = abs(ballDY);
            float hitPos = (ballX-paddleX)/(paddleWidth/2);
            ballDX = hitPos*0.01f; ballDY = sqrt(0.0003f - ballDX*ballDX);
        }
        
        // Brick collision
        for(int i=0; i<ROWS; i++) {
            for(int j=0; j<COLS; j++) {
                if(bricks[i][j]) {
                    float x = -1.0f + j*(brickWidth+0.02f) + 0.1f;
                    float y = 0.7f - i*(brickHeight+0.02f);
                    if(ballX+ballRadius > x && ballX-ballRadius < x+brickWidth &&
                       ballY+ballRadius > y && ballY-ballRadius < y+brickHeight) {
                        bricks[i][j] = false; ballDY = -ballDY; score += 10*level;
                        bool allDestroyed = true;
                        for(int r=0; r<ROWS && allDestroyed; r++) 
                            for(int c=0; c<COLS && allDestroyed; c++) 
                                if(bricks[r][c]) allDestroyed = false;
                        if(allDestroyed) { level++; resetBricks(); }
                        goto endLoop;
                    }
                }
            }
        }
        endLoop:
        
        if(ballY < -1.0f) {
            lives--; 
            if(lives <= 0) currentState = GAME_OVER;
            else { ballX = paddleX; ballY = -0.4f; ballLaunched = false; }
        }
    }
    glutPostRedisplay(); glutTimerFunc(16, update, 0);
}

void mouseMove(int x, int y) {
    if(currentState == PLAYING) {
        float glX = (float)(x - WIDTH/2) / (WIDTH/2);
        paddleX = std::max(-1.0f + paddleWidth/2, std::min(1.0f - paddleWidth/2, glX));
    }
}


void handleClick(int button, int state, int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float glX = (float)(x - WIDTH/2) / (WIDTH/2);   // map to -1..1
        float glY = -(float)(y - HEIGHT/2) / (HEIGHT/2); // flip Y axis

        if(currentState == MENU) {
            // NEW GAME button
            if(glX >= -0.2f && glX <= 0.2f && glY >= 0.125f && glY <= 0.275f) {
                startNewGame();
            }
            // CREDITS button
            else if(glX >= -0.2f && glX <= 0.2f && glY >= -0.175f && glY <= -0.025f) {
                currentState = CREDITS;
            }
            // QUIT button
            else if(glX >= -0.2f && glX <= 0.2f && glY >= -0.475f && glY <= -0.325f) {
                exit(0);
            }
        }
        else if(currentState == PLAYING && !ballLaunched) {
            // Launch ball with mouse click
            ballLaunched = true;
            ballDX = 0.008f + level*0.002f;
            ballDY = 0.012f + level*0.003f;
        }
        else if(currentState == CREDITS) {
            // Back button at (0, -0.4f)
            if(glX >= -0.2f && glX <= 0.2f && glY >= -0.46f && glY <= -0.34f) {
                currentState = MENU;
            }
        }
        else if(currentState == GAME_OVER) {
            // RESTART button
            if(glX >= -0.2f && glX <= 0.2f && glY >= -0.36f && glY <= -0.24f) {
                startNewGame();
            }
            // MENU button
            else if(glX >= -0.2f && glX <= 0.2f && glY >= -0.56f && glY <= -0.44f) {
                currentState = MENU;
            }
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case ' ': 
            if(currentState == PLAYING && !ballLaunched) { 
                ballLaunched = true; 
                ballDX = 0.008f+level*0.002f; 
                ballDY = 0.012f+level*0.003f; 
            } 
            break;
        case 'p': case 'P': 
            if(currentState == PLAYING) currentState = PAUSED;
            else if(currentState == PAUSED) currentState = PLAYING; 
            break;
        case 'r': case 'R': 
            if(currentState == GAME_OVER) startNewGame(); 
            break;
        case 'b': case 'B':
            if(currentState == CREDITS) currentState = MENU;
            break;
        case 'm': case 'M':
            if(currentState == GAME_OVER) currentState = MENU;
            break;
        case 'n': case 'N':
            if(currentState == MENU) startNewGame();
            break;
        case 'q': case 'Q': 
            exit(0); 
            break;
        case 27: // Escape
            if(currentState != MENU) currentState = MENU; 
            break;
    }
}

void changeBallColor(int c) { ballColor = c-1; }
void changePaddleColor(int c) { paddleColor = c-1; }
void changeBrickColor(int c) { brickColor = c-1; }
void menuCallback(int) {}

void createMenus() {
    const char* colors[] = {"Red","Blue","Green","Purple","Yellow","Cyan","White","Orange"};
    int ballMenu = glutCreateMenu(changeBallColor);
    int paddleMenu = glutCreateMenu(changePaddleColor);
    int brickMenu = glutCreateMenu(changeBrickColor);
    
    for(int i=0; i<8; i++) {
        glutSetMenu(ballMenu); glutAddMenuEntry(colors[i], i+1);
        glutSetMenu(paddleMenu); glutAddMenuEntry(colors[i], i+1);
        glutSetMenu(brickMenu); glutAddMenuEntry(colors[i], i+1);
    }
    
    glutCreateMenu(menuCallback);
    glutAddSubMenu("Ball Color", ballMenu);
    glutAddSubMenu("Paddle Color", paddleMenu);
    glutAddSubMenu("Brick Color", brickMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("PixelShatter - Mouse Control");
    
    glClearColor(0,0,0,1); glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    srand(time(nullptr)); createMenus();
    
    glutDisplayFunc(display); glutKeyboardFunc(keyboard); glutMouseFunc(handleClick);
    glutPassiveMotionFunc(mouseMove); glutTimerFunc(16, update, 0);
    
    glutMainLoop(); return 0;
}