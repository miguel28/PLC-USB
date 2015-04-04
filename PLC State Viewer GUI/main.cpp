#include <stdlib.h>
#include <gtk/gtk.h>
#include <iostream>
#include "USBInterfaceClass.h"
#include "GUI/GuiClass.h"
#include "GUI/Drawing.h"
#include "usleep.c"

using namespace std;

void OpenHIDUSB(GtkWidget *wid, GtkWidget *win);
void CloseHIDUSB(GtkWidget *wid, GtkWidget *win);

void SetOutput0(GtkWidget *wid, GtkWidget *win);
void SetOutput1(GtkWidget *wid, GtkWidget *win);
void SetOutput2(GtkWidget *wid, GtkWidget *win);
void SetOutput3(GtkWidget *wid, GtkWidget *win);
void SetOutput4(GtkWidget *wid, GtkWidget *win);
void SetOutput5(GtkWidget *wid, GtkWidget *win);
void SetOutput6(GtkWidget *wid, GtkWidget *win);
void SetOutput7(GtkWidget *wid, GtkWidget *win);

static gboolean DrawInput0(GtkWidget *widget,GdkEventExpose *event,gpointer data);
static gboolean DrawInput1(GtkWidget *widget,GdkEventExpose *event,gpointer data);
static gboolean DrawInput2(GtkWidget *widget,GdkEventExpose *event,gpointer data);
static gboolean DrawInput3(GtkWidget *widget,GdkEventExpose *event,gpointer data);
static gboolean DrawInput4(GtkWidget *widget,GdkEventExpose *event,gpointer data);
static gboolean DrawInput5(GtkWidget *widget,GdkEventExpose *event,gpointer data);
static gboolean DrawInput6(GtkWidget *widget,GdkEventExpose *event,gpointer data);
static gboolean DrawInput7(GtkWidget *widget,GdkEventExpose *event,gpointer data);

static gboolean time_handler(GtkWidget *widget);
static gboolean on_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer data);

unsigned char Input,pOutput[8],Output;
cairo_surface_t *image[2];
USBInterface USB;

