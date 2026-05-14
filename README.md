# Syobon Action (しょぼんのアクション) For Linux

![Platform](https://img.shields.io/badge/platform-Linux-orange.svg)
![Language](https://img.shields.io/badge/language-C++-blue.svg)

这是一个为 Linux 系统深度适配的《猫里奥》（Syobon Action）源码版。

本项目基于日本开发者 **Chikuwa (ちくわ)** 的原始代码，针对 Linux 环境下的文件系统特性、音频后端以及输入映射进行了全方位的修复。

## 🛠️ 针对 Linux 的修复项 (Fixes)

作为一名“重度 Windows 患者”，在 Linux 下编译此项目时我遇到了不少令人脑壳痛的问题，现已全部解决：

- **大小写敏感修复 (Case-Sensitivity)**：
  - Linux 的 ext4 等文件系统对大小写极其敏感。
  - 修复了代码中 `res/Player.png` 无法加载 `res/player.png` 的问题。
  - 统一了 `res/`, `SE/`, `BGM/` 文件夹内所有资源的文件名。
- **输入系统适配 (Input Mapping)**：
  - 修复了 `DxLibForLinux` 下 `GetJoypadInputState` 无法直接读取键盘方向键的 Bug。
  - 通过 `CheckHitKey` 手动同步状态，确保了原汁原味的按键体验。
- **音频系统优化 (Audio Backend)**：
  - 统一了音频路径解析，修复了 BGM 无法播放的问题。

## 🚀 如何编译 (How to Build)

### 1. 安装依赖 (Dependencies)

确保你的系统已安装 `SDL2` 相关开发库（以 Ubuntu 为例）：

```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-mixer-dev libsdl2-image-dev libx11-dev
```
### 2、编译并运行
- **Linux用户**：运行`make`即可看到`syobon`程序，双击即可打开
- **Windows用户**：请运行`gcc main.cpp`即可看到`test.exe`程序，双击即可打开
- **其他系统**：目前不知道`make`能不能用（主要是没其他系统）但可尝试编译看看

## ⚠️ 已知缺陷 (Known Issues)

- **窗口最大化黑屏**：由于 DxLib 底层渲染限制，点击窗口最大化会导致渲染上下文丢失（黑屏）。目前建议在窗口模式下运行，请勿尝试拉伸窗口。
- **无需做任何事？**：那是编译器抽风，运行以下指令就好了
```bash
make rebuild
```

### 小插曲
- **趣闻**: 在这个源码里面有一个`Microsoft AppLocale`，那个是原作者留给以前Windows（非日文系统）的小工具，专门用来解决日语乱码问题的，现在已经不需要了
## 操作说明：
- **方向键**：移动 / 蹲下
- **Z键**：跳跃
- **O键**：自杀（如果你被困在里面）
- **F1**：返回标题界面
- **备注**：按方向键`↑`跟`Z`是一样的效果
## 玩法：
想必都知道`超级马里奥`这款经典游戏吧？这个可以说是坑爹版的
### 📜 致谢 (Credits)
- Original Author: Chikuwa (ちくわ)
- Library: [dxlib-for-linux](https://github.com/dragoon2014/dxlib-for-linux)
- Fixes: [wuwenjun9939](https://github.com/wuwenjun9939)

# 注：本项目仅供学习与交流使用，请勿用于商业用途。
