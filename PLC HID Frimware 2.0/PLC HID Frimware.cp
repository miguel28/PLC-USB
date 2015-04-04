#line 1 "C:/Users/Mike/Desktop/PICS/USB/PLC HID Frimware 2.0/PLC HID Frimware.c"
#line 1 "c:/users/mike/desktop/pics/usb/plc hid frimware 2.0/usbdsc.c"
const unsigned int USB_VENDOR_ID = 0x3995;
const unsigned int USB_PRODUCT_ID = 0x0001;
const char USB_SELF_POWER = 0x80;
const char USB_MAX_POWER = 50;
const char HID_INPUT_REPORT_BYTES = 64;
const char HID_OUTPUT_REPORT_BYTES = 64;
const char USB_TRANSFER_TYPE = 0x03;
const char EP_IN_INTERVAL = 1;
const char EP_OUT_INTERVAL = 1;

const char USB_INTERRUPT = 1;
const char USB_HID_EP = 1;
const char USB_HID_RPT_SIZE = 33;


const struct {
 char bLength;
 char bDescriptorType;
 unsigned int bcdUSB;
 char bDeviceClass;
 char bDeviceSubClass;
 char bDeviceProtocol;
 char bMaxPacketSize0;
 unsigned int idVendor;
 unsigned int idProduct;
 unsigned int bcdDevice;
 char iManufacturer;
 char iProduct;
 char iSerialNumber;
 char bNumConfigurations;
} device_dsc = {
 0x12,
 0x01,
 0x0200,
 0x00,
 0x00,
 0x00,
 8,
 USB_VENDOR_ID,
 USB_PRODUCT_ID,
 0x0001,
 0x01,
 0x02,
 0x00,
 0x01
 };


const char configDescriptor1[]= {

 0x09,
 0x02,
 0x29,0x00,
 1,
 1,
 0,
 USB_SELF_POWER,
 USB_MAX_POWER,


 0x09,
 0x04,
 0,
 0,
 2,
 0x03,
 0,
 0,
 0,


 0x09,
 0x21,
 0x01,0x01,
 0x00,
 1,
 0x22,
 USB_HID_RPT_SIZE,0x00,


 0x07,
 0x05,
 USB_HID_EP | 0x80,
 USB_TRANSFER_TYPE,
 0x40,0x00,
 EP_IN_INTERVAL,


 0x07,
 0x05,
 USB_HID_EP,
 USB_TRANSFER_TYPE,
 0x40,0x00,
 EP_OUT_INTERVAL
};

const struct {
 char report[USB_HID_RPT_SIZE];
}hid_rpt_desc =
 {
 {0x06, 0x00, 0xFF,
 0x09, 0x01,
 0xA1, 0x01,

 0x19, 0x01,
 0x29, 0x40,
 0x15, 0x00,
 0x26, 0xFF, 0x00,
 0x75, 0x08,
 0x95, HID_INPUT_REPORT_BYTES,
 0x81, 0x02,

 0x19, 0x01,
 0x29, 0x40,
 0x75, 0x08,
 0x95, HID_OUTPUT_REPORT_BYTES,
 0x91, 0x02,
 0xC0}
 };


const struct {
 char bLength;
 char bDscType;
 unsigned int string[1];
 } strd1 = {
 4,
 0x03,
 {0x0409}
 };



const struct{
 char bLength;
 char bDscType;
 unsigned int string[12];
 }strd2={
 26,
 0x03,
 {'M','i','g','u','e','l',' ','S','i','l','v','a'}
 };


const struct{
 char bLength;
 char bDscType;
 unsigned int string[20];
}strd3={
 42,
 0x03,
 {'V','i','r','t','u','a','l',' ','P','L','C',' ','T','e','r','m','i','n','a','l'}
 };


const char* USB_config_dsc_ptr[1];


const char* USB_string_dsc_ptr[3];

