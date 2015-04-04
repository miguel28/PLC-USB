#include <cairo.h>
#include <gtk/gtk.h>
#include <SDL/SDL.h>
#include <string>
#include <iostream>
using namespace std;

cairo_surface_t *image;



static gboolean
on_expose_event(GtkWidget *widget,
    GdkEventExpose *event,
    gpointer data)
{
  cairo_t *cr;

  cr = gdk_cairo_create (widget->window);

  cairo_set_source_surface(cr, image, 10, 10);
  cairo_paint(cr);

  cairo_destroy(cr);

  return FALSE;
}


int main(int argc, char *argv[])
{
  GtkWidget *window;

if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return 1;
    }

    //Set up the screen
    //putenv("SDL_WINDOWID=1");
    SDL_Surface *screen;
    screen = SDL_SetVideoMode( 500, 500,32, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return 1;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Hello World", NULL );

    //Load the images
    SDL_Surface *sdlsurf;
    sdlsurf = SDL_LoadBMP( "ig.bmp" );
    //SDL_BlitSurface( sdlsurf, NULL, screen, NULL);
    //image = cairo_image_surface_create_for_data ((unsigned char *)(sdlsurf->pixels),CAIRO_FORMAT_RGB24,200,152,sdlsurf->pitch);
    image = cairo_image_surface_create_from_png("castleimage.png");
    //image = cairo_image_surface_create (CAIRO_FORMAT_ARGB32,sdlsurf->w,sdlsurf->h);
    
    SDL_BlitSurface( sdlsurf, NULL, screen, NULL);

cout <<sdlsurf->pitch <<endl;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  g_signal_connect(window, "expose-event",
      G_CALLBACK (on_expose_event), NULL);
  g_signal_connect(window, "destroy",
      G_CALLBACK (gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500); 
  gtk_widget_set_app_paintable(window, TRUE);

  gtk_widget_show_all(window);

  gtk_main();

  cairo_surface_destroy(image);

  return 0;
}
