/*----------------------------------------------------------------------------
LAB EXERCISE 11.4- SPI and I2C interface
SERIAL COMMUNICATION
 ----------------------------------------
 Display the temperature from the virtual DS1631 temperature sensor on the 
      virtual LCD
 
 Input: virtual DS1631 temperature sensor
 Output: virtual LCD display
	
	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "/headers/NHD_0216HZ.h"
#include "/headers/DS1631.h"
#include "pindef.h"

#define MOD2EX1 0
#define MOD2EX2 0

// Serial tx, rx connected to the PC via an USB cable
Serial device(UART_TX, UART_RX);

//Define the LCD and the temperature sensor
NHD_0216HZ lcd (SPI_CS, SPI_MOSI, SPI_SCLK);
DS1631 sensor (I2C_SDA, I2C_SCL, 0x01);
//Write your code here
BusOut LEDS(LED1, LED_b);   // LD2, PA_9
BusIn buttons(PA_10, PB_3, PB_5, PB_4);

PwmOut buzzer(PB_10);
AnalogIn volume(PA_0);
AnalogIn pitch(PA_1);

#if MOD2EX2
InterruptIn button_press1(PA_10);
InterruptIn button_press2(PB_3);
InterruptIn button_press3(PB_5);
InterruptIn button_press4(PB_4);

void button_press1_ISR(){
    LEDS[0] = 1;    // turn on LD2
}

void button_press2_ISR(){
    LEDS[0] = 0;    // turn on LD2
}

void button_press3_ISR(){
    LEDS[1] = 1;    // turn on PA_9
}

void button_press4_ISR(){
    LEDS[1] = 0;    // turn off PA_9
}
#endif

float map(float in, float inMin, float inMax, float outMin, float outMax) {
  // check if it's within the range
  if (inMin < inMax) { 
    if (in <= inMin) 
      return outMin;
    if (in >= inMax)
      return outMax;
  } else {
    if (in >= inMin) 
      return outMin;
    if (in <= inMax)
      return outMax;
  }
  float scale = (in - inMin) / (inMax - inMin);
  return outMin + scale * (outMax - outMin);
}

void update(float *vert, int16_t *horiz){
    if(volume.read() != 0 && volume.read() < 1.0){
        *vert = volume.read();
        device.printf("Volume changed to %f\r\n", *vert);
    }
    if(pitch.read() != 0 && pitch.read() < 1.0){
        *horiz = map(pitch.read(), 0.0, 1.0, 125, 3125);
        device.printf("Period changed to %d uS\r\n", *horiz);
    }
}

void sawTooth()
{
    static float height = 1.0, slope, i;
    static int16_t width = 125;
    slope = height / width;
    while(1) {       // thread loop
        update(&height, &width);
        slope = height / width;
        for(i = 0; i < width; i+=0.05) {
            buzzer.write(i * slope);
        }
    }
}

//Define a variable to store temperature measurement
float temp;
/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() {
    device.printf("Hello mbed\r\n");
	//Initialise the LCD
	//Write your code here
#if MOD2EX2
	button_press1.rise(&button_press1_ISR);
    button_press2.rise(&button_press2_ISR);
    button_press3.rise(&button_press3_ISR);
    button_press4.rise(&button_press4_ISR);
#endif
	while(1){
        // __wfi();
        sawTooth();
#if MOD2EX1
        // device.printf("Read: %x\r\n", buttons.read());
        switch (buttons) {
        case 14:    // button 1
            LEDS[0] = 1;    // turn on LD2
            device.printf("Button 1 pressed\r\n");
            break;
        case 13:    // button 2
            LEDS[0] = 0;    // turn LD2 off
            device.printf("Button 2 pressed\r\n");
            break;
        case 11:    // button 3
            LEDS[1] = 1;    // turn PA_9 on
            device.printf("Button 3 pressed\r\n");
            break;
        case 7:    // button 4
            LEDS[1] = 0;    // turn PA_9 off
            device.printf("Button 4 pressed\r\n");
            break;
        default:
            break;
        }
#endif
		/*
		Read the temperature from the DS1631
		Update the LCD with new temperature measurement
		*/
		
		//Write your code here
		// sensor.read();
	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
