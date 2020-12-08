/*
 * keyboard.inc
 *
 * This file contains a driver for reading the keypad on the
 * "iBridge keypad shield". The driver is adapted for a shield mounted
 * on a Arduino Mega board (ATmega2560).
 *
 * Key mapping:
 *					-----------------   ------------------
 *					| 1 | 2 | 3 | A |   | 0 | 4 | 8  | 12 |
 *					-----------------   ------------------
 *					| 4 | 5 | 6 | B |   | 1 | 5 | 9  | 13 |
 *					-----------------   ------------------
 *					| 7 | 8 | 9 | C |   | 2 | 6 | 10 | 14 |
 *					-----------------   ------------------
 *					| * | 0 | # | D |   | 3 | 7 | 11 | 15 |
 *					-----------------   ------------------
 *
 * Created by N.N, xx Nov 2015, for the course DA346A at Malmo University.
 * Edited: 2020-12-03
 * Editor: Robin Andersson
 */

;==============================================================================
; Table of characters, for mapping with keys
;==============================================================================
keys:	
	.DB				"147*2580369#ABCD", NO_KEY, 0
	//EJ SÄKER PÅ OM DETTA ÄR RÄTT om rätt så är det för att vi går rad för rad i varje kolumn


;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU			NO_KEY		=	0x10 ;no key was pressed!


;==============================================================================
; Read keyboard
; Returns the corresponding character for the pressed key. If no key is
; pressed, then '\0' (null) is returned.
;
; Uses registers:
;	R19			Counter and contains key when returning
;
; Important! A minimum delay of 2 NOP's is necessary after setting port,
; before pin can be read.
;==============================================================================
read_keyboard:

	RCALL read_keyboard_num
	//LOGIC CHECK
	CP R26, RVAL						// Checks if same value is registered from keyboard
	BREQ read_keyboard					// if key same as previous, wait for different key (or key_release)
	LDI R26, NO_KEY

	CPI RVAL, NO_KEY					// Check if NO_KEY pressed
	BREQ read_keyboard					// and jump if true else proceed
	MOV R26, RVAL						// set recieved value as latest value
	//END OF LOGIC CHECK

	LDI ZH, HIGH(keys <<1)
	LDI ZL, LOW(keys <<1)
	ADD ZL, RVAL
	LDI RVAL, 0x00
	ADC ZH, RVAL
	LPM RVAL, Z
	
	RET




read_keyboard_dice:
	RCALL read_keyboard_num
	//LOGIC CHECK
	CPI RVAL, NO_KEY					// Check if NO_KEY pressed
	BREQ read_keyboard_dice						// and jump if true else proceed
	MOV R26, RVAL					// set recieved value as latest value
	//END OF LOGIC CHECK

	LDI ZH, HIGH(keys <<1)
	LDI ZL, LOW(keys <<1)
	ADD ZL, RVAL
	LDI RVAL, 0x00
	ADC ZH, RVAL
	LPM RVAL, Z
	
	RET

;==============================================================================
; Read keyboard
; Returns the corresponding number (0-15) of the key pressed.
; Includes code to handle key bounce.
; If no key is pressed, 16 is returned.
; Uses Registers:
;	R24(RVAL) for returning key-value
;	R16(TEMP) for mapping pins and ports
;==============================================================================
read_keyboard_num:

LDI RVAL, 0x0						; Initiate RVAL to 0
//COLUMN 0 --------------
SBI PORTG, 5						; Set Col0=1
CBI PORTE, 3						; Set Col1=0
LDS TEMP, PORTH						; speciellt för ATMEL med H-register
ANDI TEMP, 0b11110111				; Set Col 2 = 0
STS PORTH, TEMP
LDS TEMP, PORTH						; speciellt för ATMEL med H-register
ANDI TEMP, 0b11101111				; Set Col 3 = 0
STS PORTH, TEMP
LDI R24, 0x05						; delay 5ms before reading port
RCALL delay_ms
//ROW 0
SBIC PINF, 5						; Test Row 0, skip if=0
RJMP END							; Row 0=1, jump to end
INC RVAL							; Increment RVAL
//ROW 1
SBIC PINF, 4						; Test Row 1, skip if=0
RJMP END							; Row 1=1, jump to end
INC RVAL							; Increment RVAL
//ROW 2
SBIC PINE, 4						; Test Row 2, skip if=0
JMP END								; Row 2=1, jump to end
INC RVAL							; Increment RVAL
//ROW 3
SBIC PINE, 5						; Test Row 3, skip if=0
JMP END								; Row 3=1, jump to end
INC RVAL							; Increment RVAL

