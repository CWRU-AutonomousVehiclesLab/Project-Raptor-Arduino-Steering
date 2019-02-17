#include <Servo.h>
#define SERIAL_PORT_SPEED 115200  // Serial Speed DO NOT CHANGE!
#define MESSAGE_LENGTH 4

//! Pin Setup:
#define Servo_OUTPUT 22
#define ADAS_STEERING_OUTPUT 23

//! Servo Definition:
#define STEERING_MIN 700
#define STEERING_MAX 2300
#define ADAS_MIN 0  //? Need fix
#define ADAS_MAX 0  //? Need fix

//! Message Decoding:
#define STATE 0     // DO NOT CHANGE!
#define STEERING 1  // DO NOT CHANGE!
#define THROTTLE 2  // DO NOT CHANGE!

//! Global Variables
volatile uint8_t messagein[MESSAGE_LENGTH];

//! Global Struct
Servo steering;
Servo ADAS_steering;

void setup() {
    Serial.begin(SERIAL_PORT_SPEED);
    Serial3.begin(SERIAL_PORT_SPEED);
    steering.attach(Servo_OUTPUT, STEERING_MIN, STEERING_MAX);
    ADAS_steering.attach(ADAS_STEERING_OUTPUT);
}
//! Used for convering the pulse back to the steering angle
int pulse2percentage() {
    //* map is used for easier converting 1ms~2ms for remote to 700us~2300us for
    // servo map(value, fromLow, fromHigh, toLow, toHigh)
    int temp = messagein[STEERING];
    if (temp > 100) {
        temp = (temp - 100) * -1;
        int val = map(temp, -100, 100, 700, 2300);
        Serial.println(val);
        return val;
    } else if (temp >= 200 || temp < 0) {
        return 0;
    } else {
        int val = map(temp, -100, 100, 700, 2300);
        Serial.println(val);
        return val;
    }
}
void print_recieved_message() {
    Serial.print("State:");
    Serial.print((int)messagein[STATE]);
    Serial.print("\t");
    Serial.print("STEERING:");
    Serial.print((int)messagein[STEERING]);
    Serial.print("\t");
    Serial.print("THROTTLE:");
    Serial.print((int)messagein[THROTTLE]);
    Serial.print("\t");
    Serial.print("Command:");
    Serial.print(pulse2percentage());
    Serial.println("\t");
}

void SerialInterpretation() {
    if (Serial3.available() == MESSAGE_LENGTH) {
        Serial.println("Recieved...");
        for (int counter = 0; counter < MESSAGE_LENGTH; counter++) {
            messagein[counter] = Serial3.read();
        }
    } else {
        Serial3.flush();
    }
    print_recieved_message();
}
void loop() {
    SerialInterpretation();
    int ServoDeg = pulse2percentage();
    steering.write(ServoDeg);
}
