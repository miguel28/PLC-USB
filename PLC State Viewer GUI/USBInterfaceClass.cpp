#include "USBInterfaceClass.h"

void USBInterface::EnumerateHIDs()
{
    Devs = hid_enumerate(0x0, 0x0);
	CurDev = Devs;	
	while (CurDev) {
		printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls",
			CurDev->vendor_id, CurDev->product_id, CurDev->path, CurDev->serial_number);
		printf("\n");
		printf("  Manufacturer: %ls\n", CurDev->manufacturer_string);
		printf("  Product:      %ls\n", CurDev->product_string);
		printf("\n");
		CurDev = CurDev->next;
	}
	hid_free_enumeration(Devs);     
     
}

void USBInterface::HIDOpen(const unsigned short VendorID, const unsigned short ProductID)
{
    if(!HIDisOpen)
    {
        DeviceHandle = hid_open(VendorID, ProductID, NULL);
        if(DeviceHandle)HIDisOpen= true;
    }
}

void USBInterface::HIDDescription()
{
    if(HIDisOpen)
    {
        res = hid_get_manufacturer_string(DeviceHandle, wstr, MAX_STR);
        printf("Manufacturer String: %ls\n", wstr);
        res = hid_get_product_string(DeviceHandle, wstr, MAX_STR);
        printf("Product String: %ls\n", wstr);
        res =  hid_get_serial_number_string(DeviceHandle, wstr, MAX_STR);
        printf("Serial String: %ls\n", wstr);
    }
}

void USBInterface::HIDClose()
{
     if(HIDisOpen)
     {
        hid_close(DeviceHandle);
        hid_set_nonblocking(DeviceHandle,1);
        HIDisOpen= false;    
     }
}

int USBInterface::SendBuffer()
{
    if(HIDisOpen)
        return hid_write(DeviceHandle, BufferOUT, 65);
    else return -1;
}

int USBInterface::SendOutputPort(const unsigned char Value)
{
    if(HIDisOpen)
    {
        CleanBufferOUT();
        BufferOUT[0]=0x00;
        BufferOUT[1]=Value;
        return SendBuffer();
    }
    else return -1;
    
}

void USBInterface::CleanBufferOUT()
{
    for(i=0; i<65; i++)BufferOUT[i]=0x00;     
    
}

void USBInterface::CleanBufferIN()
{
    for(i=0; i<65; i++)BufferIN[i]=0x00;     
}

int USBInterface::ReciveBuffer()
{
    CleanBufferIN();
    if(HIDisOpen)
    {
        //res = hid_read_timeout(DeviceHandle, BufferIN, 65,1);
        res = hid_read(DeviceHandle, BufferIN, 65);
        return res;
    }
    else return -1;
}

void USBInterface::PrintBufferIN()
{
    for (i = 0; i < res; i++)
		printf("buf[%d]: %d\n", i, BufferIN[i]);
    
}

unsigned char USBInterface::GetInputBuffer()
{
    CleanBufferIN();
    ReciveBuffer();
    if(HIDisOpen)
    {
        return BufferIN[0];
    }
    else return 1;
}
