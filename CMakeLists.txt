project(copsnrobs)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(Boost REQUIRED
  COMPONENTS
  system)
  
include_directories(
  websocket
)

set(SOURCES
  game.hpp
  game.cpp
  assign.hpp
  assign.cpp
  message.hpp
  message.cpp
  map.hpp
  map.cpp
)

add_executable(${CMAKE_PROJECT_NAME}
  ${SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} BEFORE
  PRIVATE
  ${Boost_INCLUDE_DIR}
)

message("${Boost_LIBRARIES}")

target_link_libraries(${CMAKE_PROJECT_NAME} 
  PRIVATE
  ${Boost_LIBRARIES}
)
