#ifndef AUDIO_FIX_H
#define AUDIO_FIX_H

// 音频系统修复补丁
// 解决Linux下OpenAL音频死锁问题

// 安全的音频播放函数
// 每次调用都会检查实际播放状态，避免状态不同步导致死锁
inline int SafePlayBGM(int sound_handle) {
    if (sound_handle == -1) return -1;
    
    // 直接检查实际播放状态，不使用缓存计数器
    if (CheckSoundMem(sound_handle) == 0) {
        int result = PlaySoundMem(sound_handle, DX_PLAYTYPE_LOOP);
        return result;
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