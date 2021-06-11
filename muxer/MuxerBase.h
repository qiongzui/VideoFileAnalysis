//
// Created by 郑蓁浩 on 2021/5/11.
//

#ifndef VIDEOFILEANALYSIS_MUXERBASE_H
#define VIDEOFILEANALYSIS_MUXERBASE_H

#include <iostream>
#include <vector>
#include <memory>

#include "mp4_types.h"

struct FlvInfo;

struct MediaContext {
    std::string format;
    std::string formatProfile;
    std::string codecId;
    uint32_t duration;
    uint32_t timeScale;
    uint32_t size;
    uint64_t encodeDate;
    uint64_t taggedDate;

    std::vector<Track> tracks;
    std::string encodeMode;
    uint32_t bitRate;

    std::shared_ptr<FlvInfo> flvInfos;
};

using MediaContextSp = std::shared_ptr<MediaContext>;

class MuxerBase {
public:
    MuxerBase() = default;
    virtual ~MuxerBase() = default;

    virtual int init(const std::string &filePath) = 0;

    //void SetDataFile(const std::string &filePath) { m_filePath = filePath; }
    virtual void Process() = 0;
    MediaContextSp GetMediaContext() { return m_mediaContext; };

protected:
    // 将字符串转化为数值
    uint64_t Tranform2Number(uint8_t* src, int len);
    uint64_t Tranform2Float(uint8_t * src, int len, int potIndex);
    bool OpenFile();
    void CloseFile();

    std::string m_filePath;
    FILE *m_inFile = nullptr;

    MediaContextSp m_mediaContext;
};


#endif //VIDEOFILEANALYSIS_MUXERBASE_H
