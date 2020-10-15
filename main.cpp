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

// Serial tx, rx connected to the PC via an USB cable
Serial device(UART_TX, UART_RX);

//Define the LCD and the temperature sensor
NHD_0216HZ lcd (SPI_CS, SPI_MOSI, SPI_SCLK);
DS1631 sensor (I2C_SDA, I2C_SCL, 0x01);
//Write your code here
BusOut LEDS(LED1, LED_b);   // LD2, PA_9
BusIn buttons(PA_10, PB_3, PB_5, PB_4);

//Define a variable to store temperature measurement
float temp;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() {
    device.printf("Hello mbed");
	//Initialise the LCD
	//Write your code here
	
	
	while(1){
        switch (LEDS) {
        case 0b0001:    // button 1
            LEDS = 0b01;    // turn on LD2
            break;
        case 0b0010:    // button 2
            LEDS = 0b00;    // turn LD2 off
            break;
        case 0b0100:    // button 3
            LEDS = 0b10;    // turn PA_9 on
            break;
        case 0b1000:    // button 4
            LEDS = 0b00;    // turn PA_9 off
        }
		/*
		Read the temperature from the DS1631
		Update the LCD with new temperature measurement
		*/
		
		//Write your code here
		sensor.read();
	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
