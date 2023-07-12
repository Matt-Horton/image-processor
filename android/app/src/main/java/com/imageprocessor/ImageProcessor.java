package com.imageprocessor;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.util.Base64;
import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.Promise;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableArray;
import com.facebook.react.bridge.WritableMap;


import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;

public class ImageProcessor extends ReactContextBaseJavaModule {

    private static final String TAG = "ImageProcessor";

    public ImageProcessor(ReactApplicationContext context) {
        super(context);
    }

    @NonNull
    @Override
    public String getName() {
        return "ImageProcessor";
    }

    private native void myFlip(Bitmap bitmapIn, Bitmap polaroidFrameBitmap, Bitmap bitmapOut);

    @ReactMethod
    public void processImage(Promise promise) throws IOException {

        Bitmap sampleImage = BitmapFactory.decodeResource(getReactApplicationContext().getResources(), R.drawable.sample_picture);
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;
        Bitmap polaroidFrame = BitmapFactory.decodeResource(getReactApplicationContext().getResources(), R.drawable.polaroid_frame, options);
        Bitmap outputBitmap = Bitmap.createBitmap(440, 533, sampleImage.getConfig());
        myFlip(sampleImage, polaroidFrame, outputBitmap);

        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        outputBitmap.compress(Bitmap.CompressFormat.PNG, 100, stream);
        byte[] byteArray = stream.toByteArray();
        outputBitmap.recycle();

        String bitmapBase64Encoded = Base64.encodeToString(byteArray, Base64.DEFAULT);
        promise.resolve(bitmapBase64Encoded);
    }

    private Uri getFileUri(String filepath)  {
        Uri uri = Uri.parse(filepath);
        if (uri.getScheme() == null) {
            // No prefix, assuming that provided path is absolute path to file
            File file = new File(filepath);
            uri = Uri.parse("file://" + filepath);
        }
        return uri;
    }

    public WritableArray readDir(String directory) {
        try {
            File file = new File(directory);

            if (!file.exists()) throw new Exception("Folder does not exist");

            File[] files = file.listFiles();

            WritableArray fileMaps = Arguments.createArray();

            for (File childFile : files) {
                WritableMap fileMap = Arguments.createMap();

                fileMap.putDouble("mtime", (double) childFile.lastModified() / 1000);
                fileMap.putString("name", childFile.getName());
                fileMap.putString("path", childFile.getAbsolutePath());
                fileMap.putDouble("size", (double) childFile.length());
                fileMap.putInt("type", childFile.isDirectory() ? 1 : 0);
                fileMaps.pushMap(fileMap);
            }
            return fileMaps;
        } catch (Exception ex) {
            ex.printStackTrace();
           Log.e(TAG, "An error occured: " + ex.getMessage());
        }
        return null;
    }
}
