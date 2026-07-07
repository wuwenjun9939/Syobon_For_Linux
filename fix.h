#ifndef FIX_H
#define FIX_H
int actual_main(int argc, char** argv);
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

// ============================================================================
// 🐧 Linux 平台专属适配层
// ============================================================================
#ifndef _WIN32  // 如果不是 Windows 平台（即 Linux/Unix）

    // 1. 解决 Linux 下的 byte 冲突（将 Windows 习惯的 byte 映射为 unsigned char）
    typedef unsigned char dx_old_byte;
    #define byte dx_old_byte

    // 2. 全局命名冲突拦截：把大杀器变量 main 替换掉，防止污染系统入口
    #define main_var _chikuwa_main_var
    #define wait_func _chikuwa_wait_func
    #define wait2_func _chikuwa_wait2_func

    // 3. 抹平 Windows 专属类型：让 Linux 编译器认识 WINAPI 和相关参数类型
    #define WINAPI
    //#define HINSTANCE int
    //#define LPSTR char*

    // 4. 入口函数终极映射：把复杂的 WinMain 签名直接暴力扭转为标准 Linux 的 main 入口
    // 舞台幕后：把 hInstance 等变量拦截后丢弃，强制让编译器生成标准的 main(int argc, char** argv)
    #define WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow) \
        main(int argc, char** argv)

    // 5. 链接阶段桩函数修复：针对你在 WinMain 调用的各种 Linux 版 DxLib 没有的函数进行空实现
    inline int ChangeWindowMode(int Flag) { return 0; }
    inline int SetWindowIconID(int ID) { return 0; }
    inline int SetWindowSizeChangeEnableFlag(int Flag1, int Flag2) { return 0; }
    inline int SetWindowMaximizeButtonEnableFlag(int Flag) { return 0; }
    inline int SetSoundInitParam(int SampleRate, int BitDepth, int ChannelNum) { return 0; }
    //inline int SetOutApplicationLogValidFlag(int Flag) { return 0; }
    inline int SetErrorLogFmtAdd(int Flag) { return 0; }

    // 6. 跨平台音频开关
    #define USE_OPENAL_AUDIO 1

// ============================================================================
// 🪟 Windows 平台专属适配层
// ============================================================================
#else
    // 在 Windows 下编译时，宏全部隐形，恢复原版代码原本的样子
    #define main_var main
    #define wait_func wait
    #define wait2_func wait2

    #define USE_OPENAL_AUDIO 0

#endif // _WIN32

#endif // FIX_H
