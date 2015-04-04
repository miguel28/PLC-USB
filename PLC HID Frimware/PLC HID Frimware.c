#include "USBdsc.c"
unsigned char readbuff[65] absolute 0x500;   // Buffers should be in USB RAM, please consult datasheet
unsigned char writebuff[65] absolute 0x542;

char cnt;
char kk;

void interrupt(){
   USB_Interrupt_Proc();                   // USB servicing is done inside the interrupt
}

void main(void){
  ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
  CMCON  |= 7;                            // Disable comparators
  TRISD=0xFF;
  TRISB=0x00;

  //PORTD=0xff;
  Delay_ms(100);
  PORTD=0;
  Delay_ms(100);

  HID_Enable(&readbuff,&writebuff);       // Enable HID communication

  while(1){

      while(!HID_Read())
      ;
      
      for(cnt=0;cnt<64;cnt++)
      {
          writebuff[cnt]=readbuff[cnt];
      }
      writebuff[0]=PORTD;
      while(!HID_Write(&writebuff,64))
      ;
      PORTB=readbuff[0];
  }
}