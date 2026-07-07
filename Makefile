# ============================================================================
# 🌍 自动检测操作系统
# ============================================================================
ifeq ($(OS),Windows_NT)
    PLATFORM = Windows
else
    PLATFORM = Linux
endif

# 编译器定义
CC = g++
# 目标程序名（Windows下会自动带上 .exe 后缀）
TARGET = syobon

# 基础源代码（两边都要用的核心代码）
BASE_SRCS = main.cpp loadg.cpp

# 编译参数：-include fix.h 强行注入跨平台补丁
CFLAGS = -std=c++11 -O2 -include fix.h -fpermissive

# ============================================================================
# 🐧 Linux 平台配置
# ============================================================================
ifeq ($(PLATFORM), Linux)
    # 加上 Linux 专属的着色器二进制文件
    SRCS = $(BASE_SRCS) \
           DxLibMake/Linux/DxShaderCodeBin_Base_Linux.o \
           DxLibMake/Linux/DxShaderCodeBin_Model_Linux.o \
           DxLibMake/Linux/DxShaderCodeBin_Base3D_Linux.o \
           DxLibMake/Linux/DxShaderCodeBin_Filter_Linux.o \
           DxLibMake/Linux/DxShaderCodeBin_RgbaMix_Linux.o

    # 包含路径与静态库
    CFLAGS += -I./DxLibMake -D DX_GCC_COMPILE
    STATIC_LIB = ./DxLibMake/libDxLib.a

    # Linux 的所有视窗和音频底层依赖
    LIBS = -lX11 -lEGL -lGLESv2 -lopenal -lpng -ljpeg -ltiff \
           -lvorbisfile -lvorbis -logg -lopusfile -lopus \
           -ltheora -ltheoradec -lz -lpthread -ldl -lm

    RM = rm -f
    EXE =
endif

# ============================================================================
# 🪟 Windows 平台配置 (基于 MinGW)
# ============================================================================
ifeq ($(PLATFORM), Windows)
    TARGET := $(TARGET).exe
    SRCS = $(BASE_SRCS)

    # 假设 Windows 用户的 DxLib 放在项目目录的 DxLibWindows 文件夹下
    # -mwindows 参数可以隐藏 Windows 命令行黑窗口，使其作为纯正的窗口程序运行
    CFLAGS += -I./DxLibWindows -D DX_GCC_COMPILE -mwindows
    STATIC_LIB = ./DxLibWindows/libDxLib.a

    # Windows 原生多媒体底层依赖 (GDI, DirectX, WinMM等)
    LIBS = -lopenal32 -lpng -ljpeg -lz -lgdi32 -lole32 -luuid -lwinmm -ld3d9 -ldxguid

    RM = del /f /q
    EXE = .exe
endif

# ============================================================================
# ⚙️ 核心编译规则
# ============================================================================

# 默认规则
all: $(TARGET)

# 编译生成目标程序
$(TARGET): $(SRCS)
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(STATIC_LIB) $(LIBS)

# 伪目标
.PHONY: clean rebuild all

# 清理规则 (自动适配 Linux 的 rm 和 Windows 的 del)
clean:
	$(RM) syobon$(EXE)

# 重建规则
rebuild: clean all
