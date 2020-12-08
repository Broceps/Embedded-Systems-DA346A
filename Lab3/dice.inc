/*
 * dice.inc
 *
 *  Created: 2020-12-06 17:10:21
 *  Author: Robin
 */ 


;==============================================================================
; Definitions of registers, etc. ("constants")
;==============================================================================

.EQU			ROLL_KEY		 =	0x02				//key for rolling the dice



;==============================================================================
; Roll a Dice
; Rolling a dice by checking if key 2 is pressed, 
; while pressed the dice is rolling and on release 
; the dice will be rolled, printing the result on the LDC
; Uses Registers:
;	R16(TEMP) - for storing and working with values of a dice
;
;==============================================================================
roll_dice:
	LDI TEMP, 6						//Load all the possible values of a dice
	load:
		NOP
		NOP							//Keyboard driver needs two NOP's to read correctly from keyboard
		PUSH TEMP					//Save temp to stack so it doesnt get destroyed in subroutine
		RCALL read_keyboard_dice			//get value from keyboard
		POP TEMP					//Retrieve temp from stack
		CPI RVAL, 50				//check if rolling-key is pressed down (i.e. key 2) with ASCII offset
		BREQ roll					//if still pressed
		RET							//else if key is released
	roll:
		DEC TEMP					//"Rotating" the dice by decrementing the value stored (from 6 -> 1)
		BREQ roll_dice				//If we get to 0, we start again from top and reload the value 6 to the dice
		RJMP load					//Else keep rolling
			