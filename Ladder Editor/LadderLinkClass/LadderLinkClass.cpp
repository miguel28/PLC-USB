#include "LadderLinkClass.h"
using namespace std;


LineClass::LineClass()
{
    Links = (_ladder_links*) calloc(1,sizeof(_ladder_links));      
} 
void LineClass::AddLinkStart(short Type, char* Label, char* Instruction, int Time)
{  
    TempLink = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    
    /////// BackUp Links///////////
    int i;
    for(i=0; i<TotalLinks; i++)
    {
        TempLink[i]=Links[i];    
    }
    free(Links);
    ////////////////////Add New Link////////////////
    TotalLinks++;
    Links = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    Links[0].Type=Type;
    
    sprintf(Links[0].Label,"%s",Label);
    sprintf(Links[0].Instruction,"%s",Instruction);
    Links[0].Position=1;
    Links[0].Time=Time;
    ////////////// Restores Another Links   
    for(i=1; i<TotalLinks; i++)
    {
        Links[i]=TempLink[i-1];    
        Links[i].Position++;  
    }
    free(TempLink);
}

void LineClass::AddLinkEOF(short Type, char* Label, char* Instruction, int Time)
{
    TempLink = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    /////// BackUp Links///////////
    int i;
    for(i=0; i<TotalLinks; i++)
    {
        TempLink[i]=Links[i];    
    }
    free(Links);
    ////////////////////Add New Link////////////////
    TotalLinks++;
    Links = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    Links[TotalLinks-1].Type=Type;
    
    sprintf(Links[TotalLinks-1].Label,"%s",Label);
    sprintf(Links[TotalLinks-1].Instruction,"%s",Instruction);
    Links[TotalLinks-1].Position=TotalLinks;
    Links[TotalLinks-1].Time=Time;
    ////////////// Restores Another Links   
    for(i=0; i<TotalLinks-1; i++)
    {
        Links[i]=TempLink[i];  
    }
    free(TempLink);
}

void LineClass::AddLinkAtPosition(short Position,short Type, char* Label, char* Instruction, int Time)
{
    
    /////// BackUp Links///////////
    TempLink = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    
    int i;
    for(i=0; i<Position; i++)
    {
        TempLink[i]=Links[i];    
    }
    TempLink2 = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    
    for(i=Position; i<TotalLinks; i++)
    {
        TempLink2[i]=Links[i];    
    }
    free(Links);
    
    ////////////////////Add New Link////////////////
    TotalLinks++;
    Links = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    Links[Position].Type=Type;
    
    sprintf(Links[Position].Label,"%s",Label);
    sprintf(Links[Position].Instruction,"%s",Instruction);
    Links[Position].Position=Position;
    Links[Position].Time=Time;
    ////////////// Restores Another Links   
    for(i=0; i<Position; i++)
    {
        Links[i]=TempLink[i];    
    }
    
    for(i=Position+1; i<TotalLinks; i++)
    {
        Links[i]=TempLink2[i-1];    
    }
    
    free(TempLink);
    free(TempLink2);
}
void LineClass::DeleteLink(short Position)
{

    if(Position > TotalLinks) return;

        
    TempLink = (_ladder_links*) calloc(TotalLinks+1,sizeof(_ladder_links));  
    int i;
    for(i=0; i<TotalLinks; i++)
    {
        TempLink[i]=Links[i];    
    }
    free(Links);
    ////////////////////Delete Link////////////////
   
    ////////////// Restores Another Links 
    TotalLinks--;
    Links = (_ladder_links*) calloc(TotalLinks+1,sizeof(_ladder_links));  
    for(i=0; i<Position-1; i++)
    {
        Links[i]=TempLink[i];     
    }
    for(i=Position; i<TotalLinks+1; i++)
    {
        Links[i-1]=TempLink[i];    
        Links[i].Position--;  
    }    
    free(TempLink);        
}

