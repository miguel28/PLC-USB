#include <stdlib.h>
#include <gtk/gtk.h>
#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <time.h>

#include "USBInterfaceClass.h"
#include "GUI/GuiClass.h"
#include "GUI/Drawing.h"
#include "LadderLinkClass/LadderLinkClass.h"

#define LINKSEPARATION 85
#define LINESEPARATION 80
#define DELAY_OF_STEP 5
#define DELAY_OF_STEP2 2
#define DELAY_OF_REDRAW 400

#define __WINDOWS__

#ifdef __WINDOWS__
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

using namespace std;

LadderClass Ladder(true);
_ladder_links ClipBoardLink;
_ProjectConfig CurrentProject;

USBInterface USB;

int width, height;
short SelectedLine,SelectedLink,tSelectedLine=-1,tSelectedLink=0;
char *FileNameTemp;
bool OpenedProject;
char CurrentPath[255];
char pOutputBuffer[8], pInputBuffer[8], InputBuffer, OutputBuffer;
cairo_surface_t *image[2];
bool RuningSimulator;
int Timers[8],RegTimers[8];
char RuningType;
bool Compiled;
string text;


void Mensaje(GtkWidget *wid, GtkWidget *win);
void Delete_Funtion();
void Copy_Funtion();
void Cut_Funtion();
void Paste_Funtion();

void NewProject_Funtion();
void ChooseNewProject(GtkWidget *wid, GtkWidget *win);
void SaveProject_Funtion();
void CloseProject_Funtion();
void OpenProject_Funtion();
void EditProject_Funtion();
void SimulatorProject_Funtion();
void TerminalProject_Funtion();
void Build_Porject();
void ShowLog_Funtion();

void Maximize_window();
void UnMaximize_window();
void Quit();


void Play_Funtion();
void Stop_Funtion();
void RunProject();
gboolean Simulator_handler(GtkWidget *widget);
gboolean Terminal_handler(GtkWidget *widget);

void SetOutput0(GtkWidget *wid, GtkWidget *win);
void SetOutput1(GtkWidget *wid, GtkWidget *win);
void SetOutput2(GtkWidget *wid, GtkWidget *win);
void SetOutput3(GtkWidget *wid, GtkWidget *win);
void SetOutput4(GtkWidget *wid, GtkWidget *win);
void SetOutput5(GtkWidget *wid, GtkWidget *win);
void SetOutput6(GtkWidget *wid, GtkWidget *win);
void SetOutput7(GtkWidget *wid, GtkWidget *win);

void AddLine();

gboolean EditLink_Funtion(GtkWidget *wid, GtkWidget *win);
gboolean AddLinkSetHigh(GtkWidget *wid, GtkWidget *win);
gboolean AddLinkSetLow(GtkWidget *wid, GtkWidget *win);
gboolean AddLinkIf(GtkWidget *wid, GtkWidget *win);
gboolean AddLinkIfNot(GtkWidget *wid, GtkWidget *win);
gboolean AddLinkTimer(GtkWidget *wid, GtkWidget *win);

gboolean time_handler(GtkWidget *widget);
gboolean on_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer data);
gboolean clicked(GtkWidget *widget, GdkEventButton *event,gpointer user_data);


MKWindow Window(800,600,(char*)"Ladder Editor",true);

MKMenuBar MenuBar(true);  
MKMenu Menu1((char*)"File");
MKMenu Menu2((char*)"Edit");
MKMenu Menu3((char*)"Project");
MKMenu Menu4((char*)"Window");
MKMenu Menu5((char*)"Help");

MKMenuElement Mnu_New(GTK_STOCK_NEW, NULL);
MKMenuElement Mnu_Open(GTK_STOCK_OPEN, NULL);
MKMenuElement Mnu_Save(GTK_STOCK_SAVE, NULL);
MKMenuElement Mnu_Close((char*)"Close");
MKMenuElement Mnu_Sep1(true);
MKMenuElement Mnu_Quit(GTK_STOCK_QUIT,NULL);

MKMenuElement Mnu_EditLink((char*)"Edit Link");
MKMenuElement Mnu_Copy((char*)"Copy");
MKMenuElement Mnu_Cut((char*)"Cut");
MKMenuElement Mnu_Paste((char*)"Paste");
MKMenuElement Mnu_Delete((char*)"Delete");

MKMenuElement Mnu_Options((char*)"Project Options");
MKMenuElement Mnu_Sep2(true);
MKMenuElement Mnu_Build((char*)"Build");
MKMenuElement Mnu_RunSimulator((char*)"Run In Simulator");
MKMenuElement Mnu_RunTerminal((char*)"Run In Virtual PLC Terminal");
MKMenuElement Mnu_Sep3(true);
MKMenuElement Mnu_ShowLog((char*)"Show Log");

MKMenuElement Mnu_Maximize((char*)"Maximize Window ");
MKMenuElement Mnu_UnMaximize((char*)"UnMaximize Window");

MKVBox vbox(false);

MKDrawArea darea(50,50);
MKHBox hbox(false);

MKEntry EntPathNew(255);

MKToolBar ToolBar(true);
MKToolBarElement Addline(GTK_STOCK_ADD);
MKToolBarElement EditTool(GTK_STOCK_DND);
MKToolBarElement DeleteTool(GTK_STOCK_DELETE);
MKToolBarElement SepTool(true);

MKImage icn_SetHigh((char*)"Icons/inc_SetHigh.png");
MKToolBarElement SetHighTool(icn_SetHigh.Widget,(char*)"Set High");

MKImage icn_SetLow((char*)"Icons/inc_SetLow.png");
MKToolBarElement SetLowTool(icn_SetLow.Widget,(char*)"Set Low");

MKImage icn_If((char*)"Icons/inc_If.png");
MKToolBarElement IfTool(icn_If.Widget,(char*)"If");

MKImage icn_IfNot((char*)"Icons/inc_IfNot.png");
MKToolBarElement IfNotTool(icn_IfNot.Widget,(char*)"If Not");

MKImage icn_Timer((char*)"Icons/inc_Timer.png");
MKToolBarElement TimerTool(icn_Timer.Widget,(char*)"Timer");

