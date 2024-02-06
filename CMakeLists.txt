cmake_minimum_required(VERSION 3.20)
project(hello VERSION "0.1.0" LANGUAGES "CXX")

find_package(ZXing REQUIRED)
find_package(stb REQUIRED)

add_executable(RawReader RawReader.cpp)
target_compile_features(RawReader PUBLIC cxx_std_20)
target_link_libraries(RawReader PRIVATE ZXing::ZXing)

add_executable(Reader Reader.cpp)
target_compile_features(Reader PUBLIC cxx_std_20)
target_link_libraries(Reader PRIVATE ZXing::ZXing stb::stb)
