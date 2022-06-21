#make sure to run the paired bash script

cmake_minimum_required(VERSION 3.20)


set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_STANDARD 11)


#variables
set(OUTPUT_EXE Terminal3D.out)	#name of final executable

set(SOURCE_LIST ./main.c
	./Controller/KeyBoardStream.c
	./Controller/Controller.c
	./View/Display.c
	./View/Camera.c
	./View/ViewPortMath.c
	./Math/AngleMath.c
	./Math/VectorMath.c
	./Math/MatrixMath.c)



project(${OUTPUT_EXE})
add_executable(${OUTPUT_EXE} ${SOURCE_LIST})


target_include_directories(${OUTPUT_EXE} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${OUTPUT_EXE} 
	PRIVATE -std=gnu11 -g
	PRIVATE -Wall -Wextra -Wpedantic -Werror		#add all warnings by default
	PRIVATE -Wno-dollar-in-identifier-extension -Wno-implicit-function-declaration
	PRIVATE -Wno-unused-parameter -Wunused-function -Wno-sign-compare)	#remove unwanted warnings

target_link_libraries(${OUTPUT_EXE}	m)