int main( int argc, char *argv[])
{

    Window.AddEvents();
    Window.AddContainer(vbox.Widget);
    
    MenuBar.AddSubMenu(Menu1.SubMenu,Menu1.Menu);
    MenuBar.AddSubMenu(Menu2.SubMenu,Menu2.Menu);
    MenuBar.AddSubMenu(Menu3.SubMenu,Menu3.Menu);
    MenuBar.AddSubMenu(Menu4.SubMenu,Menu4.Menu);
    //MenuBar.AddSubMenu(Menu5.SubMenu,Menu5.Menu);

    Menu1.AddElement(Mnu_New.Element);
    Menu1.AddElement(Mnu_Open.Element);
    Menu1.AddElement(Mnu_Save.Element);
    Menu1.AddElement(Mnu_Close.Element);
    Menu1.AddElement(Mnu_Sep1.Element);
    Menu1.AddElement(Mnu_Quit.Element);
    
    Menu2.AddElement(Mnu_EditLink.Element);
    Mnu_EditLink.SetActive(false);
    Menu2.AddElement(Mnu_Copy.Element);
    Mnu_Copy.SetActive(false);
    Menu2.AddElement(Mnu_Cut.Element);
    Mnu_Cut.SetActive(false);
    Menu2.AddElement(Mnu_Paste.Element);
    Mnu_Paste.SetActive(false);
    Menu2.AddElement(Mnu_Delete.Element);
    
    Menu3.AddElement(Mnu_Options.Element);
    Menu3.AddElement(Mnu_Sep2.Element);
    Menu3.AddElement(Mnu_Build.Element);
    Menu3.AddElement(Mnu_RunSimulator.Element);
    Menu3.AddElement(Mnu_RunTerminal.Element);
    Menu3.AddElement(Mnu_Sep3.Element);
    Menu3.AddElement(Mnu_ShowLog.Element);
    
    Menu4.AddElement(Mnu_Maximize.Element);
    Menu4.AddElement(Mnu_UnMaximize.Element);
    
    vbox.AddElement(MenuBar.Widget);
    vbox.AddElement(ToolBar.Widget);
    
    Addline.SetLabel((char*)"Add Line"); 
    ToolBar.AddElement(Addline.Element);
    EditTool.SetLabel((char*)"Edit"); 
    ToolBar.AddElement(EditTool.Element);
    EditTool.SetActive(false);
    ToolBar.AddElement(DeleteTool.Element);
    ToolBar.AddElement(SepTool.Element);
    ToolBar.AddElement(SetHighTool.Element);
    ToolBar.AddElement(SetLowTool.Element);
    ToolBar.AddElement(IfTool.Element);
    ToolBar.AddElement(IfNotTool.Element);
    ToolBar.AddElement(TimerTool.Element);
////////////////////////////////////////////////    
    darea.AddEvents();
    vbox.AddElement(hbox.Widget);
    vbox.AddElement(darea.Widget);
///////////////////////////////////////////
    Addline.SetActive(false);
    DeleteTool.SetActive(false);
    EditTool.SetActive(false);
    Mnu_Delete.SetActive(false);
    Mnu_EditLink.SetActive(false);
    Mnu_Copy.SetActive(false);
    Mnu_Cut.SetActive(false);
    Mnu_Paste.SetActive(false);
    Mnu_Close.SetActive(false);
    Mnu_Options.SetActive(false);
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(false);
    SetHighTool.SetActive(false);
    SetLowTool.SetActive(false);
    IfTool.SetActive(false);
    IfNotTool.SetActive(false);
    TimerTool.SetActive(false);
///////////////////////////////////////////

    image[0] = cairo_image_surface_create_from_png((char*)"Icons/LEDOFF.png");
    image[1] = cairo_image_surface_create_from_png((char*)"Icons/LEDON.png");

    g_signal_connect(Addline.Element, "clicked",G_CALLBACK (AddLine), (gpointer) Window.Widget);
    g_signal_connect(DeleteTool.Element, "clicked",G_CALLBACK(Delete_Funtion), NULL);
    g_signal_connect(EditTool.Element, "clicked",G_CALLBACK(EditLink_Funtion), NULL);
    g_signal_connect(SetHighTool.Element, "clicked",G_CALLBACK (AddLinkSetHigh), (gpointer) Window.Widget);
    g_signal_connect(SetLowTool.Element, "clicked",G_CALLBACK (AddLinkSetLow), (gpointer) Window.Widget);
    g_signal_connect(IfTool.Element, "clicked",G_CALLBACK (AddLinkIf), (gpointer) Window.Widget);
    g_signal_connect(IfNotTool.Element, "clicked",G_CALLBACK (AddLinkIfNot), (gpointer) Window.Widget);
    g_signal_connect(TimerTool.Element, "clicked",G_CALLBACK (AddLinkTimer), (gpointer) Window.Widget);

    g_signal_connect(darea.Widget, "expose-event",G_CALLBACK (on_expose_event), (gpointer) Window.Widget);
    g_signal_connect(darea.Widget, "button-press-event",G_CALLBACK(clicked), (gpointer) Window.Widget);
    
    g_signal_connect(Mnu_Delete.Element, "activate",G_CALLBACK(Delete_Funtion), NULL);
    g_signal_connect(Mnu_EditLink.Element, "activate",G_CALLBACK(EditLink_Funtion), NULL);
    g_signal_connect(Mnu_Copy.Element, "activate",G_CALLBACK(Copy_Funtion), NULL);
    g_signal_connect(Mnu_Cut.Element, "activate",G_CALLBACK(Cut_Funtion), NULL);
    g_signal_connect(Mnu_Paste.Element, "activate",G_CALLBACK(Paste_Funtion), NULL);
    
    g_signal_connect(Mnu_New.Element, "activate",G_CALLBACK(NewProject_Funtion), NULL);
    g_signal_connect(Mnu_Save.Element, "activate",G_CALLBACK(SaveProject_Funtion), NULL);
    g_signal_connect(Mnu_Close.Element, "activate",G_CALLBACK(CloseProject_Funtion), NULL);
    g_signal_connect(Mnu_Open.Element, "activate",G_CALLBACK(OpenProject_Funtion), NULL);
    
    g_signal_connect(Mnu_Options.Element, "activate",G_CALLBACK(EditProject_Funtion), NULL);
    g_signal_connect(Mnu_RunSimulator.Element, "activate",G_CALLBACK(SimulatorProject_Funtion), NULL);
    g_signal_connect(Mnu_RunTerminal.Element, "activate",G_CALLBACK(TerminalProject_Funtion), NULL);
    g_signal_connect(Mnu_Build.Element, "activate",G_CALLBACK(Build_Porject), NULL);
    g_signal_connect(Mnu_ShowLog.Element, "activate",G_CALLBACK(ShowLog_Funtion), NULL);

    g_signal_connect(Mnu_Maximize.Element, "activate",G_CALLBACK(Maximize_window), NULL);
    g_signal_connect(Mnu_UnMaximize.Element, "activate",G_CALLBACK(UnMaximize_window), NULL);
    
    //g_signal_connect_swapped(G_OBJECT(Widget), "destroy",G_CALLBACK(gtk_main_quit), G_OBJECT(Widget));
    g_signal_connect_swapped(G_OBJECT(Window.Widget), "delete-event",G_CALLBACK(Quit), G_OBJECT(Window.Widget));
    g_signal_connect(Mnu_Quit.Element, "activate",G_CALLBACK(Quit), NULL);
    
    g_timeout_add(DELAY_OF_REDRAW, (GSourceFunc) time_handler, (gpointer) Window.Widget);
    
    Window.ShowAll();
    gtk_main();

    return 0;
}

void Quit()
{
    if(OpenedProject){
        MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, (char*)"Would you like to save your project?");
        if(Info.Run()==-8)
        {
            SaveProject_Funtion();    
        }
        Info.Destroy();
        OpenedProject=false;
        CloseProject_Funtion();
    }
    gtk_main_quit();
}

void Maximize_window()
{
    gtk_window_maximize( GTK_WINDOW(Window.Widget));
    
}
void UnMaximize_window()
{
    gtk_widget_set_size_request(Window.Widget, 800, 600);
    gtk_window_unmaximize(GTK_WINDOW(Window.Widget));
    gtk_widget_set_size_request(darea.Widget, 800-50, 600-180);
    gtk_widget_set_size_request(Window.Widget, 800, 600);
}

void ChooseNewProject(GtkWidget *wid, GtkWidget *win)
{
    
    MKFileChooser Chooser(GTK_WINDOW(win), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, GTK_STOCK_OPEN,(char*)"Select Folder...");
    if(Chooser.Run()== GTK_RESPONSE_ACCEPT)
    {
        EntPathNew.SetText(Chooser.GetFileName());
    }
    Chooser.Destroy();  
}

void NewProject_Funtion()
{
    if(OpenedProject){
        MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, (char*)"Would you like to save your project?");
        if(Info.Run()==-8)
        {
            SaveProject_Funtion();    
        }
        Info.Destroy();
        OpenedProject=false;
        CloseProject_Funtion();
    }
    
    MKCustomDialog Dialog (Window.Widget,(char*)"Create New Project...");
    MKFixed fix(true);
        
    MKLabel LblName((char*)"Project Name");
    MKLabel LblPath((char*)"Path");
    MKLabel LblVersion((char*)"Version");

    MKEntry EntName(25);
    MKEntry EntVersion(7);
    MKButton BtnBrowse((char*)"Browse Folder...");
        
    LblName.SetJustify(GTK_JUSTIFY_LEFT);
    fix.AddElement(LblName.Widget, 5, 5, 80, 25);
    LblPath.SetJustify(GTK_JUSTIFY_LEFT);
    fix.AddElement(LblPath.Widget, 0, 40, 80, 25);
    LblVersion.SetJustify(GTK_JUSTIFY_LEFT);
    fix.AddElement(LblVersion.Widget, 0, 75, 80, 25);  
     
    EntPathNew.Widget = gtk_entry_new();
        
    fix.AddElement(EntName.Widget, 95, 5, 150, 25);
    fix.AddElement(EntPathNew.Widget, 95, 40, 300, 25);
    fix.AddElement(BtnBrowse.Widget, 260, 5, 100, 25);
    
    fix.AddElement(EntVersion.Widget, 95, 75, 150, 25);
    EntVersion.SetText((char*)"1.00");
    
    g_signal_connect(BtnBrowse.Widget, "clicked",G_CALLBACK (ChooseNewProject), (gpointer)Dialog.dialog);      
                         
    Dialog.AddElement(fix.Widget);
    
    Retrun:
    int Response = Dialog.Run();
    if(Response==-5 && EntPathNew.GetLen()>0 && EntName.GetLen()>0)
    {
        char TempPath[255];
        
        chdir(EntPathNew.GetText());
        
        #ifdef __WINDOWS__
        if(_mkdir(EntName.GetText())==0)
        #else
        if(mkdir(EntName.GetText(),755)==0)
        #endif
        { 
            
            ////Create NewProject();
            
            #ifdef __WINDOWS__
            sprintf(TempPath, "%s%s%s",EntPathNew.GetText(),"\\", EntName.GetText());
            #else
            sprintf(TempPath, "%s%s%s",EntPathNew.GetText(),"/",EntName.GetText());
            #endif
            chdir(TempPath);
            system("ls");
            
            short i,j;
            sprintf(CurrentProject.Version, "%s", EntVersion.GetText());
            sprintf(CurrentProject.Name, "%s", EntName.GetText());
            sprintf(CurrentProject.Description, "%s", "Write a Descripton...");
            
            //sprintf(CurrentProject.Description, "%s", EntName.GetText());
            
            //for(i=0; i<255; i++)CurrentProject.Description[i]=' ';
            CurrentProject.InverseLogicInput=0x00;
            CurrentProject.InverseLogicOutput=0x00;
            CurrentProject.UseAnalogInputs=0x00;
            for(i=0; i<8; i++)
            {
                CurrentProject.Alarm[i].Hour=0; 
                CurrentProject.Alarm[i].Minute=0; 
                for(j=0; j<6; j++)CurrentProject.Alarm[i].Data[j]=0;
            }
            CurrentProject.TotalLines=1;
            
            sprintf(TempPath, "%s.lpj",EntName.GetText()); 
            FILE* testRead1 = fopen (TempPath, "wb");
            if(testRead1)
            {
	           fwrite(&CurrentProject, 1, sizeof(CurrentProject), testRead1);
	           fclose(testRead1);
            }
            else
            {
                MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, (char*)"Error: Creating The Project!");
                Info.Run();
                Info.Destroy();
                Dialog.Destroy(); 
                return;
            }
            #ifdef __WINDOWS__
            _mkdir("Data");
            #else
            mkdir("Data",777);
            #endif
            
            
            for(i=0; i<Ladder.TotalLines; i++)
            {
                sprintf(TempPath, "Data/Line_%03d.lad",i); 
                FILE* FileName = fopen (TempPath, "wb");
                if(FileName)
                {
        	        fwrite(Ladder.Line[i].Links, 1, sizeof(_ladder_links)*Ladder.Line[i].TotalLinks, FileName);
        	        fclose(FileName);
                }
                else
                {
                    MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, (char*)"Error: Saving the project!");
                    Info.Run();
                    Info.Destroy();
                    return;
                }
            }      
            
            /////////////////////Setup the Interface///////////
            Addline.SetActive(true);
            DeleteTool.SetActive(true);
            Mnu_Delete.SetActive(true);
            OpenedProject=true;
            Mnu_Close.SetActive(true);
            Mnu_Options.SetActive(true);
    	    Mnu_Build.SetActive(true);
            SetHighTool.SetActive(true);
            SetLowTool.SetActive(true);
            IfTool.SetActive(true);
            IfNotTool.SetActive(true);
            TimerTool.SetActive(true);
        }
        else{
            MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, (char*)"Error: Invalid Project Name or Path!");
            Info.Run();
            Info.Destroy();
            Dialog.Destroy(); 
            return;
        }
        Dialog.Destroy(); 
        MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, (char*)"The Project was created!");
        Info.Run();
        Info.Destroy();
    }
    else if(Response==-6)
    {
        Dialog.Destroy(); 
    }
    else goto Retrun;
}

