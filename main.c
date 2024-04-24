/*
 * GccApplication1.c
 *
 * Created: 4/21/2024 2:19:26 PM
 * Author : Peyton M
 */ 

#include <avr/io.h>

#define longDelayLoad -128 // Scaled by 1024. Must be run 13 times to achieve the desired .2 second delay
#define shortDelayLoad -79 // Scaled by 64


void shortDelayA()  {
	TCNT0 = longDelayLoad;
	TCCR0A = 0b00000000;
	TCCR0B = 0b00000011;
	while(!(TIFR0 & (1<<TOV0)));
	TCCR0B = 0x00;
	TIFR0 = 1<<TOV0;
}
void shortDelayB()  {
	TCNT0 = shortDelayLoad;
	TCCR0A = 0b00000000;
	TCCR0B = 0b00000011;
	while(!(TIFR0 & (1<<TOV0)));
	TCCR0B = 0x00;
	TIFR0 = 1<<TOV0;
}
//didn't really do a calculation just picked a number
void shortDelayC()  {
	TCNT0 = -250;	
	TCCR0A = 0b00000000;
	TCCR0B = 0b00000011;
	while(!(TIFR0 & (1<<TOV0)));
	TCCR0B = 0x00;
	TIFR0 = 1<<TOV0;
}

void soundA() {// i used this to fill out the playSoundB and playSoundC functions 
				  //this could be merged into playSoundA
	
	for (unsigned char i = 0; i < 4; i++){
		for (unsigned char i = 0; i < 255; i++) { //Toggle PORTE5 100 times(4*255 times)
			PORTE = PORTE ^ 0b00010000;
			shortDelayA();
		}
	}
}

void soundB() {
	for (unsigned char i = 0; i < 7; i++){
		for (unsigned char i = 0; i < 236; i++) { //Toggle PORTE5 1652 times
			PORTE = PORTE ^ 0b00010000;
			shortDelayB();
		}
	}
}
void soundC() {
	for (unsigned char i = 0; i < 6; i++){
		for (unsigned char i = 0; i < 87; i++) { //Toggle PORTE5 522 times
			PORTE = PORTE ^ 0b00010000;
			shortDelayC();
		}
	}
}

void playsoundA(int songMode){
	//sound A
	PORTD &= 0b11011111;
	soundA();
	if (songMode == 0){
		PORTD |= 0b11101111;
	}
	else{
		PORTD |= 0b11110111;
	}	
}

void playsoundB(int songMode){
	//sound B
	PORTD &= 0b10111111;
	soundB();
	if (songMode == 0){
		PORTD |= 0b11101111;
	}
	else{
		PORTD |= 0b11110111;
	}
}

void playsoundC(int songMode){
	//sound C
	PORTD &= 0b01111111;
	soundC();
	if (songMode == 0){
		PORTD |= 0b11101111;
	}
	else{
		PORTD |= 0b11110111;
	}
}

void keyboardSound(int songMode, char key){
	if(songMode == 0){
		switch(key){
			case 'A':
				playsoundA(songMode);
				break;
			case 'B':
				playsoundB(songMode);
				break;
			case 'C':
				playsoundC(songMode);
				break;
		}
	}else{
		playsoundA(songMode);
		playsoundB(songMode);
		playsoundC(songMode);
		playsoundB(songMode);
		playsoundC(songMode);
		playsoundA(songMode);
		playsoundB(songMode);
		playsoundC(songMode);
		playsoundA(songMode);
	}
}

int main(void) {
	DDRD = DDRD | 0b11111111; 
	DDRE = DDRE | 0b00010000;
	PORTE = PORTE & 0b11101111;
	DDRA = DDRA | 0b00000111; //SW4-7
	PORTA = PORTA | 0b11111000;
	int songMode = 0; //defaults to songMode off
	PORTD = 0b11101111;
    while(1) {
	while (songMode == 0) {
		//PORTD &= 0b11101111;
		if ((PINA & 0b00010000) == 0) {
			PORTD = 0b11110111;
			//PORTA = PORTA | 0b01100000; what does this line do?
			songMode = 1;
		}
		if((PINA & 0b00100000) == 0) {
			
			keyboardSound(songMode, 'A');
		}
		if((PINA & 0b01000000) == 0) {
			keyboardSound(songMode, 'B');
		}
		if((PINA & 0b10000000) == 0){
			keyboardSound(songMode, 'C');
		}
	}
	while (songMode == 1) {
		if ((PINA & 0b00001000) == 0) {
			PORTD = 0b11101111;
			songMode = 0;
		}
		if((PINA & 0b00100000) == 0) {
			keyboardSound(songMode, 'C'); //'C' is a default value because 
							     //theres no way to set up a default parameter
		}	
	}
	}
	return 0;
}


