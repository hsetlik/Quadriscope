#pragma once
#include "Pins.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <RotaryEncoder.h>
#include <functional>
#include <OneButton.h>
#include <Adafruit_NeoPixel.h>
using EncoderCallback = std::function<void(bool)>;

#define FRAME_LENGTH_MS 41

#define DISPLAY_W 360
#define DISPLAY_H 240

class Encoder
{
private:
    long position;
    RotaryEncoder enc;
public:
    Encoder(uint8_t l, uint8_t r);
    void tick();
    EncoderCallback callback;
};
//===================================================

// store the state and display data for each channel
struct ScopeChannel
{
    uint8_t index = 0;
    bool active = false;
    float vAmplitude = 12.0f; // vertical scale, in terms of voltage at the top of the display
    uint16_t hWindow = 50; // the length of time in ms represented across the width of the display
    int16_t screenBuffer[DISPLAY_W]; //store a voltage value for each pixel to use all of the display's resolution
};
//===================================================
class Quadriscope // represents The Device
{
private:
// state
    Adafruit_ILI9341* display;
    unsigned long prevUpdateMs;
    Encoder* lEnc;
    Encoder* rEnc;
    OneButton* b1;
    OneButton* b2;
    OneButton* b3;
    OneButton* b4;
    Adafruit_NeoPixel* pixels;
// the state that gets modified
    uint8_t selectedChannel;
    ScopeChannel channels[4];
// the dac and its associated data
// these boys get wired into the encoder and button callbacks
    void leftEncTurn(bool up);
    void rightEncTurn(bool up);
    void buttonPress(uint8_t idx);
    void buttonHold(uint8_t idx);
// clears the display and draws the appropriate waves
    void drawFrame();
// update the NeoPixels for the current state
    void updatePixels();
    uint32_t fromRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((uint32_t)b) & ((uint32_t)g << 8) & ((uint32_t)r << 16);
    }
public: // main.cpp should only have to touch this code from here down
    Quadriscope();
    ~Quadriscope();
    void init();
    void tick();
};
