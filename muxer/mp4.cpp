//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "mp4.h"
#include <iostream>

mp4::~mp4() {
}

int mp4::init(const std::string &filePath) {
    m_filePath = filePath;
    m_mediaContext = std::make_shared<MediaContext>();
    m_mediaContext->format = "MPEG-4";
    return 0;
}

void mp4::Process() {
    root->header.size = INT32_MAX;
    memcpy(root->header.type, "root", 4);
    root->startPos = 0;
    root->endPos = INT32_MAX;

    OneBoxAnalysis(root);
    PrintBoxInfo(root, 0);
    ParseBoxInfo();
}

void mp4::OneBoxAnalysis(const std::shared_ptr<Box>& box) {
    if (strcmp(box->header.type, "mdat") == 0) {
        return;
    }
    if (!OpenFile()) {
        return;
    }

    uint64_t start = strcmp(box->header.type, "root") == 0? 0 : box->startPos + 8;
    if (box != nullptr) {
        if (box->header.size == 1) {
            start += 8;
        }

        if (strcmp(box->header.type, "uuid") == 0) {
            start += 16;
        }
    }

    uint64_t end = box->endPos;

    uint32_t readPos = start;
    fseek(m_inFile, readPos, SEEK_SET);
    while (!feof(m_inFile) && readPos < end) {
        std::shared_ptr<Box> tmpBox = std::make_shared<Box>();
        tmpBox->startPos = readPos;

        uint8_t context[8];
        memset(context, 0 , 8);

        fread(context, 1, 8, m_inFile);
        uint64_t size = Tranform2Number(context, 4);
        if (size < 8 || size > end - start) {
            break;
        }

        tmpBox->header.size = size;
        memcpy(tmpBox->header.type, context + 4, 4);

        /* 扩展size */
        if (size == 1) {
            fread(context, 1, 8, m_inFile);
            size = Tranform2Number(context, 8);
            tmpBox->extend.largesize = size;
        }

        /* 用户自定义box */
        if (strcmp(tmpBox->header.type, "uuid") == 0) {
            char uuid[128];
            fread(uuid, 1, 128, m_inFile);
            memcpy(tmpBox->extend.userType, uuid, 128);
        }

        tmpBox->endPos = tmpBox->header.size > 1? tmpBox->startPos + size : tmpBox->startPos + tmpBox->extend.largesize;
        readPos = tmpBox->endPos;

        OneBoxAnalysis(tmpBox);

        tmpBox->fatherBox = box;
        box->fullBox.emplace_back(std::move(tmpBox));
        fseek(m_inFile, readPos, SEEK_SET);
    }
    //CloseFile();
}

void mp4::PrintBoxInfo(const std::shared_ptr<Box>& box, int depth) {
    if (box == nullptr) {
        return;
    }

    if (strcmp(box->header.type, "root") != 0) {
        std::cout<<box->header.type<<" : "<<box->header.size<<std::endl;
    }

    for (auto& item : box->fullBox) {
        for (int i = 0; i < depth; ++i) {
            std::cout<<"----";
        }
        PrintBoxInfo(item, depth + 1);
    }
}

int mp4::GetFtypInfo(const std::shared_ptr<Box> &box) {
    if (strcmp(box->header.type, "ftyp") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }

    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    uint8_t major_brand[4];
    uint8_t minor_version[4];
    memset(major_brand, 0, 4);
    memset(minor_version, 0, 4);
    std::vector<std::string> compatible_brands;

    fread(major_brand, 1, 4, m_inFile);
    fread(minor_version, 1, 4, m_inFile);
    for (int i = 0; i < box->header.size - 16; i += 4) {
        char brand[4];
        fread(brand, 1, 4, m_inFile);
        compatible_brands.emplace_back(brand);
    }

    m_mediaContext->codecId = std::string((char*)major_brand) +
            std::to_string(Tranform2Number(minor_version, 4)) + "(";
    for (int i = 0; i < compatible_brands.size(); ++i) {
        m_mediaContext->codecId += compatible_brands[i];
        if (i != compatible_brands.size() - 1) {
            m_mediaContext->codecId += "|";
        }
    }
    m_mediaContext->codecId += ")";
    return 0;
}