void SaveProject_Funtion()
{
        int i;
        char TempPath[255];
        
        CurrentProject.TotalLines = Ladder.TotalLines;
        
        sprintf(TempPath, "%s.lpj",CurrentProject.Name); 
        FILE* testRead1 = fopen (TempPath, "wb");
        if(testRead1)
        {
	        fwrite(&CurrentProject, 1, sizeof(CurrentProject), testRead1);
	        fclose(testRead1);
        }
        else
        {
            MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, (char*)"Error: Saving the project!");
            Info.Run();
            Info.Destroy();
            return;
        }
        
        for(i=0; i<Ladder.TotalLines; i++)
        {
            sprintf(TempPath, "Data/Line_%03d.lad",i); 
            FILE* FileName = fopen (TempPath, "wb");
            if(FileName)
            {
    	        fwrite(Ladder.Line[i].Links, 1, sizeof(_ladder_links)*Ladder.Line[i].TotalLinks, FileName);
    	        fclose(FileName);
            }
            else
            {
                MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, (char*)"Error: Saving the project!");
                Info.Run();
                Info.Destroy();
                return;
            }
        }      
    return;    
}

void CloseProject_Funtion()
{
    if(OpenedProject){
        MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, (char*)"Would you like to save your project?");
        if(Info.Run()==-8)
        {
            SaveProject_Funtion();    
        }
        Info.Destroy();
    }
    
    SelectedLine=0;
    SelectedLink=0;
    Ladder.DeleteAllLines();
    OpenedProject=false;
    
    Addline.SetActive(false);
    DeleteTool.SetActive(false);
    EditTool.SetActive(false);
    Mnu_Delete.SetActive(false);
    Mnu_EditLink.SetActive(false);
    Mnu_Copy.SetActive(false);
    Mnu_Cut.SetActive(false);
    Mnu_Paste.SetActive(false);
    Mnu_Close.SetActive(false);
    Mnu_Options.SetActive(false);
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(false);
    SetHighTool.SetActive(false);
    SetLowTool.SetActive(false);
    IfTool.SetActive(false);
    IfNotTool.SetActive(false);
    TimerTool.SetActive(false);
}

void OpenProject_Funtion()
{
    if(OpenedProject){
        MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_YES_NO, (char*)"Would you like to save your project?");
        if(Info.Run()==-8)
        {
            SaveProject_Funtion();    
        }
        Info.Destroy();
    }
    
    MKFileChooser Chooser(GTK_WINDOW(Window.Widget), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, NULL,(char*)"Select Folder...");
    char TempPath[255];
    char TempName[25];
    int Len;
    
    if(Chooser.Run()== GTK_RESPONSE_ACCEPT)
    {
        
        sprintf(TempPath,"%s",Chooser.GetFileName());
        Len = strlen(TempPath)-1;
        int i;
        for(i=Len ; i>0 ; i--)
        {
            #ifdef __WINDOWS__
            if(TempPath[i]=='\\') 
                break;
            #else
                if(TempPath[i]=='/') 
                break;
            #endif
            
        }
        int Dif = Len -i;
        
        for(i=0; i<Dif ; i++)
        {
            TempName[i]=TempPath[i+(Len-Dif)+1];
            
        }    
        #ifdef __WINDOWS__
        TempName[Dif+1]='\0';
        #else
        TempName[Dif]='\0';
        #endif
        chdir(TempPath);  
        //cout << TempPath<< endl;
        //cout << TempName<< endl<< Len;
        
        sprintf(TempPath,"%s.lpj",TempName);
        FILE* Readfile = fopen (TempPath, "rb");
        if(Readfile)
        {
	        
	        cout << "Opended File" << endl;
            
            fread(&CurrentProject, 1, sizeof(CurrentProject), Readfile);
	        fclose(Readfile);
	        Ladder.TotalLines=CurrentProject.TotalLines;
	        
	        cout << CurrentProject.TotalLines << endl;
            
            cout << "Creating Lines" << endl;
            
            for(i=0; i<Ladder.TotalLines; i++)
            {
                sprintf(TempPath, "Data/Line_%03d.lad",i); 
                FILE* FileName = fopen (TempPath, "rb");
                int size;
                if(FileName)
                {
        	        fseek(FileName, 0, SEEK_END); // seek to end of file
                    size = ftell(FileName); // get current file pointer
                    cout << "File Size: " << size << endl;
                    rewind(FileName);
        	        Ladder.Line[i].CreateLineFromSize(size);
                    fread(Ladder.Line[i].Links, 1, size, FileName);
        	        fclose(FileName);
        	        
        	        Ladder.Line[i].TotalLinks=size/sizeof(_ladder_links);
        	        
        	        cout << Ladder.Line[i].Links[0].Type;
                }
                else
                {
                    sprintf(TempPath,"Couldn't find the file: Line_%03d.lad",i);
                    MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, TempPath);
                    Info.Run();
                    Info.Destroy();
                    return;
                }
                   
            }      
	        
	        Addline.SetActive(true);
            DeleteTool.SetActive(true);
            Mnu_Delete.SetActive(true);
            OpenedProject=true;
            Mnu_Close.SetActive(true);
            Mnu_Options.SetActive(true);
            Mnu_RunSimulator.SetActive(false);
    	    Mnu_RunTerminal.SetActive(false);
    	    Mnu_Build.SetActive(true);
            SetHighTool.SetActive(true);
            SetLowTool.SetActive(true);
            IfTool.SetActive(true);
            IfNotTool.SetActive(true);
            TimerTool.SetActive(true);
	        
            Chooser.Destroy(); 
            return;
        }
        else
        {
            sprintf(TempPath,"Couldn't find the file: %s.lpj",TempName);
            MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, TempPath);
            Info.Run();
            Info.Destroy();
            Chooser.Destroy(); 
            return;
        }
        
    }
    Chooser.Destroy();  
    
}

void EditProject_Funtion()
{
    MKCustomDialog Dialog (Window.Widget,(char*)"Edit Project Options...");
    MKFixed fix(true);
        
    MKLabel LblVersion((char*)"Version");
    MKLabel LblDescription((char*)"Description");
    
    MKCheckButton ChcInput((char*)"Inverse Logic Input");
    if(CurrentProject.InverseLogicInput==0x00)ChcInput.SetState(false);
    else ChcInput.SetState(true);
    
    MKCheckButton ChcOutput((char*)"Inverse Logic Output");
    if(CurrentProject.InverseLogicOutput==0x00)ChcOutput.SetState(false);
    else ChcOutput.SetState(true);
    
    MKCheckButton ChcAnalog((char*)"Use Analog Inputs");
    if(CurrentProject.UseAnalogInputs==0x00)ChcAnalog.SetState(false);
    else ChcAnalog.SetState(true);
    
    MKTextView Description(50);
    Description.SetText(CurrentProject.Description,strlen(CurrentProject.Description));
    
    MKEntry EntVersion(7);
    EntVersion.SetText(CurrentProject.Version);
    
    LblVersion.SetJustify(GTK_JUSTIFY_LEFT);
    fix.AddElement(LblVersion.Widget, 8, 5, 80, 25);
    LblDescription.SetJustify(GTK_JUSTIFY_LEFT);
    fix.AddElement(LblDescription.Widget, 0, 40, 80, 25);
    
    fix.AddElement(EntVersion.Widget, 95, 5, 100, 25); 
    fix.AddElement(Description.Widget, 95, 40, 300, 150);
    fix.AddElement(ChcInput.Widget, 95, 200, 150, 25);
    fix.AddElement(ChcOutput.Widget, 95, 220, 150, 25);
    fix.AddElement(ChcAnalog.Widget, 95, 240, 150, 25);
    
             
    Dialog.AddElement(fix.Widget);
    if(Dialog.Run()==-5)
    {
        if(ChcInput.GetState())CurrentProject.InverseLogicInput=0xFF;
        else CurrentProject.InverseLogicInput=0x00;
        
        if(ChcOutput.GetState())CurrentProject.InverseLogicOutput=0xFF;
        else CurrentProject.InverseLogicOutput=0x00;
        
        if(ChcAnalog.GetState())CurrentProject.UseAnalogInputs=0xFF;
        else CurrentProject.UseAnalogInputs=0x00;
        
        sprintf(CurrentProject.Description,"%s",Description.GetText());
        CurrentProject.Description[254]= '\0';
        sprintf(CurrentProject.Version,"%s",EntVersion.GetText());
    }
    Dialog.Destroy();
}

MKTextView Description(50);

