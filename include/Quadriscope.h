#pragma once
#include "Pins.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <RotaryEncoder.h>
#include <functional>
#include <OneButton.h>
using EncoderCallback = std::function<void(bool)>;

#define FRAME_LENGTH_MS 41

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
// these boys get wired into the encoder and button callbacks
    void leftEncTurn(bool up);
    void rightEncTurn(bool up);
    void buttonPress(uint8_t idx);
    void buttonHold(uint8_t idx);
// clears the display and draws the appropriate waves
    void drawFrame();
public: // main.cpp should only have to touch this code from here down
    Quadriscope();
    ~Quadriscope();
    void init();
    void tick();
};
