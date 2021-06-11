//
// Created by 郑蓁浩 on 2021/6/7.
//

#ifndef VIDEOFILEANALYSIS_FLV_H
#define VIDEOFILEANALYSIS_FLV_H

#include "MuxerBase.h"

struct DataTag {
    Data data;
};

struct VideoTag : public DataTag{
    uint8_t frameType:4;
    uint8_t codecId:4;
    uint8_t avcPacketType[1];
    uint8_t cts[3];
};

struct AudioTag : public DataTag{
    uint8_t soundFormat:4;
    uint8_t sampleRate:2;
    uint8_t sampleSize:1;
    uint8_t channelLayout:1;
    uint8_t aacPacketType[1];
};

struct ScriptTag : public DataTag{
    uint8_t type;
    Data data;
};

struct FlvInfo {
    struct FlvHeader {
        uint8_t signature1[1];
        uint8_t signature2[1];
        uint8_t signature3[1];
        uint8_t version[1];
        uint8_t typeFlagsReserved:5;
        uint8_t typeFlagsAudio:1;
        uint8_t typeFlagsReserved1:1;
        uint8_t typeFlagsVideo:1;
        uint8_t DataOffset[4];
    } flvHeader;
    struct FlvBody {
        struct Tag {
            uint8_t preTagSize[4];
            struct FlvTag {
                uint8_t reserved:2;
                uint8_t filter:1;
                uint8_t tagType:5;
                uint8_t dataSize[3];
                uint8_t timeStamp[3];
                uint8_t timestampExtended[1];
                uint8_t streamId[3];
                std::shared_ptr<DataTag> tag;
            } flvTag;
        };
        std::vector<Tag> tagVec;
    } tagBody;
};

class flv : public MuxerBase {
public:
    void Process() override ;
    int init(const std::string &filePath) override ;

private:
    int ParseFlvHeader(FlvInfo::FlvHeader &header);
    int ParseFlvBady(FlvInfo::FlvBody &body, uint32_t& startPos);
    int ParseTag(FlvInfo::FlvBody::Tag &tag, uint32_t& startPos);
    int ParseVideoTag(FlvInfo::FlvBody::Tag::FlvTag &flvTag, uint32_t& startPos);
    int ParseSoundTag(FlvInfo::FlvBody::Tag::FlvTag &flvTag, uint32_t& startPos);
    int ParseScriptTag(FlvInfo::FlvBody::Tag::FlvTag &flvTag, uint32_t& startPos);
};


#endif //VIDEOFILEANALYSIS_FLV_H
