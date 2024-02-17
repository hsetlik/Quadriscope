#include "Quadriscope.h"
Encoder::Encoder(uint8_t l, uint8_t r) : position(0), enc(l, r, RotaryEncoder::LatchMode::FOUR3)
{

}
    
void Encoder::tick()
{
    long currentPos = enc.getPosition();
    if(currentPos > position)
        callback(true);
    else if(currentPos < position)
        callback(false);
    position = currentPos;
}

//==========================================================================
Quadriscope::Quadriscope() : 
display(nullptr),
prevUpdateMs(0),
lEnc(nullptr),
rEnc(nullptr),
b1(nullptr),
b2(nullptr),
b3(nullptr),
b4(nullptr),
pixels(nullptr),
selectedChannel(0)
{

}

Quadriscope::~Quadriscope()
{
    // deal with all those manual heap allocations from init()
    delete display;
    delete rEnc;
    delete lEnc;
    delete b1;
    delete b2;
    delete b3;
    delete b4;
    delete pixels;
}

void Quadriscope::init()
{
    Serial.begin(9600);

    //display
    display = new Adafruit_ILI9341(D_CS, D_DCRC, D_MOSI, D_SCK, D_RST, D_MISO);
    display->begin();

    //encoders
    lEnc = new Encoder(ENCA_L, ENCA_R);
    rEnc = new Encoder(ENCB_L, ENCB_R);

    lEnc->callback = [this](bool up) {
        this->leftEncTurn(up);
    };

    rEnc->callback = [this](bool up) {
        this->rightEncTurn(up);
    };

    //buttons
    b1 = new OneButton(CH1, true);
    b2 = new OneButton(CH2, true);
    b3 = new OneButton(CH3, true);
    b4 = new OneButton(CH4, true);

    b1->attachClick([](void* q){((Quadriscope*)q)->buttonPress(0);}, this);
    b2->attachClick([](void* q){((Quadriscope*)q)->buttonPress(1);}, this);
    b3->attachClick([](void* q){((Quadriscope*)q)->buttonPress(2);}, this);
    b4->attachClick([](void* q){((Quadriscope*)q)->buttonPress(3);}, this);

    b1->attachLongPressStop([](void* q){((Quadriscope*)q)->buttonHold(0);}, this);
    b2->attachLongPressStop([](void* q){((Quadriscope*)q)->buttonHold(1);}, this);
    b3->attachLongPressStop([](void* q){((Quadriscope*)q)->buttonHold(2);}, this);
    b4->attachLongPressStop([](void* q){((Quadriscope*)q)->buttonHold(3);}, this);

    //NeoPixels
    pixels = new Adafruit_NeoPixel(4, PIXELS, NEO_GRB + NEO_KHZ800);
    pixels->begin();

    //now initialize the state
    for(int i = 0; i < 4; i ++)
    {
        auto& c = channels[i];
        c.index = (uint8_t)i;
    }
}

void Quadriscope::tick()
{
    unsigned long now = millis();
    lEnc->tick();
    rEnc->tick();

    b1->tick();
    b2->tick();
    b3->tick();
    b4->tick();

    if(now - prevUpdateMs >= FRAME_LENGTH_MS)
    {
        prevUpdateMs = now;
        drawFrame();
    }
}
//==========================================================================
void Quadriscope::drawFrame()
{
    //TODO
}

void Quadriscope::updatePixels()
{
    //designated colors for each channel
    static const uint32_t selColors[] = {fromRGB(255, 138, 51), fromRGB(130, 194, 65), fromRGB(133, 213, 255), fromRGB(188, 145, 209)};
    static const uint32_t onColors[] = {fromRGB(191, 104, 38), fromRGB(74, 134, 13), fromRGB(77, 151, 191), fromRGB(108, 54, 134)};
    pixels->clear();
    pixels->setBrightness(60);
    for(uint8_t i = 0; i < 4; i++)
    {
        if(selectedChannel == i)
            pixels->setPixelColor(i, selColors[i]);
        else if(channels[i].active)
            pixels->setPixelColor(i, onColors[i]);
        else
            pixels->setPixelColor(i, fromRGB(0, 0, 0));
    }
    pixels->show();
}
//==========================================================================
void Quadriscope::leftEncTurn(bool up)
{

}

void Quadriscope::rightEncTurn(bool up)
{

}

void Quadriscope::buttonPress(uint8_t idx)
{

}

void Quadriscope::buttonHold(uint8_t idx)
{

}