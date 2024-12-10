cmake_minimum_required(VERSION 3.10)
project(SVGDemo)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include source files
file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

# Add the executable
add_executable(SVGDemo ${SOURCES})

# Link Windows libraries (needed for GUI apps, icons, etc.)
if(WIN32)
    target_link_libraries(SVGDemo PRIVATE gdi32 gdiplus)
endif()
