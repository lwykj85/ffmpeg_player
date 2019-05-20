package com.houde.ffmpeg.player

import android.content.Context
import android.util.AttributeSet
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView

/**
 * @author : Houde
 * Date : 2019/5/19 10:44
 * Desc :
 */
class VideoView (context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0) :
        SurfaceView(context, attrs, defStyleAttr), SurfaceHolder.Callback  {
    override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
    }

    override fun surfaceDestroyed(holder: SurfaceHolder?) {
    }

    override fun surfaceCreated(holder: SurfaceHolder?) {
    }

    constructor(context: Context):this(context,null)
    constructor(context: Context,attrs: AttributeSet?):this(context,attrs,0)
    private var surface : Surface = holder.surface
    private var player:Long = 0
    var path:String? = null
    set(value) {
        if(value.isNullOrEmpty()){
            Log.e("FFmpeg_test","path is empty!!!")
            return
        }
        field = value
        display(surface)
    }
    fun play(){
        if(path == null || path.isNullOrEmpty()){
            Log.e("ffmpeg_player","path is null !!!")
            return
        }
        play(path!!)
    }

    external fun play(path: String): Int

    external fun display(surface: Surface)

    external fun release()

    external fun stop()

    external fun getTotalTime(): Int

    external fun getCurrentPosition(): Double

    external fun seekTo(msec: Int)

    external fun stepBack() //快退

    external fun stepUp() //快进
    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}