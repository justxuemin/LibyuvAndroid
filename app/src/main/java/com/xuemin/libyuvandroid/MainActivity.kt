package com.xuemin.libyuvandroid

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity


class MainActivity : AppCompatActivity(),Constant {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        initBtn()
    }

    private fun initBtn() {
        val imageBtn = findViewById<Button>(R.id.image)
        val cameraBtn = findViewById<Button>(R.id.camera)
        imageBtn.setText("ImageActivity")
        imageBtn.setOnClickListener{
            val intent = Intent()
            intent.setClass(this, ImageActivity::class.java)
            startActivity(intent)
            Log.v(TAG, "[MainActivity] start ImageActivity")
        }
        cameraBtn.setText("CameraActivity")
        cameraBtn.setOnClickListener {
            val intent = Intent()
            intent.setClass(this, CameraActivity::class.java)
            startActivity(intent)
            Log.v(TAG, "[MainActivity] start CameraActivity")
        }
    }
}
