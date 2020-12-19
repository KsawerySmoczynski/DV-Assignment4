cmake_minimum_required(VERSION 2.8.7)
project(assignment4)

find_package(VTK COMPONENTS vtkRenderingOpenGL2 vtkInteractionStyle vtkRenderingFreeType NO_MODULE)

include(${VTK_USE_FILE})

set(SOURCES ../../source/assignment4.cpp)

add_executable(assignment4 ${SOURCES})
target_link_libraries(assignment4 ${VTK_LIBRARIES})