void USB_Init_Desc(){
 USB_config_dsc_ptr[0] = &configDescriptor1;
 USB_string_dsc_ptr[0] = (const char*)&strd1;
 USB_string_dsc_ptr[1] = (const char*)&strd2;
 USB_string_dsc_ptr[2] = (const char*)&strd3;
}
#line 2 "C:/Users/Mike/Desktop/PICS/USB/PLC HID Frimware 2.0/PLC HID Frimware.c"
unsigned char ReadBuff[65] absolute 0x500;
unsigned char WriteBuff[65] absolute 0x542;

enum PLC_MODES
{
 SIMPLE_PLC_MODE,
 ANALOG_PLC_MODE,
 SIMPLE_ASME_DRIVER,
 ANALOG_ASME_DRIVER
};

enum RFCoding
{
 UNUSED,
 DISTANCE_LOW,
 DISTANCE_HIGH,
 ANGLE_LOW,
 ANGLE_HIGH,
 AXIS_X_LOW,
 AXIS_X_HIGH,
 AXIS_Y_LOW,
 AXIS_Y_HIGH,
 AXIS_Z_LOW,
 AXIS_Z_HIGH
};

char cnt;
char rByte;
char CurrentMode;
char Distance,tDistance,tDistance2,fDistance;
char Angle,tAngle,tAngle2,fAngle;
char AxisX,AxisY,AxisZ;

void SetMode(char Mode)
{
 switch(Mode)
 {
 case SIMPLE_PLC_MODE:
 ADCON1 |= 0x0F;
 CMCON |= 7;
 TRISD = 0xFF;
 TRISB = 0x00;
 break;
 case SIMPLE_ASME_DRIVER:
 ADCON1 |= 0x0F;
 CMCON |= 7;
 TRISD = 0xFF;
 TRISB = 0x00;

 break;
 }
}

void RF_Decoding()
{

 rByte = PORTD;

 switch((rByte&0xF0)>>4)
 {
 case DISTANCE_LOW:
 tDistance=0x00;
 tDistance |= (rByte&0x0F);
 fDistance |= 0x01;
 break;
 case DISTANCE_HIGH:
 tDistance2=0x00;
 tDistance2 |= (rByte&0x0F);
 fDistance |= 0x02;
 break;
 case ANGLE_LOW:
 tAngle=0x00;
 tAngle |= (rByte&0x0F);
 fAngle |= 0x01;
 break;
 case ANGLE_HIGH:
 tAngle2=0x00;
 tAngle2 |= (rByte&0x0F);
 fAngle |= 0x02;
 break;
 case AXIS_X_LOW:
 case AXIS_X_HIGH:
 case AXIS_Y_LOW:
 case AXIS_Y_HIGH:
 case AXIS_Z_LOW:
 case AXIS_Z_HIGH:
 break;
 }


 if(fDistance=0x03)
 {
 Distance=(tDistance2<<4)|(tDistance);
 fDistance=0x00;
 }
 if(fAngle=0x03)
 {
 Angle=(tAngle2<<4)|(tAngle);
 fAngle=0x00;
 }
 WriteBuff[1] = Distance;
 WriteBuff[2] = Angle;
 WriteBuff[3] = AxisX;
 WriteBuff[4] = AxisY;
 WriteBuff[5] = AxisZ;
}

void interrupt(){
 USB_Interrupt_Proc();
}

void main(void){
 ADCON1 |= 0x0F;
 CMCON |= 7;
 TRISD = 0xFF;
 TRISB = 0x00;
 CurrentMode = SIMPLE_PLC_MODE;
 HID_Enable(&ReadBuff,&WriteBuff);

 while(1){


 while(!HID_Read());
 PORTB = ReadBuff[0];
 CurrentMode = ReadBuff[63];


 if(SIMPLE_ASME_DRIVER)RF_Decoding();
 WriteBuff[0]=PORTD;

 while(!HID_Write(&WriteBuff,64));
 SetMode(CurrentMode);
 }
}
