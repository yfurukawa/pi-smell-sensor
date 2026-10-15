/*! @file DigitalOut.h
 *  @brief Header file for the DigitalOut class
 *  @details
 *  GPIOの出力を扱うクラス
 *  引数で渡されたBCM番号のGPIOピンをデジタル出力にセットして出力を制御する
 *  GPIOの制御にはwiringPi（https://github.com/WiringPi/WiringPi）を使用
 *  動作確認OSは、Ubuntu 64bit
 */
#ifndef DIGITALOUT_H_
#define DIGITALOUT_H_

#include <string>
#include <wiringPi.h>
#include "IGpioPin.h"

class DigitalOut: public IGpioPin
{
public:
    /*!
     * @brief Constructor for DigitalOut
     * @param name The purpose of the GPIO
     * @param bcmNumber The GPIO number (2-27)
     */
    DigitalOut(const std::string& name, const int& bcmNumber);

    /*!
     * @brief Destructor for DigitalOut
     */
    virtual ~DigitalOut();

    /*!
     * @brief Outputs a logic level high
     */
    void outputHigh();

    /*!
     * @brief Outputs a logic level low
     */
    void outputLow();

private:
    DigitalOut(){};
};

#endif

