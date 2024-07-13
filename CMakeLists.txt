cmake_minimum_required(VERSION 3.28)
project(XimerTetrisGame)

set(EXEC_TARGET AppPrj)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/include/*.h ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(${EXEC_TARGET} ${HEADERS} ${SOURCES})
target_include_directories(${EXEC_TARGET} PRIVATE include src)

#Library linking and Headers import 
find_package(raylib REQUIRED)

target_link_libraries(${EXEC_TARGET} PRIVATE 
  raylib
)

# Post build: Copy Resources to bin
set(ResourcesPath ${CMAKE_CURRENT_SOURCE_DIR}/assets)
add_custom_command(TARGET ${EXEC_TARGET} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Copy Assets..."
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ResourcesPath} $<TARGET_FILE_DIR:${EXEC_TARGET}>/assets
)