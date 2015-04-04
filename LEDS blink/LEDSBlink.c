#include "USBdsc.c"
unsigned char readbuff[1] absolute 0x500;   // Buffers should be in USB RAM, please consult datasheet
unsigned char writebuff[1] absolute 0x540;

char cnt;
char kk;

void interrupt(){
   USB_Interrupt_Proc();                   // USB servicing is done inside the interrupt
}

void main(void){
  ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
  CMCON  |= 7;                            // Disable comparators
  TRISD=0x00;
  TRISB=0x00;
  
  PORTD=0xff;
  Delay_ms(100);
  PORTD=0;
  Delay_ms(100);
  //interrupt();
  HID_Enable(&readbuff,&writebuff);       // Enable HID communication

  while(1){
  PORTD=0xff;
  Delay_ms(100);
  PORTD=0;
  Delay_ms(100);
  PORTB=readbuff[0];
  
    while(!HID_Read())
      ;

    while(!HID_Write(&writebuff,64))
      ;
  }
}