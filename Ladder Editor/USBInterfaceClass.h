#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "hidapi.h"

#define MAX_STR 255

class USBInterface
{
      public:
      int i;
      int res;
	  unsigned char BufferOUT[65];
	  unsigned char BufferIN[65];
	  wchar_t wstr[MAX_STR];
	  hid_device *DeviceHandle;
	  struct hid_device_info *Devs, *CurDev;
	  bool HIDisOpen;
	  
	  void EnumerateHIDs(); // List All HID Devices conected in the computer
      void HIDOpen(const unsigned short VendorID, const unsigned short ProductID); // Open Specific HID device to work
      void HIDDescription();
      void HIDClose(); // Close the device of work
      
      int SendBuffer(); // Send BufferOUT to the HID
      int SendOutputPort(const unsigned char Value);
      void CleanBufferOUT();
      void CleanBufferIN();
      
      int ReciveBuffer();
      
      void PrintBufferIN();
      
      unsigned char GetInputBuffer();
};
