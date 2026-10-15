/*! @file DigitalIn.h
 *  @brief Header file for the DigitalIn class
 *  @details
 *  GPIOの入力を扱うクラス
 *  Raspberry PiのGPIOを入力として扱う。
 *  GPIOの制御にはwiringPi（https://github.com/WiringPi/WiringPi）を使用
 *  動作確認OSは、Ubuntu 64bit
 */
#ifndef DIGITALIN_H_
#define DIGITALIN_H_

#include <string>
#include <wiringPi.h>
#include "IGpioPin.h"

class DigitalIn: public IGpioPin
{
public:
    /*!
     * @brief Constructor for DigitalIn
     * @param name The purpose of the GPIO
     * @param bcmNumber The GPIO number (2-27)
     */
    DigitalIn(const std::string& name, const int& bcmNumber);

    /*!
     * @brief Destructor for DigitalIn
     */
    virtual ~DigitalIn();

private:
    DigitalIn(){};
};

#endif

