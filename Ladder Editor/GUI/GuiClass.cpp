#include "GuiClass.h"

using namespace std;

MKWindow::MKWindow(gint Width, gint Height,char *Title, bool EndAppOnClose)
{
    gtk_init(NULL, NULL);
    Widget = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(Widget), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(Widget), Width, Height);
    gtk_window_set_title(GTK_WINDOW(Widget), Title);
    gtk_container_set_border_width(GTK_CONTAINER(Widget), 1);  
    //if(EndAppOnClose) g_signal_connect_swapped(G_OBJECT(Widget), "destroy",G_CALLBACK(gtk_main_quit), G_OBJECT(Widget));
}

void MKWindow::ShowAll()
{
    gtk_widget_show_all(Widget);   
}

void MKWindow::AddContainer(GtkWidget *Cont)
{
    gtk_container_add(GTK_CONTAINER(Widget), Cont);    
}
void MKWindow::AddEvents()
{
    gtk_widget_add_events(GTK_WIDGET(Widget), GDK_CONFIGURE);
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
void MKButton::SetSize(int x, int y)
{
    gtk_widget_set_size_request(Widget,x,y);
}
////////////////////////////////////////////////////////
MKToggleButton::MKToggleButton(char *Caption)
{
    Widget = gtk_toggle_button_new_with_label(Caption);
}
void MKToggleButton::ReOpen(char *Caption)
{
    Widget = gtk_toggle_button_new_with_label(Caption);  
}
////////////////////////////////////////////////////////
MKCheckButton::MKCheckButton(char *Caption)
{
    Widget = gtk_check_button_new_with_label(Caption);
}
bool MKCheckButton::GetState()
{
    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Widget));
}
void MKCheckButton::SetState(bool state)
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Widget), state);
}
////////////////////////////////////////////////////////
MKImage::MKImage(char *File)
{
    Widget = gtk_image_new_from_file(File);
    pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(Widget));
}
void MKImage::SetActive(bool Active)
{
    gtk_widget_set_sensitive(Widget,Active);   
}
void MKImage::ReOpen()
{
    Widget = gtk_image_new_from_pixbuf(pixbuf);
    
}
////////////////////////////////////////////////////////
MKDialog::MKDialog(GtkWindow *parent, GtkDialogFlags flags, GtkMessageType type, GtkButtonsType buttons, char *Caption)
{
    Widget = gtk_message_dialog_new (parent, flags, type, buttons, Caption);
    gtk_window_set_position (GTK_WINDOW (Widget), GTK_WIN_POS_CENTER);
    //gtk_dialog_run (GTK_DIALOG (Widget));
    //gtk_widget_destroy (Widget);    
}
gint MKDialog::Run()
{
    response = gtk_dialog_run(GTK_DIALOG (Widget));
    return response;  
}

void MKDialog::Destroy()
{
    gtk_widget_destroy(Widget);
}

///////////////////////////////////////////////////////////
MKFileChooser::MKFileChooser(GtkWindow *parent,GtkFileChooserAction action,gchar *button,char *Caption)
{
    
    dialog = gtk_file_chooser_dialog_new (Caption,
										  GTK_WINDOW(parent),
										  action,
										  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
										  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT);
	//gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (Widget), TRUE);
    //gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog), "C:\\");
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER); 
}
gint MKFileChooser::Run()
{
    response = gtk_dialog_run(GTK_DIALOG (dialog));
    return response;  
}
char* MKFileChooser::GetFileName()
{
    return gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));    
}
void MKFileChooser::Destroy()
{
    gtk_widget_destroy (dialog);
}
////////////////////////////////////////////////////////
MKCustomDialog::MKCustomDialog(GtkWidget *win, char* Caption)
{  
     dialog = gtk_dialog_new_with_buttons (Caption,
    					GTK_WINDOW (win),
    					GTK_DIALOG_MODAL,
    					GTK_STOCK_OK,
    					GTK_RESPONSE_OK,
                        "_Cancel",
                        GTK_RESPONSE_CANCEL,
    					NULL);
    
      hbox = gtk_hbox_new (FALSE, 0);
      gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
      gtk_container_set_border_width (GTK_CONTAINER (hbox), 0);
      gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), hbox, FALSE, FALSE, 0);   
}
void MKCustomDialog::AddButton(char *Caption, gint id)
{
    gtk_dialog_add_button(GTK_DIALOG (dialog), Caption,id);
} 
void MKCustomDialog::AddElement(GtkWidget *Element)
{
    gtk_box_pack_start (GTK_BOX (hbox), Element, TRUE, TRUE, 0);
}
gint MKCustomDialog::Run()
{
    gtk_widget_show_all (hbox);
    response = gtk_dialog_run(GTK_DIALOG (dialog));
    return response;  
}
void MKCustomDialog::Destroy()
{
    gtk_widget_destroy (dialog);
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
void MKLabel::Destroy()
{
    gtk_widget_destroy (Widget);
}
//////////////////////////////////////////////////////
MKEntry::MKEntry(unsigned int MaxLen)
{
    Widget = gtk_entry_new();
    if(MaxLen>0)
        gtk_entry_set_max_length(GTK_ENTRY(Widget),MaxLen);
        
}
void MKEntry::SetText(char *Text)
{ 
    gtk_entry_set_text(GTK_ENTRY(Widget),Text);      
}

gchar *MKEntry::GetText()
{
    return (gchar *)gtk_entry_get_text(GTK_ENTRY(Widget)); 
}
int MKEntry::GetLen()
{
    return strlen(GetText());
}
//////////////////////////////////////////////////////
MKTextView::MKTextView(unsigned int MaxLen)
{
    Widget = gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Widget ));
    gtk_text_buffer_create_tag(buffer, "gap",
        "pixels_above_lines", 30, NULL);

  gtk_text_buffer_create_tag(buffer, "lmarg", 
      "left_margin", 5, NULL);
  gtk_text_buffer_create_tag(buffer, "blue_fg", 
      "foreground", "blue", NULL); 
  gtk_text_buffer_create_tag(buffer, "gray_bg", 
      "background", "gray", NULL); 
  gtk_text_buffer_create_tag(buffer, "italic", 
      "style", PANGO_STYLE_ITALIC, NULL);
  gtk_text_buffer_create_tag(buffer, "bold", 
      "weight", PANGO_WEIGHT_BOLD, NULL);

    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    
    gtk_text_buffer_get_start_iter(buffer,&start);
    gtk_text_buffer_get_end_iter(buffer,&end);


