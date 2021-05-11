//
// Created by 郑蓁浩 on 2021/5/11.
//

#ifndef VIDEOFILEANALYSIS_MUXERBASE_H
#define VIDEOFILEANALYSIS_MUXERBASE_H

#include <fstream>

class MuxerBase {
public:
    MuxerBase() = default;
    virtual ~MuxerBase() = default;

    void SetDataFile(std::ifstream file) { m_infile = move(file); }
    virtual void Process() = 0;

protected:
    std::ifstream m_infile;
};


#endif //VIDEOFILEANALYSIS_MUXERBASE_H
