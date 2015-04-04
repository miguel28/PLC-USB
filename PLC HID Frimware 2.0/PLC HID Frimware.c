#include "USBdsc.c"
unsigned char ReadBuff[65] absolute 0x500;   // Buffers should be in USB RAM, please consult datasheet
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
         ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
         CMCON  |= 7;                            // Disable comparators
         TRISD = 0xFF;
         TRISB = 0x00;
         break;
      case SIMPLE_ASME_DRIVER:
         ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
         CMCON  |= 7;                            // Disable comparators
         TRISD = 0xFF;
         TRISB = 0x00;
         //UART1_Init(9600);
         break;
   }
}

void RF_Decoding()
{
   //if (UART1_Data_Ready() == 1) rByte = UART1_Read();
   rByte = PORTD;
   ////////// Interpreting Data
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
   
   //// Asing Data
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
   USB_Interrupt_Proc();                   // USB servicing is done inside the interrupt
}

void main(void){
  ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
  CMCON  |= 7;                            // Disable comparators
  TRISD = 0xFF;
  TRISB = 0x00;
  CurrentMode = SIMPLE_PLC_MODE;
  HID_Enable(&ReadBuff,&WriteBuff);       // Enable HID communication

  while(1){

      //////Read State
      while(!HID_Read());
      PORTB = ReadBuff[0];
      CurrentMode = ReadBuff[63];

      ////// Write State
      if(SIMPLE_ASME_DRIVER)RF_Decoding();
      WriteBuff[0]=PORTD;
      
      while(!HID_Write(&WriteBuff,64));
      SetMode(CurrentMode);
  }
}