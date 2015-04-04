#include <gtk/gtk.h>

class MKWindow
{
    public:
    GtkWidget *Widget;  
    MKWindow(gint Width, gint Height,char *Title, bool EndAppOnClose);  
    void AddContainer(GtkWidget *Cont);
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
};

class MKToggleButton
{
    public:
    GtkWidget *Widget;    
    MKToggleButton(char *Caption);
};

class MKImage
{
    public:
    GtkWidget *Widget;    
    MKImage(char *File);
};

class MKDialog
{
    public:
    GtkWidget *Widget;   
    MKDialog(GtkWindow *parent,GtkDialogFlags flags,GtkMessageType type,GtkButtonsType buttons,char *Caption);
};

class MKLabel
{
    public:
    GtkWidget *Widget;   
    MKLabel(char *Caption);
    void SetLabel(char *Caption);
    void SetJustify(GtkJustification type);
};


///////////////////////////////////////////////////////////
class MKDrawArea
{
    public:
    GtkWidget *Widget; 
    MKDrawArea(int Widht, int Height);    
    
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
    MKMenuElement(char* Caption);    
    
};
