APP_NAME     := Eun
SRC_DIR      := ./src
OUT_DIR      := ./bin
OBJ_DIR      := ./obj/$(APP_NAME)
OUT_FILE     := $(OUT_DIR)/$(APP_NAME)

SRCFILES     := $(shell find $(SRC_DIR) -type f -name "*.cpp")
JOLT_SRC 		 := $(shell find vendor/JoltPhysics/Jolt -type f -name "*.cpp")
OBJFILES     := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%.o, $(basename $(SRCFILES)))
OBJFILES 		 += $(patsubst vendor/%, $(OBJ_DIR)/vendor/%.o, $(basename $(JOLT_SRC)))

CC           := clang++
CXXFLAGS     := -std=c++20 -g -Wall -Wno-deprecated -MMD -MP
INCLUDES     := -Isrc \
								-Isrc/Common \
								-Ivendor \
								-Ivendor/JoltPhysics \
								-Ivendor/glad/include \
								-I/opt/homebrew/include \
								-I/opt/homebrew/include \
								-Ivendor/JoltPhysics \

LDFLAGS      := vendor/glad/src/glad.c \
								-framework OpenGL \
								/opt/homebrew/Cellar/glfw/3.4/lib/libglfw.3.4.dylib \
								-L/opt/homebrew/lib -lassimp \

all: $(OUT_FILE)

$(OUT_FILE): $(OBJFILES)
	@mkdir -p $(OUT_DIR)
	@echo "  Linking $(OUT_FILE)"
	@$(CC) $(OBJFILES) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "  Compiling $<"
	@$(CC) -c $< -o $@ $(CXXFLAGS) $(INCLUDES)

$(OBJ_DIR)/vendor/%.o: vendor/%.cpp
	@mkdir -p $(dir $@)
	@echo "  Compiling $<"
	@$(CC) -c $< -o $@ $(CXXFLAGS) $(INCLUDES)

clean:
	rm -rf $(OUT_DIR) $(OBJ_DIR)

-include $(OBJFILES:.o=.d)
