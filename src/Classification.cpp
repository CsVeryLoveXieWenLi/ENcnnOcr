/*
 * @Author: CsVeryLoveXieWenLi
 * @Date: 2024-07-20 14:47:35
 * @LastEditTime: 2024-07-20 15:12:07
 * @Description: Classification
 * @Sign: 有些故事，总是美妙又缥缈，郁郁不得终。
 * Copyright (c) 2024 by CsVeryLoveXieWenLi, All Rights Reserved.
 */
 
#include "Classification.h"


/**
 * 初始化
 */
Classification::Classification(const char* param, const char* bin, short threads) {
    net.opt.num_threads         = threads;
    net.opt.use_fp16_packed     = false;
    net.opt.use_fp16_storage    = false;
    net.opt.use_fp16_arithmetic = false;

    net.load_param(param);
    net.load_model(bin);
}


/**
 * 推理
 */
short Classification::inference(cv::Mat& image) {
    ncnn::Extractor extractor = net.create_extractor();

    int imageWidth  = image.cols;
    int imageHeight = image.rows;

    ncnn::Mat input, output;

    if (imageWidth == size[0] && imageHeight == size[1]) {
        input = ncnn::Mat::from_pixels(image.data, ncnn::Mat::PIXEL_BGR2RGB, imageWidth, imageHeight);
    } else {
        input = ncnn::Mat::from_pixels_resize(
            image.data,
            ncnn::Mat::PIXEL_BGR2RGB,
            imageWidth,
            imageHeight,
            size[0],
            size[1]
        );
    }
    input.substract_mean_normalize(0, norm);

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