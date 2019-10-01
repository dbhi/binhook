cmake_minimum_required(VERSION 3.10)

option(SHOW_RESULTS "Option description" ON)

if(SHOW_RESULTS)
  add_definitions(-DSHOW_RESULTS)
endif()

add_library(drclient SHARED ../../../src/hook.c client.c)
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

find_package(DynamoRIO)
if (NOT DynamoRIO_FOUND)
  message(FATAL_ERROR "DynamoRIO package required to build")
endif(NOT DynamoRIO_FOUND)

use_DynamoRIO_extension(drclient drwrap)

configure_DynamoRIO_client(drclient)
