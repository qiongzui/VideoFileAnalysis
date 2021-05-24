//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "mp4.h"
#include <iostream>

void mp4::Process() {

    std::shared_ptr<Box> root = std::make_shared<Box>();
    root->header.size = INT32_MAX;
    memcpy(root->header.type, "root", 4);
    root->startPos = 0;
    root->endPos = INT32_MAX;

    OneBoxAnalysis(root);

}

void mp4::OneBoxAnalysis(const std::shared_ptr<Box>& box) {
    if (!OpenFile()) {
        return;
    }
    uint64_t start = strcmp(box->header.type, "root") == 0? 0 : box->startPos + 8;
    if (box != nullptr) {
        if (box->header.size == 1) {
            start += 8;
        }

        if (strcmp(box->header.type, "uuid") == 0) {
            start += 16;
        }
    }

    uint64_t end = box->endPos;

    uint32_t readPos = start;
    m_inFile.seekg(readPos);
    int index = m_inFile.tellg();
    while (!m_inFile.eof() && readPos < end) {
        std::shared_ptr<Box> tmpBox = std::make_shared<Box>();
        tmpBox->startPos = readPos;

        char context[9];
        memset(context, 0 , 9);

        m_inFile.get(context, 9);
        uint64_t size = Tranform2Number(context, 4);
        if (size == 0 || size > end - start - 8) {
            break;
        }

        tmpBox->header.size = size;
        memcpy(tmpBox->header.type, context + 4, 5);

        /* 扩展size */
        if (size == 1) {
            m_inFile.get(context, 9);
            size = Tranform2Number(context, 8);
            tmpBox->extend.largesize = size;
        }

        /* 用户自定义box */
        if (strcmp(tmpBox->header.type, "uuid") == 0) {
            char uuid[129];
            m_inFile.get(uuid, 129);
            memcpy(tmpBox->extend.userType, uuid, 128);
        }

        tmpBox->endPos = tmpBox->header.size > 1? tmpBox->startPos + size : tmpBox->startPos + tmpBox->extend.largesize;
        readPos = tmpBox->endPos;

        std::cout<< tmpBox->header.size << std::endl;
        std::cout<< tmpBox->header.type << std::endl;

        OneBoxAnalysis(tmpBox);
        if (!m_inFile.is_open()) {
            OpenFile();
        }

        box->containers.emplace_back(std::move(tmpBox));
        m_inFile.seekg(readPos);
    }
    //CloseFile();
}
