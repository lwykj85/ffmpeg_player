package com.houde.ffmpeg.player

import android.content.Intent
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.View

class MainActivity2 : AppCompatActivity() {
    private val inputFilePath = "/storage/emulated/0/GreenCheng/video/g4.mp4"
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main2)
    }

    fun open(view:View){
        startActivity(Intent(this,MainActivity::class.java))
    }

}
