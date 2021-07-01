//
// Created by 郑蓁浩 on 2021/6/23.
//

#ifndef VIDEOFILEANALYSIS_H264PASER_H
#define VIDEOFILEANALYSIS_H264PASER_H

#include <vector>

#include "PaserBase.h"

class H264Paser : public PaserBase{
public:
    H264Paser() = default;
    ~H264Paser() override;

    int32_t start() override ;
    int32_t stop() override ;
    int32_t flush() override ;
    void AsynSendData(const uint8_t* src) override ;
    MediaPacketSp AsynRecivePacket() override ;

private:
    void ProcessThread();
    MediaPacketSp PaserOnePacket();
    int32_t FindStartCode();
};


#endif //VIDEOFILEANALYSIS_H264PASER_H
