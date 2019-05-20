//
// Created by houde on 2019/5/19.
//

#ifndef FFMPEGMUSIC_LOG_H
#define FFMPEGMUSIC_LOG_H
#include <android/log.h>

#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"FFmpeg_player",FORMAT,##__VA_ARGS__);

#endif //FFMPEGMUSIC_LOG_H
