//
// Created by 郑蓁浩 on 2021/5/11.
//

#ifndef VIDEOFILEANALYSIS_MUXERBASE_H
#define VIDEOFILEANALYSIS_MUXERBASE_H

#include <fstream>
#include <vector>
#include <memory>

class MuxerBase {
public:
    MuxerBase() = default;
    virtual ~MuxerBase() = default;

    void SetDataFile(const std::string &filePath) { m_filePath = filePath; }
    virtual void Process() = 0;

protected:
    // 将字符串转化为数值
    uint64_t Tranform2Number(char* src, int len);
    bool OpenFile();
    void CloseFile();

    std::string m_filePath;
    std::ifstream m_inFile;
};


#endif //VIDEOFILEANALYSIS_MUXERBASE_H
