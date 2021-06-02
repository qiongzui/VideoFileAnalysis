//
// Created by 郑蓁浩 on 2021/5/11.
//

#ifndef VIDEOFILEANALYSIS_MUXERBASE_H
#define VIDEOFILEANALYSIS_MUXERBASE_H

#include <fstream>
#include <vector>
#include <memory>

struct Track {
    std::string trackType;
    uint32_t ID;
    std::string format;
    std::string formatInfo;
    std::string formatProfile;
    struct FormatSetting {
        bool isCABAC;
        uint32_t reframes;
    } setting;
    std::string codecId;
    std::string codecIdInfo;
    uint32_t duration;
    uint32_t bitRate;
    uint32_t frameRate;
    uint32_t streamSize;

    /* video */
    uint32_t width;
    uint32_t height;
    std::string frameRateMode;
    std::string colorRange;

    /* audio */
    uint32_t channel;
    uint32_t channelLayout;
    uint32_t sampleRate;
    std::string compressionMode;
};

struct MediaContext {
    std::string format;
    std::string formatProfile;
    std::string codecId;
    uint32_t duration;
    uint32_t size;

    std::vector<Track> tracks;
    std::string encodeMode;
    uint32_t bitRate;
};

using MediaContextSp = std::shared_ptr<MediaContext>;

class MuxerBase {
public:
    MuxerBase() = default;
    virtual ~MuxerBase() = default;

    void SetDataFile(const std::string &filePath) { m_filePath = filePath; }
    virtual void Process() = 0;
    MediaContextSp GetMediaContext() { return m_mediaContext; };

protected:
    // 将字符串转化为数值
    uint64_t Tranform2Number(char* src, int len);
    bool OpenFile();
    void CloseFile();

    std::string m_filePath;
    std::ifstream m_inFile;

    MediaContextSp m_mediaContext;
};


#endif //VIDEOFILEANALYSIS_MUXERBASE_H
