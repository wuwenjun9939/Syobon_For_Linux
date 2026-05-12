# 编译器与标准
CC = g++
STD = -std=c++11

# 目标文件名
TARGET = syobon

# 源代码文件（根据你的脚本，包含 main.cpp 和 loadg.cpp）
SRCS = main.cpp loadg.cpp

# 编译参数
# -include fix.h 强制包含修复头文件
# -I./DxLibMake 包含库头文件路径
# -fpermissive 允许老代码的一些非标准写法
CFLAGS = $(STD) -include fix.h -I./DxLibMake -D DX_GCC_COMPILE -fpermissive

# 静态库路径
STATIC_LIB = ./DxLibMake/libDxLib.a

# 动态链接库（底层依赖）
LIBS = -lX11 -lEGL -lGLESv2 -lopenal -lpng -ljpeg -ltiff \
       -lvorbisfile -lvorbis -logg -lopusfile -lopus \
       -ltheora -ltheoradec -lz -lpthread -ldl -lm

# 编译规则
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(STATIC_LIB) $(LIBS)

# 清理
clean:
	rm -f $(TARGET)

.PHONY: all clean
