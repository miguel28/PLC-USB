
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

_SetMode:

;PLC HID Frimware.c,35 :: 		void SetMode(char Mode)
;PLC HID Frimware.c,37 :: 		switch(Mode)
	GOTO        L_SetMode0
;PLC HID Frimware.c,39 :: 		case SIMPLE_PLC_MODE:
L_SetMode2:
;PLC HID Frimware.c,40 :: 		ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
	MOVLW       15
	IORWF       ADCON1+0, 1 
;PLC HID Frimware.c,41 :: 		CMCON  |= 7;                            // Disable comparators
	MOVLW       7
	IORWF       CMCON+0, 1 
;PLC HID Frimware.c,42 :: 		TRISD = 0xFF;
	MOVLW       255
	MOVWF       TRISD+0 
;PLC HID Frimware.c,43 :: 		TRISB = 0x00;
	CLRF        TRISB+0 
;PLC HID Frimware.c,44 :: 		break;
	GOTO        L_SetMode1
;PLC HID Frimware.c,45 :: 		case SIMPLE_ASME_DRIVER:
L_SetMode3:
;PLC HID Frimware.c,46 :: 		ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
	MOVLW       15
	IORWF       ADCON1+0, 1 
;PLC HID Frimware.c,47 :: 		CMCON  |= 7;                            // Disable comparators
	MOVLW       7
	IORWF       CMCON+0, 1 
;PLC HID Frimware.c,48 :: 		TRISD = 0xFF;
	MOVLW       255
	MOVWF       TRISD+0 
;PLC HID Frimware.c,49 :: 		TRISB = 0x00;
	CLRF        TRISB+0 
;PLC HID Frimware.c,51 :: 		break;
	GOTO        L_SetMode1
;PLC HID Frimware.c,52 :: 		}
L_SetMode0:
	MOVF        FARG_SetMode_Mode+0, 0 
	XORLW       0
	BTFSC       STATUS+0, 2 
	GOTO        L_SetMode2
	MOVF        FARG_SetMode_Mode+0, 0 
	XORLW       2
	BTFSC       STATUS+0, 2 
	GOTO        L_SetMode3
L_SetMode1:
;PLC HID Frimware.c,53 :: 		}
L_end_SetMode:
	RETURN      0
; end of _SetMode

_RF_Decoding:

;PLC HID Frimware.c,55 :: 		void RF_Decoding()
;PLC HID Frimware.c,58 :: 		rByte = PORTD;
	MOVF        PORTD+0, 0 
	MOVWF       _rByte+0 
;PLC HID Frimware.c,60 :: 		switch((rByte&0xF0)>>4)
	MOVLW       240
	ANDWF       _rByte+0, 0 
	MOVWF       R2 
	MOVF        R2, 0 
	MOVWF       R1 
	RRCF        R1, 1 
	BCF         R1, 7 
	RRCF        R1, 1 
	BCF         R1, 7 
	RRCF        R1, 1 
	BCF         R1, 7 
	RRCF        R1, 1 
	BCF         R1, 7 
	GOTO        L_RF_Decoding4
;PLC HID Frimware.c,62 :: 		case DISTANCE_LOW:
L_RF_Decoding6:
;PLC HID Frimware.c,63 :: 		tDistance=0x00;
	CLRF        _tDistance+0 
;PLC HID Frimware.c,64 :: 		tDistance |= (rByte&0x0F);
	MOVLW       15
	ANDWF       _rByte+0, 0 
	MOVWF       _tDistance+0 
;PLC HID Frimware.c,65 :: 		fDistance |= 0x01;
	BSF         _fDistance+0, 0 
;PLC HID Frimware.c,66 :: 		break;
	GOTO        L_RF_Decoding5
;PLC HID Frimware.c,67 :: 		case DISTANCE_HIGH:
L_RF_Decoding7:
;PLC HID Frimware.c,68 :: 		tDistance2=0x00;
	CLRF        _tDistance2+0 
;PLC HID Frimware.c,69 :: 		tDistance2 |= (rByte&0x0F);
	MOVLW       15
	ANDWF       _rByte+0, 0 
	MOVWF       _tDistance2+0 
;PLC HID Frimware.c,70 :: 		fDistance |= 0x02;
	BSF         _fDistance+0, 1 
;PLC HID Frimware.c,71 :: 		break;
	GOTO        L_RF_Decoding5
;PLC HID Frimware.c,72 :: 		case ANGLE_LOW:
L_RF_Decoding8:
;PLC HID Frimware.c,73 :: 		tAngle=0x00;
	CLRF        _tAngle+0 
