CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
INCLUDES = -I./include
SRCDIR = src
OBJDIR = obj
TESTDIR = tests

# 源文件
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# 主程序
MAIN = main
MAIN_SRC = main.cpp
MAIN_OBJ = $(OBJDIR)/main.o

# 测试程序
TEST = test
TEST_SRC = $(TESTDIR)/test.cpp
TEST_OBJ = $(OBJDIR)/test.o

.PHONY: all clean main test run

all: main test

# 创建对象目录
$(OBJDIR):
	mkdir -p $(OBJDIR)

# 编译对象文件
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/main.o: $(MAIN_SRC) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/test.o: $(TEST_SRC) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 链接主程序
main: $(OBJECTS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $(OBJECTS) $(MAIN_OBJ)

# 链接测试程序
test: $(OBJECTS) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST) $(OBJECTS) $(TEST_OBJ)

# 运行主程序
run: main
	./$(MAIN)

# 运行测试
runtest: test
	./$(TEST)

# 清理
clean:
	rm -rf $(OBJDIR) $(MAIN) $(TEST) *.txt *.csv

# 帮助信息
help:
	@echo "可用目标："
	@echo "  make        - 编译主程序和测试程序"
	@echo "  make main   - 只编译主程序"
	@echo "  make test   - 只编译测试程序"
	@echo "  make run    - 编译并运行主程序"
	@echo "  make runtest - 编译并运行测试程序"
	@echo "  make clean  - 清理生成的文件"
	@echo "  make help   - 显示此帮助信息"