int main( int argc, char *argv[])
{

    gtk_init(&argc, &argv);
    
    MKWindow Window(500,200,"Virtual PLC State Viewer",true);
    MKFixed fixed(false);
    MKVBox vbox(false);
    MKHBox hbox(false);
    
    image[0] = cairo_image_surface_create_from_png("LEDOFF.png");
    image[1] = cairo_image_surface_create_from_png("LEDON.png");
    
    Window.AddContainer(vbox.Widget);
    vbox.AddElement(fixed.Widget);
    
    MKButton OpenBTN("Open Hid");
    fixed.AddElement(OpenBTN.Widget,0,0,100,30);
    
    MKButton OpenClose("Close HID");
    fixed.AddElement(OpenClose.Widget,100,0,100,30);
    
    MKToggleButton Bit0("Bit 0");
    fixed.AddElement(Bit0.Widget,50,80,50,50);
    
    MKToggleButton Bit1("Bit 1");
    fixed.AddElement(Bit1.Widget,100,80,50,50);
    
    MKToggleButton Bit2("Bit 2");
    fixed.AddElement(Bit2.Widget,150,80,50,50);
    
    MKToggleButton Bit3("Bit 3");
    fixed.AddElement(Bit3.Widget,200,80,50,50);
    
    MKToggleButton Bit4("Bit 4");
    fixed.AddElement(Bit4.Widget,250,80,50,50);
    
    MKToggleButton Bit5("Bit 5");
    fixed.AddElement(Bit5.Widget,300,80,50,50);
    
    MKToggleButton Bit6("Bit 6");
    fixed.AddElement(Bit6.Widget,350,80,50,50);
    
    MKToggleButton Bit7("Bit 7");
    fixed.AddElement(Bit7.Widget,400,80,50,50);
    
    MKLabel Label("HID Is does not loaded.");
    Label.SetJustify(GTK_JUSTIFY_LEFT);
    fixed.AddElement(Label.Widget,80,0,500,50);
    
    fixed.AddElement(hbox.Widget,50,150,410,50);
    

    MKDrawArea Input0(50,50);
    hbox.AddElement(Input0.Widget);
    
    MKDrawArea Input1(50,50);
    hbox.AddElement(Input1.Widget);
    
    MKDrawArea Input2(50,50);
    hbox.AddElement(Input2.Widget);
    
    MKDrawArea Input3(50,50);
    hbox.AddElement(Input3.Widget);
    
    MKDrawArea Input4(50,50);
    hbox.AddElement(Input4.Widget);
    
    MKDrawArea Input5(50,50);
    hbox.AddElement(Input5.Widget);
    
    MKDrawArea Input6(50,50);
    hbox.AddElement(Input6.Widget);
    
    MKDrawArea Input7(50,50);
    hbox.AddElement(Input7.Widget);
    
    
    g_signal_connect(OpenBTN.Widget, "clicked",G_CALLBACK (OpenHIDUSB), (gpointer) Label.Widget);
    g_signal_connect(OpenClose.Widget, "clicked",G_CALLBACK (CloseHIDUSB), (gpointer) Label.Widget);
  
    g_signal_connect(Input0.Widget, "expose-event",G_CALLBACK (DrawInput0), (gpointer)Input1.Widget);
    g_signal_connect(Input1.Widget, "expose-event",G_CALLBACK (DrawInput1), NULL);
    g_signal_connect(Input2.Widget, "expose-event",G_CALLBACK (DrawInput2), NULL);
    g_signal_connect(Input3.Widget, "expose-event",G_CALLBACK (DrawInput3), NULL);
    g_signal_connect(Input4.Widget, "expose-event",G_CALLBACK (DrawInput4), NULL);
    g_signal_connect(Input5.Widget, "expose-event",G_CALLBACK (DrawInput5), NULL);
    g_signal_connect(Input6.Widget, "expose-event",G_CALLBACK (DrawInput6), NULL);
    g_signal_connect(Input7.Widget, "expose-event",G_CALLBACK (DrawInput7), NULL);
                                                    
    g_signal_connect(Bit0.Widget,"clicked",G_CALLBACK(SetOutput0),NULL);
    g_signal_connect(Bit1.Widget,"clicked",G_CALLBACK(SetOutput1),NULL);
    g_signal_connect(Bit2.Widget,"clicked",G_CALLBACK(SetOutput2),NULL);
    g_signal_connect(Bit3.Widget,"clicked",G_CALLBACK(SetOutput3),NULL);
    g_signal_connect(Bit4.Widget,"clicked",G_CALLBACK(SetOutput4),NULL);
    g_signal_connect(Bit5.Widget,"clicked",G_CALLBACK(SetOutput5),NULL);
    g_signal_connect(Bit6.Widget,"clicked",G_CALLBACK(SetOutput6),NULL);
    g_signal_connect(Bit7.Widget,"clicked",G_CALLBACK(SetOutput7),NULL);
    
    g_timeout_add(1, (GSourceFunc) time_handler, (gpointer) Window.Widget);
    /*Window.AddCOFFtainer(fixed.Widget);
    MKDrawArea darea(640,480);
    MKLabel label("1");
    
    MKButtOFF message("Message");
    MKButtOFF add("Add");
   
    MKMenuBar MenuBar(true);  
    MKMenu Menu1("File");
    MKMenu Menu2("Edit");
    MKMenuElement newf("New");
    MKMenuElement quit("Quit");

    MenuBar.AddSubMenu(Menu1.SubMenu,Menu1.Menu);
    MenuBar.AddSubMenu(Menu2.SubMenu,Menu2.Menu);
    Menu1.AddElement(newf.Element);
    Menu1.AddElement(quit.Element);
    
    
    vbox.AddElement(MenuBar.Widget);
    vbox.AddElement(hbox.Widget);
    
    hbox.AddElement(message.Widget);
    hbox.AddElement(add.Widget);
    hbox.AddElement(label.Widget);
    
    vbox.AddElement(darea.Widget);

    fixed.AddElement(MenuBar.Widget,0,0,500,500);


*/

    //g_signal_connect(message.Widget, "clicked",G_CALLBACK(Mensaje), (gpointer) Window.Widget);
    //g_signal_connect(add.Widget, "clicked",G_CALLBACK(AddLine), (gpointer) label.Widget);
    
    //g_signal_connect(darea.Widget, "expose-event",G_CALLBACK (OFF_expose_event), NULL);
    
    //g_timeout_add(10, (GSourceFunc) time_handler, (gpointer) Window.Widget);

    Window.ShowAll();
    //time_handler(Window.Widget);
    
    gtk_main();

    return 0;
}