;PLC HID Frimware.c,74 :: 		tAngle |= (rByte&0x0F);
	MOVLW       15
	ANDWF       _rByte+0, 0 
	MOVWF       _tAngle+0 
;PLC HID Frimware.c,75 :: 		fAngle |= 0x01;
	BSF         _fAngle+0, 0 
;PLC HID Frimware.c,76 :: 		break;
	GOTO        L_RF_Decoding5
;PLC HID Frimware.c,77 :: 		case ANGLE_HIGH:
L_RF_Decoding9:
;PLC HID Frimware.c,78 :: 		tAngle2=0x00;
	CLRF        _tAngle2+0 
;PLC HID Frimware.c,79 :: 		tAngle2 |= (rByte&0x0F);
	MOVLW       15
	ANDWF       _rByte+0, 0 
	MOVWF       _tAngle2+0 
;PLC HID Frimware.c,80 :: 		fAngle |= 0x02;
	BSF         _fAngle+0, 1 
;PLC HID Frimware.c,81 :: 		break;
	GOTO        L_RF_Decoding5
;PLC HID Frimware.c,82 :: 		case AXIS_X_LOW:
L_RF_Decoding10:
;PLC HID Frimware.c,83 :: 		case AXIS_X_HIGH:
L_RF_Decoding11:
;PLC HID Frimware.c,84 :: 		case AXIS_Y_LOW:
L_RF_Decoding12:
;PLC HID Frimware.c,85 :: 		case AXIS_Y_HIGH:
L_RF_Decoding13:
;PLC HID Frimware.c,86 :: 		case AXIS_Z_LOW:
L_RF_Decoding14:
;PLC HID Frimware.c,87 :: 		case AXIS_Z_HIGH:
L_RF_Decoding15:
;PLC HID Frimware.c,88 :: 		break;
	GOTO        L_RF_Decoding5
;PLC HID Frimware.c,89 :: 		}
L_RF_Decoding4:
	MOVF        R1, 0 
	XORLW       1
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding6
	MOVF        R1, 0 
	XORLW       2
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding7
	MOVF        R1, 0 
	XORLW       3
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding8
	MOVF        R1, 0 
	XORLW       4
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding9
	MOVF        R1, 0 
	XORLW       5
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding10
	MOVF        R1, 0 
	XORLW       6
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding11
	MOVF        R1, 0 
	XORLW       7
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding12
	MOVF        R1, 0 
	XORLW       8
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding13
	MOVF        R1, 0 
	XORLW       9
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding14
	MOVF        R1, 0 
	XORLW       10
	BTFSC       STATUS+0, 2 
	GOTO        L_RF_Decoding15
L_RF_Decoding5:
;PLC HID Frimware.c,92 :: 		if(fDistance=0x03)
	MOVLW       3
	MOVWF       _fDistance+0 
;PLC HID Frimware.c,94 :: 		Distance=(tDistance2<<4)|(tDistance);
	MOVF        _tDistance2+0, 0 
	MOVWF       _Distance+0 
	RLCF        _Distance+0, 1 
	BCF         _Distance+0, 0 
	RLCF        _Distance+0, 1 
	BCF         _Distance+0, 0 
	RLCF        _Distance+0, 1 
	BCF         _Distance+0, 0 
	RLCF        _Distance+0, 1 
	BCF         _Distance+0, 0 
	MOVF        _tDistance+0, 0 
	IORWF       _Distance+0, 1 
;PLC HID Frimware.c,95 :: 		fDistance=0x00;
	CLRF        _fDistance+0 
;PLC HID Frimware.c,97 :: 		if(fAngle=0x03)
	MOVLW       3
	MOVWF       _fAngle+0 
;PLC HID Frimware.c,99 :: 		Angle=(tAngle2<<4)|(tAngle);
	MOVF        _tAngle2+0, 0 
	MOVWF       _Angle+0 
	RLCF        _Angle+0, 1 
	BCF         _Angle+0, 0 
	RLCF        _Angle+0, 1 
	BCF         _Angle+0, 0 
	RLCF        _Angle+0, 1 
	BCF         _Angle+0, 0 
	RLCF        _Angle+0, 1 
	BCF         _Angle+0, 0 
	MOVF        _tAngle+0, 0 
	IORWF       _Angle+0, 1 
;PLC HID Frimware.c,100 :: 		fAngle=0x00;
	CLRF        _fAngle+0 
;PLC HID Frimware.c,102 :: 		WriteBuff[1] = Distance;
	MOVF        _Distance+0, 0 
	MOVWF       1347 
