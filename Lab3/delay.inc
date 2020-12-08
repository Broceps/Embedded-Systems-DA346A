/*
 * delay.inc
 *
 * This file contains delay routines to be called from other parts of the program.
 *
 * Created by N.N, xx Nov 2015, for the course DA346A at Malmo University.
 * Edited: 2020-12-03
 * Editor: Robin Andersson
 */

;==============================================================================
; Delay of 1 µs (including RCALL)
; Using 16 clock cycles
;==============================================================================
delay_1_micros:		//RCALL to this subroutine takes 4 cycles
	NOP			
	NOP			// add a number of NOP instructions to make the delay 1us. CPU is 16Mhz
	NOP			// needing a totalt of 16 instructions including NOP to delay the CPU for 1 micro second
	NOP
	NOP
	NOP
	NOP
	NOP			//total of 8 NOP because 4+4+8 = 16 cycles


	RET			//4 cycles
			
;==============================================================================
; Delay of X µs
;	With excess cost of 8 cycles (LDI + RCALL = 4 cycles, RET = 4 cycles)
; Uses registers:
;	R24				Input parameter data (X µs)
;==============================================================================
delay_micros:						// a total of 16 clockcycles each loop iteration
									// calling method will gain a total cycles of (16*x)+8 for RET + LDI+RCALL
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
	RET								// We get here when R24 is == 0		;4 cycles

;==============================================================================
; Delay of X ms
;	With excess cost of 8 cycles (LDI + RCALL = 4 cycles, RET = 4 cycles)
; Uses registers:
;	R18				Copy of parameter data (X ms)
;	R24				Input parameter data (X ms) and
;					also input to 'delay_micros'.
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
; Uses Registers:
;	R24 - Parameter to delay_ms subroutine
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
; Uses Registers:
;	R19  - Copy of parameter data (x seconds)
;	R24  - For Parameter of x seconds
;==============================================================================
delay_s:					//Calls delay_1_s x times
	MOV R19, R24
	L1_s:
		RCALL delay_1_s
		DEC R19
		BRNE L1_s
	RET
		