//COLUMN 1 --------------
CBI PORTG, 5						; Set Col0=0
SBI PORTE, 3						; Set Col1=1
ANDI TEMP, 0b11110111				; Set Col 2 = 0
STS PORTH, TEMP
LDS TEMP, PORTH						; speciellt för ATMEL med H-register
ANDI TEMP, 0b11101111				; Set Col 3 = 0
STS PORTH, TEMP
LDI R24, 0x05						; delay 5ms before reading port
RCALL delay_ms
//ROW 0
SBIC PINF, 5						; Test Row 0, skip if=0
RJMP END							; Row 0=1, jump to end
INC RVAL							; Increment RVAL
//ROW 1
SBIC PINF, 4						; Test Row 1, skip if=0
RJMP END							; Row 1=1, jump to end
INC RVAL							; Increment RVAL
//ROW 2
SBIC PINE, 4						; Test Row 2, skip if=0
JMP END								; Row 2=1, jump to end
INC RVAL							; Increment RVAL
//ROW 3
SBIC PINE, 5						; Test Row 3, skip if=0
JMP END								; Row 3=1, jump to end
INC RVAL							; Increment RVAL

//COLUMN 2 --------------
CBI PORTG, 5						; Set Col0=0
CBI PORTE, 3						; Set Col1=0
LDS TEMP, PORTH						; speciellt för ATMEL med H-register
ORI TEMP, 0b00001000				; Set Col 2=1
STS PORTH, TEMP
LDS TEMP, PORTH						; speciellt för ATMEL med H-register
ANDI TEMP, 0b11101111				; Set Col 3=0
STS PORTH, TEMP
LDI R24, 0x05						; delay 5ms before reading port
RCALL delay_ms
//ROW 0
SBIC PINF, 5						; Test Row 0, skip if=0
RJMP END							; Row 0=1, jump to end
INC RVAL							; Increment RVAL
//ROW 1
SBIC PINF, 4						; Test Row 1, skip if=0
RJMP END							; Row 1=1, jump to end
INC RVAL							; Increment RVAL
//ROW 2
SBIC PINE, 4						; Test Row 2, skip if=0
JMP END								; Row 2=1, jump to end
INC RVAL							; Increment RVAL
//ROW 3
SBIC PINE, 5						; Test Row 3, skip if=0
JMP END								; Row 3=1, jump to end
INC RVAL							; Increment RVAL

//COLUMN 3 --------------
CBI PORTG, 5						; Set Col0=0
CBI PORTE, 3						; Set Col1=0
LDS TEMP, PORTH						; speciellt för ATMEL med H-register
ANDI TEMP, 0b11110111				; Set Col 2=0		
STS PORTH, TEMP
LDS TEMP, PORTH						; speciellt för ATMEL med H-register
ORI TEMP, 0b00010000				; Set Col 3=1
STS PORTH, TEMP
LDI R24, 0x05						; delay 5ms before reading port
RCALL delay_ms
//ROW 0
SBIC PINF, 5						; Test Row 0, skip if=0
RJMP END							; Row 0=1, jump to end
INC RVAL							; Increment RVAL
//ROW 1
SBIC PINF, 4						; Test Row 1, skip if=0
RJMP END							; Row 1=1, jump to end
INC RVAL							; Increment RVAL
//ROW 2
SBIC PINE, 4						; Test Row 2, skip if=0
JMP END								; Row 2=1, jump to end
INC RVAL							; Increment RVAL
//ROW 3
SBIC PINE, 5						; Test Row 3, skip if=0
JMP END								; Row 3=1, jump to end
INC RVAL							; Increment RVAL

						; If you reach this point, RVAL should equal NO_KEY
END: 					; You may come here through a jump statement above
RET						; return with correct value in RVAL
