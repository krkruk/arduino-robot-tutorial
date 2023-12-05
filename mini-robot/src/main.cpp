#include <Arduino.h>
#include <AccelStepper.h>
#include <arduino-timer.h>

#define BUZZER_PIN 8
#define BUTTON_PIN PIN2
#define SERVO_PIN
#define HALFSTEP 8

static AccelStepper rightWheel(HALFSTEP, A5, A3, A4, A2);
static AccelStepper leftWheel(HALFSTEP, PIN7, PIN5, PIN6, PIN4);
static uint8_t servoPosition {90};
Timer<1> timer;

void initialize_hardware_servo();
void set_servo_pos(uint8_t degree);

void button_event();
bool reset_servo_position(void *);

void setup()
{
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Serial.begin(115200);
    while (!Serial);

    rightWheel.setMaxSpeed(1000.0);
    rightWheel.setAcceleration(2000.0);
    rightWheel.setSpeed(200);
    rightWheel.moveTo(1024);

    leftWheel.setMaxSpeed(1000.0);
    leftWheel.setAcceleration(2000.0);
    leftWheel.setSpeed(200);
    leftWheel.moveTo(-1024);

    initialize_hardware_servo();
    reset_servo_position(&servoPosition);
    delay(2000);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_event, FALLING);
}

static uint8_t state = 0;
void loop()
{
    switch (state)
    {
    case 0:
    case 1:
    case 4:
    {
        if (rightWheel.distanceToGo() == 0) {
            rightWheel.moveTo(-rightWheel.currentPosition());
            leftWheel.moveTo(-leftWheel.currentPosition());
            ++state;
        }
        break;
    }
    case 2: 
    case 5: {
        if (rightWheel.distanceToGo() == 0) {
            rightWheel.moveTo(0);
            leftWheel.moveTo(0);
            ++state;
            delay(1000);
        }
        break;
    }
    case 3: {
        if (rightWheel.distanceToGo() == 0) {
            rightWheel.moveTo(1024);
            leftWheel.moveTo(1024);
            ++state;
        }
        break;
    }
    default:
        state = 0;
        rightWheel.moveTo(1024);
        leftWheel.moveTo(-1024);
        break;
    }

    rightWheel.run();
    leftWheel.run();
    timer.tick();
}

void initialize_hardware_servo() {

    DDRB |= (1<<PB1); // Set PB1 as output
    /*
        Enable PWM on PIN PB1/Arduin D9

        CPU CLK = 16,000,000
        Timer: Phase Correct PWM
        Formula: fOCnxPCPWM = fclk_I/O / {2* N * TOP}
        TOP: ICR1: 50Hz
        Prescaler: 8
    */
    TCCR1A = (1<<COM1A1) | (1<<WGM11);
    TCCR1B = (1 << WGM13) | (1 << CS11);
    ICR1 = 20000;
}

void set_servo_pos(uint8_t degree) {
    servoPosition = degree;
    OCR1A = map(degree, 0, 180, 500, 2500);
}

void button_event() {
    set_servo_pos(0);
    timer.in(350, reset_servo_position);
}

bool reset_servo_position(void *) {
    set_servo_pos(90);
    return true;
}