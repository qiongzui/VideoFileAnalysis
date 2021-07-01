//
// Created by 郑蓁浩 on 2021/6/23.
//

#ifndef VIDEOFILEANALYSIS_DECODERBASE_H
#define VIDEOFILEANALYSIS_DECODERBASE_H

#include <iostream>
#include "MediaType.h"

class DecoderBase {

public:
    virtual int32_t DecodeOneFrame(MediaPacketSp packet, MediaFrameSp frame) = 0;
};


#endif //VIDEOFILEANALYSIS_DECODERBASE_H
