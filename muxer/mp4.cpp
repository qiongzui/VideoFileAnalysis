//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "mp4.h"
#include <iostream>

void mp4::Process() {
    root->header.size = INT32_MAX;
    memcpy(root->header.type, "root", 4);
    root->startPos = 0;
    root->endPos = INT32_MAX;

    OneBoxAnalysis(root);
    PrintBoxInfo(root, 0);
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

        char context[8];
        memset(context, 0 , 8);

        fread(context, 1, 8, m_inFile);
        uint64_t size = Tranform2Number(context, 4);
        if (size < 8 || size > end - start - 8) {
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

        if (strcmp(tmpBox->header.type, "ftyp") == 0) {
            GetFtypInfo(tmpBox);
        } else {
            OneBoxAnalysis(tmpBox);
        }

        box->containers.emplace_back(std::move(tmpBox));
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

    for (auto& item : box->containers) {
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

    char major_brand[5];
    char minor_version[5];
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

    m_mediaContext->codecId = std::string(major_brand) +
            std::to_string(Tranform2Number(minor_version, 4)) + "(";
    for (int i = 0; i < compatible_brands.size(); ++i) {
        m_mediaContext->codecId += compatible_brands[i];
        if (i != compatible_brands.size() - 1) {
            m_mediaContext->codecId += "|";
        }
    }
    m_mediaContext->codecId += ")";
}
