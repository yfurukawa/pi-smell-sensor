/*! @file IGpioPin.h
 *  @brief Header file for the IGpioPin interface
 *  @details
 *  GPIOの入出力Pinを管理するインターフェース
 *  Raspberry PiのGPIOを入出力として扱う。
 *  GPIOの制御にはwiringPi（https://github.com/WiringPi/WiringPi）を使用
 *  動作確認OSは、Ubuntu 64bit
 */
#ifndef IGPIOPIN_H_
#define IGPIOPIN_H_

#include <string>

class IGpioPin
{
public:
  /*!
   * @brief Constructor for IGpioPin
   * @param name The purpose of the GPIO
   * @param bcmNumber The GPIO number (2-27), ID_SD:28, ID_SC:29
   */
  IGpioPin(const std::string& name, const int& bcmNumber):name_(name), bcmNumber_(bcmNumber){};
  /*!
   * @brief Destructor for IGpioPin
   */
  virtual ~IGpioPin() = default;

protected:
  std::string name_;  //!< GPIOの用途
  int bcmNumber_;     //!< 制御するGPIOのBCM番号

  IGpioPin(){};
};

#endif

