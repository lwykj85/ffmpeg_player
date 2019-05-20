////
//// Created by houde on 2019/5/19.
////
//
//#include "FFmpegPlayer.h"
//FFmpegPlayer::FFmpegPlayer(const char* path) {
//    pthread_mutex_init(&mutex, NULL);
//    pthread_cond_init(&cond, NULL);
//    //这个这个地方进行初始化,打开文件
//    openFile(path);
//}
//FFmpegPlayer::~FFmpegPlayer() {
//    pthread_cond_destroy(&cond);
//    pthread_mutex_destroy(&mutex);
//}
//
//void FFmpegPlayer::openFile(const char *path) {
//    av_register_all();
//    avformat_network_init();
//    pFormatCxt = avformat_alloc_context();
//    if(avformat_open_input(&pFormatCxt,path,NULL,NULL) != 0){
//        LOGE("%s","打开文件失败");
//        return ;
//    }
//    if(avformat_find_stream_info(pFormatCxt,NULL) < 0){
//        LOGE("%s","获取视频信息失败");
//        return;
//    }
//    av_dump_format(pFormatCxt,0,path,0);
//    for(int i = 0 ; i < pFormatCxt->nb_streams; i++){
//        AVCodecParameters* params = pFormatCxt->streams[i]->codecpar;
//        if(params->codec_type == AVMEDIA_TYPE_VIDEO){
//            videoIndex = i;
//            AVCodecID  id = params->codec_id;
//            AVCodec* codec = avcodec_find_decoder(id);
//            pVideoCodecCxt = avcodec_alloc_context3(codec);
//            avcodec_parameters_to_context(pVideoCodecCxt,params);
//            videoTimeBase = pVideoCodecCxt->time_base;
//        }else if(params->codec_type == AVMEDIA_TYPE_AUDIO){
//            audioIndex = i;
//            AVCodecID  id = params->codec_id;
//            AVCodec* codec = avcodec_find_decoder(id);
//            pAudioCodecCxt = avcodec_alloc_context3(codec);
//            avcodec_parameters_to_context(pAudioCodecCxt,params);
//            audioTimeBase = pAudioCodecCxt->time_base;
//        }
//    }
//    //输出视频信息
//    LOGD("视频的文件格式：%s",pFormatCxt->iformat->name);
//    LOGD("视频时长：%ld", (pFormatCxt->duration* av_q2d(pFormatCxt->streams[videoIndex]->time_base))  /1000000);//* av_q2d(pFormatCxt->streams[videoIndex]->time_base)/1000000
//    LOGD("视频的宽高：%d,%d",pVideoCodecCxt->width,pVideoCodecCxt->height);
//    LOGD("解码器的名称：%s",pVideoCodecCxt->codec->name);
//};
//extern "C" JNIEXPORT
//int FFmpegPlayer::getAudioPacket(AVPacket *avPacket) {
//    LOGE("取出队列")
//    pthread_mutex_lock(&mutex);
//    while (isPlay){
//        LOGE("取出对垒 xxxxxx")
//        if(!audioQueue.empty()&&isPlay){
//            LOGE("ispause %d",isPlay);
//            //如果队列中有数据可以拿出来
//            if(av_packet_ref(avPacket,audioQueue.front())){
//                break;
//            }
//            //取成功了，弹出队列，销毁packet
//            AVPacket *packet2 = audioQueue.front();
//            audioQueue.erase(audioQueue.begin());
//            av_free(packet2);
//            break;
//        } else{
//            LOGE("音频执行wait")
//            pthread_cond_wait(&cond,&mutex);
//        }
//    }
//    pthread_mutex_unlock(&mutex);
//    return 0;
//}
//int getPcm(void **pcm,size_t *pcm_size,FFmpegPlayer* player){
//    AVPacket *packet = (AVPacket *) av_mallocz(sizeof(AVPacket));
//    AVFrame *frame = av_frame_alloc();
//    int size;
//    int gotframe;
//    LOGE("准备解码")
//    //这个循环只是用于解码一个packet包
//    while (player->isPlay){
//        size=0;
//        player->getAudioPacket(packet);
//        //时间矫正
//        if (packet->pts != AV_NOPTS_VALUE) {
//            player->audioClock = av_q2d(player->audioTimeBase) * packet->pts;
//        }
//        //            解码  mp3   编码格式frame----pcm   frame
//        int aa = avcodec_send_packet(player->pAudioCodecCxt,packet);
//        if(aa == 0){
//            avcodec_receive_frame(player->pAudioCodecCxt,frame);
//
//            swr_convert(player->swrContext, &player->out_buffer, 44100 * 2, (const uint8_t **) frame->data, frame->nb_samples);
////                缓冲区的大小
//            size = av_samples_get_buffer_size(NULL, player->pAudioCodecCxt->channels, frame->nb_samples,
//                                              AV_SAMPLE_FMT_S16, 1);
//            break;
//        }
//    }
//    av_free(packet);
//    av_frame_free(&frame);
//    return size;
//}
//void getQueueCallBack(SLAndroidSimpleBufferQueueItf  slBufferQueueItf, void* context){
//    FFmpegPlayer* player = (FFmpegPlayer * ) context;
//    size_t buffersize=0;
//    void* buffer;
//    getPcm(&buffer,&buffersize,player);
//    if(buffer!=NULL&&buffersize!=0){
//        //将得到的数据加入到队列中
//        (*slBufferQueueItf)->Enqueue(slBufferQueueItf,buffer,buffersize);
//    }
//}
//void *musicPlay(void* args){
//    FFmpegPlayer* player = (FFmpegPlayer*)args;
//    player->out_buffer = (uint8_t *) av_malloc(44100 * 2);
//    slCreateEngine(&player->engineObject,0,NULL,0,NULL,NULL);//创建引擎
//    (*player->engineObject)->Realize(player->engineObject,SL_BOOLEAN_FALSE);//实现engineObject接口对象
//    (*player->engineObject)->GetInterface(player->engineObject,SL_IID_ENGINE,&player->engineEngine);//通过引擎调用接口初始化SLEngineItf
//    SLDataLocator_AndroidBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,2};
//    /**
//    typedef struct SLDataFormat_PCM_ {
//        SLuint32 		formatType;  pcm
//        SLuint32 		numChannels;  通道数
//        SLuint32 		samplesPerSec;  采样率
//        SLuint32 		bitsPerSample;  采样位数
//        SLuint32 		containerSize;  包含位数
//        SLuint32 		channelMask;     立体声
//        SLuint32		endianness;    end标志位
//    } SLDataFormat_PCM;
//     */
//
//    SLDataFormat_PCM pcm = {SL_DATAFORMAT_PCM,(SLuint32)player->pAudioCodecCxt->channels,(SLuint32)player->pAudioCodecCxt->sample_rate*1000
//            ,SL_PCMSAMPLEFORMAT_FIXED_16
//            ,SL_PCMSAMPLEFORMAT_FIXED_16
//            ,SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,SL_BYTEORDER_LITTLEENDIAN};
//
//    /*
//     * typedef struct SLDataSource_ {
//	        void *pLocator;//缓冲区队列
//	        void *pFormat;//数据样式,配置信息
//        } SLDataSource;
//     * */
//    SLDataSource dataSource = {&android_queue,&pcm};
//
//
//    SLDataLocator_OutputMix slDataLocator_outputMix={SL_DATALOCATOR_OUTPUTMIX,player->outputMixObject};
//
//
//    SLDataSink slDataSink = {&slDataLocator_outputMix,NULL};
//
//
//    const SLInterfaceID ids[3]={SL_IID_BUFFERQUEUE,SL_IID_EFFECTSEND,SL_IID_VOLUME};
//    const SLboolean req[3]={SL_BOOLEAN_FALSE,SL_BOOLEAN_FALSE,SL_BOOLEAN_FALSE};
//
//    /*
//     * SLresult (*CreateAudioPlayer) (
//		SLEngineItf self,
//		SLObjectItf * pPlayer,
//		SLDataSource *pAudioSrc,//数据设置
//		SLDataSink *pAudioSnk,//关联混音器
//		SLuint32 numInterfaces,
//		const SLInterfaceID * pInterfaceIds,
//		const SLboolean * pInterfaceRequired
//	);
//     * */
//    (*player->engineEngine)->CreateAudioPlayer(player->engineEngine,&player->audioplayer,&dataSource,&slDataSink,3,ids,req);
//    (*player->audioplayer)->Realize(player->audioplayer,SL_BOOLEAN_FALSE);
//
//    (*player->audioplayer)->GetInterface(player->audioplayer,SL_IID_PLAY,&player->slPlayItf);//初始化播放器
//    //注册缓冲区,通过缓冲区里面 的数据进行播放
//    (*player->audioplayer)->GetInterface(player->audioplayer,SL_IID_BUFFERQUEUE,&player->slBufferQueueItf);
//    //设置回调接口
//    (*player->slBufferQueueItf)->RegisterCallback(player->slBufferQueueItf,getQueueCallBack,player);
//    //播放
//    (*player->slPlayItf)->SetPlayState(player->slPlayItf,SL_PLAYSTATE_PLAYING);
//
//    //开始播放
//    getQueueCallBack(player->slBufferQueueItf,player);
//}
//int FFmpegPlayer::putVideoPacket(AVPacket *avPacket) {
//    LOGE("插入队列 video")
//    AVPacket *avPacket1 = (AVPacket *) av_mallocz(sizeof(AVPacket));
//    //克隆
//    if(av_packet_ref(avPacket1,avPacket)){
//        //克隆失败
//        return 0;
//    }
//    //push的时候需要锁住，有数据的时候再解锁
//    pthread_mutex_lock(&mutex);
//    videoQueue.push_back(avPacket1);//将packet压入队列
//    //压入过后发出消息并且解锁
//    pthread_cond_signal(&cond);
//    pthread_mutex_unlock(&mutex);
//    return 1;
//}
//int FFmpegPlayer::getVideoPacket(AVPacket *avPacket) {
//    LOGE("取出队列")
//    pthread_mutex_lock(&mutex);
//    while (isPlay){
//        if(!videoQueue.empty()){
//            //如果队列中有数据可以拿出来
//            if(av_packet_ref(avPacket,videoQueue.front())){
//                break;
//            }
//            //取成功了，弹出队列，销毁packet
//            AVPacket *packet2 = videoQueue.front();
//            videoQueue.erase(videoQueue.begin());
//            av_free(packet2);
//            break;
//        } else{
//            pthread_cond_wait(&cond,&mutex);
//        }
//    }
//    LOGE("解锁")
//    pthread_mutex_unlock(&mutex);
//    return 0;
//}
//
//static void (*video_call)(AVFrame *frame);
//
//void *videoPlay(void *args){
//    FFmpegPlayer *ffmpegVideo = (FFmpegPlayer *) args;
//    //申请AVFrame
//    AVFrame *frame = av_frame_alloc();//分配一个AVFrame结构体,AVFrame结构体一般用于存储原始数据，指向解码后的原始帧
//    AVFrame *rgb_frame = av_frame_alloc();//分配一个AVFrame结构体，指向存放转换成rgb后的帧
//    AVPacket *packet = (AVPacket *) av_mallocz(sizeof(AVPacket));
//    //输出文件
//    //FILE *fp = fopen(outputPath,"wb");
//
//
//    //缓存区
//    uint8_t  *out_buffer= (uint8_t *)av_mallocz(avpicture_get_size(AV_PIX_FMT_RGBA,
//                                                                   ffmpegVideo->pVideoCodecCxt->width,ffmpegVideo->pVideoCodecCxt->height));
//    //与缓存区相关联，设置rgb_frame缓存区
//    avpicture_fill((AVPicture *)rgb_frame,out_buffer,AV_PIX_FMT_RGBA,ffmpegVideo->pVideoCodecCxt->width,ffmpegVideo->pVideoCodecCxt->height);
//
//
//    LOGE("转换成rgba格式")
//    ffmpegVideo->swsContext = sws_getContext(ffmpegVideo->pVideoCodecCxt->width,ffmpegVideo->pVideoCodecCxt->height,ffmpegVideo->pVideoCodecCxt->pix_fmt,
//                                             ffmpegVideo->pVideoCodecCxt->width,ffmpegVideo->pVideoCodecCxt->height,AV_PIX_FMT_RGBA,
//                                             SWS_BICUBIC,NULL,NULL,NULL);
//
//
//
//    LOGE("LC XXXXX  %f",ffmpegVideo->pVideoCodecCxt);
//
//    double  last_play  //上一帧的播放时间
//    ,play             //当前帧的播放时间
//    ,last_delay    // 上一次播放视频的两帧视频间隔时间
//    ,delay         //两帧视频间隔时间
//    ,audio_clock //音频轨道 实际播放时间
//    ,diff   //音频帧与视频帧相差时间
//    ,sync_threshold
//    ,start_time  //从第一帧开始的绝对时间
//    ,pts
//    ,actual_delay//真正需要延迟时间
//    ;
//
//    //从第一帧开始的绝对时间
//    start_time = av_gettime() / 1000000.0;
//    int frameCount;
//    LOGE("解码 ")
//    while (ffmpegVideo->isPlay) {
//        ffmpegVideo->getVideoPacket(packet);
//        LOGE("解码 %d",packet->stream_index)
//        avcodec_decode_video2(ffmpegVideo->pVideoCodecCxt, frame, &frameCount, packet);
//        if(!frameCount){
//            continue;
//        }
//        //转换为rgb格式
//        sws_scale(ffmpegVideo->swsContext,(const uint8_t *const *)frame->data,frame->linesize,0,
//                  frame->height,rgb_frame->data,
//                  rgb_frame->linesize);
//        LOGE("frame 宽%d,高%d",frame->width,frame->height);
//        LOGE("rgb格式 宽%d,高%d",rgb_frame->width,rgb_frame->height);
//
//        if((pts=av_frame_get_best_effort_timestamp(frame))==AV_NOPTS_VALUE){
//            pts=0;
//        }
//
//        play = pts*av_q2d(ffmpegVideo->videoTimeBase);
//        //纠正时间
//        play = ffmpegVideo->synchronize(frame,play);
//
//        delay = play - last_play;
//        if (delay <= 0 || delay > 1) {
//            delay = last_delay;
//        }
//        audio_clock = ffmpegVideo->audioClock;
//        last_delay = delay;
//        last_play = play;
////音频与视频的时间差
//        diff = ffmpegVideo->audioClock - audio_clock;
////        在合理范围外  才会延迟  加快
//        sync_threshold = (delay > 0.01 ? 0.01 : delay);
//
//        if (fabs(diff) < 10) {
//            if (diff <= -sync_threshold) {
//                delay = 0;
//            } else if (diff >=sync_threshold) {
//                delay = 2 * delay;
//            }
//        }
//        start_time += delay;
//        actual_delay=start_time-av_gettime()/1000000.0;
//        if (actual_delay < 0.01) {
//            actual_delay = 0.01;
//        }
//        av_usleep(actual_delay*1000000.0+6000);
//        LOGE("播放视频")
//        //利用这个回调是把解码后的rgb数据显示到界面上
//        video_call(rgb_frame);
////        av_packet_unref(packet);
////        av_frame_unref(rgb_frame);
////        av_frame_unref(frame);
//    }
//    LOGE("free packet");
//    av_free(packet);
//    LOGE("free packet ok");
//    LOGE("free packet");
//    av_frame_free(&frame);
//    av_frame_free(&rgb_frame);
//    sws_freeContext(ffmpegVideo->swsContext);
//    size_t size = ffmpegVideo->videoQueue.size();
//    for (int i = 0; i < size; ++i) {
//        AVPacket *pkt = ffmpegVideo->videoQueue.front();
//        av_free(pkt);
//        ffmpegVideo->videoQueue.erase(ffmpegVideo->videoQueue.begin());
//    }
//    LOGE("VIDEO EXIT");
//    pthread_exit(0);
//
//}
//void FFmpegPlayer::setPlayCall(void (*call)(AVFrame *)) {
//        video_call=call;
//}
//void *demux(void* args){
//    FFmpegPlayer* player = (FFmpegPlayer*)args;
//    //开启一个线程开始播放音频
//    pthread_create(player->videoThread,NULL,musicPlay,player);
//    //开启一个线程开始播放视频
//    pthread_create(player->videoThread,NULL,videoPlay,player);
//
//    AVPacket * packet = av_packet_alloc();
//    int ret;
//    while (player->isPlay){
//        ret = av_read_frame(player->pFormatCxt,packet);
//        if(packet->stream_index == player->videoIndex){
//            player->putVideoPacket(packet);
//        }else if(packet->stream_index == player->audioIndex){
//            player->putAudioPacket(packet);
//        }else if (ret == AVERROR_EOF) {
//            // 读完了
//            //读取完毕 但是不一定播放完毕
//            while (player->isPlay) {
//                if (player->videoQueue.empty() && player->videoQueue.empty()) {
//                    break;
//                }
//                // LOGE("等待播放完成");
//                av_usleep(10000);
//            }
//        }
//        av_packet_unref(packet);
//    }
//    //解码完过后可能还没有播放完
//    player->isPlay = 0;
//    if(packet){
//        av_packet_free(&packet);
//    }
//    if(player->swsContext){
//    }
//    if(player->pVideoCodecCxt){
//        avcodec_free_context(&player->pVideoCodecCxt);
//    }
//    if(player->pVideoCodecCxt){
//        avcodec_free_context(&player->pAudioCodecCxt);
//    }
//    if(player->pFormatCxt) {
//        avformat_free_context(player->pFormatCxt);
//    }
//    pthread_exit(0);
//}
//double FFmpegPlayer::synchronize(AVFrame *frame, double play) {
//    //clock是当前播放的时间位置
//    if (play != 0)
//        videoClock=play;
//    else //pst为0 则先把pts设为上一帧时间
//        play = videoClock;
//    //可能有pts为0 则主动增加clock
//    //frame->repeat_pict = 当解码时，这张图片需要要延迟多少
//    //需要求出扩展延时：
//    //extra_delay = repeat_pict / (2*fps) 显示这样图片需要延迟这么久来显示
//    double repeat_pict = frame->repeat_pict;
//    //使用AvCodecContext的而不是stream的
//    double frame_delay = av_q2d(pVideoCodecCxt->time_base);
//    //如果time_base是1,25 把1s分成25份，则fps为25
//    //fps = 1/(1/25)
//    double fps = 1 / frame_delay;
//    //pts 加上 这个延迟 是显示时间
//    double extra_delay = repeat_pict / (2 * fps);
//    double delay = extra_delay + frame_delay;
////    LOGI("extra_delay:%f",extra_delay);
//    videoClock += delay;
//    return play;
//}
////将packet压入队列,生产者
//int FFmpegPlayer::putAudioPacket(AVPacket *avPacket) {
//    LOGE("插入队列")
//    AVPacket *avPacket1 = (AVPacket *) av_mallocz(sizeof(AVPacket));
//    //克隆
//    if(av_packet_ref(avPacket1,avPacket)){
//        //克隆失败
//        return 0;
//    }
//    //push的时候需要锁住，有数据的时候再解锁
//    pthread_mutex_lock(&mutex);
//    audioQueue.push_back(avPacket1);//将packet压入队列
//    //压入过后发出消息并且解锁
//    pthread_cond_signal(&cond);
//    pthread_mutex_unlock(&mutex);
//    return 1;
//}
//void FFmpegPlayer::play() {
//    LOGE("%s","play thread start");
//    pthread_create(demuxThread,NULL,demux,this);
//    LOGE("%s","play thread end");
//
//}
//
