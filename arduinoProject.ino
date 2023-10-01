#include <Servo.h>  // Include the Servo library

Servo servo;  // Create a Servo object
int angle = 120;  // Initial angle of the servo

// Defining the pins
const int trigPin = 2;  // Trigger pin for ultrasonic sensor
const int echoPin = 4;  // Echo pin for ultrasonic sensor
const int greenLED = 13;  // LED that indicates trash is open
const int redLED = 12;  // LED that indicates trash is closed
const int buttonPin = 7;  // Push button to manually open/close trash

// Defining variables
float duration;  // Duration for which the echo pin is HIGH
float distance;  // Calculated distance using the ultrasonic sensor
int buttonState = 0;  // Current state of the button
int lastButtonState = 0;  // Previous state of the button (to detect state change)
bool trashOpen = false;  // Flag to check if trash is open
bool manualToggle = false;  // Flag to check if trash is manually toggled

void setup() {
    servo.attach(3);  // Attach servo on pin 3
    servo.write(angle);  // Initialize with trash can closed
    pinMode(trigPin, OUTPUT);  // Set trigPin as OUTPUT
    pinMode(echoPin, INPUT);  // Set echoPin as INPUT
    pinMode(greenLED, OUTPUT);  // Set greenLED as OUTPUT
    pinMode(redLED, OUTPUT);  // Set redLED as OUTPUT
    pinMode(buttonPin, INPUT_PULLUP);  // Set buttonPin as INPUT with internal pull-up resistor
    Serial.begin(9600);  // Begin serial communication at 9600 baud rate
}

void loop() {
    // Read the state of the pushbutton:
    buttonState = digitalRead(buttonPin);

    // Check for button state change (pressed)
    if (buttonState == LOW && lastButtonState == HIGH) {
        manualToggle = !manualToggle;  // Toggle the manual mode
        trashOpen = manualToggle;
        if (trashOpen) {
            openTrash();  // Open the trash
        } else {
            closeTrash();  // Close the trash
        }
    }
    lastButtonState = buttonState;  // Store the current state for next iteration

    // Send and receive signals from ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;  // Calculate distance

    // Check if object is within 7cm and trash isn't manually toggled
    if (!manualToggle && distance <= 7 && !trashOpen) {
        openTrash();  // Open the trash
        delay(5000);  // Keep the trash open for 5 seconds
        closeTrash();  // Close the trash
    }

    // Print the distance to the serial monitor
    Serial.print(distance);
    Serial.println(" cm");
    delay(1000);  // Delay for a second
}

// Function to open trash
void openTrash() {
    for (angle = 120; angle > 20; angle--) {
        servo.write(angle);  // Move the servo
        delay(15);  // Short delay for smooth movement
    }
    digitalWrite(greenLED, HIGH);  // Turn on the green LED
    digitalWrite(redLED, LOW);  // Turn off the red LED
    trashOpen = true;  // Update the trash state
}

// Function to close trash
void closeTrash() {
    for (angle = 20; angle < 120; angle++) {
        servo.write(angle);  // Move the servo
        delay(15);  // Short delay for smooth movement
    }
    digitalWrite(greenLED, LOW);  // Turn off the green LED
    digitalWrite(redLED, HIGH);  // Turn on the red LED
    trashOpen = false;  // Update the trash state
}