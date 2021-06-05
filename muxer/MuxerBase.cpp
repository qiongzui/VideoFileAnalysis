//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "MuxerBase.h"
#include <iostream>

using namespace std;
uint64_t MuxerBase::Tranform2Number(uint8_t * src, int len)
{
    uint64_t ret = 0;
    for (int i = 0; i < len; ++i) {
        ret = ret << 8;
        auto c = src[i];
        uint64_t tmp = c - 0;
        ret += tmp;
    }
    return ret;
}

bool MuxerBase::OpenFile() {
    if (m_inFile != nullptr) {
        return true;
    }

    m_inFile = fopen(m_filePath.c_str(), "rb+");
    if (m_inFile == nullptr) {
        cout<< "ERROR: file is not exist"<< endl;
        return false;
    }
    return true;
}

void MuxerBase::CloseFile() {
    fclose(m_inFile);
    m_inFile = nullptr;
}

uint64_t MuxerBase::Tranform2Float(uint8_t *src, int len, int potIndex) {
    float ret = Tranform2Number(src, potIndex);
    float decimal = Tranform2Number(src, len - potIndex);
    while (decimal >= 1) {
        decimal /= 10;
    }

    ret += decimal;
    return ret;
}

