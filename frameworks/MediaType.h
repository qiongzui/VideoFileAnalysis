//
// Created by 郑蓁浩 on 2021/6/23.
//

#ifndef VIDEOFILEANALYSIS_MEDIATYPE_H
#define VIDEOFILEANALYSIS_MEDIATYPE_H

#endif //VIDEOFILEANALYSIS_MEDIATYPE_H

#include <iostream>

struct MediaFrame {
    uint32_t size;
    uint32_t pts;
    uint8_t *data;
};

struct VideoFrame : MediaFrame{
    uint32_t width;
    uint32_t height;
};

struct AudioFrame : MediaFrame{
    uint32_t sampleRate;
    uint32_t channels;
};

struct MediaPacket {
    uint32_t pts;
    uint8_t *data;
};

struct H264Packet : MediaPacket {
    struct NalType {

    } type;
};

struct AacPacket : MediaPacket {

};

using MediaFrameSp = std::shared_ptr<MediaFrame>;
using MediaPacketSp = std::shared_ptr<MediaPacket>;