cmake_minimum_required(VERSION 3.14)
project(AdaptixBeacon VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)

set(BEACON_SOURCES
    src/beacon.cc
    src/beacon_config.cc
    src/command_executor.cc
    src/http_transport.cc
    src/task.cc
)

set(BEACON_HEADERS
    include/adaptix/beacon/beacon.h
    include/adaptix/beacon/beacon_config.h
    include/adaptix/beacon/command_executor.h
    include/adaptix/beacon/http_transport.h
    include/adaptix/beacon/task.h
    include/adaptix/beacon/transport.h
)

add_library(adaptix_beacon STATIC ${BEACON_SOURCES} ${BEACON_HEADERS})
target_include_directories(adaptix_beacon PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_executable(beacon_agent src/main.cc)
target_link_libraries(beacon_agent PRIVATE adaptix_beacon)

if(UNIX)
    target_link_libraries(adaptix_beacon PUBLIC pthread)
    target_link_libraries(beacon_agent PRIVATE pthread)
endif()

install(TARGETS adaptix_beacon beacon_agent
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/adaptix DESTINATION include)
