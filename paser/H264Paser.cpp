//
// Created by 郑蓁浩 on 2021/6/23.
//

#include "H264Paser.h"


H264Paser::~H264Paser() {
    if (thread.joinable()) {
        thread.join();
    }
}

void H264Paser::AsynSendData(const uint8_t *src) {

}

MediaPacketSp H264Paser::AsynRecivePacket() {
    return MediaPacketSp();
}

int32_t H264Paser::start() {
    thread = std::thread(std::bind(&H264Paser::ProcessThread, this));
    return 0;
}

void H264Paser::ProcessThread() {
    while (!m_stop) {
        std::unique_lock<std::mutex> lock(paserMutex);
        variable.wait(lock, [&]() {
           return m_stop || data != nullptr;
        });

        if (m_stop) {
            break;
        }

        MediaPacketSp packet = PaserOnePacket();
        if (packet != nullptr) {
            mediaPacketCache.emplace(packet);
        }
    }
}

MediaPacketSp H264Paser::PaserOnePacket() {
    MediaPacketSp packet = std::make_shared<MediaPacket>();
    if (data == nullptr) {
        return packet;
    }

    int32_t len = FindStartCode();
    if (len <= 0) {
        return packet;
    }
    uint8_t * onePacketBuf = reinterpret_cast<uint8_t*>(malloc(len));
    if (onePacketBuf == nullptr) {
        return packet;
    }

    memcpy(onePacketBuf, data, len);
    packet->data = onePacketBuf;
    return packet;
}

int32_t H264Paser::stop() {
    std::lock_guard<std::mutex> lock(paserMutex);
    m_stop = true;
    variable.notify_all();
    return 0;
}

int32_t H264Paser::flush() {
    std::lock_guard<std::mutex> lock(paserMutex);
    data = nullptr;
    while (!mediaPacketCache.empty()) {
        mediaPacketCache.pop();
    }


    return 0;
}

int32_t H264Paser::FindStartCode() {
    return -1;
}


