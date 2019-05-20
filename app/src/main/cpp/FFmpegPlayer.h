////
//// Created by houde on 2019/5/19.
////
//
//#ifndef FFMPEGPLAYER_FFMPEGPLAYER_H
//#define FFMPEGPLAYER_FFMPEGPLAYER_H
//#include "vector"
//#include <pthread.h>
//extern "C" {
//#include <SLES/OpenSLES_Android.h>
//#include <android/log.h>
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libswresample/swresample.h>
//#include <libswscale/swscale.h>
//#include "libavutil/time.h"
//#include <SLES/OpenSLES_Android.h>
//#include <android/native_window.h>
//#include <android/native_window_jni.h>
//
//#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg_player",FORMAT,##__VA_ARGS__);
//#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO,"ffmpeg_test",FORMAT,##__VA_ARGS__);
//
//class FFmpegPlayer {
//public:
//    int audioIndex, videoIndex;
//    AVCodecContext *pAudioCodecCxt;
//    AVCodecContext *pVideoCodecCxt;
//    std::vector<AVPacket *> audioQueue;
//    std::vector<AVPacket *> videoQueue;
//    AVFormatContext *pFormatCxt;
//    SwrContext *swrContext;//音频冲采样
//    SwsContext *swsContext;//视频缩放
//    double audioClock;//audio 播放的事件
//    double videoClock;
//    //同步锁
//    pthread_mutex_t mutex;
//    //条件变量
//    pthread_cond_t cond;
//    pthread_t *demuxThread;//解复用的线程
//    pthread_t *videoThread;//视频解码线程
//    pthread_t *aduioThread;//视频解码播放线程
//    AVRational videoTimeBase;//视频时间基
//    AVRational audioTimeBase;//音频时间基
//    int isPlay = 1;
//    SLObjectItf engineObject;
//    SLEngineItf engineEngine;
//    SLEnvironmentalReverbItf outputMixEnvironmentalReverb;
//    SLObjectItf outputMixObject;
//    SLObjectItf audioplayer;
//    SLEffectSendItf bqPlayerEffectSend;
//    SLVolumeItf bqPlayerVolume;
//    SLPlayItf slPlayItf;
//    SLAndroidSimpleBufferQueueItf slBufferQueueItf;
//    uint8_t *out_buffer;
//
//public:
//    FFmpegPlayer(const char *path);
//
//    ~FFmpegPlayer();
//
//    int getAudioPacket(AVPacket *pkt);
//
//    int getVideoPacket(AVPacket *pkt);
//
//    int putAudioPacket(AVPacket *pkt);
//
//    int putVideoPacket(AVPacket *pkt);
//
//    void play();
//
//    double synchronize(AVFrame *frame, double play);
//
//    //用于回调在主线程显示画面
//    void setPlayCall(void (*call)(AVFrame *frame));
//
//    void openFile(const char *path);
//};
//};
//#endif //FFMPEGPLAYER_FFMPEGPLAYER_H
