#ifndef AUDIO_FIX_H
#define AUDIO_FIX_H

// 确保能读取到 DxLib.h 里的 Mix_Chunk 和 PlaySoundMem
#include "DxLib.h"

// 音频系统修复补丁
// 解决Linux下OpenAL音频死锁问题

// 安全的音频播放函数
inline int SafePlayBGM(int sound_handle) {
    if (sound_handle == -1) return -1;

    // 直接检查实际播放状态
    if (CheckSoundMem(sound_handle) == 0) {
        // 核心修复：通过 long 强制将 int 句柄转换为 SDL_mixer 的 Mix_Chunk 指针
        // 并适应 void 返回值，手动返回 0 满足原本的 int result 逻辑
        Mix_Chunk* chunk = (Mix_Chunk*)(long)sound_handle;
        PlaySoundMem(chunk, DX_PLAYTYPE_LOOP);
        return 0;
    }

    return 0;
}

// 安全的音频停止函数
inline void SafeStopBGM(int sound_handle) {
    if (sound_handle != -1) {
        StopSoundMem(sound_handle);
    }
}

#endif // AUDIO_FIX_H
