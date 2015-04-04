#include "GuiClass.h"

MKWindow::MKWindow(gint Width, gint Height,char *Title, bool EndAppOnClose)
{
    Widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(Widget), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(Widget), Width, Height);
    gtk_window_set_title(GTK_WINDOW(Widget), Title);
    gtk_container_set_border_width(GTK_CONTAINER(Widget), 5);  
    if(EndAppOnClose) g_signal_connect_swapped(G_OBJECT(Widget), "destroy",G_CALLBACK(gtk_main_quit), G_OBJECT(Widget));
}

void MKWindow::ShowAll()
{
    gtk_widget_show_all(Widget);   
}

void MKWindow::AddContainer(GtkWidget *Cont)
{
    gtk_container_add(GTK_CONTAINER(Widget), Cont);    
}

///////////////////////////////////////////////////
MKVBox::MKVBox(bool Homogeneus)
{
    Widget = gtk_vbox_new(Homogeneus, 1);    
}
void MKVBox::AddElement(GtkWidget *Cont)
{
    gtk_box_pack_start(GTK_BOX(Widget), Cont, FALSE, FALSE, 3);    
}
///////////////////////////////////////////////////////
MKHBox::MKHBox(bool Homogeneus)
{
    Widget = gtk_hbox_new(Homogeneus, 1);    
}
void MKHBox::AddElement(GtkWidget *Cont)
{
    gtk_box_pack_start(GTK_BOX(Widget), Cont, TRUE, TRUE, 0);    
}
///////////////////////////////////////////////////////
MKFixed::MKFixed(bool Init)
{
    Widget = gtk_fixed_new();    
}
void MKFixed::AddElement(GtkWidget *element, int x, int y, int sizex, int sizey)
{
    gtk_fixed_put(GTK_FIXED(Widget), element, x, y); 
    gtk_widget_set_size_request(element, sizex, sizey);   
}
///////////////////////////////////////////////////////
MKButton::MKButton(char *Caption)
{
    Widget = gtk_button_new_with_label(Caption);
}
////////////////////////////////////////////////////////
MKToggleButton::MKToggleButton(char *Caption)
{
    Widget = gtk_toggle_button_new_with_label(Caption);
}
////////////////////////////////////////////////////////
MKImage::MKImage(char *File)
{
    Widget = gtk_image_new_from_file(File);
}
////////////////////////////////////////////////////////
MKDialog::MKDialog(GtkWindow *parent, GtkDialogFlags flags, GtkMessageType type, GtkButtonsType buttons, char *Caption)
{
    Widget = gtk_message_dialog_new (GTK_WINDOW (parent), flags, type, buttons, Caption);
    gtk_window_set_position (GTK_WINDOW (Widget), GTK_WIN_POS_CENTER);
    gtk_dialog_run (GTK_DIALOG (Widget));
    gtk_widget_destroy (Widget);    
}
//////////////////////////////////////////////////////////////
MKLabel::MKLabel(char *Caption)
{
    Widget = gtk_label_new(Caption);
}

void MKLabel::SetLabel(char *Caption)
{
    gtk_label_set_text(GTK_LABEL (Widget),Caption);
}

void MKLabel::SetJustify(GtkJustification type)
{
    gtk_label_set_justify(GTK_LABEL (Widget),type);
}
/////////////////////////////////////////////////////////
MKDrawArea::MKDrawArea(int Widht, int Height)
{
    Widget = gtk_drawing_area_new();
    gtk_widget_set_size_request (Widget, Widht, Height);
}  


/////////////////////////////////////////////////////////
MKMenu::MKMenu(char *Caption)
{
    Menu = gtk_menu_new();
    SubMenu = gtk_menu_item_new_with_label(Caption);    
}
void MKMenu::AddElement(GtkWidget *element)
{
    gtk_menu_shell_append(GTK_MENU_SHELL(Menu), element);
    
}
///////////////////////////////////////
MKMenuBar::MKMenuBar(bool init)
{
    Widget = gtk_menu_bar_new(); 
}
void MKMenuBar::AddSubMenu(GtkWidget *submenubar, GtkWidget *menu)
{
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(submenubar), menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(Widget), submenubar);
}
///////////////////////////////////////////////////
MKMenuElement::MKMenuElement(char* Caption)
{
    Element = gtk_menu_item_new_with_label(Caption);
}




