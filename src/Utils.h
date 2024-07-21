/*
 * @Author: CsVeryLoveXieWenLi
 * @Date: 2024-07-20 15:15:58
 * @LastEditTime: 2024-07-20 16:08:26
 * @Description: 工具
 * @Sign: 有些故事，总是美妙又缥缈，郁郁不得终。
 * Copyright (c) 2024 by CsVeryLoveXieWenLi, All Rights Reserved.
 */

#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


class Utils {
public:
    /**
     * 从字节集解码图片
     */
    static cv::Mat bytes2mat(uchar* bytes, size_t length);

    /**
     * 分割字符串并转化
     */
    static std::vector<int> splits(std::string& str, const char line);

    /**
     * 数字转化到字节集
     */
    static const uchar* ints2bytes(std::vector<int>& ints);

    /**
     * 复制字符串到堆上
     */
    static const char* string2copy(std::string text);
};
