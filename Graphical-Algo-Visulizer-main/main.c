#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <graphics.h>
#include <conio.h>

// Screen dimensions
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 700

// Origin for the mathematical coordinate system
#define ORIGIN_X 400
#define ORIGIN_Y 350

// Cohen-Sutherland Region Codes
#define INSIDE 0 // 0000
#define LEFT   1 // 0001
#define RIGHT  2 // 0010
#define BOTTOM 4 // 0100
#define TOP    8 // 1000

// Animation Delays
int anim_delay = 50; // Medium by default

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    double x;
    double y;
} DoublePoint;

typedef struct {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
} ClipWindow;

// --- Function Prototypes ---
void initGraphicsWindow();
void drawCoordinateSystem();
void drawMenuUI();
void drawInfoPanel(const char* title);
void clearInfoPanel();
void updateInfoPanelText(int line, const char* text);
void drawPixelGrid(int x, int y, int color);
int getGraphicalInput(const char* prompt, int line_num);

// Coordinates mapping
int toScreenX(int x);
int toScreenY(int y);

// Algorithms
void ddaLine(int x1, int y1, int x2, int y2);
void bresenhamLine(int x1, int y1, int x2, int y2);
void midpointLine(int x1, int y1, int x2, int y2);
void midpointCircle(int xc, int yc, int r);
void plotCirclePoints(int xc, int yc, int x, int y, int color);
int computeOutCode(double x, double y, ClipWindow win);
void cohenSutherlandClip(double x1, double y1, double x2, double y2, ClipWindow win);
int isInside(DoublePoint p, int edge, ClipWindow win);
DoublePoint intersectEdge(DoublePoint v1, DoublePoint v2, int edge, ClipWindow win);
void drawClipWindow(ClipWindow win, int activeEdge);
int clipAgainstEdge(DoublePoint inPoly[], int inCount, DoublePoint outPoly[], int edge, ClipWindow win, DoublePoint origPoly[], int origCount);
void drawPolygon(DoublePoint poly[], int count, int color);
void sutherlandHodgmanClip(DoublePoint inPoly[], int inCount, ClipWindow win);

// Screens
void compareAlgorithms();
void algorithmInfo(int algo);
void aboutProject();
void setAnimationSpeed();

// --- Main Function ---
int main() {
    int choice = 0;
    int x1, y1, x2, y2;
    int xc, yc, r;
    ClipWindow win;
    char inputBuffer[100];

    initGraphicsWindow();

    while (1) {
        cleardevice();
        drawMenuUI();
        
        // Wait for keyboard input
        choice = getch();

        if (choice == '1') {
            cleardevice();
            drawCoordinateSystem();
            drawInfoPanel("DDA LINE DRAWING");
            
            updateInfoPanelText(1, "Enter Coordinates:");
            x1 = getGraphicalInput("X1:", 3);
            y1 = getGraphicalInput("Y1:", 4);
            x2 = getGraphicalInput("X2:", 5);
            y2 = getGraphicalInput("Y2:", 6);
            
            clearInfoPanel();
            
            ddaLine(x1, y1, x2, y2);
            updateInfoPanelText(9, "Algorithm Completed!");
            getch();
        } 
        else if (choice == '2') {
            cleardevice();
            drawCoordinateSystem();
            drawInfoPanel("BRESENHAM LINE");
            
            updateInfoPanelText(1, "Enter Coordinates:");
            x1 = getGraphicalInput("X1:", 3);
            y1 = getGraphicalInput("Y1:", 4);
            x2 = getGraphicalInput("X2:", 5);
            y2 = getGraphicalInput("Y2:", 6);
            
            clearInfoPanel();
            
            bresenhamLine(x1, y1, x2, y2);
            updateInfoPanelText(9, "Algorithm Completed!");
            getch();
        }
        else if (choice == '3') {
            cleardevice();
            drawCoordinateSystem();
            drawInfoPanel("MIDPOINT LINE");
            
            updateInfoPanelText(1, "Enter Coordinates:");
            x1 = getGraphicalInput("X1:", 3);
            y1 = getGraphicalInput("Y1:", 4);
            x2 = getGraphicalInput("X2:", 5);
            y2 = getGraphicalInput("Y2:", 6);
            
            clearInfoPanel();
            
            midpointLine(x1, y1, x2, y2);
            updateInfoPanelText(9, "Algorithm Completed!");
            getch();
        }
        else if (choice == '4') {
            cleardevice();
            drawCoordinateSystem();
            drawInfoPanel("MIDPOINT CIRCLE");
            
            updateInfoPanelText(1, "Enter Center & Radius:");
            xc = getGraphicalInput("Center X:", 3);
            yc = getGraphicalInput("Center Y:", 4);
            r  = getGraphicalInput("Radius R:", 5);
            if (r < 0) r = abs(r);
            if (r == 0) r = 1;
            
            clearInfoPanel();
            
            midpointCircle(xc, yc, r);
            updateInfoPanelText(11, "Press any key...");
            getch();
        }
        else if (choice == '5') {
            cleardevice();
            drawCoordinateSystem();
            drawInfoPanel("COHEN-SUTHERLAND");
            
            updateInfoPanelText(1, "Enter Line Coords:");
            x1 = getGraphicalInput("Line X1:", 2);
            y1 = getGraphicalInput("Line Y1:", 3);
            x2 = getGraphicalInput("Line X2:", 4);
            y2 = getGraphicalInput("Line Y2:", 5);

            updateInfoPanelText(7, "Enter Window:");
            win.xmin = getGraphicalInput("X Min:", 8);
            win.ymin = getGraphicalInput("Y Min:", 9);
            win.xmax = getGraphicalInput("X Max:", 10);
            win.ymax = getGraphicalInput("Y Max:", 11);
            
            clearInfoPanel();
            
            // Draw window
            setcolor(YELLOW);
            rectangle(toScreenX(win.xmin), toScreenY(win.ymax), toScreenX(win.xmax), toScreenY(win.ymin));
            
            cohenSutherlandClip(x1, y1, x2, y2, win);
            updateInfoPanelText(13, "Algorithm Completed!");
            getch();
        }
        else if (choice == '6') {
            cleardevice();
            setcolor(YELLOW);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
            outtextxy(180, 50, "SUTHERLAND-HODGMAN POLYGON CLIPPING");
            
            setcolor(WHITE);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            outtextxy(280, 160, "Select Polygon Input Mode:");
            outtextxy(300, 210, "1. Sample Triangle");
            outtextxy(300, 255, "2. Sample Quadrilateral");
            outtextxy(300, 300, "3. Sample Arrow Shape");
            outtextxy(300, 345, "4. Enter Custom Polygon Vertices");
            
            setcolor(LIGHTGREEN);
            outtextxy(280, 500, "Press Key (1-4)...");
            
            int polyChoice = getch();
            DoublePoint poly[40];
            int polyCount = 0;
            
            win.xmin = -70; win.ymin = -50; win.xmax = 70; win.ymax = 80;
            
            if (polyChoice == '1') {
                polyCount = 3;
                poly[0].x = -120; poly[0].y = -70;
                poly[1].x = 0;    poly[1].y = 140;
                poly[2].x = 120;  poly[2].y = -70;
            } else if (polyChoice == '2') {
                polyCount = 4;
                poly[0].x = -100; poly[0].y = -80;
                poly[1].x = -40;  poly[1].y = 120;
                poly[2].x = 110;  poly[2].y = 70;
                poly[3].x = 70;   poly[3].y = -90;
            } else if (polyChoice == '3') {
                polyCount = 4;
                poly[0].x = -110; poly[0].y = -20;
                poly[1].x = 0;    poly[1].y = 130;
                poly[2].x = 110;  poly[2].y = -20;
                poly[3].x = 0;    poly[3].y = 30;
            } else {
                cleardevice();
                drawCoordinateSystem();
                drawInfoPanel("SUTHERLAND-HODGMAN");
                updateInfoPanelText(1, "Enter Number of Vertices:");
                polyCount = getGraphicalInput("Vertices N (3-8):", 2);
                if (polyCount < 3) polyCount = 3;
                if (polyCount > 8) polyCount = 8;
                
                char promptBuf[50];
                for (int i = 0; i < polyCount; i++) {
                    clearInfoPanel();
                    sprintf(promptBuf, "Enter Vertex %d of %d:", i + 1, polyCount);
                    updateInfoPanelText(1, promptBuf);
                    sprintf(promptBuf, "P%d X:", i + 1);
                    poly[i].x = getGraphicalInput(promptBuf, 3);
                    sprintf(promptBuf, "P%d Y:", i + 1);
                    poly[i].y = getGraphicalInput(promptBuf, 4);
                }
                
                clearInfoPanel();
                updateInfoPanelText(1, "Enter Clip Window:");
                win.xmin = getGraphicalInput("X Min:", 2);
                win.ymin = getGraphicalInput("Y Min:", 3);
                win.xmax = getGraphicalInput("X Max:", 4);
                win.ymax = getGraphicalInput("Y Max:", 5);
                
                if (win.xmin > win.xmax) { int t = win.xmin; win.xmin = win.xmax; win.xmax = t; }
                if (win.ymin > win.ymax) { int t = win.ymin; win.ymin = win.ymax; win.ymax = t; }
            }
            
            cleardevice();
            drawCoordinateSystem();
            drawInfoPanel("SUTHERLAND-HODGMAN");
            
            sutherlandHodgmanClip(poly, polyCount, win);
            updateInfoPanelText(13, "Press any key...");
            getch();
        }
        else if (choice == '7') {
            compareAlgorithms();
        }
        else if (choice == '8') {
            aboutProject();
        }
        else if (choice == '9') {
            // Algorithm Info submenu
            cleardevice();
            setcolor(WHITE);
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
            outtextxy(300, 50, "Select Algorithm Info");
            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
            outtextxy(350, 130, "1. DDA Line");
            outtextxy(350, 175, "2. Bresenham Line");
            outtextxy(350, 220, "3. Midpoint Line");
            outtextxy(350, 265, "4. Midpoint Circle");
            outtextxy(350, 310, "5. Cohen-Sutherland");
            outtextxy(350, 355, "6. Sutherland-Hodgman");
            
            int infoChoice = getch();
            algorithmInfo(infoChoice - '0');
        }
        else if (choice == 's' || choice == 'S') {
            setAnimationSpeed();
        }
        else if (choice == '0' || choice == 'q' || choice == 27) {
            break;
        }
    }

    closegraph();
    return 0;
}

