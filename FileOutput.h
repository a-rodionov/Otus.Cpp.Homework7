#pragma once

#include <fstream>
#include "IOutput.h"

class FileOutput : public IOutput
{
  using IOutput::Output;

public:

  void Output(const std::size_t timestamp, const std::list<std::string>& data) override {
    std::string filename = "bulk" + std::to_string(timestamp) + ".log";
    std::ofstream ofs{filename.c_str(), std::ofstream::out | std::ofstream::trunc};
    Output(ofs, data);
    ofs.close();
  }

};
