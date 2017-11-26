//
//  main.cpp
//  ImageDeformation
//
//  Created by Tayfun Ateş on 26.11.2017.
//  Copyright © 2017 Tayfun Ateş. All rights reserved.
//

#include "SuperPixelProvider.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int argc, const char * argv[]) {
    //Print OpenCV version here
    std::cout << CV_VERSION << std::endl;
    
    cv::Mat inputImage = cv::imread("/Users/tates/Workspace/self/ys/ImageDeformation/test.jpg", CV_LOAD_IMAGE_UNCHANGED);
    
    SP::SuperPixelLSCExtractionParams* params = new SP::SuperPixelLSCExtractionParams;
    SP::SuperPixelProvider::extractSuperPixels(inputImage, params, true);
    
    if(params)
    {
        delete params;
    }
    
    return 0;
}
