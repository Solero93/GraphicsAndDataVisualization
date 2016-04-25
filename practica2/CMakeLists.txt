cmake_minimum_required(VERSION 3.5)
include(glm/CMakeLists.txt)
project(practica2)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(AFTER "/opt/X11/include")

set(SOURCE_FILES
    Camera.cpp
    Camera.h
    Object.cpp
    Object.h
    Ray.h
    RayTracer.cpp
    RayTracer.h
    Scene.cpp
    Scene.h)
add_executable(RayTracer ${SOURCE_FILES})

target_link_libraries(RayTracer GL GLU glut)


#[[CC=g++ -I/opt/X11/include
LIBS= -lGL -lglut

all:
$(CC) -L/opt/X11/lib/ *.cpp $(LIBS) -o RayTracer

run: all
./RayTracer

clean:
rm -f RayTracer *.o]]