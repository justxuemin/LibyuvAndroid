package com.xuemin.libyuvandroid

import android.graphics.*
import android.net.Uri
import android.os.Bundle
import android.util.Log
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity
import com.xuemin.libyuv.utils.YUVUtil
import java.io.File
import java.io.FileOutputStream
import java.nio.ByteBuffer


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val imageView: ImageView = findViewById<ImageView>(R.id.result)
        val bitmap = BitmapFactory.decodeResource(resources, R.drawable.test)
        val yuv = ByteArray(bitmap.width * bitmap.height * 3 / 2)
        val argb = ByteArray(bitmap.width * bitmap.height * 4)
        val startTime = System.currentTimeMillis()
        val result = YUVUtil.bitmapToYuv(bitmap, argb)
        Log.e("xuemin", "coast time : " + (System.currentTimeMillis() - startTime) + ", result : " + result)
//        val yuvImage = YuvImage(yuv, ImageFormat.NV21, bitmap.width, bitmap.height, null)
        val file = File("/sdcard/result.jpeg")
        val fos = FileOutputStream(file)
        val resultBitmap = Bitmap.createBitmap(bitmap.width, bitmap.height, Bitmap.Config.ARGB_8888);
        val byteBuffer = ByteBuffer.wrap(argb, 0, argb.size)
        resultBitmap.copyPixelsFromBuffer(byteBuffer)
        imageView.setImageBitmap(resultBitmap)
//        resultBitmap.copyPixelsToBuffer()
//        yuvImage.compressToJpeg(Rect(0, 0, bitmap.width, bitmap.height), 100, fos)
//        fos.close()
//        imageView.setImageURI(Uri.fromFile(file))
    }
}
