/**
* @file main.cpp
* @brief this main file contains the code to 
* run the buttons, interrupts, analog parts of module 2.
*
* @author Dhruva Koley
*
* @date 10/26/2020
*/
#include "mbed.h"
#include "pindef.h"

// preprocessor defines to enable/disable parts of the program
#define MOD2EX1 1   // buttons
#define MOD2EX2 0   // interrupts
#define MOD2EX3 0   // analog

// Serial tx, rx connected to the PC via an USB cable
Serial device(UART_TX, UART_RX);

// set up bus variables
BusOut LEDS(LED1, LED_b);   // LD2, PA_9
BusIn buttons(PA_10, PB_3, PB_5, PB_4);

// set up analog variables
PwmOut buzzer(PB_10);
AnalogIn volume(PA_0);
AnalogIn pitch(PA_1);

#if MOD2EX2
// set up button interrupts
InterruptIn button_press1(PA_10);
InterruptIn button_press2(PB_3);
InterruptIn button_press3(PB_5);
InterruptIn button_press4(PB_4);

/**
* This ISR function is used to turn on LD2.
* @author Dhruva Koley
* @date 10/26/2020
*/
void button_press1_ISR() {
    LEDS[0] = 1;    // turn on LD2
}

/**
* This ISR function is used to turn off LD2.
* @author Dhruva Koley
* @date 10/26/2020
*/
void button_press2_ISR() {
    LEDS[0] = 0;    // turn off LD2
}

/**
* This ISR function is used to turn on PA_9.
* @author Dhruva Koley
* @date 10/26/2020
*/
void button_press3_ISR() {
    LEDS[1] = 1;    // turn on PA_9
}

/**
* This ISR function is used to turn off PA_9.
* @author Dhruva Koley
* @date 10/26/2020
*/
void button_press4_ISR() {
    LEDS[1] = 0;    // turn off PA_9
}
#endif

#if MOD2EX3
/**
* This function is used to map analog pitch values to scale
* between 125 uS and 3125 uS.
* @author Dhruva Koley
* @date 10/26/2020
*/
float map(float inVal, float inMin, float inMax, float outMin, float outMax) {
  // check if inVal is within the range
  if (inMin < inMax) {
    if (inVal <= inMin) 
      return outMin;
    if (inVal >= inMax)
      return outMax;
  } else {
    if (inVal >= inMin) 
      return outMin;
    if (inVal <= inMax)
      return outMax;
  }
  float scale = (inVal - inMin) / (inMax - inMin);
  return outMin + scale * (outMax - outMin);
}

/**
* This function is used to update the pitch and volume
* values read by the potentiometers.
* @author Dhruva Koley
* @date 10/26/2020
*/
void update(float *vert, int16_t *horiz) {
    if(volume.read() != 0 && volume.read() < 1.0) {
        *vert = volume.read();
        device.printf("Volume changed to %0.2f\r\n", *vert);
    }
    if(pitch.read() != 0 && pitch.read() < 1.0) {
        *horiz = map(pitch.read(), 0.0, 1.0, 125, 3125);
        device.printf("Period changed to %d uS\r\n", *horiz);
    }
}

/**
* This function is used to create sawtooth wave PWM samples
* to write to the buzzer.
* @author Dhruva Koley
* @date 10/26/2020
*/
void sawTooth() {
    static float height = 1.0; // full volume
    static float slope, i;
    static int16_t width = 125; // 125 uS period
    slope = height / width; // get initial slope of the sawtooth wave
    while(1) {
        update(&height, &width);    // update height (volume) and width (pitch)
        slope = height / width;
        for(i = 0; i < width; i += 0.05) {
            buzzer.write(i * slope);    // write PWM to buzzer. LPF to form actual sawtooth wave
        }
    }
}
#endif

/**
* This function is used to run the button, interrupts, analog
* portions of module 2.
* @author Dhruva Koley
* @date 10/26/2020
*/
int main() {
    device.printf("Hello mbed\r\n");
#if MOD2EX2
    // attach interrupts to ISR
	button_press1.rise(&button_press1_ISR);
    button_press2.rise(&button_press2_ISR);
    button_press3.rise(&button_press3_ISR);
    button_press4.rise(&button_press4_ISR);
#endif
	while(1) {
#if MOD2EX2
        __wfi();
#endif
#if MOD2EX3
        sawTooth();
#endif
#if MOD2EX1
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
	}
}
