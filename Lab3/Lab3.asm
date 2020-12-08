/*
* Lab3.asm
*
* This is the core file of the project, initializes constants and ports with initial Data Direction
* After initilazations the main loop will be running.
* The main loop is the essential loop in the software that runs the basic logic and calls other subroutines
* This software is used to simulate a Dice-rollin game on the Arduino ATMEGA 2560.
*
* Created: 2020-12-02 20:27:02
* Author : Robin
*/

;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================
	.EQU			RESET		 =	0x0000			; reset vector
	.EQU			PM_START	 =	0x0072			; start of program
	.DEF TEMP = R16
	.DEF COUNTER = R17
	.DEF RVAL = R23 //står 24 i labben men det krockar ju bara med alla subrutiner???

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
	.INCLUDE		"monitor.inc"
	.INCLUDE		"stats.inc"
	.INCLUDE		"dice.inc"
	.INCLUDE		"stat_data.inc"
;==============================================================================
; Basic initializations of stack pointer, pins and starting the main-loop of program
;==============================================================================
init:
	LDI				TEMP,			LOW(RAMEND)		; Set stack pointer
	OUT				SPL,			TEMP				; at the end of RAM.
	LDI				TEMP,			HIGH(RAMEND)
	OUT				SPH,			TEMP
	RCALL			init_pins						; Initialize pins
	RCALL			lcd_init
	RCALL			init_stat
	RCALL			init_monitor
	RJMP			main							; Jump to main

;==============================================================================
; Initialize I/O pins and their initial DDR for reading keyboard
;	[COL 0 = G5, COL 1 = E3, COL 2 = H3, COL 3 = H4]
;	[ROW 0 = F5, ROW 1 = F4, ROW 2 = E4, ROW 3 = E5]
;
; Uses registers:
; R16(TEMP) - values to DDR and PORT of keyboard connected pins
;==============================================================================
init_pins:
	//Initializes pins corresponding ROWS and COLUMNS of the keyboard.

	//COLUMNS = Output, ROWS = Inputs with Pullup
	LDI TEMP, 0b00100000					;COL 0, Pin G5
	OUT DDRG, TEMP
	LDI TEMP, 0b00001000					;COL 1, Pin E3.	 ROW 2-3, Pin E4-E5
	OUT DDRE, TEMP
	LDS TEMP, DDRH							;Special solution for ATMEL with H-register
	ORI TEMP, 0b00001000					;COL 2, PIN H3
	STS DDRH, TEMP
	LDS TEMP, DDRH							;Special solution for ATMEL with H-register
	ORI TEMP, 0b00010000					;COL 3, PIN H4
	STS DDRH, TEMP
	LDS TEMP, PORTH							;Special solution for ATMEL with H-register
	OUT DDRF, TEMP

	//Create Pullup on all ROWS of keyboard 
	SBI PORTF, 5
	SBI PORTF, 4
	SBI PORTE, 4
	SBI PORTE, 5


	RET


;==============================================================================
; Main part of program
; Uses registers:
;	Rnn				xxxx  ÄNDRA HÄR SEN
;==============================================================================
main:
											

	main_loop:
		RCALL lcd_clear_display					//Clear display for new message
		RCALL write_welcome						//Write a welcome-message
		LDI R24, 3								//Wait 3 sec
		RCALL delay_s
		RCALL lcd_clear_display					//Clear display for new message
		LCD_WRITE_STR Str_press_2				//prints "press 2 to roll"
		LCD_WRITE_STR Str_press_3				//prints "press 3 to show stat"
		LCD_WRITE_STR Str_press_8				//prints "press 8 to clear stat"
		LCD_WRITE_STR Str_press_9				//prints "press 9 for monitor"
		
		RCALL read_keyboard

		MOV R24, RVAL			
		CPI RVAL, 50			//is the ASCII value 2 in decimal (since RVAL returns the ASCII character)
		BREQ menu_2				//Check if 2 is pressed

		CPI RVAL, 51			// is the ASCII value 2 in decimal (since RVAL returns the ASCII character)
		BREQ menu_3				//Check if 3 is pressed

		CPI RVAL, 56			// is the ASCII value 2 in decimal (since RVAL returns the ASCII character)
		BREQ menu_8				//Check if 8 is pressed

		CPI RVAL, 57			// is the ASCII value 2 in decimal (since RVAL returns the ASCII character)
		BREQ menu_9				//Check if 9 is pressed

		RJMP main_loop	



		//KEY 2 pressed							
		menu_2:	
			RCALL lcd_clear_display				//Clear display for new message
			LCD_WRITE_STR Str_rolling			//prints "rolling" while key is held down

			RCALL roll_dice
			MOV RVAL, TEMP						//stores the throw
			PUSH TEMP							//Save temp to stack
			RCALL store_stat					//to the RAM memory
			RCALL lcd_clear_display				//Clear display for new message
			LCD_WRITE_STR Str_value				//prints value (of rolled dice)	
			POP TEMP							//retrieve temp

			SUBI TEMP, -48						//To offset in ASCII	
			MOV R24, TEMP	
			RCALL lcd_write_char				//value of dice

			LDI R24, 1							//wait 5 sec before going back to main menu
			RCALL delay_s

			RJMP main_loop

		//KEY 3 pressed
		menu_3:
			RCALL showstat
			RJMP main_loop
		//KEY 8 pressed
		menu_8:
			RCALL clear_stat
			RJMP main_loop
		//KEY 9 pressed
		menu_9:
			RCALL monitor
			RJMP main_loop