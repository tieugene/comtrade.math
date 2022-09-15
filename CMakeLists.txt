cmake_minimum_required(VERSION 3.5)

project(ctmath LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
aux_source_directory(src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
