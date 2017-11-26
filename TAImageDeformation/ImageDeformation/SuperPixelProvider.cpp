//
//  SuperPixelProvider.cpp
//  ImageDeformation
//
//  Created by Tayfun Ateş on 26.11.2017.
//  Copyright © 2017 Tayfun Ateş. All rights reserved.
//

#include "SuperPixelProvider.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ximgproc/slic.hpp>
#include <opencv2/ximgproc/seeds.hpp>
#include <opencv2/ximgproc/lsc.hpp>

namespace SP
{
    void SuperPixelProvider::extractSuperPixels(const cv::Mat& inputImage, SuperPixelExtractionParams* params, bool visualize)
    {
        if(params != nullptr)
        {
            std::vector<cv::Mat> result;
            if(params->getSuperPixelType() == SuperPixel_SEEDS)
            {
                result = extractSEEDSSuperPixels(inputImage, (SuperPixelSEEDSExtractionParams*) params);
            }
            else if(params->getSuperPixelType() == SuperPixel_SLIC)
            {
                result = extractSLICSuperPixels(inputImage, (SuperPixelSLICExtractionParams*) params);
            }
            else if(params->getSuperPixelType() == SuperPixel_LSC)
            {
                result = extractLSCSuperPixels(inputImage, (SuperPixelLSCExtractionParams*) params);
            }
            else
            {
                assert("Superpixel extraction method is not defined.");
            }
            
            if(result.size() < 2)
            {
                assert("Label and mask images cannot be created from super pixel extraction method.");
            }
            
            if(visualize)
            {
                //Visualize the super pixels extracted on the input image
                const cv::Mat& labels = result[0];
                const cv::Mat& mask = result[1];
                
                cv::imshow("SuperPixels", mask);
                cv::waitKey(0);
            }
        }
    }
    
    SuperPixelExtractionResult SuperPixelProvider::extractSEEDSSuperPixels(const cv::Mat& inputImage, SuperPixelSEEDSExtractionParams* params)
    {
        std::vector<cv::Mat> result;
        
        if(params != nullptr)
        {
            cv::Ptr<cv::ximgproc::SuperpixelSEEDS> superPixelImpl = cv::ximgproc::createSuperpixelSEEDS(inputImage.cols, inputImage.rows, inputImage.channels(), params->m_nNumberOfSuperpixels, params->m_nNumberOfLevels, params->m_nPrior, params->m_nNumberOfHistogramBins , params->m_bEnableDoubleStep);
            superPixelImpl->iterate(inputImage, params->m_nNumberOfIterations);
            
            cv::Mat mask;
            cv::Mat labels;
            superPixelImpl->getLabels(labels);
            
            int num_label_bits = 2;
            labels &= (1<<num_label_bits)-1;
            labels *= 1<<(16-num_label_bits);
            
            superPixelImpl->getLabelContourMask(mask);
            
            result.push_back(labels);
            result.push_back(mask);
        }
        
        return result;
    }
    
    SuperPixelExtractionResult SuperPixelProvider::extractSLICSuperPixels(const cv::Mat& inputImage, SuperPixelSLICExtractionParams* params)
    {
        std::vector<cv::Mat> result;
        
        if(params != nullptr)
        {
            cv::Ptr<cv::ximgproc::SuperpixelSLIC> superPixelImpl = cv::ximgproc::createSuperpixelSLIC(inputImage, params->m_nAlgorithmType, params->m_nRegionSize, params->m_fRuler);
            superPixelImpl->iterate(params->m_nNumberOfIterations);
            
            if(params->m_bEnforceLabelConnectivity)
            {
                superPixelImpl->enforceLabelConnectivity(params->m_nLabelConnectivityMinElemSize);
            }
            
            cv::Mat mask;
            cv::Mat labels;
            superPixelImpl->getLabels(labels);
            
            int num_label_bits = 2;
            labels &= (1<<num_label_bits)-1;
            labels *= 1<<(16-num_label_bits);
            
            superPixelImpl->getLabelContourMask(mask);
            
            result.push_back(labels);
            result.push_back(mask);
        }
        
        return result;
    }
    
    SuperPixelExtractionResult SuperPixelProvider::extractLSCSuperPixels(const cv::Mat& inputImage, SuperPixelLSCExtractionParams* params)
    {
        std::vector<cv::Mat> result;
        
        if(params != nullptr)
        {
            cv::Ptr<cv::ximgproc::SuperpixelLSC> superPixelImpl = cv::ximgproc::createSuperpixelLSC(inputImage, params->m_nRegionSize, params->m_fRatio);
            superPixelImpl->iterate(params->m_nNumberOfIterations);
            
            if(params->m_bEnforceLabelConnectivity)
            {
                superPixelImpl->enforceLabelConnectivity(params->m_nLabelConnectivityMinElemSize);
            }
            
            cv::Mat mask;
            cv::Mat labels;
            superPixelImpl->getLabels(labels);
            
            int num_label_bits = 2;
            labels &= (1<<num_label_bits)-1;
            labels *= 1<<(16-num_label_bits);
            
            superPixelImpl->getLabelContourMask(mask);
            
            result.push_back(labels);
            result.push_back(mask);
        }
        
        return result;
    }
}
