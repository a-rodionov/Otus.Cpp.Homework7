#include <sys/file.h>
#include <sstream>
#include "Storage.h"
#include "FileOutput.h"
#include "CommandProcessor.h"

#define BOOST_TEST_MODULE test_file_output

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_suite_main)

class TestFileOutput : public FileOutput {

public:

  std::string GetLastFileName() const {
    return lastFileName;
  }

private:

  void PostOutputAction(const std::string& filename) const override {
    lastFileName = filename;
  }

  mutable std::string lastFileName;
};

BOOST_AUTO_TEST_CASE(simple_file_output)
{
  std::string result;
  std::string goodResult{"bulk: cmd100, cmd200, cmd300"};
  std::string testData{"cmd100\ncmd200\ncmd300\n"};
  std::istringstream iss(testData);

  auto commandProcessor = std::make_unique<CommandProcessor>();
  auto storage = std::make_shared<Storage>(3);
  auto fileOutput = std::make_shared<TestFileOutput>();

  storage->Subscribe(fileOutput);
  commandProcessor->Subscribe(storage);

  commandProcessor->Process(iss);

  auto filename = fileOutput->GetLastFileName();
  std::ifstream ifs{filename.c_str(), std::ifstream::in};
  BOOST_CHECK_EQUAL(false, ifs.fail());

  std::getline(ifs, result);
  BOOST_CHECK_EQUAL(goodResult, result);

  std::getline(ifs, result);
  BOOST_CHECK_EQUAL(true, ifs.eof());
  goodResult.clear();
  BOOST_CHECK_EQUAL(goodResult, result);

  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(file_output_to_locked_file)
{
  FileOutput fileOutput;
  std::string result;
  std::string goodResult{"bulk: cmd1, cmd2, cmd3"};
  std::list<std::string> testData{"cmd1", "cmd2", "cmd3"};
  size_t timestamp = 123;
  auto filename = MakeFilename(timestamp);

  std::remove(filename.c_str());

  auto file_handler = open(filename.c_str(), O_CREAT);
  BOOST_REQUIRE_EQUAL(true, -1 != file_handler);
  BOOST_REQUIRE_EQUAL(true, -1 != flock( file_handler, LOCK_EX | LOCK_NB ));
  
  BOOST_CHECK_THROW(fileOutput.Output(timestamp, testData), std::runtime_error);
  
  flock(file_handler, LOCK_UN | LOCK_NB);
  close(file_handler);
  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_SUITE_END()
