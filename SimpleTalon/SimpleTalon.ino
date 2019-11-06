#include <Servo.h>

/*
 * Description:
 * Example for setting the minimal and maximal angle.
 */ 

static const int servoPin = 4; //availible {4, 16, 18, 19, 21}

Servo servo1;

void setup() {
    Serial.begin(115200);
    servo1.attach(
        servoPin, 
        Servo::CHANNEL_NOT_ATTACHED, 
        0,
        180,
        1000,
        2000
    );
}

void loop() {
    servo1.write(0);
    delay(2000);
    servo1.write(90);
    delay(2000);
    servo1.write(180);
    delay(2000);
    servo1.write(90);
    delay(2000);
}
