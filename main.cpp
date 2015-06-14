#include <iostream>

#include <glog/logging.h>


using namespace std;
#include "content_converter.h"

int main(int argc, char **argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = true;
    const char *path = "./test_mesh2.meshdata.testoutput";
    LOG(INFO) << "Loading data from " << path;
    convert_scene("./test-data.fbx", path);
    LOG(INFO) << "Finished conversion.";
    return 0;
}

