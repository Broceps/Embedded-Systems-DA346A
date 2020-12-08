/*
 * stat_data.inc
 *
 * Contains references and operations to manipulate statistics from Dice throws. Stores number of throws and occurance
 * of each side of the dice in the RAM memory
 *
 * Created: 2020-12-07 20:29:10
 * Author: Robin Andersson
 */ 

;==============================================================================
; RAM allocation of stats
;==============================================================================
.DSEG									//Wokring with RAM
.ORG 0x200								//Set starting point address of data segment to 0x200
statistical_data:
	.BYTE 7								//index 1-6 of the string is the frequency of each dice throw, index 0 is amount of throws. 
.CSEG									//Go back to working with Program/flash-memory

;==============================================================================
; Operation to store statistics in the RAM-memory
; The function will increment the Total number of throws and the
; number of throws with results equals RVAL.
; Registers used:
;	RVAL - Input parameter containing the dice throw
;	X - Pointer to RAM memory
;	TEMP(R16) - for temporary values
;==============================================================================
store_stat:
	LDI XH, HIGH(statistical_data)	//assign pointer to the RAM memory
	LDI XL, LOW(statistical_data)	//where we store our statistical data

	//First we need to increment the number of throws (at index 0)
	LD TEMP, X						//Load the current value of throws into TEMP (already at index 0, no need for offset)
	INC TEMP						//Increment the value with one
	ST X, TEMP						//And save it

	//Then we need to increment the value of the dice-throw (at index = RVAL)
	ADD XL, RVAL						//Offset of the value of the dice-throw to get to the correct index (1-6)
	LDI RVAL, 0x00
	ADC XH, RVAL
	LD TEMP, X						//Load the current value into TEMP
	INC TEMP						//Increment the value with one
	ST X, TEMP						//And save it

	RET

;==============================================================================
; Operation get statistics from the RAM-memory
; RVAL gives the value for which the result is retrieved. 
; If RVAL = 0, the total number of throws shall be returned. The result is returned in RVAL.
; Registers used:
;	RVAL - Input and Output parameter containing which stat is to be retrieved and returns the value
;	X - Pointer to RAM memory
;==============================================================================
get_stat:
	LDI XH, HIGH(statistical_data)	//assign pointer to the RAM memory
	LDI XL, LOW(statistical_data)	//where we store our statistical data

	ADD XL, RVAL						//Get the offset for the value we want to retrieve
	LDI RVAL, 0x00
	ADC XH, RVAL

	LD RVAL, X						//Load the value into RVAL
	
	RET

;==============================================================================
; Operation clear all the statistics from the RAM-memory
; Clears all data
; Registers used:
;	X - Pointer to RAM memory
;	TEMP(R16) - for temporary values
;==============================================================================
clear_stat:
	LDI XH, HIGH(statistical_data)	//assign pointer to the RAM memory
	LDI XL, LOW(statistical_data)	//where we store our statistical data

	LDI TEMP, 0						//What we will be writing over with to reset memory
	ST X+, TEMP						//Store the reset-value and increment to go through all 7 slots
	ST X+, TEMP
	ST X+, TEMP
	ST X+, TEMP
	ST X+, TEMP
	ST X+, TEMP
	ST X, TEMP

	RET

