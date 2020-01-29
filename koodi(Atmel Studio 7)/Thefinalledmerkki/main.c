
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define FFT_N 128  //128, 256 work (but not 64)

#include "ffft.h"


void printLed(int row, int column);
void printColumn(int level);
void handleNewData(uint16_t spectrum[]);
void closeLed();
void ledTest();


// FFT defines and globals
#define ADC_CHANNEL 1
int16_t       capture[FFT_N];    // Audio capture buffer
complex_t     bfly_buff[FFT_N];  // FFT "butterfly" buffer
uint16_t      spectrum[FFT_N/2]; // Spectrum output buffer
volatile uint8_t samplePos = 0;     // Buffer position counter
uint16_t resultSpectrum[8] = {0,0,0,0,0,0,0,0};
uint16_t newData[8] = {0,0,0,0,0,0,0,0};



// Set the ADC up to run in free-running mode
void fftlib_init_adc(void) {
	// Init ADC free-run mode; f = ( 16MHz/prescaler ) / 13 cycles/conversion
	ADMUX  = ADC_CHANNEL; // Channel sel, right-adj, use AREF pin
	ADCSRA = _BV(ADEN)  | // ADC enable
	_BV(ADSC)  | // ADC start
	_BV(ADATE) | // Auto trigger
	_BV(ADIE)  | // Interrupt enable
	_BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); // 128:1 / 13 = 9615 Hz
	ADCSRB = 0;                // Free ru mode, no high MUX bit
	DIDR0  = 1 << ADC_CHANNEL; // Turn off digital input for ADC pin
	TIMSK0 = 0;                // Timer0 off --> this will stop the delay() function from working properly, but doesn't seem to affect the FFT

	sei(); // Enable interrupts
	
	memset(spectrum, 0, sizeof(spectrum));
}

// Audio-sampling interrupt
ISR(ADC_vect) {
	static const int16_t noiseThreshold = 4;
	int16_t sample = ADC; // 0-1023

	capture[samplePos] =
	((sample > (512 - noiseThreshold)) &&
	(sample < (512 + noiseThreshold))) ? 0 :
	sample - 512; // Sign-convert for FFT; -512 to +511

	if(++samplePos >= FFT_N) ADCSRA &= ~_BV(ADIE); // Buffer full, interrupt off
}

int main(void)
{	
	
	// Start the ADC in free running mode
	fftlib_init_adc();
	
	while (1)
	{
		// Compute the FFT
		fft_input(capture, bfly_buff);   // Samples -> complex #s
		samplePos = 0;                   // Reset sample counter
		ADCSRA |= _BV(ADIE);             // Resume sampling interrupt
		fft_execute(bfly_buff);          // Process complex data
		fft_output(bfly_buff, spectrum); // Complex -> spectrum
		
		
		// Print the results+
		handleNewData(spectrum);		//convert the new data to fit 8x8 matrix
		
		for(int j = 0; j < 8; j++){		//drop the level by one if no bigger peak is present in the new data
			if(newData[j] >= resultSpectrum[j]){
				resultSpectrum[j] = newData[j];	
			}
			else if(resultSpectrum[j] != 0){
				resultSpectrum[j] -= 1;
			}

		}
		
		for (int j = 0; j < 2; j++){	//print twice in order to increase brightness
			for (int i = 0; i < 8; i++){
//				ledTest();				//uncomment to enable led test pattern
				printLed(i, resultSpectrum[i]);
			}
		}
	}
}

void printLed(int row, int column){		//handle printing on leds
	if(row == 0){						//decide which row has to be activated
		PORTC |= 0b00010000;
	}
	else if(row == 1){
		PORTC |= 0b00001000;
	}
	else if(row == 2){
		PORTC |= 0b00000100;
	}
	else if(row == 3){
		PORTC |= 0b00100000;
	}
	else if(row == 4){
		PORTD |= 0b00100000;
	}
	else if(row == 5){
		PORTE |= 0b00000100;
	}
	else if(row == 6){
		PORTE |= 0b00000001;
	}
	else if(row == 7){
		PORTC |= 0b00000001;
	}
	printColumn(column);
	_delay_ms(1);
	closeLed();
}


void closeLed(){						//close the ports to avoid multiple rows to be on at the same time
	PORTC &= 0b11000010;
	PORTD &= 0b00000011;
	PORTE &= 0b11111010;
	PORTB &= 0b11111000;
}

void ledTest(){
while(1){
	printLed(0,8);
	_delay_ms(1000);
	printLed(1,8);
	_delay_ms(1000);
	printLed(2,8);
	_delay_ms(1000);
	printLed(3,8);
	_delay_ms(1000);
	printLed(4,8);
	_delay_ms(1000);
	printLed(5,8);
	_delay_ms(1000);
	printLed(6,8);
	_delay_ms(1000);
	printLed(7,8);
	_delay_ms(1000);
}
}

void printColumn(int level){			//open column ports the right amount
	if(level > 0){
		PORTB |= 0b00000100;
		if(level > 1){
			PORTB |= 0b00000010;
				if(level > 2){
					PORTB |= 0b00000001;
						if(level > 3){
							PORTD |= 0b10000000;
								if(level > 4){
									PORTD |= 0b01000000;
										if(level > 5){
											PORTD |= 0b00000100;
												if(level > 6){
													PORTD |= 0b00001000;
														if(level > 7){
															PORTD |= 0b00010000;
														}
												}
										}
								}
						}
				}
		}
	}
}





void handleNewData(uint16_t spectrum[]){
	uint16_t noise[8]={12,4,2,3,1,1,1,2};			//noise level, can be edited to fit better
	int result = 0;
	int helper = 0;
	for(int i = 0; i < 64; i++){
		result += spectrum[i];
		if(((i+1) % 8 == 0) && ((i+1) != 0)){
				helper = (i+1)/8 - 1;
				result /= 8;
					
				newData[helper] = result;
				result = 0;
					
				if(newData[helper] < noise[helper]){
					newData[helper] = 0;
				}
				else{
					newData[helper] = newData[helper] - noise[helper];
				}
				if(newData[helper] > 8){
					newData[helper] = 8;
				}
		}
	}
}