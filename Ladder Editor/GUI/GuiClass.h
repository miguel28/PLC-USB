#include <gtk/gtk.h>
#include <iostream>
#include <string.h>

class MKWindow
{
    public:
    GtkWidget *Widget;  
    MKWindow(gint Width, gint Height,char *Title, bool EndAppOnClose);  
    void AddContainer(GtkWidget *Cont);
    void AddEvents();
    void ShowAll();
};

class MKVBox
{
    public:
    GtkWidget *Widget;
    MKVBox(bool Homogeneus);  
    void AddElement(GtkWidget *Cont);    
};

class MKHBox
{
    public:
    GtkWidget *Widget;
    MKHBox(bool Homogeneus);  
    void AddElement(GtkWidget *Cont);    
};

class MKFixed
{
    public:
    GtkWidget *Widget;
    MKFixed(bool Init);
    void AddElement(GtkWidget *element, int x, int y, int sizex, int sizey);
};

class MKButton
{
    public:
    GtkWidget *Widget;    
    MKButton(char *Caption);
    void SetSize(int x, int y);
};

class MKToggleButton
{
    public:
    GtkWidget *Widget;    
    MKToggleButton(char *Caption);
    void ReOpen(char *Caption);
};

class MKCheckButton
{
    public:
    GtkWidget *Widget;    
    MKCheckButton(char *Caption);
    bool GetState();
    void SetState(bool state);
};

class MKImage
{
    public:
    GtkWidget *Widget;   
    GdkPixbuf* pixbuf; 
    MKImage(char *File);
    void SetActive(bool Active);
    void ReOpen();
};

class MKDialog
{
    public:
    gint response;
    GtkWidget *Widget;   
    MKDialog(GtkWindow *parent,GtkDialogFlags flags,GtkMessageType type,GtkButtonsType buttons,char *Caption);
    gint Run();
    void Destroy();
    //void AddButton(GtkButtonsType buttons);
};
class MKFileChooser
{
    public:
    GtkWidget *dialog;   
    gint response; 
    
    MKFileChooser(GtkWindow *parent,GtkFileChooserAction action,gchar *button,char *Caption);
    gint Run();
    void Destroy();
    char* GetFileName();
    
};

class MKCustomDialog
{
    public:
    GtkWidget *dialog;
    GtkWidget *hbox;
    gint response; 
    
    
    MKCustomDialog(GtkWidget *win, char* Caption); 
    void AddButton(char *Caption, gint id); 
    void AddElement(GtkWidget *Element);  
    gint Run();
    void Destroy();
};


class MKLabel
{
    public:
    GtkWidget *Widget;   
    MKLabel(char *Caption);
    void SetLabel(char *Caption);
    void SetJustify(GtkJustification type);
    void Destroy();
};

class MKEntry
{
    public:
    GtkWidget *Widget;  
    MKEntry(unsigned int MaxLen);
    void SetText(char *Text);
    gchar *GetText();
    int GetLen();
};

class MKTextView
{
    public:
    GtkWidget *Widget;  
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    GtkTextIter iter;
    
    MKTextView(unsigned int MaxLen);
    void ReOpen(unsigned int MaxLen);
    void SetText(char *Text, int Len);
    gchar *GetText();
    //int GetLen();
};

///////////////////////////////////////////////////////////
class MKDrawArea
{
    public:
    GtkWidget *Widget; 
    MKDrawArea(int Widht, int Height);   
    void AddEvents(); 
    void SetActive(bool Active);
};
//////////////////////////////////////////// Menus //////////////////
class MKMenu
{
    public:
    GtkWidget *Menu,*SubMenu;  
    MKMenu(char *Caption);
    void AddElement(GtkWidget *element);
};

class MKMenuBar
{
    public:
    GtkWidget *Widget;  
    MKMenuBar(bool init);
    void AddSubMenu(GtkWidget *submenubar, GtkWidget *menu);
};

class MKMenuElement
{
    public:
    GtkWidget *Element;
    GtkAccelGroup *accel_group;
    MKMenuElement(char* Caption); 
    MKMenuElement(const gchar *stock_id, GtkAccelGroup *accel_group);   
    MKMenuElement(bool Separator);   
    void SetAccelerator();
    void SetActive(bool Active);
    
};
/////////////////////////////////////Tool Bars////////////////
class MKToolBar
{
    public:
    GtkWidget *Widget;  
    MKToolBar(bool Init);
    void AddElement(GtkToolItem *element);
};
class MKToolBarElement
{
    public:
    GtkToolItem *Element;
    MKToolBarElement(const gchar *stock_id);   
    MKToolBarElement(GtkWidget *icon_widget, const gchar *label);   
    MKToolBarElement(bool Init);
    void ReOpen(const gchar *stock_id);
    void SetLabel(const char* Label);
    void SetActive(bool Active);
};
