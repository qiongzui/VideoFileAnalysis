//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "mp4.h"
#include <iostream>

int Tranform2Number(char* src, int len)
{
    int ret = 0;
    for (int i = 0; i < len; ++i) {
        ret = ret << 8;
        unsigned char c = (uint8_t)src[i];
        int tmp = c - 0;
        ret += tmp;
    }
    return ret;
}

void mp4::Process() {
    uint32_t readPos = 0;
    while (!m_infile.eof()) {
        Box box;
        box.startPos = readPos;

        char context[9];
        memset(context, 0 , 9);

        m_infile.get(context, 9);
        uint32_t size = Tranform2Number(context, 4);
        if (size == 0) {
            break;
        }

        box.header.size = size;

        memcpy(box.header.type, context + 4, 5);

        if (size > 1) {
            box.endPos = box.startPos + size;
            readPos = box.endPos;
        }
        std::cout<< box.header.size << std::endl;
        std::cout<< box.header.type << std::endl;

        m_boxs.emplace_back(box);
        m_infile.seekg(readPos);
    }
}
