#include "Storage.h"
#include "ConsoleOutput.h"
#include "FileOutput.h"
#include "CommandProcessor.h"

int main(int argc, char const* argv[])
{
  try
  {
    if(2 != argc)
      throw std::invalid_argument("Only one positive integer must be provided as programm parameter.");

    std::string block_size{argv[1]};
    auto commandProcessor = std::make_unique<CommandProcessor>();
    std::shared_ptr<Storage> storage = std::make_shared<Storage>(std::stoull(block_size));
    std::shared_ptr<IOutput> consoleOutput = std::make_shared<ConsoleOutput>(std::cout);
    std::shared_ptr<IOutput> fileOutput = std::make_shared<FileOutput>();

    storage->Subscribe(consoleOutput);
    storage->Subscribe(fileOutput);
    commandProcessor->Subscribe(storage);

    commandProcessor->Process(std::cin);
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}