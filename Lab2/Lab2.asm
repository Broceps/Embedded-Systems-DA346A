;
; Lab2.asm
;
; Created: 2020-11-26 16:49:25
; Author : Robin
;


;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU			RESET		 =	0x0000			; reset vector
	.EQU			PM_START	 =	0x0072			; start of program
	.DEF TEMP = R16
	.DEF COUNTER = R17

;==============================================================================
; Start of program
;==============================================================================
	.CSEG
	.ORG			RESET
	RJMP			init

	.ORG			PM_START
	.INCLUDE		"keyboard.inc"
	.INCLUDE		"delay.inc"
	.INCLUDE		"lcd.inc"
;==============================================================================
; Basic initializations of stack pointer, etc.
;==============================================================================
init:
	LDI				TEMP,			LOW(RAMEND)		; Set stack pointer
	OUT				SPL,			TEMP				; at the end of RAM.
	LDI				TEMP,			HIGH(RAMEND)
	OUT				SPH,			TEMP
	RCALL			init_pins						; Initialize pins
	RCALL			lcd_init
	RJMP			main							; Jump to main

;==============================================================================
; Initialize I/O pins
;==============================================================================
init_pins:
	; PORT C
	; output:	7
	LDI				TEMP,			0x80
	OUT				DDRB,			TEMP

	LDI R20, 0xF ;Laddar in hex "F" (decimal = 15) för att addressera pins D0..D3 (1111)
	OUT DDRD, R20 ;Tilldelar Data Direction som 1 för dessa pins (D0...D3)



	//Initerar pins som motsvarar Tangentbordets kolumner och rader. Använder R16(TEMP) för att temporärt lagra 
	//värdet av varje PIN
	//[COL 0 = G5, COL 1 = E3, COL 2 = H3, COL 3 = H4]
	//[ROW 0 = F5, ROW 1 = F4, ROW 2 = E4, ROW 3 = E5]
	//COLUMNS = Utgångar, ROWS = Ingångar med Pullup-motstånd
	LDI TEMP, 0b00100000					;COL 0, Pin G5
	OUT DDRG, TEMP
	LDI TEMP, 0b00001000					;COL 1, Pin E3.	 ROW 2-3, Pin E4-E5
	OUT DDRE, TEMP
	LDS TEMP, DDRH							;speciellt för ATMEL med H-register
	ORI TEMP, 0b00001000					;COL 2, PIN H3
	STS DDRH, TEMP
	LDS TEMP, DDRH							;speciellt för ATMEL med H-register
	ORI TEMP, 0b00010000					;COL 3, PIN H4
	STS DDRH, TEMP
	LDS TEMP, PORTH							;speciellt för ATMEL med H-register
	OUT DDRF, TEMP
	//Sätt Pullup motstånd på alla ROWS 
	SBI PORTF, 5
	SBI PORTF, 4
	SBI PORTE, 4
	SBI PORTE, 5
	RET

;==============================================================================
; Main part of program
; Uses registers:
;	Rnn				xxxx
;==============================================================================
main:
											//Routine for blinking led with 1 sec between
	RCALL lcd_clear_display					//Clear LCD on reset
											
											//Print "KEY:" on first row of LCD
	LCD_WRITE_CMD	0x80	; Set position: col 0
	LCD_WRITE_CMD	0x40	; row 0
	LCD_WRITE_CHR	'K'
	LCD_WRITE_CHR	'E'
	LCD_WRITE_CHR	'Y'
	LCD_WRITE_CHR	':'
	LCD_WRITE_CMD	0x80	; Set position: col 0
	LCD_WRITE_CMD	0x41	; row 1


											// Write from keyboard to display using ASCII, 
	LDI R26, 0x0							//Initialize to 0, will be compared to check if theres new val from keyboard
	main_loop:
		RCALL read_keyboard_num
		CP R26, COUNTER						// Checks if no value is registered from keyboard
		BREQ main_loop						// if key same as previous, wait for different key
		CPI COUNTER, 0x10					// Check if NO_KEY pressed
		BREQ main_loop						// and jump if true else proceed

		MOV R26, COUNTER					// Copy last key to be compered as last key for next loop iteration
		LDI R18, 0x39						// Used to be compared with keyboard val to see if keyboard val is > 9 (in ASCII)
		LDI TEMP, 0x30						// 0 in ASCII starts on 48 decimal (0x30) 
		ADD TEMP, COUNTER					// by adding we can translate to ASCII starting from 0 based on keyboard values


		CP R18, TEMP						// if keyboard value in ASCII is > than 9 (0x39) we need to increment it so we
		BRCS inc_counter					// can skip characters until we have A in ASCII
		back_from_increment:
											//Since we cant load macro with register, I just copy whats in the LCD_WRITE_CHR macro
		SBI	PORTB, 4						// CLear D/C pin
		MOV R24, TEMP						// Loads parameter with ASCII value to be used in lcd_write_char
		RCALL lcd_write_char				// and call the subroutine 

	RJMP main_loop								

	inc_counter:								// To map values greater than 9 to A-F in ASCII
		SUBI TEMP, -7							// Increment with 7 so we get to (decimal) 65 (A in ASCII)
		RJMP back_from_increment				// jump back to where we left to increment
