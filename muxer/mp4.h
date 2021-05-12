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
        char userType[128];
    } extend;

    uint64_t startPos;
    uint64_t endPos;
    std::vector<Box> containers;
};

class mp4 : public MuxerBase{
public:
    ~mp4() override = default;

    void Process() override ;

private:
    void OneBoxAnalysis(const std::shared_ptr<Box>& box, std::vector<Box>& boxs);
    void RootBoxAnalysis(std::vector<Box>& boxs);
    void ReadFileInfo();
    std::vector<Box> m_boxs;
};


#endif //VIDEOFILEANALYSIS_MP4_H
