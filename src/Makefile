CC       := g++ 
CFLAGS   := -Wall -pthread -std=c++17
# LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := ../out
OBJ_DIR  := $(BUILD)/obj
APP_DIR  := $(BUILD)/bin
TARGET   := program
#INCLUDE  := -Iinclude/
SRC      := $(wildcard *.cpp) $(wildcard Devices/*.cpp) 
   #$(wildcard src/module1/*.cpp) \
   
OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean debug release info

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CFLAGS += -g3
debug: all

arm: CC = aarch64-linux-gnu-g++-7
arm: all

arm32: CC = arm-linux-gnueabi-g++-7
arm32: all

lars: CC = arm-linux-gnueabi-g++
lars: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*

info:
	@echo "[*] Application dir: ${APP_DIR}     "
	@echo "[*] Object dir:      ${OBJ_DIR}     "
	@echo "[*] Sources:         ${SRC}         "
	@echo "[*] Objects:         ${OBJECTS}     "
	@echo "[*] Dependencies:    ${DEPENDENCIES}"
