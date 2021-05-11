//
// Created by 郑蓁浩 on 2021/5/11.
//

#include "mp4.h"
#include <iostream>

void mp4::Process() {
    char context[4];

    while (context) {
        m_infile.get(context, 4);
        std::cout<< context << std::endl;
    }

}
