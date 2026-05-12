# 编译器定义
CC = g++

# 目标程序名
TARGET = syobon

# 源代码
SRCS = main.cpp loadg.cpp DxLibMake/Linux/DxShaderCodeBin_Base_Linux.o DxLibMake/Linux/DxShaderCodeBin_Model_Linux.o DxLibMake/Linux/DxShaderCodeBin_Base3D_Linux.o DxLibMake/Linux/DxShaderCodeBin_Filter_Linux.o DxLibMake/Linux/DxShaderCodeBin_RgbaMix_Linux.o

# 静态库路径
STATIC_LIB = ./DxLibMake/libDxLib.a

# 编译参数 (保留你之前的所有硬核设置)
# -fpermissive: 对老代码宽容
# -include fix.h: 强行注入你的 Linux 修复补丁
CFLAGS = -std=c++11 -O2 -include fix.h -I./DxLibMake -D DX_GCC_COMPILE -fpermissive

# 链接库 (所有的依赖库)
LIBS = -lX11 -lEGL -lGLESv2 -lopenal -lpng -ljpeg -ltiff \
       -lvorbisfile -lvorbis -logg -lopusfile -lopus \
       -ltheora -ltheoradec -lz -lpthread -ldl -lm

# --- 核心规则 ---

# 默认规则：输入 make 就会执行这里
all: $(TARGET)

# 编译规则
$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(STATIC_LIB) $(LIBS)

# 伪目标：强制重新编译 (即使文件已经是最新)
.PHONY: clean rebuild all

# 清理规则：输入 make clean 就会删掉生成的程序
clean:
	rm -f $(TARGET)

# 重建规则：输入 make rebuild 就会先清理再编译
rebuild: clean all
