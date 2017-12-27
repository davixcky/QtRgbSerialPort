cmake_minimum_required(VERSION 3.7)
project(QtUiArduinoRgb)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets REQUIRED)

list(APPEND THIRD_PARTY_LIBS Qt5::Widgets Qt5::Core)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/control.cpp src/control.ui src/main.cpp)
set(INCLUDE_FILES src/control.h)

add_executable(QtUiArduinoRgb ${SOURCE_FILES} ${INCLUDE_FILES})

qt5_use_modules(${PROJECT_NAME} Core SerialPort)

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        )

