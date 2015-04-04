#include "USBInterfaceClass.h"
#include <windows.h>
using namespace std;
USBInterface USB;
int main(int argc, char *argv[])
{
	
	
	USB.EnumerateHIDs();
    USB.HIDOpen(0x3995,0x0001);	
    USB.HIDDescription();	
	USB.SendOutputPort((unsigned char) 0x01);
    Sleep(1000);
    USB.SendOutputPort((unsigned char) 0x02);
    Sleep(1000);	
	
    USB.ReciveBuffer();
    USB.PrintBufferIN();
    Sleep(20);
    USB.ReciveBuffer();
    USB.PrintBufferIN();
    	
    system("PAUSE");
    return EXIT_SUCCESS;
}
