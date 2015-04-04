#include "Drawing.h"

CairoImg::CairoImg(GdkDrawable *drawable)
{
    Image = gdk_cairo_create(drawable);
}
void CairoImg::SetColor(double r, double g, double b)
{
     cairo_set_source_rgb(Image, r,g,b);
}  
void CairoImg::DrawLine(int x1, int y1, int x2, int y2, double width)
{
    //cairo_set_line_width (Image, (double)(width/10));
    cairo_set_line_width (Image, width);
    cairo_move_to(Image, x1, y1);
    cairo_line_to(Image, x2, y2); 
    cairo_stroke(Image);
       
}
void CairoImg::Destroy()
{
    cairo_stroke(Image);
    cairo_destroy(Image);  
}

void CairoImg::SetFont(char* font)
{
    cairo_select_font_face(Image, font,CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_NORMAL);
}
void CairoImg::DrawTextA(int x,int y,int size,char* text)
{
    cairo_set_font_size(Image, size);
    cairo_move_to(Image, x, y);
    cairo_show_text(Image, text); 
}
void CairoImg::DrawSurface(cairo_surface_t *surface)
{
    cairo_set_source_surface(Image, surface,0, 0);
    cairo_paint(Image);
}
    
void CairoImg::DrawRectangle(int x1, int y1, int x2, int y2, double width)
{
    cairo_rectangle(Image, x1, y1, x2, y2);
    cairo_stroke_preserve(Image);
}
void CairoImg::DrawArc(double x, double y, double radium, double angle1, double angle2, double width)
{
    
    cairo_set_line_width (Image, width);
    cairo_arc(Image, x, y, radium, angle1, angle2);
    //cairo_close_path(Image);
    cairo_stroke_preserve(Image);
    cairo_stroke(Image);
}

void CairoImg::Fill(double r, double g, double b)
{
    cairo_set_source_rgb(Image, r, g, b);
    cairo_fill(Image);
    
}
