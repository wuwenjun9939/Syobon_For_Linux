#ifndef FIX_H
#define FIX_H

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

// 1. 解决 byte 冲突
typedef unsigned char dx_old_byte;
#define byte dx_old_byte

// 2. 映射入口函数
#define WinMain(a,b,c,d) actual_main(int argc, char** argv)

// 3. 链接阶段修复：提供空函数体
#ifdef __cplusplus
extern "C" {
#endif

    // 使用 inline 并在 fix.h 直接给出 {} 实现
    // 这样链接器就不会报 undefined reference 了
    inline int ChangeWindowMode(int Flag) { return 0; }
    inline int SetWindowIconID(int ID) { return 0; }

    // 如果后续还有报 undefined 的，也按这个格式写在这里

#ifdef __cplusplus
}
#endif

#endif
