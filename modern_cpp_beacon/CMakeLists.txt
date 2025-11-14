if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  cmake_minimum_required(VERSION 3.14)
  project(AdaptixBeacon VERSION 1.0.0 LANGUAGES CXX)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()

if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

set(BEACON_SOURCES
    src/beacon.cc
    src/beacon_config.cc
    src/command_executor.cc
    src/http_transport.cc
    src/websocket_transport.cc
    src/task.cc
)

set(BEACON_HEADERS
    include/adaptix/beacon/beacon.h
    include/adaptix/beacon/beacon_config.h
    include/adaptix/beacon/command_executor.h
    include/adaptix/beacon/concepts.h
    include/adaptix/beacon/http_transport.h
    include/adaptix/beacon/websocket_transport.h
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

add_executable(beacon_agent_cpp20 src/main_cpp20.cc)
target_link_libraries(beacon_agent_cpp20 PRIVATE adaptix_beacon)

add_executable(beacon_agent_websocket src/main_websocket.cc)
target_link_libraries(beacon_agent_websocket PRIVATE adaptix_beacon)

if(UNIX)
    target_link_libraries(adaptix_beacon PUBLIC pthread)
    target_link_libraries(beacon_agent PRIVATE pthread)
    target_link_libraries(beacon_agent_cpp20 PRIVATE pthread)
    target_link_libraries(beacon_agent_websocket PRIVATE pthread)
endif()

install(TARGETS adaptix_beacon beacon_agent beacon_agent_cpp20 beacon_agent_websocket
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/adaptix DESTINATION include)
