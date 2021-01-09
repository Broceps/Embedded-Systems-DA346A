/*
 * Lab4.c
 *
 * Created: 2020-12-16
 *  Author: staff
 * Editor: Robin Andersson
 * Edited: 2020-12-27
 */ 


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>			//rand
#include "lcd/lcd.h"
#include "numkey/numkey.h"
#include "delay/delay.h"
#include "hmi/hmi.h"
#include "guess_nr.h"

// for storage of pressed key
char key;
// for generation of variable string
char str[17];

int main(void)

{
	
	
	uint16_t rnd_nr;
	// initialize HMI (LCD and numeric keyboard)
	hmi_init();
	// generate seed for the pseudo-random number generator
		srand(255);															//random_seed();
	// show start screen for the game
	output_msg("WELCOME!", "LET'S PLAY...", 3);
	// play game
	while (1) {
		// generate a random number
		rnd_nr = rand()  % 101;														//random_get_nr(100) + 1;
		// play a round...
		play_guess_nr(rnd_nr);
	}
	
	
	
/******************************************************************************
	OVANF÷R FINNS HUVUDPROGRAMMET, DET SKA NI INTE MODIFIERA!
	NEDANF÷R KAN NI SKRIVA ERA TESTER. GL÷M INTE ATT PROGRAMMET M?STE HA EN
	OƒNDLIG LOOP I SLUTET!

	NƒR DET ƒR DAGS ATT TESTA HUVUDPROGRAMMET KOMMENTERAR NI UT (ELLER RADERAR)
	ER TESTKOD. GL÷M INTE ATT AVKOMMENTERA HUVUDPROGRAMMET
******************************************************************************/
	//TEST 1
	//DDRD = 0xFF;
	//lcd_init();
	//numkey_init();
	//lcd_clear();
	//lcd_write_str("HELLO");
	//PORTD = 0xFF;
	//delay_s(2);
	//PORTD = 0x0;
	//lcd_clear();
	//while (1){
		//char mychar = numkey_read();
		//if(mychar!= '\0'){
			//lcd_write(CHR,mychar);
		//}
		//
	//}
	
	//TEST 2
	//hmi_init();
	//uint16_t nbr;
	//input_int("WELCOME",&nbr);
	//
	//while(1);
	
		
}