cmake_minimum_required(VERSION 3.10)
project(postfix_base)

# БИБЛИОТЕКА для postfix
add_library(postfix_lib STATIC
    postfix.cpp
)

# ОТДЕЛЬНОЕ ПРИЛОЖЕНИЕ sample_postfix
add_executable(sample_postfix
    sample_postfix.cpp
)

# Линкуем sample_postfix с библиотекой
target_link_libraries(sample_postfix postfix_lib)

# Указываем пути для заголовков
target_include_directories(postfix_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(sample_postfix PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})