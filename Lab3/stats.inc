/*
 * stats.inc
 *
 *			LÄGG TILL BESKRIVNING
 *
 *  Created: 2016-11-16 20:03:40
 *	Edited: 2020-12-03
 *  Author: Magnus Krampell
 *  Edited: Robin Andersson
 */ 


clearstring:
	.DB "CLEARING...",0 
totalstring:
	.DB "THROWS:",0
countones:
	.DB "NBR OF 1'S:",0
counttwos:
	.DB "NBR OF 2'S:",0
countthrees:
	.DB "NBR OF 3'S:",0
countfours:
	.DB "NBR OF 4'S:",0
countfives:
	.DB "NBR OF 5'S:",0
countsixs:
	.DB "NBR OF 6'S:",0


; init statistics module
init_stat:
	RCALL clear_stat	; clear data structure
	RET

; print content of RVAL on display
; NOTE: had to change input to R24 instead of RVAL because  lcd_write_char uses R24 and not RVAL!!
hexconv: .DB "0123456789ABCDEF"
printhex:
	LDI		ZH, HIGH(hexconv<<1)
	LDI		ZL, LOW(hexconv<<1)
	MOV		R5, RVAL		; save content
	SWAP	RVAL			; 
	ANDI	RVAL, 0x0F		; mask out low nibble
	ADD		ZL, RVAL
	CLR		R6
	ADC		ZH, R6			; handle carry, if needed		
	LPM		R24, Z			; convert number to ASCII
	RCALL lcd_write_char
	//LCD_WRITE_CHR			; write to LCD
	MOV		RVAL, R5		; retrieve content
	ANDI	RVAL, 0x0F		; mask out low nibble
	LDI		ZH, HIGH(hexconv<<1)
	LDI		ZL, LOW(hexconv<<1) ; reset ZL to start
	ADD		ZL, RVAL
	CLR		R6
	ADC		ZH, R6			; handle carry, if needed		
	LPM		R24, Z			; convert number to ASCII
	//LCD_WRITE_CHR			; write to LCD
	RCALL lcd_write_char
	RET

showstat:
	RCALL lcd_clear_display
	LCD_WRITE_STR totalstring
	LDI RVAL, 0
	RCALL get_stat			; get total number of throws	
	RCALL printhex			; print as 2 hex numbers
	RCALL delay_1_s

	RCALL lcd_clear_display
	LCD_WRITE_STR countones
	LDI RVAL, 1
	RCALL get_stat			; get total number of throws	
	RCALL printhex			; print as 2 hex numbers
	RCALL delay_1_s

	RCALL lcd_clear_display
	LCD_WRITE_STR counttwos
	LDI RVAL, 2
	RCALL get_stat			; get total number of throws	
	RCALL printhex			; print as 2 hex numbers
	RCALL delay_1_s

	RCALL lcd_clear_display
	LCD_WRITE_STR countthrees
	LDI RVAL, 3
	RCALL get_stat			; get total number of throws	
	RCALL printhex			; print as 2 hex numbers
	RCALL delay_1_s

	RCALL lcd_clear_display
	LCD_WRITE_STR countfours
	LDI RVAL, 4
	RCALL get_stat			; get total number of throws	
	RCALL printhex			; print as 2 hex numbers
	RCALL delay_1_s

	RCALL lcd_clear_display
	LCD_WRITE_STR countfives
	LDI RVAL, 5
	RCALL get_stat			; get total number of throws	
	RCALL printhex			; print as 2 hex numbers
	RCALL delay_1_s

	RCALL lcd_clear_display
	LCD_WRITE_STR countsixs
	LDI RVAL, 6
	RCALL get_stat			; get total number of throws	
	RCALL printhex			; print as 2 hex numbers
	RCALL delay_1_s

	RCALL lcd_clear_display
	RET

clearstat:
	RCALL lcd_clear_display
	LCD_WRITE_STR clearstring
	RCALL clear_stat
	RET