void Clear_Log(GtkWidget *wid, GtkWidget *win)
{
    text.clear(); 
    Description.SetText((char*)text.c_str(),strlen(text.c_str())); 
}

void Save_Log(GtkWidget *wid, GtkWidget *win)
{
    
    time_t rawtime;
    time ( &rawtime );
    struct tm * ptm;
    ptm = gmtime ( &rawtime );
    
    char TEMP[40];
    sprintf(TEMP,"Log %02d_%02d_%02d %02d_%02d.txt",  ptm->tm_mday, ptm->tm_mon, ptm->tm_year, (ptm->tm_hour-7)%24, ptm->tm_min);  
    
    ofstream filelog;
    filelog.open (TEMP);
    filelog << text;
    filelog.close();

}

void Build_Porject()
{
	int i, j;
	short errors=0;
	char Temp[255];
	text.append("Compling Program... \n");

	for(i=0; i<Ladder.TotalLines; i++)
	for(j=0; j<Ladder.Line[i].TotalLinks; j++)
        {
		if(!Ladder.Line[i].Links[j].Valid)
		{
			errors++;
			sprintf(Temp, "Error: Line %03d, Link %02d \n", i, j+1);
			text.append(Temp);
		}            
        }  
	sprintf(Temp, "    %03d Error Found \n",errors);
////////////Get Time
	text.append(Temp);
	if(errors == 0)
	{ 
		Compiled = true;
		Mnu_RunSimulator.SetActive(true);
    		Mnu_RunTerminal.SetActive(true);
    		Mnu_Build.SetActive(true);
	}

	MKCustomDialog Dialog (Window.Widget,(char*)"Compiler Log");
	MKFixed fix(true);
	Description.ReOpen(50);
	MKButton BTN_Clear("Clear Log");
	MKButton BTN_Save("Save Log");
    Description.SetText((char*)text.c_str(),strlen(text.c_str()));
    fix.AddElement(Description.Widget, 0, 0, 400, 300);
	fix.AddElement(BTN_Save.Widget, 70, 320, 100, 30);
    fix.AddElement(BTN_Clear.Widget, 220, 320, 100, 30);
    
    g_signal_connect(BTN_Clear.Widget, "clicked",G_CALLBACK(Clear_Log),NULL);
    g_signal_connect(BTN_Save.Widget, "clicked",G_CALLBACK(Save_Log), NULL);
    
    Dialog.AddElement(fix.Widget);
	Dialog.Run();
	Dialog.Destroy();
}

void ShowLog_Funtion()
{
    MKCustomDialog Dialog (Window.Widget,(char*)"Compiler Log");
	MKFixed fix(true);
	Description.ReOpen(50);
	MKButton BTN_Clear("Clear Log");
	MKButton BTN_Save("Save Log");
    Description.SetText((char*)text.c_str(),strlen(text.c_str()));
    fix.AddElement(Description.Widget, 0, 0, 400, 300);
	fix.AddElement(BTN_Save.Widget, 70, 320, 100, 30);
    fix.AddElement(BTN_Clear.Widget, 220, 320, 100, 30);
    
    g_signal_connect(BTN_Clear.Widget, "clicked",G_CALLBACK(Clear_Log),NULL);
    g_signal_connect(BTN_Save.Widget, "clicked",G_CALLBACK(Save_Log), NULL);
    
    Dialog.AddElement(fix.Widget);
	Dialog.Run();
	Dialog.Destroy();
}

MKImage Output0((char*)"Icons/LEDON.png");
MKImage Output1((char*)"Icons/LEDON.png");
MKImage Output2((char*)"Icons/LEDON.png");
MKImage Output3((char*)"Icons/LEDON.png");
MKImage Output4((char*)"Icons/LEDON.png");
MKImage Output5((char*)"Icons/LEDON.png");
MKImage Output6((char*)"Icons/LEDON.png");
MKImage Output7((char*)"Icons/LEDON.png");

MKImage Input0((char*)"Icons/LEDON2.png");
MKImage Input1((char*)"Icons/LEDON2.png");
MKImage Input2((char*)"Icons/LEDON2.png");
MKImage Input3((char*)"Icons/LEDON2.png");
MKImage Input4((char*)"Icons/LEDON2.png");
MKImage Input5((char*)"Icons/LEDON2.png");
MKImage Input6((char*)"Icons/LEDON2.png");
MKImage Input7((char*)"Icons/LEDON2.png");

MKToolBarElement Play(GTK_STOCK_MEDIA_PLAY);
MKToolBarElement Pause(GTK_STOCK_MEDIA_PAUSE);
MKToolBarElement Stop(GTK_STOCK_MEDIA_STOP);


void SimulatorProject_Funtion()
{
    RuningType=0x01;
    MKCustomDialog Dialog (Window.Widget,(char*)"Virtual PLC Simulator");
    MKFixed fixed(false);
    MKVBox vbox(false);
    MKHBox hbox(false);
    MKToolBar TBar(true);
    
    Play.ReOpen(GTK_STOCK_MEDIA_PLAY);
    Pause.ReOpen(GTK_STOCK_MEDIA_PAUSE);
    Stop.ReOpen(GTK_STOCK_MEDIA_STOP);
    
    Pause.SetActive(false);
    Stop.SetActive(false);
    
    TBar.AddElement(Play.Element);
    TBar.AddElement(Pause.Element);
    TBar.AddElement(Stop.Element);
    vbox.AddElement(TBar.Widget);
    
    Dialog.AddElement(vbox.Widget);
    vbox.AddElement(fixed.Widget);
    fixed.AddElement(hbox.Widget,5,120,410,50);
    
    MKLabel LblInputs((char*)"Inputs: ");
    MKLabel LblOutputs((char*)"Outputs: ");
    
    fixed.AddElement(LblInputs.Widget,0,0,50,50);
    fixed.AddElement(LblOutputs.Widget,0,85,50,50);
    
    MKToggleButton Bit0((char*)"Bit 0");
    fixed.AddElement(Bit0.Widget,5,30,50,50);
    
    MKToggleButton Bit1((char*)"Bit 1");
    fixed.AddElement(Bit1.Widget,55,30,50,50);
    
    MKToggleButton Bit2((char*)"Bit 2");
    fixed.AddElement(Bit2.Widget,105,30,50,50);
    
    MKToggleButton Bit3((char*)"Bit 3");
    fixed.AddElement(Bit3.Widget,155,30,50,50);
    
    MKToggleButton Bit4((char*)"Bit 4");
    fixed.AddElement(Bit4.Widget,205,30,50,50);
    
    MKToggleButton Bit5((char*)"Bit 5");
    fixed.AddElement(Bit5.Widget,255,30,50,50);
    
    MKToggleButton Bit6((char*)"Bit 6");
    fixed.AddElement(Bit6.Widget,305,30,50,50);
    
    MKToggleButton Bit7((char*)"Bit 7");
    fixed.AddElement(Bit7.Widget,355,30,50,50);
    
    Output0.ReOpen();
    Output1.ReOpen();
    Output2.ReOpen();
    Output3.ReOpen();
    Output4.ReOpen();
    Output5.ReOpen();
    Output6.ReOpen();
    Output7.ReOpen();

    hbox.AddElement(Output0.Widget);
    hbox.AddElement(Output1.Widget);
    hbox.AddElement(Output2.Widget);
    hbox.AddElement(Output3.Widget);
    hbox.AddElement(Output4.Widget);
    hbox.AddElement(Output5.Widget);
    hbox.AddElement(Output6.Widget);
    hbox.AddElement(Output7.Widget);
    
    Output0.SetActive(false);
    Output1.SetActive(false);
    Output2.SetActive(false);
    Output3.SetActive(false);
    Output4.SetActive(false);
    Output5.SetActive(false);
    Output6.SetActive(false);
    Output7.SetActive(false);
    
    g_signal_connect(Play.Element, "clicked",G_CALLBACK(Play_Funtion), NULL);
    g_signal_connect(Stop.Element, "clicked",G_CALLBACK(Stop_Funtion), NULL);
                                               
    g_signal_connect(Bit0.Widget,"clicked",G_CALLBACK(SetOutput0),NULL);
    g_signal_connect(Bit1.Widget,"clicked",G_CALLBACK(SetOutput1),NULL);
    g_signal_connect(Bit2.Widget,"clicked",G_CALLBACK(SetOutput2),NULL);
    g_signal_connect(Bit3.Widget,"clicked",G_CALLBACK(SetOutput3),NULL);
    g_signal_connect(Bit4.Widget,"clicked",G_CALLBACK(SetOutput4),NULL);
    g_signal_connect(Bit5.Widget,"clicked",G_CALLBACK(SetOutput5),NULL);
    g_signal_connect(Bit6.Widget,"clicked",G_CALLBACK(SetOutput6),NULL);
    g_signal_connect(Bit7.Widget,"clicked",G_CALLBACK(SetOutput7),NULL);
    
    g_timeout_add(DELAY_OF_STEP, (GSourceFunc) Simulator_handler, (gpointer) Dialog.dialog);
    
    Dialog.Run();
    Dialog.Destroy();
    RuningSimulator=false;
    int i;
    OutputBuffer=0x00;
    InputBuffer=0x00;
    for(i=0; i<8; i++)
    {
        pOutputBuffer[i]=0x00;
        pInputBuffer[i]=0x00;
    }
}

