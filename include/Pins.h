#pragma once

// 4-wire SPI for the ADC
// these are the default pins for the S3's 
// built-in "FSPI" peripheral but I'll specify them to be safe
#define ADC_MOSI 39
#define ADC_MISO 13
#define ADC_SCK 12
#define ADC_CS 40

//another 4-wire SPI for the display, plus two extra pins
#define D_MOSI 36
#define D_MISO 35
#define D_SCK 37
#define D_CS 38
#define D_RST 1
#define D_DCRC 2

//two rotary encoders w/ buttons
#define ENCA_L 4
#define ENCA_R 5
#define ENCA_B 6
#define ENCB_L 16
#define ENCB_R 42
#define ENCB_B 41

// four channel select buttons
#define CH1 14
#define CH2 17
#define CH3 18
#define CH4 21

// NeoPixels
#define PIXELS 47