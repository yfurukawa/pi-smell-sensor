#include "DigitalOut.h"

DigitalOut::DigitalOut(const std::string& name, const int& bcmNumber) : IGpioPin( name, bcmNumber )
{
    // Raspberry PiのGPIOのBCM番号は、2から27の範囲なので範囲内にあることを保証する
    if( bcmNumber_ < 2 ) {
        bcmNumber_ = 2;
    }
    if( bcmNumber_ > 27 ) {
        bcmNumber_ = 27;
    }

    // デジタル出力専用クラスなので、Pinの設定を出力に固定
    pinMode( bcmNumber_, OUTPUT );
}

DigitalOut::~DigitalOut()
{
    // 終了前にPinの設定をリセット
    pinMode( bcmNumber_, PM_OFF );
}

void DigitalOut::outputHigh()
{
    digitalWrite( bcmNumber_, HIGH );
}

void DigitalOut::outputLow()
{
    digitalWrite( bcmNumber_, LOW );
}
