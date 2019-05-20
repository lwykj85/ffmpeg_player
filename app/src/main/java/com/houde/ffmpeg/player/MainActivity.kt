package com.houde.ffmpeg.player

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private val inputFilePath = "/storage/emulated/0/GreenCheng/video/g4.mp4"
    private lateinit var videoView:VideoView
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Example of a call to a native method
        videoView = findViewById(R.id.sample_text)
//        content_fl.post { Runnable {
//            init2()
//        } }
        content_fl.post { init2() }
    }
    private fun init2(){
        videoView.path = inputFilePath
        Log.e("ffmpeg_player","$videoView.path")
        videoView.play()
    }

}
