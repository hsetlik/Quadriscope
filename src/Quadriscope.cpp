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
b4(nullptr)
{

}

Quadriscope::~Quadriscope()
{
    delete display;
    delete rEnc;
    delete lEnc;
    delete b1;
    delete b2;
    delete b3;
    delete b4;
}

void Quadriscope::init()
{
    Serial.begin(9600);
    //step 1: do all our heap allocations
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

}

void Quadriscope::tick()
{
    unsigned long now = millis();
    lEnc->tick();
    rEnc->tick();
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