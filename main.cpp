#include <iostream>
#include <string>
#include <fstream>
#include <mp4.h>

using namespace std;
int main() {
    string path = "/Users/zhengzhenhao/Documents/VideoSource/test1.mp4";

    mp4 muxer;
    muxer.init(path);
    muxer.Process();

    return 0;
}
