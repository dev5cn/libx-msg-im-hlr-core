cmake_minimum_required(VERSION 2.8.12.2)
#
ADD_DEFINITIONS(-DLINUX)
#
INCLUDE_DIRECTORIES(../libmisc-cpp)
INCLUDE_DIRECTORIES(../libxsc-cpp)
INCLUDE_DIRECTORIES(../libxsc-proto-cpp)
INCLUDE_DIRECTORIES(../libx-msg-im-xsc)
INCLUDE_DIRECTORIES(../libx-msg-common-data-struct-cpp)
#
INCLUDE_DIRECTORIES(../libx-msg-im-hlr-pb)
#
INCLUDE_DIRECTORIES($ENV{PROTOBUF}/include)
#
ADD_COMPILE_OPTIONS(-std=c++17 -fPIC -Wall)
ADD_DEFINITIONS(-Wno-builtin-macro-redefined)
SET(CMAKE_CXX_FLAGS $ENV{CXX_FLAGS})
#
FILE(GLOB SRC0 "./*.cpp")
FILE(GLOB SRC1 "./*/*.cpp")
#
PROJECT(x-msg-im-hlr-core)
#
ADD_LIBRARY(x-msg-im-hlr-core STATIC ${SRC0} ${SRC1})
INCLUDE(./__file__.cmake)
REDEFINE_FILE_MACRO(x-msg-im-hlr-core)