void OpenHIDUSB(GtkWidget *wid, GtkWidget *win)
{
    USB.EnumerateHIDs();
    USB.HIDOpen(0x3995,0x0001);	
    USB.HIDDescription();
    if(USB.HIDisOpen) gtk_label_set_text(GTK_LABEL (win),"Hid Opened, VendorID=3995, ProductID=0001");
}
void CloseHIDUSB(GtkWidget *wid, GtkWidget *win)
{
    USB.HIDClose();
    if(!USB.HIDisOpen) gtk_label_set_text(GTK_LABEL (win),"Hid Closed...");
}
void SetOutput0(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pOutput[0]=0x01;
    else pOutput[0]=0x00;
}
void SetOutput1(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pOutput[1]=0x01;
    else pOutput[1]=0x00;
}
void SetOutput2(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pOutput[2]=0x01;
    else pOutput[2]=0x00;
}
void SetOutput3(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pOutput[3]=0x01;
    else pOutput[3]=0x00;
}
void SetOutput4(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pOutput[4]=0x01;
    else pOutput[4]=0x00;
}
void SetOutput5(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pOutput[5]=0x01;
    else pOutput[5]=0x00;
}
void SetOutput6(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pOutput[6]=0x01;
    else pOutput[6]=0x00;
}
void SetOutput7(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pOutput[7]=0x01;
    else pOutput[7]=0x00;
}


static gboolean DrawInput0(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    if(!(Input&(0x01)))Img.DrawSurface(image[0]);
    else Img.DrawSurface(image[1]);
    
    Img.Destroy();

    return FALSE;
}

static gboolean DrawInput1(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    if(!(Input&(0x02)))Img.DrawSurface(image[0]);
    else Img.DrawSurface(image[1]);
    
    Img.Destroy();

    return FALSE;
}

static gboolean DrawInput2(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    if(!(Input&(0x04)))Img.DrawSurface(image[0]);
    else Img.DrawSurface(image[1]);
    
    Img.Destroy();

    return FALSE;
}

static gboolean DrawInput3(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    if(!(Input&(0x08)))Img.DrawSurface(image[0]);
    else Img.DrawSurface(image[1]);
    
    Img.Destroy();

    return FALSE;
}

static gboolean DrawInput4(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    if(!(Input&(0x10)))Img.DrawSurface(image[0]);
    else Img.DrawSurface(image[1]);
    
    Img.Destroy();

    return FALSE;
}

static gboolean DrawInput5(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    if(!(Input&(0x20)))Img.DrawSurface(image[0]);
    else Img.DrawSurface(image[1]);
    
    Img.Destroy();

    return FALSE;
}

static gboolean DrawInput6(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    if(!(Input&(0x40)))Img.DrawSurface(image[0]);
    else Img.DrawSurface(image[1]);
    
    Img.Destroy();

    return FALSE;
}

static gboolean DrawInput7(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    if(!(Input&(0x80)))Img.DrawSurface(image[0]);
    else Img.DrawSurface(image[1]);
    
    Img.Destroy();

    return FALSE;
}


void Mensaje(GtkWidget *wid, GtkWidget *win)
{
    MKDialog Dialog(GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Hello World!");
}

static gboolean time_handler(GtkWidget *widget)
{
    if (widget->window == NULL) return FALSE;
    gtk_widget_queue_draw(widget);
    Output=0x00;
    Output=(pOutput[7]<<7)|(pOutput[6]<<6)|(pOutput[5]<<5)|(pOutput[4]<<4)|(pOutput[3]<<3)|(pOutput[2]<<2)|(pOutput[1]<<1)|(pOutput[0]);
    USB.SendOutputPort(Output);
    Input = USB.GetInputBuffer();
    return TRUE;
}




static gboolean on_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    CairoImg Img(widget->window);
    
    /*if(!Input)image = cairo_image_surface_create_from_png("LEDOFF.png");
    else image = cairo_image_surface_create_from_png("LEDON.png");
    Img.DrawSurface(image);
    */
    
    /*Img.SetColor(0,0,1);
    Img.DrawLine(50,20,50,100,0.5);
    Img.DrawLine(450,20,450,100,0.5);


    Img.SetFont("Sans");
    Img.DrawTextA(10,50,10,"Hola");
*/

    return FALSE;
}
