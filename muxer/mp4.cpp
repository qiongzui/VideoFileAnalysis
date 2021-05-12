//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "mp4.h"
#include <iostream>

void mp4::Process() {
    RootBoxAnalysis(m_boxs);
}

void mp4::RootBoxAnalysis(std::vector<Box>& boxs) {
    uint32_t readPos = 0;
    while (!m_infile.eof()) {
        Box box;
        box.startPos = readPos;

        char context[9];
        memset(context, 0 , 9);

        m_infile.get(context, 9);
        uint64_t size = Tranform2Number(context, 4);
        if (size == 0) {
            break;
        }

        box.header.size = size;
        memcpy(box.header.type, context + 4, 5);

        /* 扩展size */
        if (size == 1) {
            m_infile.get(context, 9);
            size = Tranform2Number(context, 8);
            box.extend.largesize = size;
        }

        /* 用户自定义box */
        if (box.header.type == "uuid") {
            char uuid[129];
            m_infile.get(uuid, 129);
            memcpy(box.extend.userType, uuid, 128);
        }

        box.endPos = box.header.size > 1? box.startPos + size : box.startPos + box.extend.largesize;
        readPos = box.endPos;

        std::cout<< box.header.size << std::endl;
        std::cout<< box.header.type << std::endl;

        m_boxs.emplace_back(box);
        m_infile.seekg(readPos);
    }
}

void mp4::OneBoxAnalysis(const std::shared_ptr<Box>& box, std::vector<Box> &boxs) {
    uint64_t start = box->startPos + 8;
    if (box->header.size == 1) {
        start += 8;
    }

    if (box->header.type == "uuid") {
        start += 16;
    }
    uint64_t end = box->endPos;

    uint64_t readPos = start;
    while (readPos < end) {

    }
}
