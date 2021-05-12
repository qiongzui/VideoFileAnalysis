//
// Created by 郑蓁浩 on 2021/5/11.
//

#ifndef VIDEOFILEANALYSIS_MP4_H
#define VIDEOFILEANALYSIS_MP4_H

#include "MuxerBase.h"

class Box {
public:
    struct Header {
        uint32_t size;
        char type[4];
    } header;

    struct HeaderExtend {
        uint64_t largesize;
        uint64_t userType[2];
    } extend;

    uint32_t startPos;
    uint32_t endPos;
    std::vector<Box> containers;
};

class mp4 : public MuxerBase{
public:
    ~mp4() override;

    void Process() override ;

private:
    std::vector<Box> m_boxs;
};


#endif //VIDEOFILEANALYSIS_MP4_H