short LineClass::ConvertArguments(short Position)
{
    
    char Letters[]="XOITSAX";
    
    Links[Position].Argument[0]=0;
    Links[Position].Argument[1]=0;
    Links[Position].Valid=false;
    switch(Links[Position].Type)
    {
        case SET_HIGH_LINK:
        case SET_LOW_LINK:
            if(Links[Position].Instruction[0] == 'O' && Links[Position].Instruction[1] == ':')
            {
                if(Links[Position].Instruction[2]>=0x30 && Links[Position].Instruction[2]<= 0x39)
                {                
                    Links[Position].Argument[0]=1;
                    Links[Position].Argument[1]=Links[Position].Instruction[2]-0x30;
                    Links[Position].Valid=true;
                } 
                else 
                {
                    Links[Position].Valid=false;
                    return 0;
                }
            } 
            else if(Links[Position].Instruction[0] == 'S' && Links[Position].Instruction[1] == ':')
            {
                if(Links[Position].Instruction[2]>=0x30 && Links[Position].Instruction[2]<= 0x39)
                {                
                    Links[Position].Argument[0]=4;
                    Links[Position].Argument[1]=Links[Position].Instruction[2]-0x30;
                    Links[Position].Valid=true;
                } 
                else 
                {
                    Links[Position].Valid=false;
                    return 0;
                }
            } 
            else 
            {
                Links[Position].Valid=false;
                return 0;
            }
            break;
        case IF_LINK:
        case IF_NOT_LINK:
            int i;
            Links[Position].Argument[0]=0;
            
            /*if(Links[Position].Instruction[0]=='I')
            {
                Links[Position].Argument[0]=2; 
            }*/

            for(i=1; i<=MAXALLOW; i++)
            {
                if(Links[Position].Instruction[0]==Letters[i])
                //if(Links[Position].Instruction[0]=='I')
                {
                    Links[Position].Argument[0]=i;
                    break;    
                }
            }
            if(Links[Position].Argument[0]==0)
            {
                Links[Position].Valid=false;
                return 0;
            }
            if(Links[Position].Instruction[1] == ':')
            {
                if(Links[Position].Instruction[2]>=0x30 && Links[Position].Instruction[2]<= 0x39)
                {                
                    Links[Position].Argument[1]=Links[Position].Instruction[2]-0x30;
                    Links[Position].Valid=true;
                }
                else 
                {
                    Links[Position].Valid=false;
                    return 0;
                }
            }
            else 
            {
                Links[Position].Valid=false;
                return 0;
            }
            break;
        case TIMER_LINK:
            if(Links[Position].Instruction[0] == 'T' && Links[Position].Instruction[1] == ':')
            {
                if(Links[Position].Instruction[2]>=0x30 && Links[Position].Instruction[2]<= 0x39)
                {                
                    Links[Position].Argument[0]=3;
                    Links[Position].Argument[1]=Links[Position].Instruction[2]-0x30;
                    Links[Position].Valid=true;
                } 
                else 
                {
                    Links[Position].Valid=false;
                    return 0;
                }
            } 
            break;
    }
    cout << Links[Position].Argument[0] << ":" << Links[Position].Argument[1] << endl;
    return 1;
}

void LineClass::PrintAllLinks()
{
    int i;
    for(i=0; i<TotalLinks; i++)
    {
        cout << "Link #"<< i << ":"<< endl;
        cout << "   Type: " << Links[i].Type << endl;
        cout << "   Label: " << Links[i].Label << endl;
    }
}

void LineClass::DeleteLine()
{
    TotalLinks=0;
    free(Links); 
    free(TempLink); 
    free(TempLink2); 
    Links = (_ladder_links*) calloc(1,sizeof(_ladder_links));  
}

void LineClass::operator= (LineClass param) {

    TotalLinks=param.TotalLinks;
    
    Links = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));   
    int i;
    for(i=0; i<TotalLinks; i++)
    {
        this->Links[i]=param.Links[i];       
    }  

}

_ladder_links LineClass::GetLink(short Position)
{
    return Links[Position];
}

void LineClass::AddLinkAtPositionFormOther(short Position,_ladder_links NewLink)
{
    /////// BackUp Links///////////
    TempLink = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    
    int i;
    for(i=0; i<Position; i++)
    {
        TempLink[i]=Links[i];    
    }
    TempLink2 = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    
    for(i=Position; i<TotalLinks; i++)
    {
        TempLink2[i]=Links[i];    
    }
    free(Links);
    
    ////////////////////Add New Link////////////////
    TotalLinks++;
    Links = (_ladder_links*) calloc(TotalLinks,sizeof(_ladder_links));  
    Links[Position]= NewLink;
    ////////////// Restores Another Links   
    for(i=0; i<Position; i++)
    {
        Links[i]=TempLink[i];    
    }
    
    for(i=Position+1; i<TotalLinks; i++)
    {
        Links[i]=TempLink2[i-1];    
    }
    
    free(TempLink);
    free(TempLink2);
    
}
void LineClass::CreateLineFromSize(int Size)
{
    
    //Links = (_ladder_links*) malloc(Size);  
    Links = (_ladder_links*) calloc(Size/sizeof(_ladder_links),sizeof(_ladder_links));  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
LadderClass::LadderClass(bool Init)
{
    TotalLines=1;    
    //Letters={'X','O','I','T','S','A','X'};
    /*Letters[0]='X';
    Letters[1]='O';
    Letters[2]='I';
    Letters[3]='T';
    Letters[4]='S';
    Letters[5]='A';
    Letters[6]='X';*/
}
void LadderClass::AddLine(short NumLine)
{
    if(NumLine > TotalLines) return;
    
    int i;
    for(i=0; i<TotalLines; i++)
    {
        TempL[i]=Line[i];
    } 
    ////////////////////////
    Line[NumLine].DeleteLine();
    TotalLines++;
    for(i=NumLine+1; i<TotalLines; i++)
    {
        Line[i]=TempL[i-1];
    } 
    
}
void LadderClass::DeleteLine(short NumLine)
{
    if(NumLine > TotalLines) return;
    
    int i;
    for(i=0; i<TotalLines; i++)
    {
        TempL[i]=Line[i];
    } 
    ////////////////////////
    Line[NumLine].DeleteLine();
    TotalLines--;
    for(i=NumLine; i<TotalLines; i++)
    {
        Line[i]=TempL[i+1];
    }      
}

void LadderClass::DeleteAllLines()
{
    AddLine(0);
    int i;
    for(i=1; i<TotalLines; i++)DeleteLine(i);
}
