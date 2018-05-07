#pragma once

#include <time.h>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include "infix_iterator.h"

class IOutput
{
public:

  virtual void Output(const std::size_t timestamp, const std::list<std::string>& data) = 0;

protected:

  void Output(std::ostream& out, const std::list<std::string>& data) {
    out << "bulk: ";
    std::copy(std::cbegin(data),
              std::cend(data),
              infix_ostream_iterator<std::string>{out, ", "});
    out << std::endl;
  }
};
