#include <gtk/gtk.h>
#include <stdio.h>
#include <fstream.h>
#include <cstdlib>
#define WIN_TITLE "CNSilva Project v0.2b"
void Init_GUI();
void PrintAction();
static gboolean delete_event( GtkWidget *widget,GdkEvent  *event,gpointer data);
static void draw_brush (GtkWidget *widget, gdouble x, gdouble y);
static gboolean expose_event( GtkWidget *widget, GdkEventExpose *event );
static gboolean configure_event( GtkWidget *widget, GdkEventConfigure *event );
static gboolean button_press_event( GtkWidget *widget, GdkEventButton *event );
static gboolean motion_notify_event( GtkWidget *widget, GdkEventMotion *event );
static void createDrawingArea();
