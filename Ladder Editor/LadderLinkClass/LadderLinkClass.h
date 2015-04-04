#include <cstdlib>
#include <iostream>
#include <stdio.h>

#define MAXLINES 50
#define MAXALLOW 6


//char Letters[]={'X','O','I','T','S','A','X'};


typedef struct
{
    short Position;
    short Type;
    bool Valid;
    char Label[15],Instruction[6];
    int Time,currentTime;
    short Argument[3]; 
}_ladder_links;

enum{
    NO_LINK,
    SET_HIGH_LINK,
    SET_LOW_LINK,
    IF_LINK,
    IF_NOT_LINK,
    TIMER_LINK
};

typedef struct
{
   char Data[6]; 
   char Hour;
   char Minute; 
}_HoursAlamrs;

typedef struct
{
    char Name[25];
    char Version[8];
    char Description[255];
    char InverseLogicInput;
    char InverseLogicOutput;
    char UseAnalogInputs;
    _HoursAlamrs Alarm[8]; 
    short TotalLines;
}_ProjectConfig;

class LineClass
{
    public:
    int TotalLinks;
    _ladder_links *Links, *TempLink,*TempLink2;
    LineClass(); 
    void AddLinkStart(short Type, char* Label, char* Instruction, int Time);   
    void AddLinkEOF(short Type, char* Label, char* Instruction, int Time);
    void AddLinkAtPosition(short Position,short Type, char* Label, char* Instruction, int Time);
    void DeleteLink(short Position);
    short ConvertArguments(short Position);
    void PrintAllLinks();
    void DeleteLine();
    void operator = (LineClass);
    _ladder_links GetLink(short Position);
    void AddLinkAtPositionFormOther(short Position,_ladder_links NewLink);
    void CreateLineFromSize(int Size);
};

class LadderClass
{
    public:
    int TotalLines;
    LineClass Line[MAXLINES], TempL[MAXLINES]; 
    LadderClass(bool Init);   
    void AddLine(short NumLine);
    void DeleteLine(short NumLine);
    void DeleteAllLines();
};
