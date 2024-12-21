/**
 * @Author: CsVeryLoveXieWenLi
 * @LastEditTime: 2024-12-22 06:38:26
 * @Description: FastestDet
 * @Copyright (c) 2024 by CsVeryLoveXieWenLi, All Rights Reserved. 
 */

#include "Detection.h"


/**
 * 激活函数 sigmoid
 */
float Detection::sigmoid(float x) { return 1.0f / (1.0f + exp(-x)); }

/**
 * 激活函数 tanh
 */
float Detection::tanh(float x) { return 2.0f / (1.0f + exp(-2 * x)) - 1; }


/**
 * 解码输出
 */
void Detection::decode(ncnn::Mat& output, std::vector<DetectionBox>& boxes, int imageWidth, int imageHeight) {
    for (short h = 0; h < output.h; h++) {
        for (short w = 0; w < output.w; w++) {
            // 前景分数
            float bgScore = output[h * output.w + w];

            // 解析类别
            short label    = 0;
            float maxScore = 0.f;

            for (short i = 0; i < classes; i++) {
                float classScore = output[((5 + i) * output.h * output.w) + (h * output.w) + w];

                if (classScore > maxScore) {
                    maxScore = classScore;
                    label    = i;
                }
            }

            // 解析分数
            float score = static_cast<float>(pow(maxScore, 0.4) * pow(bgScore, 0.6));

            // 阈值筛选
            if (score > scoreThresh) {
                // 解析坐标
                float xOffset   = tanh(output[(1 * output.h * output.w) + (h * output.w) + w]);
                float yOffset   = tanh(output[(2 * output.h * output.w) + (h * output.w) + w]);
                float boxWidth  = sigmoid(output[(3 * output.h * output.w) + (h * output.w) + w]);
                float boxHeight = sigmoid(output[(4 * output.h * output.w) + (h * output.w) + w]);

                float cx = (w + xOffset) / output.w;
                float cy = (h + yOffset) / output.h;

                int x1 = (int)((cx - boxWidth * 0.5) * imageWidth);
                int y1 = (int)((cy - boxHeight * 0.5) * imageHeight);
                int x2 = (int)((cx + boxWidth * 0.5) * imageWidth);
                int y2 = (int)((cy + boxHeight * 0.5) * imageHeight);

                boxes.push_back(DetectionBox{cv::Rect(x1, y1, x2 - x1, y2 - y1), score, label});
            }
        }
    }
}

/**
 * 非极大值抑制
 */
void Detection::nms(std::vector<DetectionBox>& boxes, std::vector<DetectionBox>& picked, cv::Rect& border) {
    sort(boxes.begin(), boxes.end(), [&](DetectionBox& a, DetectionBox& b) { return a.score > b.score; });

    for (auto& a : boxes) {
        bool keep = true;

        for (auto& b : picked) {
            // 交集
            float interArea = (a.rect & b.rect).area();

            // 并集
            float unionArea = a.rect.area() + b.rect.area() - interArea;

            // iou阈值
            float iou = interArea / unionArea;

            if (iou > iouThresh && a.label == b.label) {
                keep = false;
                break;
            }
        }

        if (keep) {
            a.rect &= border;
            picked.push_back(a);
        }
    }
}


/**
 * 初始化
 */
Detection::Detection(const char* param, const char* bin, short threads) {
    net.opt.num_threads         = threads;
    net.opt.use_fp16_packed     = false;
    net.opt.use_fp16_storage    = false;
    net.opt.use_fp16_arithmetic = false;
    net.opt.lightmode           = true;

    net.load_param(param);
    net.load_model(bin);
}


/**
 * 推理
 */
std::vector<DetectionBox> Detection::inference(cv::Mat& image) {
    ncnn::Extractor extractor = net.create_extractor();

    int imageWidth  = image.cols;
    int imageHeight = image.rows;

    cv::Rect border = cv::Rect(0, 0, imageWidth, imageHeight);

    ncnn::Mat                 input, output;
    std::vector<DetectionBox> boxes, picked;

    //  Ncnn -> shape == resize? -> cv2.INTER_LINEAR
    input =
        ncnn::Mat::from_pixels_resize(image.data, ncnn::Mat::PIXEL_BGR2RGB, imageWidth, imageHeight, size[0], size[1]);
    input.substract_mean_normalize(nullptr, norm);

    extractor.input("in0", input);
    extractor.extract("out0", output);

    decode(output, boxes, imageWidth, imageHeight);
    nms(boxes, picked, border);

    return picked;
}