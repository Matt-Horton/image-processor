//
// Created by Matt on 30/06/2023.
//
#pragma once

#include <opencv2/core.hpp>

using namespace cv;

Mat resizeAndCrop(Mat src, int width, int height);
Mat rotateMat(Mat src, double angle);
void addNoise(Mat& src);
void adjustLab(Mat& src);
Mat insertIntoPolaroidFrame(Mat& src, Mat& polaroidFrame);

#ifndef IMAGEPROCESSOR_OPENCV_UTILS_H
#define IMAGEPROCESSOR_OPENCV_UTILS_H

#endif //IMAGEPROCESSOR_OPENCV_UTILS_H
