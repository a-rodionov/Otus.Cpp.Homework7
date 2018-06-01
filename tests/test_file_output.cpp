#include <sys/file.h>
#include <sstream>
#include <deque>
#include <algorithm>
#include "Storage.h"
#include "FileOutput.h"
#include "CommandProcessor.h"

#define BOOST_TEST_MODULE test_file_output

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(test_suite_main)

class TestFileOutput : public FileOutput {

public:

  auto GetLastFileName() const {
    return fileNames;
  }

private:

  void PostOutputAction(const std::string& filename) const override {
    fileNames.push_back(filename);
  }

  mutable std::deque<std::string> fileNames;
};

BOOST_AUTO_TEST_CASE(write_multiple_files)
{
  std::string testData{"cmd1\n"
                      "cmd2\n"
                      "cmd3\n"
                      "cmd4\n"
                      "cmd5\n"
                      "cmd6\n"
                      "cmd7\n"
                      "cmd8\n"};
  std::array<std::string, 3> results = {
    "bulk: cmd1, cmd2, cmd3",
    "bulk: cmd4, cmd5, cmd6",
    "bulk: cmd7, cmd8"};
  std::istringstream iss(testData);
  std::string result_from_file;

  auto commandProcessor = std::make_unique<CommandProcessor>();
  auto storage = std::make_shared<Storage>(3);
  auto fileOutput = std::make_shared<TestFileOutput>();

  storage->Subscribe(fileOutput);
  commandProcessor->Subscribe(storage);

  commandProcessor->Process(iss);

  auto filenames = fileOutput->GetLastFileName();
  BOOST_REQUIRE_EQUAL(results.size(), filenames.size());

  std::sort(std::begin(filenames), std::end(filenames));
  BOOST_REQUIRE(std::cend(filenames) == std::adjacent_find(std::cbegin(filenames), std::cend(filenames)));

  auto result = std::cbegin(results);
  for(const auto& filename : filenames) {
    std::ifstream ifs{filename.c_str(), std::ifstream::in};
    BOOST_REQUIRE_EQUAL(false, ifs.fail());

    std::getline(ifs, result_from_file);
    BOOST_REQUIRE_EQUAL(*result, result_from_file);

    std::getline(ifs, result_from_file);
    BOOST_REQUIRE_EQUAL(true, ifs.eof());
    BOOST_REQUIRE_EQUAL(true, result_from_file.empty());

    std::remove(filename.c_str());
    ++result;
  }


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
