cmake_minimum_required(VERSION 3.20)
project(BeaconC20 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
    -fno-stack-protector
    -fno-strict-overflow
    -fno-delete-null-pointer-checks
    -fno-strict-aliasing
    -fno-builtin
    -fno-exceptions
    -fno-unwind-tables
    -fno-asynchronous-unwind-tables
    -masm=intel
    -fPIC
    -fpermissive
    -w
)

set(BEACON_SOURCES
    Agent.cpp
    AgentConfig.cpp
    AgentInfo.cpp
    ApiLoader.cpp
    Boffer.cpp
    Commander.cpp
    ConnectorHTTP.cpp
    ConnectorSMB.cpp
    ConnectorTCP.cpp
    ConnectorWS.cpp
    Crypt.cpp
    Downloader.cpp
    Encoders.cpp
    JobsController.cpp
    MemorySaver.cpp
    Packer.cpp
    Pivotter.cpp
    ProcLoader.cpp
    Proxyfire.cpp
    WaitMask.cpp
    beacon_functions.cpp
)

set(BEACON_HEADERS
    Agent.h
    AgentConfig.h
    AgentInfo.h
    ApiDefines.h
    ApiLoader.h
    Boffer.h
    Commander.h
    ConnectorHTTP.h
    ConnectorSMB.h
    ConnectorTCP.h
    ConnectorWS.h
    Crypt.h
    Downloader.h
    Encoders.h
    JobsController.h
    MemorySaver.h
    Packer.h
    Pivotter.h
    ProcLoader.h
    Proxyfire.h
    WaitMask.h
    adaptix.h
    beacon.h
    config.h
    defines.h
    main.h
    ntdll.h
    resource.h
    utils.h
)

if(DEFINED BEACON_TYPE)
    message(STATUS "Building beacon with type: ${BEACON_TYPE}")
    add_compile_definitions(${BEACON_TYPE})
else()
    message(STATUS "BEACON_TYPE not specified, defaulting to BEACON_HTTP")
    add_compile_definitions(BEACON_HTTP)
endif()

if(DEFINED BUILD_MODE)
    add_compile_definitions(${BUILD_MODE})
else()
    add_compile_definitions(BUILD_DLL)
endif()

set(TARGET_ARCH x64 CACHE STRING "Target architecture: x64 or x86")
message(STATUS "Target architecture: ${TARGET_ARCH}")

add_library(beacon_object OBJECT ${BEACON_SOURCES})

set_target_properties(beacon_object PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    POSITION_INDEPENDENT_CODE ON
)

if(WIN32 OR MINGW)
    target_link_libraries(beacon_object PUBLIC ws2_32 winsock2)
    target_compile_options(beacon_object PRIVATE -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

target_include_directories(beacon_object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