void TerminalProject_Funtion()
{
    RuningType=0x02;
    MKCustomDialog Dialog (Window.Widget,(char*)"Virtual PLC Terminal");
    MKFixed fixed(false);
    MKVBox vbox(false);
    MKHBox hbox(false);
    MKHBox hbox2(false);
    MKToolBar TBar(true);
    
    Play.ReOpen(GTK_STOCK_MEDIA_PLAY);
    Pause.ReOpen(GTK_STOCK_MEDIA_PAUSE);
    Stop.ReOpen(GTK_STOCK_MEDIA_STOP);
    
    Pause.SetActive(false);
    Stop.SetActive(false);
    
    TBar.AddElement(Play.Element);
    TBar.AddElement(Pause.Element);
    TBar.AddElement(Stop.Element);
    vbox.AddElement(TBar.Widget);
    
    Dialog.AddElement(vbox.Widget);
    vbox.AddElement(fixed.Widget);
    fixed.AddElement(hbox.Widget,5,120,410,50);
    fixed.AddElement(hbox2.Widget,5,30,410,50);
    
    MKLabel LblInputs((char*)"Inputs: ");
    MKLabel LblOutputs((char*)"Outputs: ");
    
    fixed.AddElement(LblInputs.Widget,0,0,50,50);
    fixed.AddElement(LblOutputs.Widget,0,85,50,50);

    Output0.ReOpen();
    Output1.ReOpen();
    Output2.ReOpen();
    Output3.ReOpen();
    Output4.ReOpen();
    Output5.ReOpen();
    Output6.ReOpen();
    Output7.ReOpen();

    hbox.AddElement(Output0.Widget);
    hbox.AddElement(Output1.Widget);
    hbox.AddElement(Output2.Widget);
    hbox.AddElement(Output3.Widget);
    hbox.AddElement(Output4.Widget);
    hbox.AddElement(Output5.Widget);
    hbox.AddElement(Output6.Widget);
    hbox.AddElement(Output7.Widget);
    
    Output0.SetActive(false);
    Output1.SetActive(false);
    Output2.SetActive(false);
    Output3.SetActive(false);
    Output4.SetActive(false);
    Output5.SetActive(false);
    Output6.SetActive(false);
    Output7.SetActive(false);
    
    Input0.ReOpen();
    Input1.ReOpen();
    Input2.ReOpen();
    Input3.ReOpen();
    Input4.ReOpen();
    Input5.ReOpen();
    Input6.ReOpen();
    Input7.ReOpen();

    hbox2.AddElement(Input0.Widget);
    hbox2.AddElement(Input1.Widget);
    hbox2.AddElement(Input2.Widget);
    hbox2.AddElement(Input3.Widget);
    hbox2.AddElement(Input4.Widget);
    hbox2.AddElement(Input5.Widget);
    hbox2.AddElement(Input6.Widget);
    hbox2.AddElement(Input7.Widget);
    
    Input0.SetActive(false);
    Input1.SetActive(false);
    Input2.SetActive(false);
    Input3.SetActive(false);
    Input4.SetActive(false);
    Input5.SetActive(false);
    Input6.SetActive(false);
    Input7.SetActive(false);
    
    g_signal_connect(Play.Element, "clicked",G_CALLBACK(Play_Funtion), NULL);
    g_signal_connect(Stop.Element, "clicked",G_CALLBACK(Stop_Funtion), NULL);
                                               

    g_timeout_add(DELAY_OF_STEP, (GSourceFunc) Terminal_handler, (gpointer) Dialog.dialog);
    
    Dialog.Run();
    Dialog.Destroy();
    RuningSimulator=false;
    int i;
    OutputBuffer=0x00;
    InputBuffer=0x00;
    for(i=0; i<8; i++)
    {
        pOutputBuffer[i]=0x00;
        pInputBuffer[i]=0x00;
    }
    if(USB.HIDisOpen)
    {
        USB.SendOutputPort(0x00);
        USB.HIDClose();
    }
}



