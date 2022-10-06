#include <Arduino.h>
#include <FastLED.h>
#include <Bounce2.h>

#define PIN_PI_CONTROL A0

#define LED_1_MUX  4
#define LED_1_DATA A4
#define LED_2_MUX  6
#define LED_2_DATA A5

#define NUM_LEDS 3

// Define the array of leds
CRGB leds[NUM_LEDS];
CRGB leds_2[NUM_LEDS];

// Controls whether to drive LEDs or use external control
bool inControlOfLEDs = true;

// Raspberry pi IN: A0
//   A0:  IN: Booted Yes/No 
// Mux 1:
//   4:  OUT: Mux Control
//   A4: OUT: LED Data
//
// Mux 2: 
//   6:  OUT: Mux Control
//   A5: OUT: LED Data

Bounce2::Button button = Bounce2::Button();

void setup() {
  Serial.begin(9600);
  Serial.print("SureShot LED Boot by kevinc...\n");

  FastLED.addLeds<WS2812, LED_1_DATA, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_2_DATA, RGB>(leds_2, NUM_LEDS);

  pinMode(PIN_PI_CONTROL, INPUT);
  pinMode(LED_1_MUX, OUTPUT);
  pinMode(LED_2_MUX, OUTPUT);

  button.attach(PIN_PI_CONTROL, INPUT);
  button.interval(5); 
  button.setPressedState(LOW); 
}

void loop() {
    button.update();

    if ( button.pressed() ) {
        if (!inControlOfLEDs) {
            Serial.print("Taking over control of LEDs...");
            // leds[0] = CRGB::Blue;
            // leds[1] = CRGB::Blue;
            // leds[2] = CRGB::Blue;
            inControlOfLEDs = true;
        }
        else {
            Serial.print("Giving control of LEDs to FAST...");
            // leds[0] = CRGB::Red;
            // leds[1] = CRGB::Red;
            // leds[2] = CRGB::Red;
            inControlOfLEDs = false;
        }
    }

    digitalWrite(LED_1_MUX, inControlOfLEDs ? HIGH : LOW);
    digitalWrite(LED_2_MUX, inControlOfLEDs ? HIGH : LOW);

    leds[0] = CRGB::Blue;
    leds[1] = CRGB::Blue;
    leds[2] = CRGB::Blue;

    leds_2[0] = CRGB::Green;
    leds_2[1] = CRGB::Green;
    leds_2[2] = CRGB::Green;

    FastLED.show();
}