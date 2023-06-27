cmake_minimum_required(VERSION 3.21)
project(zlaya_zmeyka)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED graphics network audio window system)

add_executable(zlaya_zmeyka main.cpp Sourse.cpp)
target_link_libraries(zlaya_zmeyka PUBLIC sfml-graphics sfml-system sfml-window)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/image/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/image/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fonts/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/fonts/)

project(zlaya_zmeyka_test)
enable_testing()
add_subdirectory(external/doctest)
add_executable(zlaya_zmeyka_test tests.cpp Sourse.cpp)
target_link_libraries(zlaya_zmeyka_test PUBLIC doctest sfml-graphics sfml-system sfml-window)