int mp4::GetMvhdInfo(const std::shared_ptr<Box> &box) {
    if (strcmp(box->header.type, "mvhd") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }
    struct MvhdInfo {
        uint8_t version[1];
        uint8_t label[3];
        uint8_t createTime[4];
        uint8_t modifyTime[4];
        uint8_t TimeScale[4];
        uint8_t duration[4];
        uint8_t speed[4];
        uint8_t volume[2];
        uint8_t saveByte[10];
        uint8_t matFormat[36];
        uint8_t previewTime[4];
        uint8_t previewDuration[4];
        uint8_t posterTime[4];
        uint8_t selectionTime[4];
        uint8_t selecionDuration[4];
        uint8_t curTime[4];
        uint8_t nextTrackId[4];
    } mvhdInfo{};

    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    fread(mvhdInfo.version, 1, 1, m_inFile);
    fread(mvhdInfo.label, 1, 3, m_inFile);
    fread(mvhdInfo.createTime, 1, 4, m_inFile);
    fread(mvhdInfo.modifyTime, 1, 4, m_inFile);
    fread(mvhdInfo.TimeScale, 1, 4, m_inFile);
    fread(mvhdInfo.duration, 1, 4, m_inFile);
    fread(mvhdInfo.speed, 1, 4, m_inFile);
    fread(mvhdInfo.volume, 1, 2, m_inFile);
    fread(mvhdInfo.saveByte, 1, 10, m_inFile);
    fread(mvhdInfo.matFormat, 1, 36, m_inFile);
    fread(mvhdInfo.previewTime, 1, 4, m_inFile);
    fread(mvhdInfo.previewDuration, 1, 4, m_inFile);
    fread(mvhdInfo.posterTime, 1, 4, m_inFile);
    fread(mvhdInfo.selectionTime, 1, 4, m_inFile);
    fread(mvhdInfo.selecionDuration, 1, 4, m_inFile);
    fread(mvhdInfo.curTime, 1, 4, m_inFile);
    fread(mvhdInfo.nextTrackId, 1, 4, m_inFile);

    m_mediaContext->timeScale = Tranform2Number(mvhdInfo.TimeScale, 4);
    m_mediaContext->duration = Tranform2Number(mvhdInfo.duration, 4) / m_mediaContext->timeScale;
    m_mediaContext->encodeDate = Tranform2Number(mvhdInfo.createTime, 4);
    m_mediaContext->taggedDate = Tranform2Number(mvhdInfo.modifyTime, 4);

    return 0;
}

int mp4::GetTkhdInfo(const std::shared_ptr<Box> &box, Track &track) {
    if (strcmp(box->header.type, "tkhd") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }
    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    fread(track.tkhdInfo.version, 1, 1, m_inFile);
    fread(track.tkhdInfo.flag, 1, 3, m_inFile);
    fread(track.tkhdInfo.createTime, 1, 4, m_inFile);
    fread(track.tkhdInfo.modifyTime, 1, 4, m_inFile);
    fread(track.tkhdInfo.trackId, 1, 4, m_inFile);
    fread(track.tkhdInfo.saveByte, 1, 4, m_inFile);
    fread(track.tkhdInfo.duration, 1, 4, m_inFile);
    fread(track.tkhdInfo.saveByte1, 1, 8, m_inFile);
    fread(track.tkhdInfo.layer, 1, 2, m_inFile);
    fread(track.tkhdInfo.alternateGroup, 1, 2, m_inFile);
    fread(track.tkhdInfo.volume, 1, 2, m_inFile);
    fread(track.tkhdInfo.saveByte2, 1, 2, m_inFile);
    fread(track.tkhdInfo.matFormat, 1, 36, m_inFile);
    fread(track.tkhdInfo.width, 1, 4, m_inFile);
    fread(track.tkhdInfo.height, 1, 4, m_inFile);

    return 0;
}

