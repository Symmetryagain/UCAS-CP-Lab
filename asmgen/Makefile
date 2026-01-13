# 可执行文件名称
EXEC = asmgen

# 源文件
SRCS = $(wildcard ./src/*.cpp)
OBJS = $(patsubst ./src/%.cpp, build/%.o, $(SRCS))
DEPS = $(patsubst ./src/%.cpp, build/%.d, $(SRCS))

# 构建目录
BUILD_DIR = ./build
TEMP_DIR = $(BUILD_DIR)/temp

# 默认参数
IR_FILE ?= /home/skyhgzsh/cplab/asmgen/070_multiplication_puzzle.ir
ASM_FILE ?= $(TEMP_DIR)/test.s
ELF_FILE ?= $(TEMP_DIR)/test.elf

# 编译器配置
CXX = g++
CXXFLAGS = -std=c++23 -ggdb3 -I./src
RISCV_CXX = riscv64-unknown-elf-gcc
RISCV_OBJDUMP = riscv64-unknown-elf-objdump

# 创建构建目录
$(shell mkdir -p $(BUILD_DIR) $(TEMP_DIR))

# 默认目标：构建可执行文件
default: $(BUILD_DIR)/$(EXEC)

# 链接可执行文件
$(BUILD_DIR)/$(EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

# 编译源文件到目标文件
build/%.o: ./src/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# 生成依赖文件
build/%.d: ./src/%.cpp
	@$(CXX) -MM -MT "$(patsubst ./src/%.cpp, build/%.o, $<)" $< > $@

# 包含依赖文件
-include $(DEPS)

# 测试目标
test: default
	@echo "Generating assembly..."
	$(BUILD_DIR)/$(EXEC) $(ASM_FILE) $(IR_FILE)
	@echo "Compiling to RISC-V ELF..."
	$(RISCV_CXX) $(ASM_FILE) -L../lib -lcactio -o $(ELF_FILE)
	@echo "Generating disassembly..."
	$(RISCV_OBJDUMP) -D $(ELF_FILE) > $(ELF_FILE).dump

	@echo "Running on spike..."
	spike pk -s $(ELF_FILE)
	@echo "Test completed!"

# 优化级别 O1 的测试
test_o1: default
	@echo "Generating assembly with O1 optimization..."
	$(BUILD_DIR)/$(EXEC) $(ASM_FILE) $(IR_FILE) -O1
	@echo "Compiling to RISC-V ELF..."
	$(RISCV_CXX) $(ASM_FILE) -L../lib -lcactio -o $(ELF_FILE)
	@echo "Generating disassembly..."
	$(RISCV_OBJDUMP) -D $(ELF_FILE) > $(ELF_FILE).dump

	@echo "Running on spike..."
	spike pk -s $(ELF_FILE)
	@echo "Test completed!"

run: default
	$(BUILD_DIR)/$(EXEC) $(ASM_FILE) $(IR_FILE)

run_o1: default
	$(BUILD_DIR)/$(EXEC) $(ASM_FILE) $(IR_FILE) -O1

gdb: default
	gdb --args $(BUILD_DIR)/$(EXEC) $(ASM_FILE) $(IR_FILE)

gdb_o1: default
	gdb --args $(BUILD_DIR)/$(EXEC) $(ASM_FILE) $(IR_FILE) -O1

# 清理所有生成的文件
clean:
	rm -rf $(BUILD_DIR)

# 仅清理测试生成的文件
clean-test:
	rm -rf $(TEMP_DIR)

# 显示构建信息
info:
	@echo "Source files: $(SRCS)"
	@echo "Object files: $(OBJS)"
	@echo "Dependency files: $(DEPS)"
	@echo "Build directory: $(BUILD_DIR)"
	@echo "Temp directory: $(TEMP_DIR)"

# 伪目标声明
.PHONY: default clean clean-test test test_o1 info gdb gdb_o1 run run_o1