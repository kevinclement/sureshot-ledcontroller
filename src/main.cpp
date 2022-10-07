#include <Arduino.h>
#include <FastLED.h>
#include <Bounce2.h>

#define PIN_PI_CONTROL A0

#define LED_1_MUX  4
#define LED_1_DATA A4
#define LED_2_MUX  6
#define LED_2_DATA A5

#define NUM_LEDS 70

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

// Define the array of leds
// CRGB leds[NUM_LEDS];
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
    Serial.setTimeout(10);
    Serial.print("SureShot LED Boot by kevinc...\n");

    // FastLED.addLeds<WS2812, LED_1_DATA, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812, LED_2_DATA, GRB>(leds_2, NUM_LEDS)
           .setCorrection(TypicalPixelString);
    FastLED.setBrightness(BRIGHTNESS);

    pinMode(PIN_PI_CONTROL, INPUT);
    pinMode(LED_1_MUX, OUTPUT);
    pinMode(LED_2_MUX, OUTPUT);

    button.attach(PIN_PI_CONTROL, INPUT);
    button.interval(5); 
    button.setPressedState(LOW); 
}

int currentLed = 6;

#define NUM_TRI_LEDS 12
int triOrder[] = {25, 29, 30, 31, 32, 33, 38, 37, 21, 22, 23, 24};

bool filledIt = false;
int currentTriIndex = 0;
uint8_t gHue = 0;
int pos = -1;

void loop() {
    button.update();

    if ( button.pressed() ) {
        if (!inControlOfLEDs) {
            Serial.println("Taking over control of LEDs...");
            inControlOfLEDs = true;
        }
        else {
            Serial.println("Giving control of LEDs to FAST...");
            // leds[0] = CRGB::Red;
            // leds[1] = CRGB::Red;
            // leds[2] = CRGB::Red;
            inControlOfLEDs = false;
        }
    }

    digitalWrite(LED_1_MUX, inControlOfLEDs ? HIGH : LOW);
    digitalWrite(LED_2_MUX, inControlOfLEDs ? HIGH : LOW);


    // EVERY_N_MILLISECONDS(300){
    //     fadeToBlackBy(leds_2, NUM_LEDS, 20);
    //     pos++;

    //     leds_2[triOrder[pos]] += CHSV(HUE_GREEN, 255, 192);  
      
    //     if (pos == NUM_TRI_LEDS) {
    //         pos = -1;
    //     }
    // }
    
    fadeToBlackBy(leds_2, NUM_LEDS, 20);
    int mapped = beat8(2) % NUM_TRI_LEDS;

    // /int pos = beatsin8( 13, 0, NUM_TRI_LEDS-1 );
    // Serial.println(pos);
    leds_2[triOrder[mapped]] += CHSV(gHue, 255, 192);

    // if (!filledIt) {
    //     leds_2[triOrder[currentTriIndex]] = CRGB::Blue;
    //     currentTriIndex++;
    //     if (currentTriIndex > NUM_TRI_LEDS) {
    //         filledIt = true;
    //     }
    //     FastLED.show();
    //     delay(400);
    // }
    
    // EVERY_N_MILLISECONDS( 5 ) { 
    //     fadeToBlackBy( leds_2, NUM_LEDS, 10);
    //  } 

    // if (Serial.available()) {
    //     String str = Serial.readString();
    //     currentLed++;
    //     Serial.print("curent LED: ");
    //     Serial.println(currentLed);
    // }

    // leds_2[currentLed] = CRGB::Blue;


    FastLED.show();

    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND); 

    EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}