int mp4::GetMdhdInfo(const std::shared_ptr<Box> &box, Track &track) {
    if (strcmp(box->header.type, "mdhd") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }
    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    auto& mdhdInfo = track.mdiaInfo.mdhdInfo;

    fread(mdhdInfo.version, 1, 1, m_inFile);
    fread(mdhdInfo.flag, 1, 3, m_inFile);
    fread(mdhdInfo.createDate, 1, 4, m_inFile);
    fread(mdhdInfo.modifyDate, 1, 4, m_inFile);
    fread(mdhdInfo.timeScale, 1, 4, m_inFile);
    fread(mdhdInfo.duration, 1, 4, m_inFile);
    fread(mdhdInfo.language, 1, 2, m_inFile);
    fread(mdhdInfo.quality, 1, 2, m_inFile);
    return 0;
}

int mp4::GetHdlrInfo(const std::shared_ptr<Box> &box, Track &track) {
    if (strcmp(box->header.type, "hdlr") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }
    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    auto& hdlrInfo = track.mdiaInfo.hdlrInfo;

    fread(hdlrInfo.version, 1, 1, m_inFile);
    fread(hdlrInfo.flag, 1, 3, m_inFile);
    fread(hdlrInfo.type, 1, 4, m_inFile);
    fread(hdlrInfo.subType, 1, 4, m_inFile);
    fread(hdlrInfo.saveByte, 1, 12, m_inFile);
    hdlrInfo.componentName.len = box->header.size - 32;
    hdlrInfo.componentName.startPos = box->startPos + 8 + 24;

    return 0;
}

int mp4::ParseBoxInfo() {
    for (auto &box : root->fullBox) {
        if (strcmp(box->header.type, "ftyp") == 0) {
            GetFtypInfo(box);
        } else if (strcmp(box->header.type, "moov") == 0) {
            GetMoovInfo(box);
        }
    }
    return 0;
}

int mp4::GetMoovInfo(const std::shared_ptr<Box> &box) {
    for (auto &fullbox : box->fullBox) {
        if (strcmp(fullbox->header.type, "mvhd") == 0) {
            GetMvhdInfo(fullbox);
        } else if (strcmp(fullbox->header.type, "trak") == 0) {
            Track track;
            GetTrakInfo(fullbox, track);
            m_mediaContext->tracks.emplace_back(std::move(track));
        }
    }
    return 0;
}

int mp4::GetTrakInfo(const std::shared_ptr<Box> &box, Track &track) {
    for (auto &fullbox : box->fullBox) {
        if (strcmp(fullbox->header.type, "tkhd") == 0) {
            GetTkhdInfo(fullbox, track);
        } else if (strcmp(fullbox->header.type, "edts") == 0) {
            GetEdtsInfo(fullbox, track);
        } else if (strcmp(fullbox->header.type, "mdia") == 0) {
            GetMdiaInfo(fullbox, track);
        }
    }
    return 0;
}

int mp4::GetMdiaInfo(const std::shared_ptr<Box> &box, Track &track) {
    for (auto &fullbox : box->fullBox) {
        if (strcmp(fullbox->header.type, "mdhd") == 0) {
            GetMdhdInfo(fullbox, track);
        } else if (strcmp(fullbox->header.type, "hdlr") == 0) {
            GetHdlrInfo(fullbox, track);
        } else if (strcmp(fullbox->header.type, "minf") == 0) {
            GetMinfInfo(fullbox, track);
        }
    }
    return 0;
}

int mp4::GetMinfInfo(const std::shared_ptr<Box> &box, Track &track) {
    for (auto &fullbox : box->fullBox) {
        if (strcmp(fullbox->header.type, "vmhd") == 0) {
            GetVmhdInfo(fullbox, track);
        } else if (strcmp(fullbox->header.type, "smhd") == 0) {
            GetSmhdInfo(fullbox, track);
        } else if (strcmp(fullbox->header.type, "dinf") == 0) {
            GetDinfInfo(fullbox, track);
        } else if (strcmp(fullbox->header.type, "stbl") == 0) {
            GetStblInfo(fullbox, track);
        }
    }
    return 0;
}

