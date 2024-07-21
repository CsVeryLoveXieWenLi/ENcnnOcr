/*
 * @Author: CsVeryLoveXieWenLi
 * @Date: 2024-07-20 14:47:35
 * @LastEditTime: 2024-07-21 09:21:06
 * @Description: Classification
 * @Sign: 有些故事，总是美妙又缥缈，郁郁不得终。
 * Copyright (c) 2024 by CsVeryLoveXieWenLi, All Rights Reserved.
 */

#pragma once

#include <ncnn/net.h>

#include <opencv2/imgcodecs.hpp>


class Classification {
public:
    /**
     * 初始化
     */
    Classification(const char* param, const char* bin, short threads, const float* mean, const float* norm);

    /**
     * 推理
     */
    short inference(cv::Mat& image);

private:
    ncnn::Net net;

    const short size[2] = {64, 64};

    float mean[3] = {0.485f * 255.f, 0.456f * 255.f, 0.406f * 255.f};
    float norm[3] = {1 / 0.229f / 255.f, 1 / 0.224f / 255.f, 1 / 0.225f / 255.f};
};