CXX := gcc
BUILD_DIR := ./bin
SRC_DIR := ./src
INCLUDE_DIR := ./include
MSG_START := "Build Started"
MSG_END := "Build Complete"
MSG_CLEAN := "Cleaning up"

# Windows-specific settings
ifeq ($(OS),Windows_NT)
    os := Windows
    # Use MSYS2 path for dependencies
    SDK := ${MSYS2}
    SDK_PATH := $(subst \,/,$(subst C:\,/c/,$(SDK)))

    # Include paths for headers
    INCLUDES := -I${SDK_PATH}/include -I. -I${INCLUDE_DIR}

    # Library paths
    LIBS := -L${SDK_PATH}/lib

    # Compiler flags
    CXXFLAGS := -std=c99 -Wall -Wextra -g ${INCLUDES}

    # Required libraries for OpenGL/GLFW on Windows
    LIBRARIES := -lglfw3 -lopengl32 -lglu32 -lgdi32 -llibglew32 -luser32 -lkernel32

    TARGET := ${BUILD_DIR}/sampleapp.exe
else
    # Unix-like systems (Linux, macOS)
    os := $(shell uname -s)

    ifeq ($(os),Darwin)
        # macOS-specific settings
        # Include paths - add Homebrew paths
        INCLUDES := -I. -I${INCLUDE_DIR} -I/opt/homebrew/include

        # Library paths - add Homebrew lib directory
       LIBS := -L/opt/homebrew/Cellar/glew/2.2.0_1/lib -L/opt/homebrew/lib


        # Compiler flags
        CXXFLAGS := -std=c99 -Wall -Wextra -g ${INCLUDES}

        # Required frameworks and libraries for macOS
        LIBRARIES := -lglfw -lGLEW -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

        TARGET := ${BUILD_DIR}/sampleapp.bin
    else
        # Linux-specific settings
        # Include paths
        INCLUDES := -I. -I${INCLUDE_DIR}

        # Library paths - add common installation directories
        LIBS := -L/usr/lib -L/usr/local/lib

        # Compiler flags
        CXXFLAGS := -std=c99 -Wall -Wextra -g ${INCLUDES}

        # Required libraries for OpenGL/GLFW on Linux
        LIBRARIES := -lglfw -lGL -lGLU -lGLEW -lm

        TARGET := ${BUILD_DIR}/sampleapp.bin
    endif
endif

# Source files
SRC := $(wildcard ${SRC_DIR}/*.c)
OBJ := $(patsubst ${SRC_DIR}/%.c,${BUILD_DIR}/%.o,${SRC})

# Default target
all: build

# Build target
build: create_build_dir ${TARGET}

# Create build directory
create_build_dir:
	@echo ${MSG_START} ${os}
	@mkdir -p ${BUILD_DIR}

# Link object files to create executable
${TARGET}: ${OBJ}
	@echo "Linking $@"
	${CXX} -o $@ $^ ${LIBS} ${LIBRARIES}
	@echo ${MSG_END}

# Compile source files to object files
${BUILD_DIR}/%.o: ${SRC_DIR}/%.c
	@echo "Compiling $<"
	${CXX} ${CXXFLAGS} -c $< -o $@

# Run the program
run: build
	./${TARGET}

# Clean target
.PHONY: clean
clean:
	@echo ${MSG_CLEAN}
	rm -rf ${BUILD_DIR}
