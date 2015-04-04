#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

class CairoImg
{
    public:
    cairo_t *Image;
    CairoImg(GdkDrawable *drawable);
    void SetColor(double r, double g, double b);   
    void DrawLine(int x1, int y1, int x2, int y2, double width);
    void Destroy(); 
    void SetFont(char* font);
    void DrawTextA(int x,int y,int size,char* text);
    void DrawSurface(cairo_surface_t *surface);
    void DrawRectangle(int x1, int y1, int x2, int y2, double width);
    void DrawArc(double x, double y, double radium, double angle1, double angle2, double width);
    void Fill(double r, double g, double b);
};
