cmake_minimum_required(VERSION 3.10)
project(motor_control)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Find the WiringPi library
find_library(WIRINGPI_LIB wiringPi)

# Add the executable
add_executable(motor_control motor_control.c)

# Link the WiringPi library
target_link_libraries(motor_control ${WIRINGPI_LIB})

