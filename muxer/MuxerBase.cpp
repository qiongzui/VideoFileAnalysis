//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "MuxerBase.h"

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