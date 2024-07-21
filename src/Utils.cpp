/*
 * @Author: CsVeryLoveXieWenLi
 * @Date: 2024-07-20 15:15:58
 * @LastEditTime: 2024-07-21 11:47:23
 * @Description: 工具
 * @Sign: 有些故事，总是美妙又缥缈，郁郁不得终。
 * Copyright (c) 2024 by CsVeryLoveXieWenLi, All Rights Reserved.
 */

#include "utils.h"


/**
 * 从字节集解码图片
 */
cv::Mat Utils::bytes2mat(uchar* bytes, size_t length) {
    return cv::imdecode(cv::Mat(1, length, CV_8UC1, bytes), cv::IMREAD_COLOR);
}

/**
 * 分割字符串并转化
 */
std::vector<int> Utils::splits(std::string& str, const char line) {
    std::istringstream iss(str);
    std::string        token;

    std::vector<int> tokens;

    while (getline(iss, token, line)) tokens.push_back(stoi(token));

    return tokens;
}

/**
 * 数字转化到字节集
 */
const uchar* Utils::ints2bytes(std::vector<int>& ints) {
    uchar* bytes = (uchar*)malloc(sizeof(int) * ints.size());

    for (int i = 0, p = 0; i < ints.size(); i++) {
        int number = ints[i];

        *(bytes + p) = number >> 24;
        p++;

        *(bytes + p) = number >> 16;
        p++;

        *(bytes + p) = number >> 8;
        p++;

        *(bytes + p) = number;
        p++;
    }

    return bytes;
}

/**
 * 复制字符串到堆上
 */
const char* Utils::string2copy(std::string text) {
    const char* chars     = text.data();
    size_t      chars_len = strlen(chars) + 1;

    const char* copys = (const char*)malloc(chars_len);

    memcpy((char*)copys, chars, chars_len);

    return copys;
}