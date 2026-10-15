#include "DigitalIn.h"

DigitalIn::DigitalIn(const std::string& name, const int& bcmNumber) : IGpioPin( name, bcmNumber )
{
    // Raspberry PiのGPIOのBCM番号は、2から27の範囲なので範囲内にあることを保証する
    if( bcmNumber_ < 2 ) {
        bcmNumber_ = 2;
    }
    if( bcmNumber_ > 27 ) {
        bcmNumber_ = 27;
    }

    // デジタル出力専用クラスなので、Pinの設定を出力に固定
    pinMode( bcmNumber_, INPUT );
}

DigitalIn::~DigitalIn()
{
    // 終了前にPinの設定をリセット
    pinMode( bcmNumber_, PM_OFF );
}

