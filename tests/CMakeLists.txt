cmake_minimum_required(VERSION 3.2)

function(test name)
  add_executable (${name} ${name}.cpp)
  target_compile_definitions(${name} PRIVATE BOOST_TEST_DYN_LINK)
  target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR})
  target_link_libraries (${name} 
      ${Boost_LIBRARIES}
  )
  add_test(${name} ../build/${name})
endfunction(test)

test(test_internal_data_structures)
test(test_parser)
test(test_file_output)
