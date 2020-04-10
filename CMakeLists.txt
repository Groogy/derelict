cmake_minimum_required(VERSION 3.0.0)
project(cpptest 
		VERSION 0.1.0
		LANGUAGES CXX)


# C++ Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# C++ Warnings
if ( CMAKE_COMPILER_IS_GNUCC )
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()
if ( MSVC )
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

# Include Libraries
find_package(SFML 2 COMPONENTS system window graphics audio REQUIRED)

# Source Folders
set(SOURCES
	main.cpp
	assert_linux.cpp
	assert_windows.cpp
)

foreach(FILE ${SOURCES})
	set(PROJECT_SOURCES ${PROJECT_SOURCES} src/${FILE})
endforeach(FILE)

# Build Project
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio sfml-window sfml-system)