int mp4::GetVmhdInfo(const std::shared_ptr<Box> &box, Track &track) {
    if (strcmp(box->header.type, "vmhd") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }
    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    auto &vmhdInfo = track.mdiaInfo.minfInfo.vmhdInfo;
    fread(vmhdInfo.version, 1, 1, m_inFile);
    fread(vmhdInfo.flag, 1, 3, m_inFile);
    fread(vmhdInfo.graphMode, 1, 2, m_inFile);
    fread(vmhdInfo.Opcolor, 1, 6, m_inFile);

    return 0;
}

int mp4::GetSmhdInfo(const std::shared_ptr<Box> &box, Track &track) {
    if (strcmp(box->header.type, "smhd") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }
    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    auto &smhdInfo = track.mdiaInfo.minfInfo.smhdInfo;
    fread(smhdInfo.version, 1, 1, m_inFile);
    fread(smhdInfo.flag, 1, 3, m_inFile);
    fread(smhdInfo.balance, 1, 2, m_inFile);
    fread(smhdInfo.save, 1, 2, m_inFile);
    return 0;
}

int mp4::GetDinfInfo(const std::shared_ptr<Box> &box, Track &track) {
    for (auto& fullbox : box->fullBox) {
        GetDrefInfo(fullbox, track);
    }

    return 0;
}

int mp4::GetStblInfo(const std::shared_ptr<Box> &box, Track &track) {
    return 0;
}

int mp4::GetDrefInfo(const std::shared_ptr<Box> &box, Track &track) {
    if (strcmp(box->header.type, "dref") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }
    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    auto &drefInfo = track.mdiaInfo.minfInfo.dinfInfo.drefInfo;
    fread(drefInfo.version, 1, 1, m_inFile);
    fread(drefInfo.flag, 1, 3, m_inFile);
    fread(drefInfo.dataRefNum, 1, 4, m_inFile);

    uint32_t pos = box->startPos + 8 + 4;
    for (int i = 0; i < Tranform2Number(drefInfo.dataRefNum, 4); ++i) {
        Track::MdiaInfo::MinfInfo::DinfInfo::DrefInfo::DataRefence dataReference = {};
        fread(dataReference.size, 1, 4, m_inFile);
        fread(dataReference.type, 1, 4, m_inFile);
        fread(dataReference.version, 1, 1, m_inFile);
        fread(dataReference.flag, 1, 3, m_inFile);
        drefInfo.dataVec.emplace_back(std::move(dataReference));
    }
    return 0;
}

int mp4::GetEdtsInfo(const std::shared_ptr<Box> &box, Track &track) {
    for (auto& fullbox : box->fullBox) {
        GetElstInfo(fullbox, track);
    }
    return 0;
}

int mp4::GetElstInfo(const std::shared_ptr<Box> &box, Track &track) {
    if (strcmp(box->header.type, "elst") != 0) {
        return -1;
    }
    if (m_inFile == nullptr || m_mediaContext == nullptr) {
        return -1;
    }
    fseek(m_inFile, box->startPos + 8, SEEK_SET);

    auto &elstInfo = track.edtsInfo.elstInfo;
    fread(elstInfo.version, 1, 1, m_inFile);
    fread(elstInfo.flag, 1, 3, m_inFile);
    fread(elstInfo.dataNum, 1, 4, m_inFile);

    int readSize = elstInfo.version[0] == 1? 8 : 4;
    fread(elstInfo.segmentDuration, 1, 8, m_inFile);
    if (elstInfo.version[0] == 1) {
        fread(elstInfo.mediaTime, 1, 8, m_inFile);
    } else {
        memcpy(elstInfo.mediaTime, elstInfo.segmentDuration + 4, 4);
        memset(elstInfo.segmentDuration + 4, 0, 4);
    }

    fread(elstInfo.mediaRate, 1, 2, m_inFile);
    fread(elstInfo.mediaRateFraction, 1, 2, m_inFile);

    return 0;
}