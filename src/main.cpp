#include <Arduino.h>
#include <FastLED.h>

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
bool inControlOfLEDs = false;

// Raspberry pi IN: A0
//   A0:  IN: Booted Yes/No 
// Mux 1:
//   4:  OUT: Mux Control
//   A4: OUT: LED Data
//
// Mux 2: 
//   6:  OUT: Mux Control
//   A5: OUT: LED Data

void setup() {
  Serial.begin(9600);
  Serial.print("Hello from Metro Mini...\n");

  FastLED.addLeds<WS2812, LED_1_DATA, RGB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812, LED_2_DATA, RGB>(leds_2, NUM_LEDS);

  pinMode(PIN_PI_CONTROL, INPUT);
  pinMode(LED_1_MUX, OUTPUT);
  pinMode(LED_2_MUX, OUTPUT);
}

void loop() {
  inControlOfLEDs = digitalRead(PIN_PI_CONTROL) == LOW;
  digitalWrite(LED_1_MUX, inControlOfLEDs ? HIGH : LOW);
  digitalWrite(LED_2_MUX, inControlOfLEDs ? HIGH : LOW);

  // for(int i = 0; i < NUM_LEDS; i++) {
  //   // set our current dot to red, green, and blue
  //   leds[i] = CRGB::Red;
  //   leds_2[i] = CRGB::Green;
  //   FastLED.show();
  // }

  leds[0] = CRGB::Blue;
  leds[1] = CRGB::Blue;
  leds[2] = CRGB::Blue;

  leds_2[0] = CRGB::Green;
  leds_2[1] = CRGB::Green;
  leds_2[2] = CRGB::Green;

  FastLED.show();

  
}