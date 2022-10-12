#include <Arduino.h>
#include <FastLED.h>
#include <Bounce2.h>

// Raspberry pi IN: A0
//   A0:  IN: Booted Yes/No 
// Mux 1:
//   4:  OUT: Mux Control
//   A4: OUT: LED Data
//
// Mux 2: 
//   6:  OUT: Mux Control
//   A5: OUT: LED Data
#define PIN_PI_CONTROL A0
#define LED_1_MUX  4
#define LED_1_DATA A4
#define LED_2_MUX  6
#define LED_2_DATA A5

// LED Settings
#define NUM_LEDS            70
#define BRIGHTNESS          96
#define FRAMES_PER_SECOND   120
#define NUM_TRIANGLE_LEDS   12
#define HUE_GREEN           96  // Green: found in example lib, under HUE_GREEN
#define USE_RAINBOW_FADE    true
int triangle[] = {25, 29, 30, 31, 32, 33, 38, 37, 21, 22, 23, 24};

// Debug flag
#define DEBUG false

// Define the array of leds
//   NOTE: only currently changing playfield LEDs
//   but we have access to all of them
CRGB leds[NUM_LEDS];

// Controls whether to drive LEDs or use external control
bool inControlOfLEDs = true;

Bounce bounce = Bounce();

int current_debug_led = 0;

// if not using rainbow, use fixed yellow color
uint8_t gHue = USE_RAINBOW_FADE ? 0 : 64;

void setup() {
    Serial.begin(9600);
    Serial.setTimeout(10);
    Serial.print("Sure Shot LED Controller by kevinc...\n");

    // FastLED.addLeds<WS2812, LED_1_DATA, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812, LED_2_DATA, GRB>(leds, NUM_LEDS)
           .setCorrection(TypicalPixelString);
    FastLED.setBrightness(BRIGHTNESS);

    pinMode(PIN_PI_CONTROL, INPUT);
    pinMode(LED_1_MUX, OUTPUT);
    pinMode(LED_2_MUX, OUTPUT);

    // Uses an external pull down
    bounce.attach( PIN_PI_CONTROL, INPUT ); 
    bounce.interval(5); 
}

void animateCircle() {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    int pos = beat8(2) % NUM_TRIANGLE_LEDS;
    leds[triangle[pos]] += CHSV(gHue, 255, 192);
}

void animateBackAndForth() {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    int pos = beatsin16(13, 0, NUM_TRIANGLE_LEDS-1);
    leds[triangle[pos]] += CHSV(gHue, 255, 192);
}

void debug() {
    if (!DEBUG) {
        return;
    }

    // Read anything from serial and move the pointer to the current LED
    // Then turn that LED on.  Allows to do a audit of LED to insert.
    if (Serial.available()) {
        String str = Serial.readString();
        current_debug_led++;
        Serial.print("curent LED: ");
        Serial.println(current_debug_led);

        leds[current_debug_led] = CRGB::Blue;
        FastLED.show();
    }
}

void checkForPiSignal() {
    bounce.update();

    // TODO: Should we implement an ultimate time-out?  
    //  Say 30s and we just hand it over

    if (bounce.rose()) {
        Serial.println("Giving control of LEDs to FAST...");
        inControlOfLEDs = false;
    }

    // Write out the control for the Mux
    digitalWrite(LED_1_MUX, inControlOfLEDs ? HIGH : LOW);
    digitalWrite(LED_2_MUX, inControlOfLEDs ? HIGH : LOW);
}

void loop() {
    debug();

    // check for the Pi signaling us that it booted
    checkForPiSignal();

    animateCircle();

    FastLED.show();

    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND); 

    // slowly cycle the "base color" through the rainbow
    EVERY_N_MILLISECONDS( 20 ) { 
        if (USE_RAINBOW_FADE) {
            gHue++; 
        }
    } 
}