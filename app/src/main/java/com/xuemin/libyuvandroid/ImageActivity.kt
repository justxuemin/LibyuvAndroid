package com.xuemin.libyuvandroid

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Log
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import com.xuemin.libyuv.utils.YUVUtil
import java.nio.ByteBuffer


class ImageActivity : AppCompatActivity(), Constant {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_image)

        val imageView: ImageView = findViewById(R.id.result)
        val bitmap = BitmapFactory.decodeResource(resources, R.drawable.test)
        val argb = ByteArray(bitmap.width * bitmap.height * 4)
        val startTime = System.currentTimeMillis()
        val result = YUVUtil.bitmapToYuvToARGB(bitmap, argb)
        Log.v(TAG, "coast time : " + (System.currentTimeMillis() - startTime) + ", result : " + result)
        val resultBitmap = Bitmap.createBitmap(bitmap.height, bitmap.width, Bitmap.Config.ARGB_8888)
        val byteBuffer = ByteBuffer.wrap(argb, 0, argb.size)
        resultBitmap.copyPixelsFromBuffer(byteBuffer)
        imageView.setImageBitmap(resultBitmap)
    }
}
