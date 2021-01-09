/*
 * delay_ams.s
 *
 * This file contains delay routines to be called from other parts of the program.
 * The delay routines are essentially built from NOPs (No Operations) taking account
 * of the sum of operations in each subroutine to get CPU busy as close to 16 cycles 
 * per given microsecond of delay as possible. 
 *
 *		Using Registers R18,R19 and R24
 *
 *
 * Author:	Robin Andersson
 *
 * Date:	2020-12-16
 */ 

.GLOBAL delay_1_micros
.GLOBAL delay_micros
.GLOBAL delay_ms
.GLOBAL delay_1_s
.GLOBAL delay_s

;==============================================================================
; Delay of 1 µs (including RCALL)
;==============================================================================
delay_1_micros:
	NOP			
	NOP			// add a number of NOP instructions to make the delay 1us. CPU is 16Mhz
	NOP			// needing a totalt of 16 instructions including NOP to delay the CPU for 1 micro second
	NOP
	NOP
	NOP
	NOP
	NOP			//total of 8 NOP because 4+4+8 = 16 cycles

	RET

;==============================================================================
; Delay of X µs
;	LDI + RCALL = 4 cycles
;==============================================================================
delay_micros:
	Loop_micros:					// start label to jump to in loop
		NOP							; 13 NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP							
		NOP	
		NOP															
												
		DEC R24						// Will Loop x-times				;1 cycle
		BRNE Loop_micros			//									;2 if jump, else 1 cycle
	RET

;==============================================================================
; Delay of X ms
;	LDI + RCALL = 4 cycles
;==============================================================================
delay_ms:
	MOV R18, R24					//Do the outer loop x times as specified in input parameter R24
	L1_ms:
		LDI R24, 0xFA				//calling delay_micros with parameter 250, with a total of 4 times to get 1 ms
		RCALL delay_micros			;(16*250)+4 cycles
		LDI R24, 0xFA				
		RCALL delay_micros			;(16*250)+4 cycles
		LDI R24, 0xFA				
		RCALL delay_micros			;(16*250)+4 cycles
		LDI R24, 0xFA				
		RCALL delay_micros			;(16*250)+4 cycles
		DEC R18
		BRNE L1_ms
	RET

;==============================================================================
; Delay of 1 second
;==============================================================================
delay_1_s:						
	
	LDI R24, 0xFA			//calling delay_ms with parameter 250, with a total of 4 times to get 1 s
	RCALL delay_ms
	LDI R24, 0xFA			
	RCALL delay_ms
	LDI R24, 0xFA			
	RCALL delay_ms
	LDI R24, 0xFA			
	RCALL delay_ms
	RET



;==============================================================================
; Delay for x seconds
;==============================================================================
delay_s:					//Calls delay_1_s x times
	MOV R19, R24
	L1_s:
		RCALL delay_1_s
		DEC R19
		BRNE L1_s
	RET




