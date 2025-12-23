cmake_minimum_required(VERSION 3.10)
project(postfix_tests)

# ТЕСТОВОЕ ПРИЛОЖЕНИЕ
add_executable(run_tests
    test_main.cpp
    test_tpostfix.cpp
    test_tstack.cpp
)

# Линкуем тесты с библиотекой (postfix_lib уже создана в base)
target_link_libraries(run_tests postfix_lib gtest)

# Указываем пути (ВАЖНО: политика CMP0079)
cmake_policy(SET CMP0079 NEW)

target_include_directories(run_tests PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../base
    ${CMAKE_CURRENT_SOURCE_DIR}/../gtest
)