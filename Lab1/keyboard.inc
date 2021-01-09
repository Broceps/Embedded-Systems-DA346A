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
 */

;==============================================================================
; Table of characters, for mapping with keys
;==============================================================================
keys:
	.DB				"XXXXXXXXXXXXXXXX", NO_KEY, 0

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

// write code here to implement function in Lab 2
// hint: use read_keyboard_num and just convert/map to character here
	RET





;==============================================================================
; Read keyboard
; Returns the corresponding number (0-15) of the key pressed.
; Includes code to handle key bounce.
; If no key is pressed, 16 is returned.
;==============================================================================

read_keyboard_num:

// write code here to implement function in Lab 1
//[COL 0 = G5, COL 1 = E3, COL 2 = H3, COL 3 = H4]
//[ROW 0 = F5, ROW 1 = F4, ROW 2 = E4, ROW 3 = E5]

LDI COUNTER, 0x0						; Initiate counter to 0
//COLUMN 0 --------------
SBI PORTG, 5						; Set Col0=1
CBI PORTE, 3						; Set Col1=0
LDS TEMP, PORTH						; speciellt för ATMEL med H-register
ORI TEMP, 0b00000000				; Set Col 2-3 = 0
CBR PORTH, TEMP
NOP									; delay before reading port
//ROW 0
SBIC PINF, 5						; Test Row 0, skip if=0
RJMP END							; Row 0=1, jump to end
INC COUNTER							; Increment counter
//ROW 1
SBIC PINF, 4						; Test Row 1, skip if=0
RJMP END							; Row 1=1, jump to end
INC COUNTER							; Increment counter
//ROW 2
SBIC PINE, 4						; Test Row 2, skip if=0
JMP END								; Row 2=1, jump to end
INC COUNTER							; Increment counter
//ROW 3
SBIC PINE, 5						; Test Row 3, skip if=0
JMP END								; Row 3=1, jump to end
INC COUNTER							; Increment counter

//COLUMN 1 --------------
CBI PORTG, 5						; Set Col0=0
SBI PORTE, 3						; Set Col1=1
;LDS TEMP, PORTH						; speciellt för ATMEL med H-register
;ORI TEMP, 0b00000000				; Set Col 2=0		3=0
;STS PORTH, TEMP
CBR PORTH, 3
CBR PORTH, 4
NOP									; delay before reading port
//ROW 0
SBIC PINF, 5						; Test Row 0, skip if=0
RJMP END							; Row 0=1, jump to end
INC COUNTER							; Increment counter
//ROW 1
SBIC PINF, 4						; Test Row 1, skip if=0
RJMP END							; Row 1=1, jump to end
INC COUNTER							; Increment counter
//ROW 2
SBIC PINE, 4						; Test Row 2, skip if=0
JMP END								; Row 2=1, jump to end
INC COUNTER							; Increment counter
//ROW 3
SBIC PINE, 5						; Test Row 3, skip if=0
JMP END								; Row 3=1, jump to end
INC COUNTER							; Increment counter

//COLUMN 2 --------------
CBI PORTG, 5						; Set Col0=0
CBI PORTE, 3						; Set Col1=0
;LDS TEMP, PORTH						; speciellt för ATMEL med H-register
;ORI TEMP, 0b00001000				; Set Col 2=1		3=0
;STS PORTH, TEMP
SBR PORTH, 3
CBR PORTH, 4
NOP									; delay before reading port
//ROW 0
SBIC PINF, 5						; Test Row 0, skip if=0
RJMP END							; Row 0=1, jump to end
INC COUNTER							; Increment counter
//ROW 1
SBIC PINF, 4						; Test Row 1, skip if=0
RJMP END							; Row 1=1, jump to end
INC COUNTER							; Increment counter
//ROW 2
SBIC PINE, 4						; Test Row 2, skip if=0
JMP END								; Row 2=1, jump to end
INC COUNTER							; Increment counter
//ROW 3
SBIC PINE, 5						; Test Row 3, skip if=0
JMP END								; Row 3=1, jump to end
INC COUNTER							; Increment counter

//COLUMN 3 --------------
CBI PORTG, 5						; Set Col0=0
CBI PORTE, 3						; Set Col1=0
;LDS TEMP, PORTH						; speciellt för ATMEL med H-register
;ORI TEMP, 0b00010000				; Set Col 2=0		3=1
;STS PORTH, TEMP
CBR PORTH, 3
SBR PORTH, 4
NOP									; delay before reading port
//ROW 0
SBIC PINF, 5						; Test Row 0, skip if=0
RJMP END							; Row 0=1, jump to end
INC COUNTER							; Increment counter
//ROW 1
SBIC PINF, 4						; Test Row 1, skip if=0
RJMP END							; Row 1=1, jump to end
INC COUNTER							; Increment counter
//ROW 2
SBIC PINE, 4						; Test Row 2, skip if=0
JMP END								; Row 2=1, jump to end
INC COUNTER							; Increment counter
//ROW 3
SBIC PINE, 5						; Test Row 3, skip if=0
JMP END								; Row 3=1, jump to end
INC COUNTER							; Increment counter


		//continue testing the other columns in the same fashion...

						; If you reach this point, counter should equal NO_KEY
END: 					; You may come here through a jump statement above
RET						; return with correct value in counter
