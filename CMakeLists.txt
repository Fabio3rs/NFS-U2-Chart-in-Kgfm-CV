cmake_minimum_required(VERSION 3.10)
project(NFS-U2-Chart-in-Kgfm-CV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SYSTEM injector/include)

add_library(nfsu2_chart SHARED src/dllmain.cpp)

if(WIN32)
  # On Windows produce an .asi (common for GTA mod loaders) without the default "lib" prefix
  set_target_properties(nfsu2_chart PROPERTIES PREFIX "" SUFFIX ".asi")
endif()
