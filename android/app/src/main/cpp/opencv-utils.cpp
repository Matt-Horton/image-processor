//
// Created by Matt on 30/06/2023.
//
#include "opencv-utils.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <android/log.h>

Mat resizeAndCrop(Mat img, int targetWidth, int targetHeight) {

    Mat resized;

    float ratio = float(targetWidth) / img.cols;

    cv::resize(img, resized, Size(targetWidth, int(img.rows * ratio)));
    int startRow = (resized.rows / 2) - (targetHeight / 2);
    Mat cropped = resized(Range(startRow, targetHeight + startRow), Range(0, 400));

    return cropped;
}

Mat rotateMat(Mat src, double angle) {

    // get rotation matrix for rotating the image around its center in pixel coordinates
    cv::Point2f center((src.cols-1)/2.0, (src.rows-1)/2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle, center not relevant
    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), src.size(), angle).boundingRect2f();
    // adjust transformation matrix
    rot.at<double>(0,2) += bbox.width/2.0 - src.cols/2.0;
    rot.at<double>(1,2) += bbox.height/2.0 - src.rows/2.0;

    cv::Mat dst;
    cv::warpAffine(src, dst, rot, bbox.size());

    return dst;
}

void addNoise(Mat& src) {
    // We need to work with signed images (as noise can be
    // negative as well as positive). We use 16 bit signed
    // images as otherwise we would lose precision.
    Mat noise_image(src.size(), CV_16SC3);
    randn(noise_image, Scalar::all(0.0), Scalar::all(5.0));
    Mat temp_image;
    src.convertTo(temp_image, CV_16SC3);
    addWeighted(temp_image, 1.0, noise_image, 1.0, 0.0, temp_image);
    temp_image.convertTo(src, src.type());
}

void adjustLab(Mat& src) {
    Mat lab;
    cvtColor(src, lab, COLOR_BGR2Lab);

    for (int row = 0; row < src.rows; row++) {
        for (int col = 0; col < src.cols; col++) {
            lab.at<Vec3b>(row, col)[0] = (int)(lab.at<Vec3b>(row, col)[0] + 30) > 255 ? 255 : (lab.at<Vec3b>(row, col)[0] + 30);
        }
    }

    cvtColor(lab, src, COLOR_Lab2BGR);
}

Mat insertIntoPolaroidFrame(Mat& src, Mat& polaroidFrame) {

    int xOffset = 20;
    int yOffset = 20;

    __android_log_print(ANDROID_LOG_INFO, "opencv-utils", "Source Channels: %d", + src.channels());
    __android_log_print(ANDROID_LOG_INFO, "opencv-utils", "Polaroid Frame Channels: %d", + polaroidFrame.channels());

    cv::Rect roi(xOffset, yOffset, src.cols, src.rows);

    Mat polaroidFrameNoAlpha;
    cvtColor(polaroidFrame, polaroidFrameNoAlpha, COLOR_BGRA2BGR);

    __android_log_print(ANDROID_LOG_INFO, "opencv-utils", "Source Channels: %d", + src.channels());
    __android_log_print(ANDROID_LOG_INFO, "opencv-utils", "Polaroid Frame Channels: %d", + polaroidFrameNoAlpha.channels());

    src.copyTo(polaroidFrameNoAlpha(roi));

    return polaroidFrameNoAlpha;
}

