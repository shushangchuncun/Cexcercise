cmake_minimum_required(VERSION 3.10)
project(simulator_student CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 库：所有 .cpp（不含 main.cpp）
add_library(simulator_core
    src/building.cpp
    src/extras.cpp
    src/game.cpp
    src/robot.cpp
)

# include/ 是 simulator_core 的 PUBLIC 头文件目录
target_include_directories(simulator_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 可执行程序：main.cpp，链接 simulator_core
add_executable(simulator src/main.cpp)
target_link_libraries(simulator PRIVATE simulator_core)