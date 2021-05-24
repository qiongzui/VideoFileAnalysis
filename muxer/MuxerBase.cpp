//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "MuxerBase.h"
#include <iostream>

using namespace std;
uint64_t MuxerBase::Tranform2Number(char* src, int len)
{
    uint64_t ret = 0;
    for (int i = 0; i < len; ++i) {
        ret = ret << 8;
        auto c = static_cast<uint8_t>(src[i]);
        uint64_t tmp = c - 0;
        ret += tmp;
    }
    return ret;
}

bool MuxerBase::OpenFile() {
    if (m_inFile.is_open()) {
        m_inFile.close();
    }
    m_inFile.open(m_filePath, ios::in | ios::binary);
    if (!m_inFile.is_open()) {
        cout<< "ERROR: file is not exist"<< endl;
        return false;
    }
    return true;
}

void MuxerBase::CloseFile() {
    m_inFile.close();
}