// --- Coordinate Systems ---
int toScreenX(int x) {
    return ORIGIN_X + x;
}

int toScreenY(int y) {
    // Y increases downwards on screen, but upwards in math
    return ORIGIN_Y - y;
}

void initGraphicsWindow() {
    int gd = DETECT, gm;
    // WinBGIm supports large resolutions directly
    initwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Graphics Algorithm Visualizer");
}

void drawCoordinateSystem() {
    setcolor(DARKGRAY);
    // X Axis
    line(0, ORIGIN_Y, ORIGIN_X * 2, ORIGIN_Y);
    // Y Axis
    line(ORIGIN_X, 0, ORIGIN_X, SCREEN_HEIGHT);
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(ORIGIN_X * 2 - 25, ORIGIN_Y + 10, "+X");
    outtextxy(10, ORIGIN_Y + 10, "-X");
    outtextxy(ORIGIN_X + 10, 15, "+Y");
    outtextxy(ORIGIN_X + 10, SCREEN_HEIGHT - 25, "-Y");
    outtextxy(ORIGIN_X - 30, ORIGIN_Y + 10, "(0,0)");
}

void drawMenuUI() {
    setcolor(LIGHTBLUE);
    rectangle(10, 10, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
    rectangle(15, 15, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 15);

    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
    outtextxy(150, 35, "GRAPHICS ALGORITHM VISUALIZER");
    
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(300, 110, "1. DDA Line Drawing");
    outtextxy(300, 152, "2. Bresenham Line Drawing");
    outtextxy(300, 194, "3. Midpoint Line Drawing");
    outtextxy(300, 236, "4. Midpoint Circle Drawing");
    outtextxy(300, 278, "5. Cohen-Sutherland Line Clipping");
    outtextxy(300, 320, "6. Sutherland-Hodgman Polygon Clipping");
    outtextxy(300, 362, "7. Compare Algorithms");
    outtextxy(300, 404, "8. About Project");
    outtextxy(300, 446, "9. Algorithm Information");
    outtextxy(300, 488, "S. Set Animation Speed");
    outtextxy(300, 530, "0. Exit");

    setcolor(LIGHTGREEN);
    outtextxy(280, 600, "Press Key (1-9, S, 0 to Exit)...");
}

void drawInfoPanel(const char* title) {
    setcolor(WHITE);
    // Panel background border
    rectangle(ORIGIN_X * 2 + 10, 10, SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10);
    
    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(ORIGIN_X * 2 + 20, 20, title);
    line(ORIGIN_X * 2 + 10, 50, SCREEN_WIDTH - 10, 50);
}

void clearInfoPanel() {
    setfillstyle(SOLID_FILL, BLACK);
    bar(ORIGIN_X * 2 + 11, 51, SCREEN_WIDTH - 11, SCREEN_HEIGHT - 11);
}

void updateInfoPanelText(int line_num, const char* text) {
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(ORIGIN_X * 2 + 20, 60 + (line_num * 20), text);
}

int getGraphicalInput(const char* prompt, int line_num) {
    char str[20] = "";
    int i = 0;
    int ch;
    char displayStr[100];
    int x = ORIGIN_X * 2 + 20;
    int y = 60 + (line_num * 20);
    
    // Clear keyboard buffer just in case
    while (kbhit()) getch();
    
    while(1) {
        sprintf(displayStr, "%s %s_", prompt, str);
        
        // clear just this line in the info panel
        setfillstyle(SOLID_FILL, BLACK);
        bar(x, y, SCREEN_WIDTH - 11, y + 18);
        
        setcolor(YELLOW);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(x, y, displayStr);
        
        ch = getch();
        if (ch == 13) { // Enter key
            // Require at least one digit (cannot submit empty or just "-")
            if (i > 0 && !(i == 1 && str[0] == '-')) break;
        } else if (ch == 8) { // Backspace
            if (i > 0) {
                i--;
                str[i] = '\0';
            }
        } else if (ch == '-') { // Minus sign: toggle negative or add at start
            if (str[0] == '-') {
                // Remove leading minus
                for (int j = 0; j < i; j++) {
                    str[j] = str[j + 1];
                }
                i--;
            } else if (i < 10) {
                // Insert leading minus
                for (int j = i; j >= 0; j--) {
                    str[j + 1] = str[j];
                }
                str[0] = '-';
                i++;
            }
        } else if (ch >= '0' && ch <= '9') {
            if (i < 10) {
                str[i++] = ch;
                str[i] = '\0';
            }
        }
    }
    
    // clear cursor and show final
    sprintf(displayStr, "%s %s", prompt, str);
    setfillstyle(SOLID_FILL, BLACK);
    bar(x, y, SCREEN_WIDTH - 11, y + 18);
    setcolor(WHITE);
    outtextxy(x, y, displayStr);
    
    return atoi(str);
}

// Visual pixel grid - drawing a small rectangle to represent a mathematical point
void drawPixelGrid(int mathX, int mathY, int color) {
    int sx = toScreenX(mathX);
    int sy = toScreenY(mathY);
    // Clip to drawing area (left of info panel)
    if (sx >= 2 && sx <= ORIGIN_X * 2 - 2 && sy >= 2 && sy <= SCREEN_HEIGHT - 2) {
        setcolor(color);
        rectangle(sx - 2, sy - 2, sx + 2, sy + 2);
        putpixel(sx, sy, color);
    }
}

// --- Algorithm Implementations ---

void ddaLine(int x1, int y1, int x2, int y2) {
    char buf[100];
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    float xInc = (steps == 0) ? 0 : (dx / (float)steps);
    float yInc = (steps == 0) ? 0 : (dy / (float)steps);
    
    float x = x1;
    float y = y1;
    
    sprintf(buf, "Start: (%d, %d)", x1, y1); updateInfoPanelText(1, buf);
    sprintf(buf, "End:   (%d, %d)", x2, y2); updateInfoPanelText(2, buf);
    sprintf(buf, "dx: %d, dy: %d", dx, dy); updateInfoPanelText(3, buf);
    sprintf(buf, "Steps: %d", steps); updateInfoPanelText(4, buf);
    
    for (int i = 0; i <= steps; i++) {
        drawPixelGrid(round(x), round(y), RED);
        
        clearInfoPanel();
        updateInfoPanelText(1, "--- DDA STEP ---");
        sprintf(buf, "Step: %d / %d", i, steps); updateInfoPanelText(3, buf);
        sprintf(buf, "X (float): %.2f", x); updateInfoPanelText(4, buf);
        sprintf(buf, "Y (float): %.2f", y); updateInfoPanelText(5, buf);
        sprintf(buf, "Plot: (%d, %d)", (int)round(x), (int)round(y)); updateInfoPanelText(7, buf);
        
        x += xInc;
        y += yInc;
        delay(anim_delay);
    }
}

void bresenhamLine(int x1, int y1, int x2, int y2) {
    char buf[100];
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    
    int err = dx - dy;
    
    sprintf(buf, "Start: (%d, %d)", x1, y1); updateInfoPanelText(1, buf);
    sprintf(buf, "End:   (%d, %d)", x2, y2); updateInfoPanelText(2, buf);
    
    int step = 0;
    while (1) {
        drawPixelGrid(x1, y1, LIGHTGREEN);
        
        clearInfoPanel();
        updateInfoPanelText(1, "--- BRESENHAM STEP ---");
        sprintf(buf, "Step: %d", step++); updateInfoPanelText(2, buf);
        sprintf(buf, "Plot: (%d, %d)", x1, y1); updateInfoPanelText(4, buf);
        sprintf(buf, "Err (Param): %d", err); updateInfoPanelText(5, buf);
        
        delay(anim_delay);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void midpointLine(int x1, int y1, int x2, int y2) {
    char buf[100];
    int origX1 = x1, origY1 = y1, origX2 = x2, origY2 = y2;
    
    // Draw initial endpoints info
    clearInfoPanel();
    updateInfoPanelText(1, "--- MIDPOINT LINE ---");
    sprintf(buf, "P1: (%d, %d)", x1, y1); updateInfoPanelText(2, buf);
    sprintf(buf, "P2: (%d, %d)", x2, y2); updateInfoPanelText(3, buf);
    
    // Check if points need swapping (ensure line is drawn left to right, or bottom to top if vertical)
    int swapped = 0;
    if (x1 > x2 || (x1 == x2 && y1 > y2)) {
        int temp = x1; x1 = x2; x2 = temp;
        temp = y1; y1 = y2; y2 = temp;
        swapped = 1;
    }
    
    if (swapped) {
        updateInfoPanelText(4, "Swapped: Left-to-Right");
    }
    
    int dx = x2 - x1;
    int dy = y2 - y1;
    int absDx = abs(dx);
    int absDy = abs(dy);
    
    // Show slope and condition
    if (dx != 0) {
        float m = (float)dy / (float)dx;
        sprintf(buf, "Slope m: %.2f", m); updateInfoPanelText(5, buf);
        if (dy > 0) {
            if (absDy <= absDx) updateInfoPanelText(6, "Cond: Slope>0 (|m|<=1)");
            else updateInfoPanelText(6, "Cond: Slope>0 (|m|>1)");
        } else if (dy < 0) {
            if (absDy <= absDx) updateInfoPanelText(6, "Cond: Slope<0 (|m|<=1)");
            else updateInfoPanelText(6, "Cond: Slope<0 (|m|>1)");
        } else {
            updateInfoPanelText(6, "Cond: Slope=0 (Horiz)");
        }
    } else {
        updateInfoPanelText(5, "Slope: Undefined");
        updateInfoPanelText(6, "Cond: Vertical Line");
    }
    
    delay(1000);
    
    int x = x1;
    int y = y1;
    int step = 0;
    int d;
    int incrStraight, incrDiag;
    
    // Plot the starting pixel
    drawPixelGrid(x, y, MAGENTA);
    
    // Handle single point
    if (dx == 0 && dy == 0) {
        clearInfoPanel();
        updateInfoPanelText(1, "Single Point Line");
        sprintf(buf, "Plot: (%d, %d)", x, y); updateInfoPanelText(3, buf);
        return;
    }
    
    // CONDITION 1: Vertical Line (dx == 0)
    if (dx == 0) {
        while (y < y2) {
            clearInfoPanel();
            updateInfoPanelText(1, "--- MIDPOINT STEP ---");
            sprintf(buf, "Step: %d", step++); updateInfoPanelText(2, buf);
            sprintf(buf, "Current: (%d, %d)", x, y); updateInfoPanelText(3, buf);
            updateInfoPanelText(4, "Cond: Vertical Line");
            updateInfoPanelText(6, "Choose North (Y++)");
            y++;
            drawPixelGrid(x, y, MAGENTA);
            sprintf(buf, "Plot: (%d, %d)", x, y); updateInfoPanelText(8, buf);
            delay(anim_delay);
        }
    }
    // CONDITION 2: Horizontal Line (dy == 0)
    else if (dy == 0) {
        while (x < x2) {
            clearInfoPanel();
            updateInfoPanelText(1, "--- MIDPOINT STEP ---");
            sprintf(buf, "Step: %d", step++); updateInfoPanelText(2, buf);
            sprintf(buf, "Current: (%d, %d)", x, y); updateInfoPanelText(3, buf);
            updateInfoPanelText(4, "Cond: Slope=0 (Horiz)");
            updateInfoPanelText(6, "Choose East (X++)");
            x++;
            drawPixelGrid(x, y, MAGENTA);
            sprintf(buf, "Plot: (%d, %d)", x, y); updateInfoPanelText(8, buf);
            delay(anim_delay);
        }
    }
    // CONDITION 3: Slope > 0 and |m| <= 1 (dy > 0, dy <= dx)
    else if (dy > 0 && absDy <= absDx) {
        d = 2 * dy - dx;
        incrStraight = 2 * dy;              // incrE
        incrDiag = 2 * (dy - dx);          // incrNE
        
        while (x < x2) {
            clearInfoPanel();
            updateInfoPanelText(1, "--- MIDPOINT STEP ---");
            sprintf(buf, "Step: %d", step++); updateInfoPanelText(2, buf);
            sprintf(buf, "Current: (%d, %d)", x, y); updateInfoPanelText(3, buf);
            updateInfoPanelText(4, "Cond: Slope>0 (|m|<=1)");
            sprintf(buf, "Param (d): %d", d); updateInfoPanelText(5, buf);
            
            if (d <= 0) {
                updateInfoPanelText(6, "d <= 0, Choose East");
                d += incrStraight;
                x++;
            } else {
                updateInfoPanelText(6, "d > 0, Choose North-East");
                d += incrDiag;
                x++;
                y++;
            }
            drawPixelGrid(x, y, MAGENTA);
            sprintf(buf, "Plot: (%d, %d)", x, y); updateInfoPanelText(8, buf);
            delay(anim_delay);
        }
    }
    // CONDITION 4: Slope < 0 and |m| <= 1 (dy < 0, |dy| <= dx)
    else if (dy < 0 && absDy <= absDx) {
        d = 2 * absDy - dx;
        incrStraight = 2 * absDy;           // incrE
        incrDiag = 2 * (absDy - dx);       // incrSE
        
        while (x < x2) {
            clearInfoPanel();
            updateInfoPanelText(1, "--- MIDPOINT STEP ---");
            sprintf(buf, "Step: %d", step++); updateInfoPanelText(2, buf);
            sprintf(buf, "Current: (%d, %d)", x, y); updateInfoPanelText(3, buf);
            updateInfoPanelText(4, "Cond: Slope<0 (|m|<=1)");
            sprintf(buf, "Param (d): %d", d); updateInfoPanelText(5, buf);
            
            if (d <= 0) {
                updateInfoPanelText(6, "d <= 0, Choose East");
                d += incrStraight;
                x++;
            } else {
                updateInfoPanelText(6, "d > 0, Choose South-East");
                d += incrDiag;
                x++;
                y--;
            }
            drawPixelGrid(x, y, MAGENTA);
            sprintf(buf, "Plot: (%d, %d)", x, y); updateInfoPanelText(8, buf);
            delay(anim_delay);
        }
    }
    // CONDITION 5: Slope > 0 and |m| > 1 (dy > 0, dy > dx, Steep)
    else if (dy > 0 && absDy > absDx) {
        d = 2 * dx - dy;
        incrStraight = 2 * dx;              // incrN
        incrDiag = 2 * (dx - dy);          // incrNE
        
        while (y < y2) {
            clearInfoPanel();
            updateInfoPanelText(1, "--- MIDPOINT STEP ---");
            sprintf(buf, "Step: %d", step++); updateInfoPanelText(2, buf);
            sprintf(buf, "Current: (%d, %d)", x, y); updateInfoPanelText(3, buf);
            updateInfoPanelText(4, "Cond: Slope>0 (|m|>1)");
            sprintf(buf, "Param (d): %d", d); updateInfoPanelText(5, buf);
            
            if (d <= 0) {
                updateInfoPanelText(6, "d <= 0, Choose North");
                d += incrStraight;
                y++;
            } else {
                updateInfoPanelText(6, "d > 0, Choose North-East");
                d += incrDiag;
                x++;
                y++;
            }
            drawPixelGrid(x, y, MAGENTA);
            sprintf(buf, "Plot: (%d, %d)", x, y); updateInfoPanelText(8, buf);
            delay(anim_delay);
        }
    }
    // CONDITION 6: Slope < 0 and |m| > 1 (dy < 0, |dy| > dx, Steep)
    else if (dy < 0 && absDy > absDx) {
        d = 2 * dx - absDy;
        incrStraight = 2 * dx;              // incrS
        incrDiag = 2 * (dx - absDy);       // incrSE
        
        while (y > y2) {
            clearInfoPanel();
            updateInfoPanelText(1, "--- MIDPOINT STEP ---");
            sprintf(buf, "Step: %d", step++); updateInfoPanelText(2, buf);
            sprintf(buf, "Current: (%d, %d)", x, y); updateInfoPanelText(3, buf);
            updateInfoPanelText(4, "Cond: Slope<0 (|m|>1)");
            sprintf(buf, "Param (d): %d", d); updateInfoPanelText(5, buf);
            
            if (d <= 0) {
                updateInfoPanelText(6, "d <= 0, Choose South");
                d += incrStraight;
                y--;
            } else {
                updateInfoPanelText(6, "d > 0, Choose South-East");
                d += incrDiag;
                x++;
                y--;
            }
            drawPixelGrid(x, y, MAGENTA);
            sprintf(buf, "Plot: (%d, %d)", x, y); updateInfoPanelText(8, buf);
            delay(anim_delay);
        }
    }
    
    // Final completion summary
    clearInfoPanel();
    updateInfoPanelText(1, "--- MIDPOINT LINE ---");
    sprintf(buf, "P1: (%d, %d)", origX1, origY1); updateInfoPanelText(3, buf);
    sprintf(buf, "P2: (%d, %d)", origX2, origY2); updateInfoPanelText(4, buf);
    sprintf(buf, "Steps Done: %d", step); updateInfoPanelText(6, buf);
    updateInfoPanelText(8, "Drawing Completed!");
}

// 8-Way Symmetry plotting for circle
void plotCirclePoints(int xc, int yc, int x, int y, int color) {
    drawPixelGrid(xc + x, yc + y, color);
    drawPixelGrid(xc - x, yc + y, color);
    drawPixelGrid(xc + x, yc - y, color);
    drawPixelGrid(xc - x, yc - y, color);
    drawPixelGrid(xc + y, yc + x, color);
    drawPixelGrid(xc - y, yc + x, color);
    drawPixelGrid(xc + y, yc - x, color);
    drawPixelGrid(xc - y, yc - x, color);
}

void midpointCircle(int xc, int yc, int r) {
    char buf[100];
    
    clearInfoPanel();
    updateInfoPanelText(1, "--- MIDPOINT CIRCLE ---");
    sprintf(buf, "Center: (%d, %d)", xc, yc); updateInfoPanelText(2, buf);
    sprintf(buf, "Radius R: %d", r); updateInfoPanelText(3, buf);
    updateInfoPanelText(5, "Initial Decision Param:");
    sprintf(buf, "d0 = 1 - R = %d", 1 - r); updateInfoPanelText(6, buf);
    updateInfoPanelText(8, "Using 8-Way Symmetry");
    delay(1000);
    
    // Draw Center point in RED
    drawPixelGrid(xc, yc, RED);
    
    int x = 0;
    int y = r;
    int d = 1 - r;
    int step = 0;
    
    // Plot initial 8 symmetric points
    plotCirclePoints(xc, yc, x, y, CYAN);
    
    while (x <= y) {
        clearInfoPanel();
        updateInfoPanelText(1, "--- CIRCLE STEP ---");
        sprintf(buf, "Step: %d", step++); updateInfoPanelText(2, buf);
        sprintf(buf, "Octant 1: (%d, %d)", x, y); updateInfoPanelText(3, buf);
        sprintf(buf, "Param (d): %d", d); updateInfoPanelText(4, buf);
        
        if (d < 0) {
            updateInfoPanelText(5, "d < 0, Choose East");
            sprintf(buf, "d += 2x + 3 = %d", 2 * x + 3); updateInfoPanelText(6, buf);
            d += 2 * x + 3;
            x++;
        } else {
            updateInfoPanelText(5, "d >= 0, Choose South-East");
            sprintf(buf, "d += 2(x-y) + 5 = %d", 2 * (x - y) + 5); updateInfoPanelText(6, buf);
            d += 2 * (x - y) + 5;
            x++;
            y--;
        }
        
        plotCirclePoints(xc, yc, x, y, CYAN);
        sprintf(buf, "Plot: 8 pts for (%d, %d)", x, y); updateInfoPanelText(8, buf);
        
        delay(anim_delay);
    }
    
    // Final completion summary
    clearInfoPanel();
    updateInfoPanelText(1, "--- MIDPOINT CIRCLE ---");
    sprintf(buf, "Center: (%d, %d)", xc, yc); updateInfoPanelText(3, buf);
    sprintf(buf, "Radius: %d", r); updateInfoPanelText(4, buf);
    sprintf(buf, "Total Steps: %d", step); updateInfoPanelText(6, buf);
    updateInfoPanelText(7, "8-Way Symmetry Done");
    updateInfoPanelText(9, "Algorithm Completed!");
}

int computeOutCode(double x, double y, ClipWindow win) {
    int code = INSIDE;
    if (x < win.xmin) code |= LEFT;
    else if (x > win.xmax) code |= RIGHT;
    
    if (y < win.ymin) code |= BOTTOM;
    else if (y > win.ymax) code |= TOP;
    
    return code;
}

void cohenSutherlandClip(double x1, double y1, double x2, double y2, ClipWindow win) {
    char buf[100];
    int outcode1 = computeOutCode(x1, y1, win);
    int outcode2 = computeOutCode(x2, y2, win);
    int accept = 0;
    int step = 1;
    
    // Draw initial unclipped line
    setcolor(DARKGRAY);
    line(toScreenX(x1), toScreenY(y1), toScreenX(x2), toScreenY(y2));
    
    while (1) {
        clearInfoPanel();
        sprintf(buf, "Step %d:", step++); updateInfoPanelText(1, buf);
        sprintf(buf, "P1 Code: %d", outcode1); updateInfoPanelText(2, buf);
        sprintf(buf, "P2 Code: %d", outcode2); updateInfoPanelText(3, buf);
        
        if (!(outcode1 | outcode2)) {
            updateInfoPanelText(5, "TRIVIALLY ACCEPTED");
            accept = 1;
            delay(1500);
            break;
        } else if (outcode1 & outcode2) {
            updateInfoPanelText(5, "TRIVIALLY REJECTED");
            delay(1500);
            break;
        } else {
            updateInfoPanelText(5, "REQUIRES CLIPPING");
            delay(1000);
            
            double x, y;
            int outcodeOut = outcode1 ? outcode1 : outcode2;
            
            if (outcodeOut & TOP) {
                x = x1 + (x2 - x1) * (win.ymax - y1) / (y2 - y1);
                y = win.ymax;
            } else if (outcodeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (win.ymin - y1) / (y2 - y1);
                y = win.ymin;
            } else if (outcodeOut & RIGHT) {
                y = y1 + (y2 - y1) * (win.xmax - x1) / (x2 - x1);
                x = win.xmax;
            } else if (outcodeOut & LEFT) {
                y = y1 + (y2 - y1) * (win.xmin - x1) / (x2 - x1);
                x = win.xmin;
            }
            
            if (outcodeOut == outcode1) {
                x1 = x; y1 = y;
                outcode1 = computeOutCode(x1, y1, win);
            } else {
                x2 = x; y2 = y;
                outcode2 = computeOutCode(x2, y2, win);
            }
            
            sprintf(buf, "Intersection: (%.1f, %.1f)", x, y); updateInfoPanelText(7, buf);
            delay(1500);
        }
    }
    
    if (accept) {
        setcolor(LIGHTCYAN);
        line(toScreenX(x1), toScreenY(y1), toScreenX(x2), toScreenY(y2));
        updateInfoPanelText(9, "Drawing Final Clipped Line");
    }
}

// --- Sutherland-Hodgman Polygon Clipping Functions ---

int isInside(DoublePoint p, int edge, ClipWindow win) {
    switch (edge) {
        case 0: return p.x >= (double)win.xmin; // Left edge: x >= xmin
        case 1: return p.x <= (double)win.xmax; // Right edge: x <= xmax
        case 2: return p.y >= (double)win.ymin; // Bottom edge: y >= ymin
        case 3: return p.y <= (double)win.ymax; // Top edge: y <= ymax
    }
    return 0;
}

DoublePoint intersectEdge(DoublePoint v1, DoublePoint v2, int edge, ClipWindow win) {
    DoublePoint out;
    double dx = v2.x - v1.x;
    double dy = v2.y - v1.y;
    
    switch (edge) {
        case 0: // Left edge: x = win.xmin
            out.x = (double)win.xmin;
            out.y = (fabs(dx) > 1e-7) ? (v1.y + dy * ((double)win.xmin - v1.x) / dx) : v1.y;
            break;
        case 1: // Right edge: x = win.xmax
            out.x = (double)win.xmax;
            out.y = (fabs(dx) > 1e-7) ? (v1.y + dy * ((double)win.xmax - v1.x) / dx) : v1.y;
            break;
        case 2: // Bottom edge: y = win.ymin
            out.y = (double)win.ymin;
            out.x = (fabs(dy) > 1e-7) ? (v1.x + dx * ((double)win.ymin - v1.y) / dy) : v1.x;
            break;
        case 3: // Top edge: y = win.ymax
            out.y = (double)win.ymax;
            out.x = (fabs(dy) > 1e-7) ? (v1.x + dx * ((double)win.ymax - v1.y) / dy) : v1.x;
            break;
        default:
            out = v1;
            break;
    }
    return out;
}

void drawClipWindow(ClipWindow win, int activeEdge) {
    // Normal window perimeter in YELLOW
    setcolor(YELLOW);
    rectangle(toScreenX(win.xmin), toScreenY(win.ymax), toScreenX(win.xmax), toScreenY(win.ymin));
    
    // Highlight active boundary in LIGHTRED
    setcolor(LIGHTRED);
    if (activeEdge == 0) { // LEFT: x = xmin
        line(toScreenX(win.xmin), toScreenY(win.ymin), toScreenX(win.xmin), toScreenY(win.ymax));
        line(toScreenX(win.xmin) + 1, toScreenY(win.ymin), toScreenX(win.xmin) + 1, toScreenY(win.ymax));
    } else if (activeEdge == 1) { // RIGHT: x = xmax
        line(toScreenX(win.xmax), toScreenY(win.ymin), toScreenX(win.xmax), toScreenY(win.ymax));
        line(toScreenX(win.xmax) - 1, toScreenY(win.ymin), toScreenX(win.xmax) - 1, toScreenY(win.ymax));
    } else if (activeEdge == 2) { // BOTTOM: y = ymin
        line(toScreenX(win.xmin), toScreenY(win.ymin), toScreenX(win.xmax), toScreenY(win.ymin));
        line(toScreenX(win.xmin), toScreenY(win.ymin) - 1, toScreenX(win.xmax), toScreenY(win.ymin) - 1);
    } else if (activeEdge == 3) { // TOP: y = ymax
        line(toScreenX(win.xmin), toScreenY(win.ymax), toScreenX(win.xmax), toScreenY(win.ymax));
        line(toScreenX(win.xmin), toScreenY(win.ymax) + 1, toScreenX(win.xmax), toScreenY(win.ymax) + 1);
    }
}

void drawPolygon(DoublePoint poly[], int count, int color) {
    if (count < 2) return;
    setcolor(color);
    for (int i = 0; i < count; i++) {
        DoublePoint p1 = poly[i];
        DoublePoint p2 = poly[(i + 1) % count];
        line(toScreenX((int)round(p1.x)), toScreenY((int)round(p1.y)),
             toScreenX((int)round(p2.x)), toScreenY((int)round(p2.y)));
        drawPixelGrid((int)round(p1.x), (int)round(p1.y), color);
    }
}

int clipAgainstEdge(DoublePoint inPoly[], int inCount, DoublePoint outPoly[], int edge, ClipWindow win, DoublePoint origPoly[], int origCount) {
    int outCount = 0;
    if (inCount == 0) return 0;
    
    char buf[100];
    const char* edgeNames[] = {"LEFT (X >= Xmin)", "RIGHT (X <= Xmax)", "BOTTOM (Y >= Ymin)", "TOP (Y <= Ymax)"};
    
    for (int i = 0; i < inCount; i++) {
        DoublePoint v1 = inPoly[i];
        DoublePoint v2 = inPoly[(i + 1) % inCount];
        
        int in1 = isInside(v1, edge, win);
        int in2 = isInside(v2, edge, win);
        
        // Redraw coordinate system and visual context for this edge test
        cleardevice();
        drawCoordinateSystem();
        drawInfoPanel("SUTHERLAND-HODGMAN");
        
        // Original polygon in DARKGRAY
        drawPolygon(origPoly, origCount, DARKGRAY);
        
        // Clipping window with active boundary edge in LIGHTRED
        drawClipWindow(win, edge);
        
        // Current intermediate polygon in LIGHTBLUE
        drawPolygon(inPoly, inCount, LIGHTBLUE);
        
        // Highlight active testing edge V1 -> V2 in CYAN
        setcolor(CYAN);
        line(toScreenX((int)round(v1.x)), toScreenY((int)round(v1.y)),
             toScreenX((int)round(v2.x)), toScreenY((int)round(v2.y)));
        
        // Highlight testing edge endpoints
        setcolor(WHITE);
        circle(toScreenX((int)round(v1.x)), toScreenY((int)round(v1.y)), 3);
        circle(toScreenX((int)round(v2.x)), toScreenY((int)round(v2.y)), 3);
        
        // Update Info Panel with exact status
        clearInfoPanel();
        updateInfoPanelText(1, "--- SUTHERLAND-HODGMAN ---");
        sprintf(buf, "Stage: %s", edgeNames[edge]); updateInfoPanelText(2, buf);
        sprintf(buf, "Test Edge: P%d -> P%d", i + 1, ((i + 1) % inCount) + 1); updateInfoPanelText(3, buf);
        sprintf(buf, "P1(%.0f,%.0f): %s", v1.x, v1.y, in1 ? "INSIDE" : "OUTSIDE"); updateInfoPanelText(4, buf);
        sprintf(buf, "P2(%.0f,%.0f): %s", v2.x, v2.y, in2 ? "INSIDE" : "OUTSIDE"); updateInfoPanelText(5, buf);
        
        if (in1 && in2) {
            // Case 1: In to In -> Output v2
            updateInfoPanelText(7, "Condition: Case 1 (IN->IN)");
            updateInfoPanelText(8, "Action: Save Vertex P2");
            if (outCount < 48) outPoly[outCount++] = v2;
        } else if (in1 && !in2) {
            // Case 2: In to Out -> Output intersection
            DoublePoint vInt = intersectEdge(v1, v2, edge, win);
            updateInfoPanelText(7, "Condition: Case 2 (IN->OUT)");
            updateInfoPanelText(8, "Action: Save Intersect V'");
            sprintf(buf, "Intersect V': (%.1f, %.1f)", vInt.x, vInt.y); updateInfoPanelText(9, buf);
            
            // Mark intersection point on screen
            setcolor(LIGHTRED);
            circle(toScreenX((int)round(vInt.x)), toScreenY((int)round(vInt.y)), 5);
            setfillstyle(SOLID_FILL, LIGHTRED);
            fillellipse(toScreenX((int)round(vInt.x)), toScreenY((int)round(vInt.y)), 3, 3);
            
            if (outCount < 48) outPoly[outCount++] = vInt;
        } else if (!in1 && !in2) {
            // Case 3: Out to Out -> Discard
            updateInfoPanelText(7, "Condition: Case 3 (OUT->OUT)");
            updateInfoPanelText(8, "Action: Discard (None Saved)");
        } else if (!in1 && in2) {
            // Case 4: Out to In -> Output intersection and v2
            DoublePoint vInt = intersectEdge(v1, v2, edge, win);
            updateInfoPanelText(7, "Condition: Case 4 (OUT->IN)");
            updateInfoPanelText(8, "Action: Save V' and P2");
            sprintf(buf, "Intersect V': (%.1f, %.1f)", vInt.x, vInt.y); updateInfoPanelText(9, buf);
            
            // Mark intersection point on screen
            setcolor(LIGHTRED);
            circle(toScreenX((int)round(vInt.x)), toScreenY((int)round(vInt.y)), 5);
            setfillstyle(SOLID_FILL, LIGHTRED);
            fillellipse(toScreenX((int)round(vInt.x)), toScreenY((int)round(vInt.y)), 3, 3);
            
            if (outCount < 48) {
                outPoly[outCount++] = vInt;
                outPoly[outCount++] = v2;
            }
        }
        
        sprintf(buf, "Output Vertices: %d", outCount); updateInfoPanelText(11, buf);
        updateInfoPanelText(13, "Red Line: Active Boundary");
        updateInfoPanelText(14, "Cyan Line: Testing Edge");
        
        delay(anim_delay < 120 ? 180 : anim_delay);
    }
    
    return outCount;
}

void sutherlandHodgmanClip(DoublePoint inPoly[], int inCount, ClipWindow win) {
    char buf[100];
    DoublePoint currentPoly[50];
    DoublePoint nextPoly[50];
    int currentCount = inCount;
    
    for (int i = 0; i < inCount; i++) {
        currentPoly[i] = inPoly[i];
    }
    
    // Initial display
    cleardevice();
    drawCoordinateSystem();
    drawInfoPanel("SUTHERLAND-HODGMAN");
    drawClipWindow(win, -1);
    drawPolygon(currentPoly, currentCount, LIGHTBLUE);
    
    clearInfoPanel();
    updateInfoPanelText(1, "--- SUTHERLAND-HODGMAN ---");
    sprintf(buf, "Input Vertices: %d", inCount); updateInfoPanelText(2, buf);
    sprintf(buf, "Window X: [%d, %d]", win.xmin, win.xmax); updateInfoPanelText(4, buf);
    sprintf(buf, "Window Y: [%d, %d]", win.ymin, win.ymax); updateInfoPanelText(5, buf);
    updateInfoPanelText(7, "Pipeline Order:");
    updateInfoPanelText(8, "1. Left Edge Clip");
    updateInfoPanelText(9, "2. Right Edge Clip");
    updateInfoPanelText(10, "3. Bottom Edge Clip");
    updateInfoPanelText(11, "4. Top Edge Clip");
    updateInfoPanelText(13, "Starting 4-Stage Clip...");
    delay(1000);
    
    const char* edgeNames[] = {"LEFT", "RIGHT", "BOTTOM", "TOP"};
    
    // Clip against all 4 edges: Left (0), Right (1), Bottom (2), Top (3)
    for (int edge = 0; edge < 4; edge++) {
        if (currentCount == 0) break;
        
        int nextCount = clipAgainstEdge(currentPoly, currentCount, nextPoly, edge, win, inPoly, inCount);
        currentCount = nextCount;
        for (int i = 0; i < currentCount; i++) {
            currentPoly[i] = nextPoly[i];
        }
        
        // Stage summary display
        cleardevice();
        drawCoordinateSystem();
        drawInfoPanel("SUTHERLAND-HODGMAN");
        
        // Unclipped original in DARKGRAY
        drawPolygon(inPoly, inCount, DARKGRAY);
        
        // Window in YELLOW
        drawClipWindow(win, -1);
        
        // Intermediate clipped in MAGENTA
        drawPolygon(currentPoly, currentCount, MAGENTA);
        
        clearInfoPanel();
        updateInfoPanelText(1, "--- STAGE COMPLETED ---");
        sprintf(buf, "Finished %s Clip", edgeNames[edge]); updateInfoPanelText(3, buf);
        sprintf(buf, "Vertices Remaining: %d", currentCount); updateInfoPanelText(5, buf);
        updateInfoPanelText(7, "Magenta: Stage Output");
        updateInfoPanelText(8, "Gray: Original Polygon");
        if (edge < 3) {
            sprintf(buf, "Next: %s Clip...", edgeNames[edge + 1]);
            updateInfoPanelText(10, buf);
        }
        delay(700);
    }
    
    // Final clipped display
    cleardevice();
    drawCoordinateSystem();
    drawInfoPanel("SUTHERLAND-HODGMAN");
    
    // Unclipped original in DARKGRAY
    drawPolygon(inPoly, inCount, DARKGRAY);
    
    // Window in YELLOW
    drawClipWindow(win, -1);
    
    // Final clipped polygon in LIGHTGREEN
    drawPolygon(currentPoly, currentCount, LIGHTGREEN);
    
    // Final summary
    clearInfoPanel();
    updateInfoPanelText(1, "--- CLIPPING COMPLETE ---");
    sprintf(buf, "Original Verts: %d", inCount); updateInfoPanelText(3, buf);
    sprintf(buf, "Clipped Verts:  %d", currentCount); updateInfoPanelText(4, buf);
    updateInfoPanelText(6, "All 4 Cases Satisfied:");
    updateInfoPanelText(7, "1. IN->IN: Save P2");
    updateInfoPanelText(8, "2. IN->OUT: Save V'");
    updateInfoPanelText(9, "3. OUT->OUT: Discard");
    updateInfoPanelText(10, "4. OUT->IN: Save V'&P2");
    updateInfoPanelText(12, "Green: Clipped Polygon");
    updateInfoPanelText(13, "Gray:  Original Polygon");
    updateInfoPanelText(15, "Algorithm Finished!");
}

// --- Menu Screens ---
void compareAlgorithms() {
    cleardevice();
    
    // Header Title
    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(270, 20, "ALGORITHM COMPARISON TABLE");
    
    setcolor(LIGHTGRAY);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(235, 60, "Comprehensive Side-by-Side Analysis of Implemented Graphics Algorithms");
    
    int left = 30;
    int right = 970;
    int top = 90;
    int bottom = 510;
    
    int col1 = 230;
    int col2 = 390;
    int col3 = 560;
    
    // Header background bar (Blue)
    setfillstyle(SOLID_FILL, BLUE);
    bar(left + 1, top + 1, right - 1, top + 50);
    
    // Outer border (Double line in LIGHTCYAN)
    setcolor(LIGHTCYAN);
    rectangle(left, top, right, bottom);
    rectangle(left - 2, top - 2, right + 2, bottom + 2);
    
    // Header bottom line (YELLOW)
    setcolor(YELLOW);
    line(left, top + 50, right, top + 50);
    line(left, top + 51, right, top + 51);
    
    // Horizontal row separator lines
    setcolor(DARKGRAY);
    for (int i = 1; i < 6; i++) {
        int y = top + 50 + i * 60;
        line(left, y, right, y);
    }
    
    // Vertical column divider lines
    setcolor(LIGHTCYAN);
    line(col1, top, col1, bottom);
    line(col2, top, col2, bottom);
    line(col3, top, col3, bottom);
    
    // Table Header Text
    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(left + 15, top + 12, "Algorithm");
    outtextxy(col1 + 15, top + 12, "Category");
    outtextxy(col2 + 15, top + 12, "Arithmetic");
    outtextxy(col3 + 15, top + 12, "Key Features & Method");
    
    // Row Data
    const char* algos[] = {
        "DDA Line",
        "Bresenham Line",
        "Midpoint Line",
        "Midpoint Circle",
        "Cohen-Sutherland",
        "Sutherland-Hodg"
    };
    
    const char* types[] = {
        "Line Drawing",
        "Line Drawing",
        "Line Drawing",
        "Circle Drawing",
        "Line Clipping",
        "Polygon Clip"
    };
    
    const char* arith[] = {
        "Floating-Point",
        "Pure Integer",
        "Pure Integer",
        "Pure Integer",
        "Bitwise & Float",
        "Float Intersect"
    };
    
    const char* feats[] = {
        "Differential increments, round() per pixel",
        "Integer error decision param (p = 2dy - dx)",
        "Evaluates midpoint across all 6 slope cases",
        "8-Way symmetric reflection (d0 = 1 - R)",
        "4-Bit region outcodes, 9 regions clip",
        "4-Stage pipeline (All 4 Cases: IN/OUT)"
    };
    
    int arithColors[] = {
        LIGHTRED,
        LIGHTGREEN,
        LIGHTGREEN,
        LIGHTGREEN,
        YELLOW,
        YELLOW
    };
    
    for (int i = 0; i < 6; i++) {
        int yText = top + 50 + i * 60 + 20;
        
        // Col 1: Algorithm Name
        setcolor(WHITE);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(left + 15, yText - 2, algos[i]);
        
        // Col 2: Category
        setcolor(LIGHTCYAN);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(col1 + 15, yText - 2, types[i]);
        
        // Col 3: Arithmetic
        setcolor(arithColors[i]);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(col2 + 15, yText - 2, arith[i]);
        
        // Col 4: Key Features
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(col3 + 15, yText + 3, feats[i]);
    }
    
    // Notes below table
    setcolor(LIGHTGRAY);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(40, 530, "[*] Integer Algorithms (Bresenham, Midpoint) eliminate float division for maximum GPU/CPU speed.");
    outtextxy(40, 552, "[*] Sutherland-Hodgman operates in 4 sequential stages (Left -> Right -> Bottom -> Top boundaries).");
    
    // Return prompt
    setcolor(LIGHTGREEN);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(340, 600, "Press Any Key to Return...");
    getch();
}

void algorithmInfo(int algo) {
    cleardevice();
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    
    if (algo == 1) {
        setcolor(YELLOW); outtextxy(100, 50, "DDA INFORMATION"); setcolor(WHITE);
        outtextxy(50, 100, "Digital Differential Analyzer.");
        outtextxy(50, 150, "Works by finding the longest distance (dx or dy).");
        outtextxy(50, 200, "Divides the shorter distance by the longer one.");
        outtextxy(50, 250, "Uses float math, making it slightly inefficient.");
    } else if (algo == 2) {
        setcolor(YELLOW); outtextxy(100, 50, "BRESENHAM INFORMATION"); setcolor(WHITE);
        outtextxy(50, 100, "Highly optimized line algorithm.");
        outtextxy(50, 150, "Uses only integer addition and subtraction.");
        outtextxy(50, 200, "Tracks a decision parameter to decide pixel placement.");
        outtextxy(50, 250, "Extremely fast for CPU rasterization.");
    } else if (algo == 3) {
        setcolor(YELLOW); outtextxy(100, 50, "MIDPOINT LINE INFORMATION"); setcolor(WHITE);
        outtextxy(50, 100, "Evaluates geometric midpoint between candidate pixels.");
        outtextxy(50, 140, "Condition 1 (Slope > 0, |m| <= 1):");
        outtextxy(70, 170, "d = 2*dy - dx.  d<=0: East (x++), d>0: North-East (x++, y++)");
        outtextxy(50, 210, "Condition 2 (Slope < 0, |m| <= 1):");
        outtextxy(70, 240, "d = 2*|dy| - dx. d<=0: East (x++), d>0: South-East (x++, y--)");
        outtextxy(50, 280, "Condition 3 (Steep Slopes |m| > 1):");
        outtextxy(70, 310, "Steps along Y axis, choosing North/South vs Diagonal.");
        outtextxy(50, 350, "Condition 4 (Horizontal / Vertical):");
        outtextxy(70, 380, "Handles dx=0 (Vertical) and dy=0 (Horizontal) directly.");
    } else if (algo == 4) {
        setcolor(YELLOW); outtextxy(100, 50, "MIDPOINT CIRCLE INFORMATION"); setcolor(WHITE);
        outtextxy(50, 100, "Bresenham's Midpoint Circle Drawing Algorithm.");
        outtextxy(50, 140, "Exploits 8-way symmetry: computes only octant 1 (x <= y).");
        outtextxy(50, 180, "Initial decision parameter: d0 = 1 - R");
        outtextxy(50, 220, "If d < 0: choose East pixel (x+1, y)");
        outtextxy(70, 250, "d_new = d + 2x + 3");
        outtextxy(50, 290, "If d >= 0: choose South-East pixel (x+1, y-1)");
        outtextxy(70, 320, "d_new = d + 2(x - y) + 5");
        outtextxy(50, 360, "Plots 8 symmetric points at each step simultaneously.");
    } else if (algo == 5) {
        setcolor(YELLOW); outtextxy(100, 50, "COHEN-SUTHERLAND INFORMATION"); setcolor(WHITE);
        outtextxy(50, 100, "Divides space into 9 regions using a clipping window.");
        outtextxy(50, 150, "Assigns 4-bit region codes (Top, Bottom, Right, Left).");
        outtextxy(50, 200, "Uses Bitwise AND / OR for trivial accept/reject.");
    } else if (algo == 6) {
        setcolor(YELLOW); outtextxy(100, 50, "SUTHERLAND-HODGMAN POLYGON CLIPPING"); setcolor(WHITE);
        outtextxy(50, 95, "Clips a polygon against 4 clip window boundaries in order:");
        outtextxy(70, 125, "1. Left   2. Right   3. Bottom   4. Top");
        outtextxy(50, 160, "For each edge from vertex V1 to V2, tests 4 conditions:");
        outtextxy(70, 195, "Case 1: IN to IN   -> Save V2");
        outtextxy(70, 230, "Case 2: IN to OUT  -> Save Intersection V'");
        outtextxy(70, 265, "Case 3: OUT to OUT -> Discard (Nothing saved)");
        outtextxy(70, 300, "Case 4: OUT to IN  -> Save Intersection V' and V2");
        outtextxy(50, 345, "Produces a completely closed, clipped output polygon.");
    } else {
        outtextxy(50, 100, "Invalid Choice.");
    }
    
    setcolor(LIGHTGREEN);
    outtextxy(300, 600, "Press Any Key to Return...");
    getch();
}

void aboutProject() {
    cleardevice();
    
    // Header
    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(100, 30, "ABOUT PROJECT");
    
    // Subtitle
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(100, 80, "GRAPHICS ALGORITHM VISUALIZER");
    outtextxy(100, 115, "A Computer Graphics Educational Project");
    
    // Department, Guide & Academic Year
    outtextxy(100, 165, "Department   : MCA Department");
    outtextxy(100, 200, "Guide        : Dr. Ravikumar Roogi");
    outtextxy(100, 235, "Academic Year: 2025 - 2027");
    
    // Students Section (Replacing USN with UUCMS)
    setcolor(YELLOW);
    outtextxy(100, 280, "Student Names & UUCMS Numbers:");
    
    setcolor(WHITE);
    outtextxy(100, 315, "1. Chaitra N   - UUCMS NO: PO2DR25S126006");
    outtextxy(100, 350, "2. Vasanta M   - UUCMS NO: PO2DR25S126038");
    outtextxy(100, 385, "3. Ananya S    - UUCMS NO: PO2DR25S126002");
    outtextxy(100, 420, "4. Riya N      - UUCMS NO: PO2DR25S126023");
    outtextxy(100, 455, "5. Misba M     - UUCMS NO: P02DR25S126015");
    outtextxy(100, 490, "6. Iranna K    - UUCMS NO: P02DR25S126010");
    
    // Return prompt
    setcolor(LIGHTGREEN);
    outtextxy(300, 580, "Press Any Key to Return...");
    getch();
}

void setAnimationSpeed() {
    cleardevice();
    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 3);
    outtextxy(100, 50, "SET ANIMATION SPEED");
    
    setcolor(WHITE);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    outtextxy(100, 150, "1. Slow (200ms)");
    outtextxy(100, 200, "2. Medium (50ms) - DEFAULT");
    outtextxy(100, 250, "3. Fast (10ms)");
    
    int choice = getch();
    if (choice == '1') anim_delay = 200;
    else if (choice == '2') anim_delay = 50;
    else if (choice == '3') anim_delay = 10;
    
    outtextxy(100, 400, "Speed Updated Successfully!");
    delay(1000);
}
