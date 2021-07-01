//
// Created by 郑蓁浩 on 2021/6/23.
//

#ifndef VIDEOFILEANALYSIS_PASERBASE_H
#define VIDEOFILEANALYSIS_PASERBASE_H

#include "MediaType.h"
#include <queue>
#include <thread>
class PaserBase {
public:
    virtual ~PaserBase() = default;

    virtual int32_t start() = 0;
    virtual int32_t stop() = 0;
    virtual int32_t flush() = 0;
    virtual void AsynSendData(const uint8_t* src) = 0;
    virtual MediaPacketSp AsynRecivePacket() = 0;

protected:
    std::condition_variable variable;
    std::mutex paserMutex;
    uint8_t *data;
    std::queue<MediaPacketSp> mediaPacketCache;
    std::thread thread;
    bool m_stop;
};


#endif //VIDEOFILEANALYSIS_PASERBASE_H
