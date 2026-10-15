#include <utility>
#include <wiringPi.h>
#include "Gpio.h"

Gpio::Gpio()
{
  wiringPiSetupPinType(WPI_PIN_BCM);
}

Gpio::~Gpio()
{
  gpioPins.clear();
}

bool Gpio::addPin(int bcmNumber, std::shared_ptr<IGpioPin> pin)
{
  if( !pin )
  {
    return false;
  }

  auto [it, inserted] = gpioPins.try_emplace(bcmNumber, std::move(pin));
  return inserted;
}

std::shared_ptr<IGpioPin> Gpio::pin(const int bcmNumber)
{
  return gpioPins.at( bcmNumber );
}
