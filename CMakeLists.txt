cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(ProtonBeamSimulation)

# Add the -g flag to enable debugging information
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Find Geant4 package
find_package(Geant4 REQUIRED ui_all vis_all)

# Set up include directories
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Locate sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add executable
add_executable(Pro Pro.cc ${sources} ${headers})

# Link executable to Geant4 libraries
target_link_libraries(Pro ${Geant4_LIBRARIES})

file(GLOB macros RELATIVE ${PROJECT_SOURCE_DIR} /macros/*.mac)

add_custom_target(ProtonBeamSimulation DEPENDS Pro)

set(ProtonBeamSimulation_SCRIPTS
    macros/vis.mac 
    macros/init.mac 
    macros/run.mac 
    macros/gps.mac
    plot.py
    plotX.py
  )

foreach(_script ${ProtonBeamSimulation_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

# Install the executable
install(TARGETS Pro DESTINATION bin)

