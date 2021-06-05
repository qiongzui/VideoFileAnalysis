//
// Created by 郑蓁浩 on 2021/5/11.
//

#ifndef VIDEOFILEANALYSIS_MP4_H
#define VIDEOFILEANALYSIS_MP4_H

#include "MuxerBase.h"
#include "mp4_types.h"

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
    std::vector<std::shared_ptr<Box>> fullBox;
    std::shared_ptr<Box> fatherBox;
};

class mp4 : public MuxerBase{
public:
    ~mp4() override;

    void Process() override ;
    int init(const std::string &filePath) override ;
private:
    void OneBoxAnalysis(const std::shared_ptr<Box>& box);
    void PrintBoxInfo(const std::shared_ptr<Box>& box, int depth);
    int ParseBoxInfo();

    int GetFtypInfo(const std::shared_ptr<Box>& box);
    int GetMoovInfo(const std::shared_ptr<Box>& box);
    int GetTrakInfo(const std::shared_ptr<Box>& box, Track& track);
    int GetMvhdInfo(const std::shared_ptr<Box>& box);
    int GetTkhdInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetEdtsInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetElstInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetMdiaInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetMdhdInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetHdlrInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetMinfInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetVmhdInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetSmhdInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetDinfInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetDrefInfo(const std::shared_ptr<Box>& box, Track &track);
    int GetStblInfo(const std::shared_ptr<Box>& box, Track &track);
    std::shared_ptr<Box> root = std::make_shared<Box>();
};


#endif //VIDEOFILEANALYSIS_MP4_H
