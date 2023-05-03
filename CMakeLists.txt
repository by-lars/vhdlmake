cmake_minimum_required(VERSION 3.15)
project(vhdlmake)
set(CMAKE_CXX_STANDARD 20)

set(SOURCES 
    "src/Main.cpp"
    "src/Node.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
