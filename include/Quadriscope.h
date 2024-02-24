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

/*
Our ADC is 16 bit so this means storing 
sample rate * (STORAGE_MS / 1000) * 2 bytes of data per 
channel. The MCU has 2MB PSRAM in addition to its 
onboard RAM, with sample rate of 50kHz and 4 seconds of
storage that means 400kB of memory per channel or
1.6MB total, which should be OK? 
*/ 
#define SAMPLE_RATE 50000
#define STORAGE_MS 250

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
const uint32_t BufferSize = (SAMPLE_RATE * STORAGE_MS) / 1000;
const float samplePeriodMs = (1000.0f / (float)SAMPLE_RATE);
class SampleBuffer
{
private:
    uint16_t data[BufferSize];
    uint16_t head;
public:
    SampleBuffer() : head(0)
    {
    }
    void push(uint16_t value)
    {
        data[head] = value;
        head = (head + 1) % BufferSize;
    }
    //get a float value representing volts
    float operator[](uint16_t idx)
    {
        idx = (head + idx) % BufferSize;
        float fNorm = (float)data[idx] / 65535.0f;
        return ((fNorm - 0.5f) * 12.0f);
    }

};

//===================================================

// store the state and display data for each channel
struct ScopeChannel
{
    uint8_t index = 0;
    bool active = false;
    float vAmplitude = 12.0f; // vertical scale, in terms of voltage at the top of the display
    float vOffset = 0.0f;
    uint16_t screenBuffer[DISPLAY_W]; //store a pixel Y value corresponding to the voltage
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
    uint16_t hWindow = 50; // the length of time in ms represented across the width of the display

// SPI stuff for the DAC
    SPIClass* fspi;
// this holds our actual data stream
    SampleBuffer sampleBuffers[4];

// these boys get wired into the encoder and button callbacks
    void leftEncTurn(bool up);
    void rightEncTurn(bool up);
    void buttonPress(uint8_t idx);
    void buttonHold(uint8_t idx);

// clears the display and draws the appropriate waves
    void drawFrame();
// helper for drawFrame, fills up the voltage values in the screen buffers
    void updateScreenBuffers();
// helper for accessing the sample buffers
    float getVoltageAtMs(uint8_t channel, float ms);
// convert the voltages from above into pixels
    uint16_t voltageToScreenY(float voltage, float amplitude=12.0f, float offset=0.0f);
// update the NeoPixels for the current state
    void updatePixels();
// just a little helper for 'packing' 32 bit RGB colors
    uint32_t fromRGB(uint8_t r, uint8_t g, uint8_t b)
    {
        return ((uint32_t)b) & ((uint32_t)g << 8) & ((uint32_t)r << 16);
    }

public: // main.cpp should only have to touch this code from here down
    Quadriscope();
    ~Quadriscope();
    void init();
    void tick();
    // This just runs the SPI transaction to load the next samples into the buffer. 
    // It needs to be connected to a hardware timer interrupt in main.cpp
    void sampleInterrupt();
};
