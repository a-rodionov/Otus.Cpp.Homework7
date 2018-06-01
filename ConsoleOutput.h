#pragma once

#include <iostream>
#include "IOutput.h"

class ConsoleOutput : public IOutput
{

public:

  explicit ConsoleOutput(std::ostream& out)
    : out{out} {}

  void Output(const std::size_t, const std::list<std::string>& data) override {
    OutputFormattedBulk(out, data);
  }

private:

  std::ostream& out;
};
