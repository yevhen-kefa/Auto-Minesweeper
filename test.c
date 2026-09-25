#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

int main(int, char**)
{
    XColor c;
    Display *d = XOpenDisplay((char *) NULL);

    int x=0;  // Pixel x 
    int y=0;  // Pixel y

    XImage *image;
    image = XGetImage (d, XRootWindow (d, XDefaultScreen (d)), x, y, 1, 1, AllPlanes, XYPixmap);
    c.pixel = XGetPixel (image, 0, 0);
    XFree (image);
    XQueryColor (d, XDefaultColormap(d, XDefaultScreen (d)), &c);
    cout << c.red/256 << " " << c.green/256 << " " << c.blue/256 << "\n";

    return 0;
}