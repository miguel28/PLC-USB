
_USB_Init_Desc:

;usbdsc.c,161 :: 		void USB_Init_Desc(){
;usbdsc.c,162 :: 		USB_config_dsc_ptr[0] = &configDescriptor1;
	MOVLW       _configDescriptor1+0
	MOVWF       _USB_config_dsc_ptr+0 
	MOVLW       hi_addr(_configDescriptor1+0)
	MOVWF       _USB_config_dsc_ptr+1 
	MOVLW       higher_addr(_configDescriptor1+0)
	MOVWF       _USB_config_dsc_ptr+2 
;usbdsc.c,163 :: 		USB_string_dsc_ptr[0] = (const char*)&strd1;
	MOVLW       _strd1+0
	MOVWF       _USB_string_dsc_ptr+0 
	MOVLW       hi_addr(_strd1+0)
	MOVWF       _USB_string_dsc_ptr+1 
	MOVLW       higher_addr(_strd1+0)
	MOVWF       _USB_string_dsc_ptr+2 
;usbdsc.c,164 :: 		USB_string_dsc_ptr[1] = (const char*)&strd2;
	MOVLW       _strd2+0
	MOVWF       _USB_string_dsc_ptr+3 
	MOVLW       hi_addr(_strd2+0)
	MOVWF       _USB_string_dsc_ptr+4 
	MOVLW       higher_addr(_strd2+0)
	MOVWF       _USB_string_dsc_ptr+5 
;usbdsc.c,165 :: 		USB_string_dsc_ptr[2] = (const char*)&strd3;
	MOVLW       _strd3+0
	MOVWF       _USB_string_dsc_ptr+6 
	MOVLW       hi_addr(_strd3+0)
	MOVWF       _USB_string_dsc_ptr+7 
	MOVLW       higher_addr(_strd3+0)
	MOVWF       _USB_string_dsc_ptr+8 
;usbdsc.c,166 :: 		}
L_end_USB_Init_Desc:
	RETURN      0
; end of _USB_Init_Desc

_interrupt:

;PLC HID Frimware.c,8 :: 		void interrupt(){
;PLC HID Frimware.c,9 :: 		USB_Interrupt_Proc();                   // USB servicing is done inside the interrupt
	CALL        _USB_Interrupt_Proc+0, 0
;PLC HID Frimware.c,10 :: 		}
L_end_interrupt:
L__interrupt13:
	RETFIE      1
; end of _interrupt

_main:

;PLC HID Frimware.c,12 :: 		void main(void){
;PLC HID Frimware.c,13 :: 		ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
	MOVLW       15
	IORWF       ADCON1+0, 1 
;PLC HID Frimware.c,14 :: 		CMCON  |= 7;                            // Disable comparators
	MOVLW       7
	IORWF       CMCON+0, 1 
;PLC HID Frimware.c,15 :: 		TRISD=0xFF;
	MOVLW       255
	MOVWF       TRISD+0 
;PLC HID Frimware.c,16 :: 		TRISB=0x00;
	CLRF        TRISB+0 
;PLC HID Frimware.c,19 :: 		Delay_ms(100);
	MOVLW       7
	MOVWF       R11, 0
	MOVLW       23
	MOVWF       R12, 0
	MOVLW       106
	MOVWF       R13, 0
L_main0:
	DECFSZ      R13, 1, 1
	BRA         L_main0
	DECFSZ      R12, 1, 1
	BRA         L_main0
	DECFSZ      R11, 1, 1
	BRA         L_main0
	NOP
;PLC HID Frimware.c,20 :: 		PORTD=0;
	CLRF        PORTD+0 
;PLC HID Frimware.c,21 :: 		Delay_ms(100);
	MOVLW       7
	MOVWF       R11, 0
	MOVLW       23
	MOVWF       R12, 0
	MOVLW       106
	MOVWF       R13, 0
L_main1:
	DECFSZ      R13, 1, 1
	BRA         L_main1
	DECFSZ      R12, 1, 1
	BRA         L_main1
	DECFSZ      R11, 1, 1
	BRA         L_main1
	NOP
;PLC HID Frimware.c,23 :: 		HID_Enable(&readbuff,&writebuff);       // Enable HID communication
	MOVLW       _readbuff+0
	MOVWF       FARG_HID_Enable_readbuff+0 
	MOVLW       hi_addr(_readbuff+0)
	MOVWF       FARG_HID_Enable_readbuff+1 
	MOVLW       _writebuff+0
	MOVWF       FARG_HID_Enable_writebuff+0 
	MOVLW       hi_addr(_writebuff+0)
	MOVWF       FARG_HID_Enable_writebuff+1 
	CALL        _HID_Enable+0, 0
;PLC HID Frimware.c,25 :: 		while(1){
L_main2:
;PLC HID Frimware.c,27 :: 		while(!HID_Read())
L_main4:
	CALL        _HID_Read+0, 0
	MOVF        R0, 1 
	BTFSS       STATUS+0, 2 
	GOTO        L_main5
;PLC HID Frimware.c,28 :: 		;
	GOTO        L_main4
L_main5:
;PLC HID Frimware.c,30 :: 		for(cnt=0;cnt<64;cnt++)
	CLRF        _cnt+0 
L_main6:
	MOVLW       64
	SUBWF       _cnt+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_main7
;PLC HID Frimware.c,32 :: 		writebuff[cnt]=readbuff[cnt];
	MOVLW       _writebuff+0
	MOVWF       FSR1L 
	MOVLW       hi_addr(_writebuff+0)
	MOVWF       FSR1H 
	MOVF        _cnt+0, 0 
	ADDWF       FSR1L, 1 
	BTFSC       STATUS+0, 0 
	INCF        FSR1H, 1 
	MOVLW       _readbuff+0
	MOVWF       FSR0L 
	MOVLW       hi_addr(_readbuff+0)
	MOVWF       FSR0H 
	MOVF        _cnt+0, 0 
	ADDWF       FSR0L, 1 
	BTFSC       STATUS+0, 0 
	INCF        FSR0H, 1 
	MOVF        POSTINC0+0, 0 
	MOVWF       POSTINC1+0 
;PLC HID Frimware.c,30 :: 		for(cnt=0;cnt<64;cnt++)
	INCF        _cnt+0, 1 
;PLC HID Frimware.c,33 :: 		}
	GOTO        L_main6
L_main7:
;PLC HID Frimware.c,34 :: 		writebuff[0]=PORTD;
	MOVF        PORTD+0, 0 
	MOVWF       1346 
;PLC HID Frimware.c,35 :: 		while(!HID_Write(&writebuff,64))
L_main9:
	MOVLW       _writebuff+0
	MOVWF       FARG_HID_Write_writebuff+0 
	MOVLW       hi_addr(_writebuff+0)
	MOVWF       FARG_HID_Write_writebuff+1 
	MOVLW       64
	MOVWF       FARG_HID_Write_len+0 
	CALL        _HID_Write+0, 0
	MOVF        R0, 1 
	BTFSS       STATUS+0, 2 
	GOTO        L_main10
;PLC HID Frimware.c,36 :: 		;
	GOTO        L_main9
L_main10:
;PLC HID Frimware.c,37 :: 		PORTB=readbuff[0];
	MOVF        1280, 0 
	MOVWF       PORTB+0 
;PLC HID Frimware.c,38 :: 		}
	GOTO        L_main2
;PLC HID Frimware.c,39 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