void Play_Funtion()
{
    int i,j;
    for(i=0; i< Ladder.TotalLines; i++)
    {
        for(j=0; j<Ladder.Line[i].TotalLinks; j++)
        {
            if(Ladder.Line[i].Links[j].Type==TIMER_LINK)
            Ladder.Line[i].Links[j].currentTime=0;
        }
    }
    
    if(RuningType==0x02)
    {
        USB.EnumerateHIDs();
        USB.HIDOpen(0x3995,0x0001);
        USB.SendOutputPort(0xFF);
        USB.SendOutputPort(0x00);
        if(!USB.HIDisOpen)
        {
            MKDialog Info(GTK_WINDOW (Window.Widget), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Couldn't open HID Terminal!!!");
            Info.Run();
            Info.Destroy();   
            return; 
        }
    }
    
    RuningSimulator=true;
    Play.SetActive(false);
    Stop.SetActive(true);
}
void Stop_Funtion()
{
    int i,j;
	USB.SendOutputPort(0x00);
    for(i=0; i< Ladder.TotalLines; i++)
    {
        for(j=0; j<Ladder.Line[i].TotalLinks; j++)
        {
            if(Ladder.Line[i].Links[j].Type==TIMER_LINK)
            Ladder.Line[i].Links[j].currentTime=0;
        }
    }//*/
	for(i=0; i< 8; i++)
	{
		RegTimers[i]=0;
		Timers[i]=0;
	}
    if(RuningType==0x02)
    {
        USB.SendOutputPort(0x00);
        USB.HIDClose();
    }
    RuningSimulator=false;
    Play.SetActive(true);
    Stop.SetActive(false);
 
}
gboolean Simulator_handler(GtkWidget *widget)
{
    if(RuningSimulator)
    {
        RunProject();
        //RuningSimulator=false;
    }
    return TRUE;
}

gboolean Terminal_handler(GtkWidget *widget)
{
    if(RuningSimulator)
    {
        RunProject();
        //RuningSimulator=false;
    }
    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////
void SetOutput0(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pInputBuffer[0]=0x01;
    else pInputBuffer[0]=0x00;
}
void SetOutput1(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pInputBuffer[1]=0x01;
    else pInputBuffer[1]=0x00;
}
void SetOutput2(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pInputBuffer[2]=0x01;
    else pInputBuffer[2]=0x00;
}
void SetOutput3(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pInputBuffer[3]=0x01;
    else pInputBuffer[3]=0x00;
}
void SetOutput4(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pInputBuffer[4]=0x01;
    else pInputBuffer[4]=0x00;
}
void SetOutput5(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pInputBuffer[5]=0x01;
    else pInputBuffer[5]=0x00;
}
void SetOutput6(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pInputBuffer[6]=0x01;
    else pInputBuffer[6]=0x00;
}
void SetOutput7(GtkWidget *wid, GtkWidget *win)
{
    if (GTK_TOGGLE_BUTTON (wid)->active)pInputBuffer[7]=0x01;
    else pInputBuffer[7]=0x00;
}

void RunProject()
{
    int i,j;
    bool ExitLine;
    OutputBuffer=0x00;
    for(i=0; i<8; i++)pOutputBuffer[i]=0x00;
    
    if(RuningType==0x02)
    {
        
        InputBuffer=0x00;
        for(i=0; i<8; i++)pInputBuffer[i]=0x00;

        InputBuffer = USB.GetInputBuffer();
        
        if(InputBuffer & 0x01)pInputBuffer[0]=0x01;
        else pInputBuffer[0]=0x00;
        
        if(InputBuffer & 0x02)pInputBuffer[1]=0x01;
        else pInputBuffer[1]=0x00;
        
        if(InputBuffer & 0x04)pInputBuffer[2]=0x01;
        else pInputBuffer[2]=0x00;
        
        if(InputBuffer & 0x08)pInputBuffer[3]=0x01;
        else pInputBuffer[3]=0x00;
        
        if(InputBuffer & 0x10)pInputBuffer[4]=0x01;
        else pInputBuffer[4]=0x00;
        
        if(InputBuffer & 0x20)pInputBuffer[5]=0x01;
        else pInputBuffer[5]=0x00;
        
        if(InputBuffer & 0x40)pInputBuffer[6]=0x01;
        else pInputBuffer[6]=0x00;
        
        if(InputBuffer & 0x80)pInputBuffer[7]=0x01;
        else pInputBuffer[7]=0x00;
    }
    
    //cout << "Running: "<< endl;
    for(i=0; i< Ladder.TotalLines; i++)
    {
        ExitLine=false;
        for(j=0; j<Ladder.Line[i].TotalLinks; j++)
        {
            //cout << "  Line: "<< i << " Link: "<< j << " Type: " << Ladder.Line[i].Links[j].Type << endl;
            switch(Ladder.Line[i].Links[j].Type)
            {   
                case SET_HIGH_LINK:
                    if(Ladder.Line[i].Links[j].Argument[0]==1)
                    {
                        pOutputBuffer[Ladder.Line[i].Links[j].Argument[1]]=0x01;
                    }
                    break;
                case SET_LOW_LINK:
                    if(Ladder.Line[i].Links[j].Argument[0]==1)
                    {
                        pOutputBuffer[Ladder.Line[i].Links[j].Argument[1]]=0x00;
                    }
                    break;
                case IF_LINK:
                    switch(Ladder.Line[i].Links[j].Argument[0])
                    {
                        case 1:
                        if(pOutputBuffer[Ladder.Line[i].Links[j].Argument[1]]==0x00)
                        {
                            ExitLine=true;
                        }
                        break;
                        case 2:
                        if(pInputBuffer[Ladder.Line[i].Links[j].Argument[1]]==0x00)
                        {
                            ExitLine=true;
                        } 
                        break;
                        case 3:
                        //cout << "If Timer: "<< Ladder.Line[2].Links[0].Time;
                        if(!(Timers[Ladder.Line[i].Links[j].Argument[1]]>=RegTimers[Ladder.Line[i].Links[j].Argument[1]]))
                        {
                            //cout << "Salio";
                            ExitLine=true;
                        } 
                        break;
                    }
                    break;
                case IF_NOT_LINK:
                    switch(Ladder.Line[i].Links[j].Argument[0])
                    {
                        case 1:
                        if(pOutputBuffer[Ladder.Line[i].Links[j].Argument[1]]==0x01)
                        {
                            ExitLine=true;
                        }
                        break;
                        case 2:
                        if(pInputBuffer[Ladder.Line[i].Links[j].Argument[1]]==0x01)
                        {
                            ExitLine=true;
                        } 
                        break;
                        case 3:
                            //cout << "If Timer: "<< Ladder.Line[2].Links[0].Time;
                        if((Timers[Ladder.Line[i].Links[j].Argument[1]]>=Ladder.Line[2].Links[0].Time))
                        {
                            ExitLine=true;
                        } 
                        break;
                    }
                    break;
                case TIMER_LINK:
                    if(Ladder.Line[i].Links[j].Argument[0]==3)
                    {
                        if(Ladder.Line[i].Links[j].currentTime<Ladder.Line[i].Links[j].Time)
                        {
                            //cout << "Counter ++ " ;
                            Ladder.Line[i].Links[j].currentTime+= DELAY_OF_STEP;
                            Timers[Ladder.Line[i].Links[j].Argument[1]]=Ladder.Line[i].Links[j].currentTime;
                            RegTimers[Ladder.Line[i].Links[j].Argument[1]]=Ladder.Line[i].Links[j].Time;
                        }
                        //cout << "   -Time : " << dec <<Timers[Ladder.Line[i].Links[j].Argument[1]] << " _ T: " << Ladder.Line[i].Links[j].Time << " _ L ->" << Ladder.Line[2].Links[0].currentTime << endl;
                    }
                    break;

            }
            if(ExitLine)break;
        }
    }
    
    OutputBuffer=(pOutputBuffer[7]<<7)|(pOutputBuffer[6]<<6)|(pOutputBuffer[5]<<5)|(pOutputBuffer[4]<<4)|(pOutputBuffer[3]<<3)|(pOutputBuffer[2]<<2)|(pOutputBuffer[1]<<1)|(pOutputBuffer[0]);  
    //cout << "Output: " << hex << (int)OutputBuffer << endl;
    
    Output0.SetActive(pOutputBuffer[0]==0x01);
    Output1.SetActive(pOutputBuffer[1]==0x01);
    Output2.SetActive(pOutputBuffer[2]==0x01);
    Output3.SetActive(pOutputBuffer[3]==0x01);
    Output4.SetActive(pOutputBuffer[4]==0x01);
    Output5.SetActive(pOutputBuffer[5]==0x01);
    Output6.SetActive(pOutputBuffer[6]==0x01);
    Output7.SetActive(pOutputBuffer[7]==0x01);
    
    if(RuningType==0x02)
    {   
        USB.SendOutputPort(OutputBuffer);
        Input0.SetActive(pInputBuffer[0]==0x01);
        Input1.SetActive(pInputBuffer[1]==0x01);
        Input2.SetActive(pInputBuffer[2]==0x01);
        Input3.SetActive(pInputBuffer[3]==0x01);
        Input4.SetActive(pInputBuffer[4]==0x01);
        Input5.SetActive(pInputBuffer[5]==0x01);
        Input6.SetActive(pInputBuffer[6]==0x01);
        Input7.SetActive(pInputBuffer[7]==0x01);
    }
}



/////////////////////////////////////////////////Operations with Link//////////////////////////////////
void Delete_Funtion()
{
    if (SelectedLine==0 && SelectedLink==0)
    {
        Ladder.AddLine(0);

        Ladder.DeleteLine(SelectedLine+1);
        SelectedLine=0;
    }
    else if(SelectedLine>0 && SelectedLink==0 && Ladder.TotalLines>1)
    {
        Ladder.DeleteLine(SelectedLine);
        SelectedLine--;
    }
    else if(SelectedLine>=0 && SelectedLink>0)
    {
        Ladder.Line[SelectedLine].DeleteLink(SelectedLink);
        SelectedLine=-1;
        SelectedLink=0;
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
}

void Copy_Funtion()
{
    if(SelectedLine>=0 && SelectedLink>=0)
    {
        ClipBoardLink = Ladder.Line[SelectedLine].GetLink(SelectedLink-1);
    }
}

void Cut_Funtion()
{
    if(SelectedLine>=0 && SelectedLink>=0)
    {
        ClipBoardLink = Ladder.Line[SelectedLine].GetLink(SelectedLink-1);
        Ladder.Line[SelectedLine].DeleteLink(SelectedLink);
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
}
void Paste_Funtion()
{
    if(SelectedLink>=0)
    {
        Ladder.Line[SelectedLine].AddLinkAtPositionFormOther(SelectedLink,ClipBoardLink);
    }
    else if(SelectedLine>=0 && SelectedLink>=0)
    {
        Ladder.Line[SelectedLine].AddLinkAtPositionFormOther(1,ClipBoardLink);
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
}

void AddLine()
{
    if(SelectedLine>=0)Ladder.AddLine(SelectedLine+1);
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
}

gboolean EditLink_Funtion(GtkWidget *wid, GtkWidget *win)
{

    bool Createobj=false;
    MKCustomDialog Dialog (win," Edit Link Instruction");
    if(SelectedLine>=0)
    {
        MKFixed fix(true);
    
        MKLabel LblInstrucion("Instruction");
        MKLabel LblLabel("Label");

        MKEntry EntInstruction(4);
        MKEntry EntLabel(14);
        MKEntry EntTime(15);
        
        LblInstrucion.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblInstrucion.Widget, 5, 5, 80, 25);
        LblLabel.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblLabel.Widget, 0, 40, 80, 25);
        
        fix.AddElement(EntInstruction.Widget, 95, 5, 150, 25);
        fix.AddElement(EntLabel.Widget, 95, 40, 150, 25);
        
        EntInstruction.SetText(Ladder.Line[SelectedLine].Links[SelectedLink-1].Instruction);
        EntLabel.SetText(Ladder.Line[SelectedLine].Links[SelectedLink-1].Label);
        
        if(Ladder.Line[SelectedLine].Links[SelectedLink-1].Type==TIMER_LINK)
        {

            MKLabel LblTime("Time (mS)");

            LblTime.SetJustify(GTK_JUSTIFY_LEFT);
            fix.AddElement(LblTime.Widget, 0, 75, 80, 25);
            fix.AddElement(EntTime.Widget, 95, 75, 150, 25); 
            char TempTime[15];
            sprintf(TempTime, "%d", Ladder.Line[SelectedLine].Links[SelectedLink-1].Time);
            EntTime.SetText(TempTime);            
        }
                      
        Dialog.AddElement(fix.Widget);
        if(Dialog.Run()==-5)
        {
            Createobj=true;
            sprintf(Ladder.Line[SelectedLine].Links[SelectedLink-1].Label,"%s",EntLabel.GetText());
            sprintf(Ladder.Line[SelectedLine].Links[SelectedLink-1].Instruction,"%s",EntInstruction.GetText());
            if(Ladder.Line[SelectedLine].Links[SelectedLink-1].Type==TIMER_LINK)
            {
                int TempTimeI;
                sscanf(EntTime.GetText(),"%d",&TempTimeI);
                Ladder.Line[SelectedLine].Links[SelectedLink-1].Time=TempTimeI;
            }
            Ladder.Line[SelectedLine].ConvertArguments(SelectedLink-1);
        }
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
    Dialog.Destroy(); 
}

gboolean AddLinkSetHigh(GtkWidget *wid, GtkWidget *win)
{
    char TempInstruction[10];
    char TempLabel[15];
    bool Createobj=false;
    MKCustomDialog Dialog (win,(char*)"Link Instruction");
    
    if(SelectedLine>=0)
    {
        MKFixed fix(true);
    
        MKLabel LblInstrucion((char*)"Instruction");
        MKLabel LblLabel("Label");

        MKEntry EntInstruction(4);
        MKEntry EntLabel(14);

        LblInstrucion.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblInstrucion.Widget, 5, 5, 80, 25);
        LblLabel.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblLabel.Widget, 0, 40, 80, 25);
        
        fix.AddElement(EntInstruction.Widget, 95, 5, 150, 25);
        fix.AddElement(EntLabel.Widget, 95, 40, 150, 25);
                      
        Dialog.AddElement(fix.Widget);
        if(Dialog.Run()==-6)Createobj=false;
        else
        {
            Createobj=true;

            sprintf(TempLabel,"%s",EntLabel.GetText());
            sprintf(TempInstruction,"%s",EntInstruction.GetText());
        }
    }
    
    if(SelectedLine>=0 && SelectedLink==0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkStart(SET_HIGH_LINK,TempLabel,TempInstruction,0);
        Ladder.Line[SelectedLine].ConvertArguments(0);
    }
    else if(SelectedLine>=0 && SelectedLink>=0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkAtPosition(SelectedLink,SET_HIGH_LINK,TempLabel,TempInstruction,0);
        Ladder.Line[SelectedLine].ConvertArguments(SelectedLink);
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
    Dialog.Destroy();
     
}

gboolean AddLinkSetLow(GtkWidget *wid, GtkWidget *win)
{
    char TempInstruction[10];
    char TempLabel[15];
    bool Createobj=false;
    MKCustomDialog Dialog (win,(char*)"Link Instruction");
    if(SelectedLine>=0)
    {
        MKFixed fix(true);
    
        MKLabel LblInstrucion((char*)"Instruction");
        MKLabel LblLabel((char*)"Label");
            
        MKEntry EntInstruction(4);
        MKEntry EntLabel(14);
                    
        LblInstrucion.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblInstrucion.Widget, 5, 5, 80, 25);
        LblLabel.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblLabel.Widget, 0, 40, 80, 25);
        
        fix.AddElement(EntInstruction.Widget, 95, 5, 150, 25);
        fix.AddElement(EntLabel.Widget, 95, 40, 150, 25);
                        
        Dialog.AddElement(fix.Widget);
        
        if(Dialog.Run()==-6)Createobj=false;
        else
        {
            Createobj=true;

            sprintf(TempLabel,"%s",EntLabel.GetText());
            sprintf(TempInstruction,"%s",EntInstruction.GetText());
        }
    }
    
    if(SelectedLine>=0 && SelectedLink==0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkStart(SET_LOW_LINK,TempLabel,TempInstruction,0);
        Ladder.Line[SelectedLine].ConvertArguments(0);
    }
    else if(SelectedLine>=0 && SelectedLink>=0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkAtPosition(SelectedLink,SET_LOW_LINK,TempLabel,TempInstruction,0);
        Ladder.Line[SelectedLine].ConvertArguments(SelectedLink);
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
    Dialog.Destroy();
}
gboolean AddLinkIf(GtkWidget *wid, GtkWidget *win)
{
    char TempInstruction[10];
    char TempLabel[15];
    bool Createobj=false;
    MKCustomDialog Dialog (win,(char*)"Link Instruction");
    if(SelectedLine>=0)
    {
        MKFixed fix(true);
    
        MKLabel LblInstrucion((char*)"Instruction");
        MKLabel LblLabel((char*)"Label");
        
        MKEntry EntInstruction(9);
        MKEntry EntLabel(14);
                    
        LblInstrucion.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblInstrucion.Widget, 5, 5, 80, 25);
        LblLabel.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblLabel.Widget, 0, 40, 80, 25);
        
        fix.AddElement(EntInstruction.Widget, 95, 5, 150, 25);
        fix.AddElement(EntLabel.Widget, 95, 40, 150, 25); 
                        
        Dialog.AddElement(fix.Widget);
        if(Dialog.Run()==-6)Createobj=false;
        else
        {
            Createobj=true;

            sprintf(TempLabel,"%s",EntLabel.GetText());
            sprintf(TempInstruction,"%s",EntInstruction.GetText());
        }
    }
    
    if(SelectedLine>=0 && SelectedLink==0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkStart(IF_LINK,TempLabel,TempInstruction,0);
        Ladder.Line[SelectedLine].ConvertArguments(0);
    }
    else if(SelectedLine>=0 && SelectedLink>=0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkAtPosition(SelectedLink,IF_LINK,TempLabel,TempInstruction,0);
        Ladder.Line[SelectedLine].ConvertArguments(SelectedLink);
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
    Dialog.Destroy();
}

gboolean AddLinkIfNot(GtkWidget *wid, GtkWidget *win)
{
    char TempInstruction[10];
    char TempLabel[15];
    bool Createobj=false;
    MKCustomDialog Dialog (win,(char*)"Link Instruction");
    if(SelectedLine>=0)
    {
        MKFixed fix(true);
    
        MKLabel LblInstrucion((char*)"Instruction");
        MKLabel LblLabel((char*)"Label");
            
        MKEntry EntInstruction(10);
        MKEntry EntLabel(15);
                    
        LblInstrucion.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblInstrucion.Widget, 5, 5, 80, 25);
        LblLabel.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblLabel.Widget, 0, 40, 80, 25);
        
        fix.AddElement(EntInstruction.Widget, 95, 5, 150, 25);
        fix.AddElement(EntLabel.Widget, 95, 40, 150, 25);
                        
        Dialog.AddElement(fix.Widget);
        if(Dialog.Run()==-6)Createobj=false;
        else
        {
            Createobj=true;

            sprintf(TempLabel,"%s",EntLabel.GetText());
            sprintf(TempInstruction,"%s",EntInstruction.GetText());
        }
    }
    
    if(SelectedLine>=0 && SelectedLink==0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkStart(IF_NOT_LINK,TempLabel,TempInstruction,0);
        Ladder.Line[SelectedLine].ConvertArguments(0);
    }
    else if(SelectedLine>=0 && SelectedLink>=0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkAtPosition(SelectedLink,IF_NOT_LINK,TempLabel,TempInstruction,0);
        Ladder.Line[SelectedLine].ConvertArguments(SelectedLink);
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
    Dialog.Destroy();
}

