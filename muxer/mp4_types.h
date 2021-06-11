//
// Created by 郑蓁浩 on 2021/6/5.
//

#ifndef VIDEOFILEANALYSIS_MP4_TYPES_H
#define VIDEOFILEANALYSIS_MP4_TYPES_H

#include <iostream>
#include <string>

struct Data {
    uint64_t startPos;
    uint32_t len;
};

struct Track {
    struct TkhdInfo {
        uint8_t version[1];
        uint8_t flag[3];
        uint8_t createTime[4];
        uint8_t modifyTime[4];
        uint8_t trackId[4];
        uint8_t saveByte[4];
        uint8_t duration[4];
        uint8_t saveByte1[8];
        uint8_t layer[2];
        uint8_t alternateGroup[2];
        uint8_t volume[2];
        uint8_t saveByte2[2];
        uint8_t matFormat[36];
        uint8_t width[4];
        uint8_t height[4];
    } tkhdInfo;

    struct EdtsInfo {
        struct ElstInfo {
            uint8_t version[1];
            uint8_t flag[3];
            uint8_t dataNum[4];
            uint8_t segmentDuration[8];
            uint8_t mediaTime[8];
            uint8_t mediaRate[2];
            uint8_t mediaRateFraction[2];
        } elstInfo;
    } edtsInfo;

    struct MdiaInfo {
        struct MdhdInfo {
            uint8_t version[1];
            uint8_t flag[3];
            uint8_t createDate[4];
            uint8_t modifyDate[4];
            uint8_t timeScale[4];
            uint8_t duration[4];
            uint8_t language[2];
            uint8_t quality[2];
        } mdhdInfo;

        struct HdlrInfo {
            uint8_t version[1];
            uint8_t flag[3];
            uint8_t type[4];
            uint8_t subType[4];
            uint8_t saveByte[12];
            Data componentName;
        } hdlrInfo;

        struct MinfInfo {
            struct SmhdInfo {
                uint8_t version[1];
                uint8_t flag[3];
                uint8_t balance[2];
                uint8_t save[2];
            } smhdInfo;

            struct VmhdInfo {
                uint8_t version[1];
                uint8_t flag[3];
                uint8_t graphMode[2];
                uint8_t Opcolor[6];
            } vmhdInfo;

            struct DinfInfo {
                uint8_t version[1];
                uint8_t flag[3];
                struct DrefInfo {
                    uint8_t size[4];
                    uint8_t type[4];
                    uint8_t version[1];
                    uint8_t flag[3];
                    uint8_t dataRefNum[4];
                    struct DataRefence {
                        uint8_t size[4];
                        uint8_t type[4];
                        uint8_t version[1];
                        uint8_t flag[3];
                    };
                    std::vector<DataRefence> dataVec;
                } drefInfo;
            } dinfInfo;
        } minfInfo;
    } mdiaInfo;
};

#endif //VIDEOFILEANALYSIS_MP4_TYPES_H