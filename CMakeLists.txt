cmake_minimum_required(VERSION 3.10)

project(CamPlay)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------COMPILER SPECIFIC---------------- #

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "-O3 -Wall -flto -fsanitize=address -stdlib=libc++")

elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "-O3 -Wall -flto -fsanitize=address -s")

elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#	Nobody Uses Windows Anyways ;)
#	set(CMAKE_CXX_FLAGS "/O1 /strip /Wall")
	MESSAGE("For MSVC you need to Compile Manually !!")
endif()

# ----------------------------------------------- #

add_executable(CamPlay src/main.cpp)
target_link_libraries(CamPlay tflite)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/app_configs
	 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