gboolean AddLinkTimer(GtkWidget *wid, GtkWidget *win)
{
    char TempInstruction[10];
    char TempLabel[15];
    int TempTime;
    bool Createobj=false;
    MKCustomDialog Dialog (win,(char*)"Link Instruction");
    if(SelectedLine>=0)
    {
        MKFixed fix(true);
    
        MKLabel LblInstrucion((char*)"Instruction");
        MKLabel LblLabel((char*)"Label");
        MKLabel LblTime((char*)"Time (mS)");
            
        MKEntry EntInstruction(4);
        MKEntry EntLabel(14);
        MKEntry EntTime(6);
                    
        LblInstrucion.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblInstrucion.Widget, 5, 5, 80, 25);
        LblLabel.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblLabel.Widget, 0, 40, 80, 25);
        LblTime.SetJustify(GTK_JUSTIFY_LEFT);
        fix.AddElement(LblTime.Widget, 0, 75, 80, 25);
        
        fix.AddElement(EntInstruction.Widget, 95, 5, 150, 25);
        fix.AddElement(EntLabel.Widget, 95, 40, 150, 25);
        fix.AddElement(EntTime.Widget, 95, 75, 150, 25);  
        
        EntTime.SetText("0");
                        
        Dialog.AddElement(fix.Widget);
        if(Dialog.Run()==-6)Createobj=false;
        else
        {
            Createobj=true;

            sprintf(TempLabel,"%s",EntLabel.GetText());
            sprintf(TempInstruction,"%s",EntInstruction.GetText());
            sscanf(EntTime.GetText(),"%d",&TempTime);
        }
    }
    
    if(SelectedLine>=0 && SelectedLink==0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkStart(TIMER_LINK,TempLabel,TempInstruction,TempTime);
        Ladder.Line[SelectedLine].ConvertArguments(0);
    }
    else if(SelectedLine>=0 && SelectedLink>=0 && Createobj)
    {
        Ladder.Line[SelectedLine].AddLinkAtPosition(SelectedLink,TIMER_LINK,TempLabel,TempInstruction,TempTime);
        Ladder.Line[SelectedLine].ConvertArguments(SelectedLink);
    }
    Compiled = false;
    Mnu_RunSimulator.SetActive(false);
    Mnu_RunTerminal.SetActive(false);
    Mnu_Build.SetActive(true);
    Dialog.Destroy();
}

gboolean clicked(GtkWidget *widget, GdkEventButton *event,gpointer user_data)
{
    /*if (event->button == 1) {
        cout<< event->x << " " ;
        cout<< event->y << ".";
    }*/

    int i;
    int x,y;
    x=(int)event->x;
    y=(int)event->y;
/////////////////////////Select All Line /////////////////
    if(x<50)
    {
        SelectedLink=0;
        SelectedLine=((y)/80);
        if(SelectedLine>=Ladder.TotalLines)SelectedLine=-1;
        
    }
///////////////////////// Select Only one Link ////////////////
    else 
    {
        SelectedLink=((x-50)/LINKSEPARATION)+1;
        SelectedLine=((y)/LINESEPARATION);
        //cout << SelectedLink << "_" <<SelectedLine<< endl;
        if(SelectedLine>=Ladder.TotalLines)SelectedLine=-1;
        if(SelectedLink>Ladder.Line[SelectedLine].TotalLinks)SelectedLink=0;
    }
/////////////////////////// For Edit Link /////////////////////////////////    
    if(SelectedLine==tSelectedLine && SelectedLink==tSelectedLink && SelectedLink>0)
    {
        EditLink_Funtion(NULL, Window.Widget);
    }
    else
    {
        tSelectedLine=SelectedLine;
        tSelectedLink=SelectedLink;
    }
    if(OpenedProject)
    { 
        Addline.SetActive(SelectedLine>=0);
        DeleteTool.SetActive(SelectedLine>=0);
        EditTool.SetActive(SelectedLine>=0&&SelectedLink>0);
        Mnu_EditLink.SetActive(SelectedLine>=0&&SelectedLink>0);
        Mnu_Copy.SetActive(SelectedLine>=0&&SelectedLink>0);
        Mnu_Cut.SetActive(SelectedLine>=0&&SelectedLink>0);
        Mnu_Paste.SetActive(SelectedLine>=0);
    }
    else
    {
        Addline.SetActive(false);
        DeleteTool.SetActive(false);
        EditTool.SetActive(false);
        Mnu_EditLink.SetActive(false);
        Mnu_Copy.SetActive(false);
        Mnu_Cut.SetActive(false);
        Mnu_Paste.SetActive(false);
    }

    return TRUE;
}

