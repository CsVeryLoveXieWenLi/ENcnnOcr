/*
 * @Author: CsVeryLoveXieWenLi
 * @Date: 2024-07-20 14:47:35
 * @LastEditTime: 2024-07-21 10:14:31
 * @Description: Classification
 * @Sign: 有些故事，总是美妙又缥缈，郁郁不得终。
 * Copyright (c) 2024 by CsVeryLoveXieWenLi, All Rights Reserved.
 */

#include "Classification.h"


/**
 * 初始化
 */
Classification::Classification(
    const char*  param,
    const char*  bin,
    short        threads,
    const float* mean,
    const float* norm
) {
    net.opt.num_threads         = threads;
    net.opt.use_fp16_packed     = false;
    net.opt.use_fp16_storage    = false;
    net.opt.use_fp16_arithmetic = false;
    net.opt.lightmode           = true;

    net.load_param(param);
    net.load_model(bin);

    this->mean[0] = mean[0] * 255.f;
    this->mean[1] = mean[1] * 255.f;
    this->mean[2] = mean[2] * 255.f;

    this->norm[0] = 1 / norm[0] / 255.f;
    this->norm[1] = 1 / norm[1] / 255.f;
    this->norm[2] = 1 / norm[2] / 255.f;
}


/**
 * 推理
 */
short Classification::inference(cv::Mat& image) {
    ncnn::Extractor extractor = net.create_extractor();

    int imageWidth  = image.cols;
    int imageHeight = image.rows;

    ncnn::Mat input, output;

    //  Ncnn -> shape == resize? -> cv2.INTER_LINEAR
    input =
        ncnn::Mat::from_pixels_resize(image.data, ncnn::Mat::PIXEL_BGR2RGB, imageWidth, imageHeight, size[0], size[1]);
    input.substract_mean_normalize(mean, norm);

    extractor.input("in0", input);
    extractor.extract("out0", output);

    short topClass = 0;
    float topScore = output[0];

    for (short index = 1; index < output.w; index++) {
        float score = output[index];

        if (score > topScore) {
            topScore = score;
            topClass = index;
        }
    }

    return topClass;
}