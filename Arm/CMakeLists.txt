cmake_minimum_required (VERSION 2.6)
project (ARM_Client)

option(BuildrootPath "Specify Buildroot project path" "")

# path
set (BASE_PATH ./)
set (SOCKETIO_PATH socket.io-client-cpp/)


if (CMAKE_VERSION VERSION_LESS "3.1")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
  endif ()
else ()
  set (CMAKE_CXX_STANDARD 11)
endif ()

if(IS_DIRECTORY "${BuildrootPath}")
# for cross_compile
set(CMAKE_SYSTEM_NAME Linux)
# where is the target environment
SET(CMAKE_FIND_ROOT_PATH "${BuildrootPath}/output/staging")
# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(BOOST_ROOT "${BuildrootPath}/output/staging/usr")
#set(BOOST_ROOT /usr/local/linaro/arm-linux-gnueabihf/boost)

set(CMAKE_C_COMPILER "${BuildrootPath}/output/host/usr/bin/arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "${BuildrootPath}/output/host/usr/bin/arm-linux-gnueabihf-g++")
endif()

set(CMAKE_CXX_FLAGS "-Og -g ${CMAKE_CXX_FLAGS}")

# Dependency
# boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.61.0 REQUIRED COMPONENTS system thread filesystem)

# pthread
find_package(Threads)

if(Boost_FOUND)

    # include
    include_directories(
        ${BASE_PATH}
        ${BASE_PATH}hw-handle
        ${BASE_PATH}client
        ${SOCKETIO_PATH}src
        ${SOCKETIO_PATH}lib/websocketpp
        ${SOCKETIO_PATH}lib/rapidjson/include
        ${Boost_INCLUDE_DIRS}
        ${JPEG_INCLUDE_DIRS}
    )

    AUX_SOURCE_DIRECTORY(${BASE_PATH}hw-handle SRC_HW_HANDLE)

    # add executable
    set (${PROJECT_NAME}_SRC
        ${SOCKETIO_PATH}src/sio_client.cpp
        ${SOCKETIO_PATH}src/sio_socket.cpp
        ${SOCKETIO_PATH}src/internal/sio_client_impl.cpp
        ${SOCKETIO_PATH}src/internal/sio_packet.cpp
        ${BASE_PATH}client/arm_client.cpp
        ${SRC_HW_HANDLE}
    )
    add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_SRC})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${PTHREAD_LIB})

endif()

# test
# TODO

