//
// Created by 郑蓁浩 on 2021/5/11.
//

#ifndef VIDEOFILEANALYSIS_MP4_H
#define VIDEOFILEANALYSIS_MP4_H

#include "MuxerBase.h"

class mp4 : public MuxerBase{
public:
    ~mp4() override = default;

    void Process() override ;
};


#endif //VIDEOFILEANALYSIS_MP4_H