;PLC HID Frimware.c,103 :: 		WriteBuff[2] = Angle;
	MOVF        _Angle+0, 0 
	MOVWF       1348 
;PLC HID Frimware.c,104 :: 		WriteBuff[3] = AxisX;
	MOVF        _AxisX+0, 0 
	MOVWF       1349 
;PLC HID Frimware.c,105 :: 		WriteBuff[4] = AxisY;
	MOVF        _AxisY+0, 0 
	MOVWF       1350 
;PLC HID Frimware.c,106 :: 		WriteBuff[5] = AxisZ;
	MOVF        _AxisZ+0, 0 
	MOVWF       1351 
;PLC HID Frimware.c,107 :: 		}
L_end_RF_Decoding:
	RETURN      0
; end of _RF_Decoding

_interrupt:

;PLC HID Frimware.c,109 :: 		void interrupt(){
;PLC HID Frimware.c,110 :: 		USB_Interrupt_Proc();                   // USB servicing is done inside the interrupt
	CALL        _USB_Interrupt_Proc+0, 0
;PLC HID Frimware.c,111 :: 		}
L_end_interrupt:
L__interrupt29:
	RETFIE      1
; end of _interrupt

_main:

;PLC HID Frimware.c,113 :: 		void main(void){
;PLC HID Frimware.c,114 :: 		ADCON1 |= 0x0F;                         // Configure all ports with analog function as digital
	MOVLW       15
	IORWF       ADCON1+0, 1 
;PLC HID Frimware.c,115 :: 		CMCON  |= 7;                            // Disable comparators
	MOVLW       7
	IORWF       CMCON+0, 1 
;PLC HID Frimware.c,116 :: 		TRISD = 0xFF;
	MOVLW       255
	MOVWF       TRISD+0 
;PLC HID Frimware.c,117 :: 		TRISB = 0x00;
	CLRF        TRISB+0 
;PLC HID Frimware.c,118 :: 		CurrentMode = SIMPLE_PLC_MODE;
	CLRF        _CurrentMode+0 
;PLC HID Frimware.c,119 :: 		HID_Enable(&ReadBuff,&WriteBuff);       // Enable HID communication
	MOVLW       _ReadBuff+0
	MOVWF       FARG_HID_Enable_readbuff+0 
	MOVLW       hi_addr(_ReadBuff+0)
	MOVWF       FARG_HID_Enable_readbuff+1 
	MOVLW       _WriteBuff+0
	MOVWF       FARG_HID_Enable_writebuff+0 
	MOVLW       hi_addr(_WriteBuff+0)
	MOVWF       FARG_HID_Enable_writebuff+1 
	CALL        _HID_Enable+0, 0
;PLC HID Frimware.c,121 :: 		while(1){
L_main18:
;PLC HID Frimware.c,124 :: 		while(!HID_Read());
L_main20:
	CALL        _HID_Read+0, 0
	MOVF        R0, 1 
	BTFSS       STATUS+0, 2 
	GOTO        L_main21
	GOTO        L_main20
L_main21:
;PLC HID Frimware.c,125 :: 		PORTB = ReadBuff[0];
	MOVF        1280, 0 
	MOVWF       PORTB+0 
;PLC HID Frimware.c,126 :: 		CurrentMode = ReadBuff[63];
	MOVF        1343, 0 
	MOVWF       _CurrentMode+0 
;PLC HID Frimware.c,129 :: 		if(SIMPLE_ASME_DRIVER)RF_Decoding();
	CALL        _RF_Decoding+0, 0
;PLC HID Frimware.c,130 :: 		WriteBuff[0]=PORTD;
	MOVF        PORTD+0, 0 
	MOVWF       1346 
;PLC HID Frimware.c,132 :: 		while(!HID_Write(&WriteBuff,64));
L_main23:
	MOVLW       _WriteBuff+0
	MOVWF       FARG_HID_Write_writebuff+0 
	MOVLW       hi_addr(_WriteBuff+0)
	MOVWF       FARG_HID_Write_writebuff+1 
	MOVLW       64
	MOVWF       FARG_HID_Write_len+0 
	CALL        _HID_Write+0, 0
	MOVF        R0, 1 
	BTFSS       STATUS+0, 2 
	GOTO        L_main24
	GOTO        L_main23
L_main24:
;PLC HID Frimware.c,133 :: 		SetMode(CurrentMode);
	MOVF        _CurrentMode+0, 0 
	MOVWF       FARG_SetMode_Mode+0 
	CALL        _SetMode+0, 0
;PLC HID Frimware.c,134 :: 		}
	GOTO        L_main18
;PLC HID Frimware.c,135 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
