#include <iostream>
#include <string>
#include <fstream>
#include <mp4.h>
#include "flv.h"

using namespace std;
int main() {
    string path = "/Users/zhengzhenhao/Documents/VideoSource/test1.mp4";
    string path1 = "/Users/zhengzhenhao/Documents/VideoSource/test1.flv";


    mp4 muxer;
    muxer.init(path);
    muxer.Process();

    flv flvmuxer;
    flvmuxer.init(path1);
    flvmuxer.Process();

    return 0;
}
