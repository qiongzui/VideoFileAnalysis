//
// Created by 郑蓁浩 on 2021/6/7.
//

#include "flv.h"

int flv::init(const std::string &filePath) {
    m_filePath = filePath;
    m_mediaContext = std::make_shared<MediaContext>();
    m_mediaContext->flvInfos = std::make_shared<FlvInfo>();
    m_mediaContext->format = "FLV";
    return 0;
}

void flv::Process() {
    if (!OpenFile() || m_mediaContext == nullptr) {
        return;
    }

    auto &header = m_mediaContext->flvInfos->flvHeader;
    if (ParseFlvHeader(header) < 0) {
        return;
    }
    uint32_t pos = 9;
    ParseFlvBady(m_mediaContext->flvInfos->tagBody, pos);
}

int flv::ParseFlvBady(FlvInfo::FlvBody &body, uint32_t& startPos) {
    fseek(m_inFile, startPos, SEEK_SET);

    do {
        FlvInfo::FlvBody::Tag tag;
        ParseTag(tag, startPos);
        body.tagVec.emplace_back(tag);
    } while (!feof(m_inFile));


    return 0;
}

int flv::ParseFlvHeader(FlvInfo::FlvHeader &header) {
    fseek(m_inFile, 0, SEEK_SET);

    fread(header.signature1, 1, 1, m_inFile);
    fread(header.signature2, 1, 1, m_inFile);
    fread(header.signature3, 1, 1, m_inFile);
    fread(header.version, 1, 1, m_inFile);

    uint8_t readBuf[1];
    fread(readBuf, 1, 1, m_inFile);
    header.typeFlagsReserved = *readBuf & 0xf8;
    header.typeFlagsAudio = *readBuf & 0x4;
    header.typeFlagsReserved1 = *readBuf & 0x2;
    header.typeFlagsVideo = *readBuf & 0x1;

    fread(header.DataOffset, 1, 4, m_inFile);
    return 0;
}

int flv::ParseTag(FlvInfo::FlvBody::Tag &tag, uint32_t& startPos) {
    fseek(m_inFile, startPos, SEEK_SET);
    fread(tag.preTagSize, 1, 4, m_inFile);

    auto &flvTag = tag.flvTag;
    uint8_t read;
    fread(&read, 1, 1, m_inFile);
    flvTag.reserved = read & 0xc0;
    flvTag.filter = read & 0x20;
    flvTag.tagType = read & 0x1f;

    fread(flvTag.dataSize, 1, 3, m_inFile);
    fread(flvTag.timeStamp, 1, 3, m_inFile);
    fread(flvTag.timestampExtended, 1, 1, m_inFile);
    fread(flvTag.streamId, 1, 3, m_inFile);

    switch (flvTag.tagType) {
        case 0x09:
            ParseVideoTag(flvTag, startPos);
            break;
        case 0x08:
            ParseSoundTag(flvTag, startPos);
            break;
        case 0x12:
            ParseScriptTag(flvTag, startPos);
            break;
        default:
            return -1;
    }
    startPos += 15 + Tranform2Number(flvTag.dataSize, 3);
    return 0;
}

int flv::ParseVideoTag(FlvInfo::FlvBody::Tag::FlvTag &flvTag, uint32_t& startPos) {
    auto videoTag = std::make_shared<VideoTag>();
    uint8_t read;
    fread(&read, 1, 1, m_inFile);

    int len = 1;
    videoTag->frameType = read & 0xf0;
    videoTag->codecId = read & 0x0f;

    // 对应h.264
    if (videoTag->codecId == 7) {
        fread(videoTag->avcPacketType, 1, 1, m_inFile);
        fread(videoTag->cts, 1, 3, m_inFile);
        len += 4;
    }

    videoTag->data.startPos = startPos + len;
    videoTag->data.len = Tranform2Number(flvTag.dataSize, 3) - len;

    flvTag.tag = videoTag;
    return 0;
}

int flv::ParseSoundTag(FlvInfo::FlvBody::Tag::FlvTag &flvTag, uint32_t& startPos) {
    auto soundTag = std::make_shared<AudioTag>();
    uint8_t read;
    fread(&read, 1, 1, m_inFile);

    int len = 1;
    soundTag->soundFormat = read & 0xf0;
    soundTag->sampleRate = read & 0xc;
    soundTag->sampleSize = read & 0x2;
    soundTag->channelLayout = read & 0x1;

    if (soundTag->soundFormat == 10) {
        fread(soundTag->aacPacketType, 1, 1, m_inFile);
        len++;
    }

    soundTag->data.startPos = startPos + len;
    soundTag->data.len = Tranform2Number(flvTag.dataSize, 3) - len;

    flvTag.tag = soundTag;
    return 0;
}

int flv::ParseScriptTag(FlvInfo::FlvBody::Tag::FlvTag &flvTag, uint32_t &startPos) {
    auto scriptTag = std::make_shared<ScriptTag>();
    fread(&scriptTag->type, 1, 1, m_inFile);

    scriptTag->data.startPos = startPos + 1;
    scriptTag->data.len = Tranform2Number(flvTag.dataSize, 3) - 1;

    flvTag.tag = scriptTag;
    return 0;
}
