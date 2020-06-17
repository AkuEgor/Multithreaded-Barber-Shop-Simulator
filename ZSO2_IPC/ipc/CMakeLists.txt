cmake_minimum_required(VERSION 3.2)
set(CMAKE_C_COMPILER "/usr/bin/gcc")
project(ipc_node_communication_c)

include_directories(include)

set(SOURCES 
	src/ipc_main.c
	src/ipc_node.c
	src/message_queue.c
)

add_executable(ipc_node_communication ${SOURCES})

install(TARGETS ipc_node_communication DESTINATION ${CMAKE_SOURCE_DIR}/bin)