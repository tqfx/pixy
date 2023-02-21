cmake_minimum_required(VERSION 2.8.12...4.0)
set(CMAKE_POSITION_INDEPENDENT_CODE 1)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(pixy)
find_package(OpenCV CONFIG)
find_path(LIBUSB-1.0_INCLUDE_DIR libusb.h PATH_SUFFIXES libusb-1.0)
add_library(pixy
    src/host/libpixyusb2/src/libpixyusb2.cpp
    src/host/libpixyusb2/src/usblink.cpp
    src/host/libpixyusb2/src/util.cpp
    src/common/src/chirp.cpp
    pixy.cpp
    pixy.h
)
target_compile_definitions(pixy PRIVATE __LINUX__)
target_include_directories(pixy PRIVATE src/common/inc PUBLIC
    src/host/arduino/libraries/Pixy2
    src/host/libpixyusb2/include
    ${LIBUSB-1.0_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(pixy PUBLIC usb-1.0 ${OpenCV_LIBRARIES})

add_executable(test test.cpp)
target_include_directories(test PRIVATE)
target_link_libraries(test PRIVATE pixy)
