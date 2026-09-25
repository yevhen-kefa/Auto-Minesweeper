#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>

//447*206 - pixel for cheking win or over


void sleep_ms(int milliseconds){
    usleep(milliseconds * 1000);
}

void check_color(Display *d, Window root, int x, int y){
    XImage *image;
    image = XGetImage(d, root, x, y, 1, 1, AllPlanes, ZPixmap);

    if (image != NULL){
        unsigned long pixel = XGetPixel(image, 0, 0);

        int r = (pixel & image->red_mask) >> 16;
        int g = (pixel & image->green_mask) >> 8;
        int b = (pixel & image->blue_mask);

        printf("Pixel (%d, %d) is: R = %d, G = %d, B = %d\n", x, y, r, g, b);

        XDestroyImage(image);
    }else{
        printf("Pixel not found");
    }
    
}

int getColor(unsigned long pixel, XImage *image) {
    int r = (pixel & image->red_mask) >> 16;
    int g = (pixel & image->green_mask) >> 8;
    int b = (pixel & image->blue_mask);

    if (r == 0 && g == 0 && b == 255) {
        return 1; // blue 1
    } else if (r == 0 && g == 255 && b == 0) {
        return 2; // green 2
    } else if (r == 255 && g == 0 && b == 0) {
        return 3; // red 3
    } else if (r == 0 && g == 0 && b == 123) {
        return 4; // blue 4
    } else if (r == 123 && g == 0 && b == 0) {
        return 5; // brown 5
    } else if (r == 189 && g == 189 && b == 189) {
        return 8; // grey
    } else if (r == 123 && g == 123 && b == 123) {
        return 9; // grey (dont open)
    } else if (r == 255 && g == 255 && b == 0) {
        return 10; // yellow (still alive)
    } else if (r == 0 && g == 0 && b == 0) {
        return 11; // black 
    } else { 
        return 0; // unknown
    }
}


int main(){
    Display *display = XOpenDisplay(NULL);
    if (display == NULL){
        fprintf(stderr, "We can't open display\n");
        return 1;
    }

    Window root = DefaultRootWindow(display);
    
    int loseX = 446;
    int loseY = 206;
    int winX = 442;
    int winY = 199;
    
    
    int upX = 370;
    int upY = 229;
    
    int cellSize = 15;
    int space = 1;
    int step = cellSize + space;
    
    int cols = 9;
    int rows = 9;
    
    int start = 1;
    int n = 1000;
    int count = 0;
    int winers = 0;
    while (start){
        int col = rand() % cols;
        int row = rand() % rows;

        int cellStartX = upX + (col * step);
        int cellStartY = upY + (row * step);


        int targetX = cellStartX + 11; //370*230
        int targetY = cellStartY + 12; //515*370
        
        XWarpPointer(display, None, root, 0, 0, 0, 0, targetX, targetY);
        
        XTestFakeButtonEvent(display, 1, True, CurrentTime);
        XFlush(display);
        
        XTestFakeButtonEvent(display, 1, False, CurrentTime);
        XFlush(display);
        sleep_ms(10);
        // check_color(display, root, loseX, loseY);

        XImage *imageLose = XGetImage(display, root, loseX, loseY, 1, 1, AllPlanes, ZPixmap);
        int colorLoseID = 0;
        if (imageLose != NULL) {
            unsigned long pixel = XGetPixel(imageLose, 0, 0);                
            colorLoseID = getColor(pixel, imageLose);
            XDestroyImage(imageLose);
        }
        XImage *imageWin = XGetImage(display, root, winX, winY, 1, 1, AllPlanes, ZPixmap);
        int colorWinID = 0;
        if (imageWin != NULL) {
            unsigned long pixel = XGetPixel(imageWin, 0, 0);                
            colorWinID = getColor(pixel, imageWin);
            XDestroyImage(imageWin);
        }

        if (colorLoseID == 11){
            XWarpPointer(display, None, root, 0, 0, 0, 0, winX, winY);
        
            XTestFakeButtonEvent(display, 1, True, CurrentTime);
            XFlush(display);
            
            XTestFakeButtonEvent(display, 1, False, CurrentTime);
            XFlush(display);
            // printf("Color lose: %d, Color win: %d\n", colorLoseID, colorWinID);
            count++;
            // sleep_ms(500);
        }
        if (colorWinID == 11){
            winers++;
            start = 0;
            break;
        }
        if (count == 5000){
            start = 0;
            break;
        }
        // for (int i = 0; i < n; i++){
        // }
        // start = 0;
    }



    XCloseDisplay(display);
    printf("Lose: %d, Win: %d\n", count, winers);

    return 0;
}