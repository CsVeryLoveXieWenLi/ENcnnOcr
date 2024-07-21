/*
 * @Author: CsVeryLoveXieWenLi
 * @Date: 2024-07-20 14:47:35
 * @LastEditTime: 2024-07-21 11:55:48
 * @Description: FastestDet
 * @Sign: 有些故事，总是美妙又缥缈，郁郁不得终。
 * Copyright (c) 2024 by CsVeryLoveXieWenLi, All Rights Reserved.
 */

#pragma once

#include <ncnn/net.h>

#include <opencv2/imgcodecs.hpp>


struct DetectionBox {
    cv::Rect rect;

    float score;
    short label;
};


class Detection {
public:
    /**
     * 初始化
     */
    Detection(const char* param, const char* bin, short threads);

    /**
     * 推理
     */
    std::vector<DetectionBox> inference(cv::Mat& image);

private:
    // ncnn
    ncnn::Net net;

    const short size[2] = {352, 352};
    const float norm[3] = {1 / 255.f, 1 / 255.f, 1 / 255.f};

    static inline const short classes     = 1;
    static inline const float scoreThresh = 0.8f;
    static inline const float iouThresh   = 0.45f;


    /**
     * 激活函数 sigmoid
     */
    static float sigmoid(float x);

    /**
     * 激活函数 tanh
     */
    static float tanh(float x);

    /**
     * 解码输出
     */
    static void decode(ncnn::Mat& output, std::vector<DetectionBox>& boxes, int imageWidth, int imageHeight);

    /**
     * 非极大值抑制
     */
    static void nms(std::vector<DetectionBox>& boxes, std::vector<DetectionBox>& picked, cv::Rect& border);
};