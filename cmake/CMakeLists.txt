# (1)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project (cssapp)

# (2)
option(WITH_GEANT4_UIVIS "Build project with Geant4 UI an Vis Drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()


# (3)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# (4)
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# (5)
add_executable(cssapp cssapp.cc ${sources} ${headers})
target_link_libraries(cssapp ${Geant4_LIBRARIES})

# (6)
set(cssapp_SCRIPTS
#  cssapp.in
#  cssapp.out
  css.mac
  vis.mac
  init_vis.mac
  )

foreach(_script ${cssapp_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

# (extra)
#add_custom_target(cssapp DEPENDS cssapp)

# (7)
install(TARGETS cssapp DESTINATION bin)
