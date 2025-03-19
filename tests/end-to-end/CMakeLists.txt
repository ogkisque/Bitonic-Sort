find_package(
  Python
  COMPONENTS Interpreter
  REQUIRED)

add_test(
  NAME e2e
  COMMAND Python::Interpreter ${CMAKE_CURRENT_SOURCE_DIR}/check_tests.py
                              $<TARGET_FILE:main>
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../..)