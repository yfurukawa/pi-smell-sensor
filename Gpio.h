/*! @file Gpio.h
 *  @brief Header file for the Gpio class
 *  @details
 *  GPIOの入出力Pinを管理するコンテナクラス
 *  ここに登録されたIGpioPinインターフェースを継承したクラスのインスタンスがGPIOの入出力を制御する。
 *  GPIOの制御にはwiringPi（https://github.com/WiringPi/WiringPi）を使用
 *  動作確認OSは、Ubuntu 64bit
 */
#ifndef GPIO_H_
#define GPIO_H_

#include <string>
#include <map>
#include <memory>
#include "IGpioPin.h"

class Gpio
{
public:
  /*!
   * @brief Constructor for Gpio
   */
  Gpio();
  /*!
   * @brief Destructor for Gpio
   */
  virtual ~Gpio();
  /*!
   * @brief Adds a GPIO pin to the container
   * @param bcmNumber The GPIO number (2-27), ID_SD:28, ID_SC:29
   * @param pin The GPIO pin instance
   * @return True if the pin was added successfully, false otherwise
   */
  bool addPin(int bcmNumber, std::shared_ptr<IGpioPin> pin);
  /*!
   * @brief Retrieves a GPIO pin from the container
   * @param bcmNumber The GPIO number (2-27), ID_SD:28, ID_SC:29
   * @return The GPIO pin instance, or nullptr if not found
   */
  std::shared_ptr<IGpioPin> pin(const int bcmNumber);

protected:
  std::map<int, std::shared_ptr<IGpioPin>> gpioPins;  //!< 入出力用に設定されたGPIO Pin
};

#endif
