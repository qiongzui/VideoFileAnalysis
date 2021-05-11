#include <iostream>
#include <string>
#include <fstream>
#include <mp4.h>

using namespace std;
int main() {
    string path = "/Users/zhengzhenhao/Documents/test.mp4";

    ifstream inFile;
    inFile.open(path, ios::in | ios::binary);
    if (!inFile.is_open()) {
        cout<< "ERROR: file is not exist"<< endl;
        return -1;
    }

    mp4 muxer;
    muxer.SetDataFile(move(inFile));
    muxer.Process();

    inFile.close();
    return 0;
}
