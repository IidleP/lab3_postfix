cmake_minimum_required(VERSION 3.10)
project(PostfixFull)

# Разрешаем линковать библиотеки между поддиректориями
cmake_policy(SET CMP0079 NEW)

# Собираем в правильном порядке
add_subdirectory(gtest)     # 1. Сначала gtest
add_subdirectory(base)      # 2. Затем библиотека и sample_postfix
add_subdirectory(base_test) # 3. Затем тесты