gboolean on_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer data)
{
    gtk_window_get_size (GTK_WINDOW(Window.Widget), &width, &height);
    gtk_widget_set_size_request(widget, width-50, height-180);
    
    if(!OpenedProject)return FALSE;    
     
    CairoImg Img(widget->window);
    
    Img.SetColor(0,0,0);
    Img.DrawRectangle(1, 1,width-50-1,height-180-2, 2);
    Img.Fill(1,1,1);
 
    if(SelectedLine>=0 && SelectedLink==0)
    {
        Img.SetColor(1,1,0);
        Img.DrawRectangle(5, 5+(LINESEPARATION*SelectedLine),width-100,75, 2);
        Img.Fill(1,1,0);
    }

    Img.SetColor(0,0,0.5);
    Img.DrawLine(50,20,50,height-100-5,1.5);
    Img.DrawLine(width-100,20,width-100,height-100-5,1.5);
    Img.SetFont("Sans");

    int i,j;
    char temp[10];
    for(i=0; i<Ladder.TotalLines ; i++)
    {
        Img.DrawLine(50,(i*80)+50,width-100,(i*80)+50,1.5);
        sprintf(temp,"Line %d",i);
        Img.DrawTextA(10,(i*80)+50,10,temp);
    }

    for(j=0; j<Ladder.TotalLines; j++)
    {
        for(i=0; i<Ladder.Line[j].TotalLinks; i++)
        {
            switch(Ladder.Line[j].Links[i].Type)
            {
                case SET_HIGH_LINK:
                    Img.SetColor(0,0.3,0);
                    Img.DrawRectangle(60+(LINKSEPARATION*i),30+(LINESEPARATION * j),40,40,2);
                    if(SelectedLine==j && SelectedLink==i+1)
                        Img.Fill(1,1,0);
                    else
                        Img.Fill(1,1,1);
                    Img.SetColor(0,0.3,0);
                    Img.DrawArc(80+(LINKSEPARATION*i),50+(LINESEPARATION * j),12,(M_PI/2)+0.25,(M_PI*3/2)-0.25,2);
                    Img.DrawArc(80+(LINKSEPARATION*i),50+(LINESEPARATION * j),12,(M_PI*3/2)+0.25,(M_PI/2)-0.25,2);
                    Img.DrawArc(80+(LINKSEPARATION * i),50+(LINESEPARATION * j),2,0,M_PI*2,5);
                    Img.DrawLine(50+(LINKSEPARATION * i),50+(LINESEPARATION * j),70+(LINKSEPARATION * i),50+(80*j),2);
                    Img.DrawLine(90+(LINKSEPARATION * i),50+(LINESEPARATION * j),110+(LINKSEPARATION * i),50+(80*j),2);
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(55+(LINKSEPARATION * i),82+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Label);
                    if(!(Ladder.Line[j].Links[i].Valid)) 
                    {
                        Img.SetColor(1,0.3,0.3);
                        Img.DrawRectangle(60+(LINKSEPARATION * i),20+(LINESEPARATION * j),30,10,2);
                        Img.Fill(1,0.05,0.05);
                    }
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(60+(LINKSEPARATION * i),27+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Instruction);
                    break;   
                case SET_LOW_LINK:
                    Img.SetColor(0,0.3,0);
                    Img.DrawRectangle(60+(LINKSEPARATION * i),30+(LINESEPARATION * j),40,40,2);
                    if(SelectedLine==j && SelectedLink==i+1)
                        Img.Fill(1,1,0);
                    else
                        Img.Fill(1,1,1);
                    Img.SetColor(0,0.3,0);
                    Img.DrawArc(80+(LINKSEPARATION * i),50+(LINESEPARATION * j),12,(M_PI/2)+0.25,(M_PI*3/2)-0.25,2);
                    Img.DrawArc(80+(LINKSEPARATION * i),50+(LINESEPARATION * j),12,(M_PI*3/2)+0.25,(M_PI/2)-0.25,2);
                    Img.DrawLine(50+(LINKSEPARATION * i),50+(LINESEPARATION * j),70+(LINKSEPARATION * i),50+(LINESEPARATION * j),2);
                    Img.DrawLine(90+(LINKSEPARATION * i),50+(LINESEPARATION * j),110+(LINKSEPARATION * i),50+(LINESEPARATION * j),2);
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(55+(LINKSEPARATION * i),82+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Label);
                    if(!(Ladder.Line[j].Links[i].Valid)) 
                    {
                        Img.SetColor(1,0.3,0.3);
                        Img.DrawRectangle(60+(LINKSEPARATION * i),20+(LINESEPARATION * j),30,10,2);
                        Img.Fill(1,0.05,0.05);
                    }
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(60+(LINKSEPARATION * i),27+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Instruction);
                    break;  
                case IF_LINK:
                    Img.SetColor(0,0.3,0);
                    Img.DrawRectangle(60+(LINKSEPARATION * i),30+(LINESEPARATION * j),40,40,2);
                    if(SelectedLine==j && SelectedLink==i+1)
                        Img.Fill(1,1,0);
                    else
                        Img.Fill(1,1,1);
                    Img.SetColor(0,0.3,0);
    
                    Img.DrawLine(50+(LINKSEPARATION * i),50+(LINESEPARATION * j),75+(LINKSEPARATION * i),50+(LINESEPARATION * j),2);
                    Img.DrawLine(85+(LINKSEPARATION * i),50+(LINESEPARATION * j),110+(LINKSEPARATION * i),50+(LINESEPARATION * j),2);
                    
                    Img.DrawLine(75+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),75+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),2);
                    Img.DrawLine(85+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),85+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),2);
                    
                    Img.DrawLine(70+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),75+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),2);
                    Img.DrawLine(70+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),75+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),2);
                    
                    Img.DrawLine(85+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),90+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),2);
                    Img.DrawLine(85+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),90+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),2);
                    
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(55+(LINKSEPARATION * i),82+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Label);
                    if(!(Ladder.Line[j].Links[i].Valid)) 
                    {
                        Img.SetColor(1,0.3,0.3);
                        Img.DrawRectangle(60+(LINKSEPARATION * i),20+(LINESEPARATION * j),30,10,2);
                        Img.Fill(1,0.05,0.05);
                    }
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(60+(LINKSEPARATION * i),27+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Instruction);
                    break;  
                case IF_NOT_LINK:
                    Img.SetColor(0,0.3,0);
                    Img.DrawRectangle(60+(LINKSEPARATION * i),30+(LINESEPARATION * j),40,40,2);

                    if(SelectedLine==j && SelectedLink==i+1)
                        Img.Fill(1,1,0);
                    else
                        Img.Fill(1,1,1);
                        
                    Img.SetColor(0,0.3,0);
    
                    Img.DrawLine(50+(LINKSEPARATION * i),50+(LINESEPARATION * j),75+(LINKSEPARATION * i),50+(LINESEPARATION * j),2);
                    Img.DrawLine(85+(LINKSEPARATION * i),50+(LINESEPARATION * j),110+(LINKSEPARATION * i),50+(LINESEPARATION * j),2);
                    
                    Img.DrawLine(75+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),75+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),2);
                    Img.DrawLine(85+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),85+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),2);
                    
                    Img.DrawLine(70+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),75+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),2);
                    Img.DrawLine(70+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),75+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),2);
                    
                    Img.DrawLine(85+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),90+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),2);
                    Img.DrawLine(85+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),90+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),2);
                    
                    Img.SetColor(1,0,0);
                    Img.DrawLine(85+(LINKSEPARATION * i),50+10+(LINESEPARATION * j),75+(LINKSEPARATION * i),50-10+(LINESEPARATION * j),2);
                    
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(55+(LINKSEPARATION * i),82+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Label);
                    if(!(Ladder.Line[j].Links[i].Valid)) 
                    {
                        Img.SetColor(1,0.3,0.3);
                        Img.DrawRectangle(60+(LINKSEPARATION * i),20+(LINESEPARATION * j),30,10,2);
                        Img.Fill(1,0.05,0.05);
                    }
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(60+(LINKSEPARATION * i),27+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Instruction);
                    break;  
                case TIMER_LINK:
                    Img.SetColor(0,0.3,0);
                    Img.DrawRectangle(60+(LINKSEPARATION * i),30+(LINESEPARATION * j),40,40,2);

                    if(SelectedLine==j && SelectedLink==i+1)
                        Img.Fill(1,1,0);
                    else
                        Img.Fill(1,1,1);
                        
                    Img.SetColor(0,0.3,0);
    
                    
                    Img.DrawArc(80+(LINKSEPARATION * i),58+(LINESEPARATION * j),8,0,M_PI*2,1);
                    Img.DrawLine(80+(LINKSEPARATION * i),58+(LINESEPARATION * j),80+(LINKSEPARATION * i),50+(LINESEPARATION * j),1);
                    Img.DrawLine(80+(LINKSEPARATION * i),58+(LINESEPARATION * j),85+(LINKSEPARATION * i),54+(LINESEPARATION * j),1);
                    Img.DrawTextA(65+(LINKSEPARATION * i),45+(LINESEPARATION * j),12,"Timer");
                    
                    Img.SetColor(0,0,0);
                    Img.DrawTextA(55+(LINKSEPARATION * i),82+(LINESEPARATION * j),10,Ladder.Line[j].Links[i].Label);
                    
                    if(!(Ladder.Line[j].Links[i].Valid)) 
                    {
                        Img.SetColor(1,0.3,0.3);
                        Img.DrawRectangle(60+(LINKSEPARATION * i),20+(LINESEPARATION * j),30,10,2);
                        Img.Fill(1,0.05,0.05);
                    }
                    Img.SetColor(0,0,0);
                    char Temp[20];
                    sprintf(Temp,"%s- %d mS", Ladder.Line[j].Links[i].Instruction, Ladder.Line[j].Links[i].Time);
                    Img.DrawTextA(60+(LINKSEPARATION * i),27+(LINESEPARATION * j),10,Temp);
                    break;  
            }    
        }   
    }
    return FALSE;
}

gboolean time_handler(GtkWidget *widget)
{
    if (widget->window == NULL) return FALSE;
    if(!RuningSimulator)gtk_widget_queue_draw(widget);
    return TRUE;
}