//gtk_text_buffer_insert(buffer, &iter, "Write a short Description of your Project...", -1);

/*  gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, 
        "Colored Text\n", -1, "blue_fg", "lmarg",  NULL);
  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
        "Text with colored background\n", -1, "lmarg", "gray_bg", NULL);

  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
        "Text in italics\n", -1, "italic", "lmarg",  NULL);

  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
        "Bold text\n", -1, "bold", "lmarg",  NULL);*/
        
}

void MKTextView::ReOpen(unsigned int MaxLen)
{
    Widget = gtk_text_view_new();
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(Widget ));
    gtk_text_buffer_create_tag(buffer, "gap",
        "pixels_above_lines", 30, NULL);

  gtk_text_buffer_create_tag(buffer, "lmarg", 
      "left_margin", 5, NULL);
  gtk_text_buffer_create_tag(buffer, "blue_fg", 
      "foreground", "blue", NULL); 
  gtk_text_buffer_create_tag(buffer, "gray_bg", 
      "background", "gray", NULL); 
  gtk_text_buffer_create_tag(buffer, "italic", 
      "style", PANGO_STYLE_ITALIC, NULL);
  gtk_text_buffer_create_tag(buffer, "bold", 
      "weight", PANGO_WEIGHT_BOLD, NULL);

    gtk_text_buffer_get_iter_at_offset(buffer, &iter, 0);
    
    gtk_text_buffer_get_start_iter(buffer,&start);
    gtk_text_buffer_get_end_iter(buffer,&end);


//gtk_text_buffer_insert(buffer, &iter, "Write a short Description of your Project...", -1);

/*  gtk_text_buffer_insert_with_tags_by_name(buffer, &iter, 
        "Colored Text\n", -1, "blue_fg", "lmarg",  NULL);
  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
        "Text with colored background\n", -1, "lmarg", "gray_bg", NULL);

  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
        "Text in italics\n", -1, "italic", "lmarg",  NULL);

  gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, 
        "Bold text\n", -1, "bold", "lmarg",  NULL);*/
        
}

void MKTextView::SetText(char *Text, int Len)
{ 
    gtk_text_buffer_set_text(buffer,Text,Len);     
}

gchar *MKTextView::GetText()
{
    gtk_text_buffer_get_start_iter(buffer,&start);
    gtk_text_buffer_get_end_iter(buffer,&end);
    return (gchar *) gtk_text_buffer_get_text(buffer,&start,&end,false);
}

/////////////////////////////////////////////////////////
MKDrawArea::MKDrawArea(int Widht, int Height)
{
    Widget = gtk_drawing_area_new();
    gtk_widget_set_size_request(Widget, Widht, Height);
}  
void MKDrawArea::AddEvents()
{
    gtk_widget_add_events (Widget, GDK_BUTTON_PRESS_MASK);
}
void MKDrawArea::SetActive(bool Active)
{
    gtk_widget_set_sensitive(Widget,Active);   
}
/////
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
MKMenuElement::MKMenuElement(const gchar *stock_id, GtkAccelGroup *accel_group)
{
    Element = gtk_image_menu_item_new_from_stock(stock_id, accel_group);    
}   
MKMenuElement::MKMenuElement(bool Separator)
{
    Element = gtk_separator_menu_item_new();
}/*
void MKMenuElement::SetAccelerator()
{
    accel_group = gtk_accel_group_new();
    //gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

    gtk_widget_add_accelerator(Element, "activate", accel_group, 
    GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE); 
    
}*/
void MKMenuElement::SetActive(bool Active)
{
    gtk_widget_set_sensitive(Element,Active);   
}
////////////////////////////////ToolBar/////////////////
MKToolBar::MKToolBar(bool Init)
{
    Widget = gtk_toolbar_new();
}
void MKToolBar::AddElement(GtkToolItem *element)
{
    gtk_toolbar_insert(GTK_TOOLBAR(Widget), element, -1);    
}

MKToolBarElement::MKToolBarElement(const gchar *stock_id)
{
    Element = gtk_tool_button_new_from_stock(stock_id);
} 
MKToolBarElement::MKToolBarElement(GtkWidget *icon_widget, const gchar *label)
{
    Element = gtk_tool_button_new(icon_widget,label);
}   
MKToolBarElement::MKToolBarElement(bool Init)
{
    Element = gtk_separator_tool_item_new();
}
void MKToolBarElement::ReOpen(const gchar *stock_id)
{
    Element = gtk_tool_button_new_from_stock(stock_id);
}
void MKToolBarElement::SetLabel(const char* Label)
{
    gtk_tool_button_set_label((GtkToolButton*)Element,Label);    
}
void MKToolBarElement::SetActive(bool Active)
{
    gtk_widget_set_sensitive(GTK_WIDGET(Element),Active);      
}
////////////////////
