/*
 * Lab1.asm
 *
 * Created: 2018-11-xx
 *
 * Created by N.N, for the course DA346A at Malmo University.
 */
 
;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU			RESET		 =	0x0000			; reset vector
	.EQU			PM_START	 =	0x0072			; start of program
	.DEF TEMP = R16
	.DEF COUNTER = R18

;==============================================================================
; Start of program
;==============================================================================
	.CSEG
	.ORG			RESET
	RJMP			init

	.ORG			PM_START
	.INCLUDE		"keyboard.inc"

;==============================================================================
; Basic initializations of stack pointer, etc.
;==============================================================================
init:
	LDI				TEMP,			LOW(RAMEND)		; Set stack pointer
	OUT				SPL,			TEMP				; at the end of RAM.
	LDI				TEMP,			HIGH(RAMEND)
	OUT				SPH,			TEMP
	RCALL			init_pins						; Initialize pins
	RJMP			main							; Jump to main

;==============================================================================
; Initialize I/O pins
;==============================================================================
init_pins:
	; PORT C
	; output:	7
	LDI				TEMP,			0x80
	OUT				DDRB,			TEMP

	LDI R20, 0xF ;Laddar in hex "F" (decimal = 15) f�r att addressera pins D0..D3 (1111)
	OUT DDRD, R20 ;Tilldelar Data Direction som 1 f�r dessa pins (D0...D3)



	//Initerar pins som motsvarar Tangentbordets kolumner och rader. Anv�nder R16(TEMP) f�r att tempor�rt lagra 
	//v�rdet av varje PIN
	//[COL 0 = G5, COL 1 = E3, COL 2 = H3, COL 3 = H4]
	//[ROW 0 = F5, ROW 1 = F4, ROW 2 = E4, ROW 3 = E5]
	//COLUMNS = Utg�ngar, ROWS = Ing�ngar med Pullup-motst�nd
	LDI TEMP, 0b00100000					;COL 0, Pin G5
	OUT DDRG, TEMP
	LDI TEMP, 0b00001000					;COL 1, Pin E3.	 ROW 2-3, Pin E4-E5
	OUT DDRE, TEMP
	LDS TEMP, DDRH							;speciellt f�r ATMEL med H-register
	ORI TEMP, 0b00011000					;COL 2-3, PIN H3-H4
	STS DDRH, TEMP
	LDS TEMP, PORTH							;speciellt f�r ATMEL med H-register
	OUT DDRF, TEMP
	//S�tt Pullup motst�nd p� alla ROWS 
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
	;SBI PORTB, 7							; 2 cycles
	;NOP									; 1 cycle
	;CBI PORTB, 7							; 2 cycles
	//S�tter p� respektive pin av PortD f�r att s�nda ut signal till LED
	;SBI PORTD, 3
	;SBI PORTD, 2
	;SBI PORTD, 1
	;SBI PORTD, 0
	
	CALL read_keyboard_num					;Anropa tangentbordsrutinen
	OUT PORTD, COUNTER
	NOP										;Vila 2 cykler
	NOP

	RJMP main								; 2 cycles

