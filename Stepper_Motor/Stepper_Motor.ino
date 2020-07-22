//www.elegoo.com
//2016.12.12

#include "Stepper.h"


/*----- Variables, Pins -----*/
#define STEPS  200  // Number of steps per revolution of Internal shaft
int  Steps2Take;  // 2048 = 1 Revolution

int i;
/*-----( Declare objects )-----*/
// Setup of proper sequencing for Motor Driver Pins
// In1, In2, In3, In4 in the sequence 1-3-2-4

Stepper small_stepper(STEPS, 8,10,9,11);

void setup()
{ 
 // Start Serial
 Serial.begin(9600);

}

void loop()
{
// VOL+ button pressed
Serial.println("Clockwise");
for (i=0; i<36; i++)
{
                      yield();
                      small_stepper.setSpeed(2 0); //Max seems to be 500
                      Steps2Take  =  200;  // Rotate CW
                      small_stepper.step(Steps2Take);
                      yield();
}

                 digitalWrite(8, LOW);
                 digitalWrite(10, LOW);
                 digitalWrite(9, LOW);
                 digitalWrite(11, LOW); 
                    
                      yield();
//                      digitalWrite(4,LOW);
//                      yield();

}
