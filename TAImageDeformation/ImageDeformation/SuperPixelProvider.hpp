//
//  SuperPixelProvider.hpp
//  ImageDeformation
//
//  Created by Tayfun Ateş on 26.11.2017.
//  Copyright © 2017 Tayfun Ateş. All rights reserved.
//

#ifndef SuperPixelProvider_hpp
#define SuperPixelProvider_hpp

#include <vector>

//OpenCV forward declarations
namespace cv {
    class Mat;
}

namespace SP
{
    //Enumaration for the type of the super pixel extraction algorithm
    enum SuperPixelType
    {
        SuperPixel_SEEDS = 1,
        SuperPixel_SLIC = 2,
        SuperPixel_LSC = 3
    };
    
    //Base class for super pixel extraction parameters
    class SuperPixelExtractionParams
    {
    public:
        SuperPixelExtractionParams() {}
        virtual ~SuperPixelExtractionParams() {}
        
        //Gets super pixel type, each parameter class implements its own version of this method
        virtual SuperPixelType getSuperPixelType() = 0;
        
        int m_nNumberOfIterations;              // Number of iterations. Higher number improves the result.
        
    protected:
        virtual void createWithDefaultValues()
        {
            m_nNumberOfIterations = 8;
        }
    };
    
    //SEEDS super pixels extraction parameters
    class SuperPixelSEEDSExtractionParams : public SuperPixelExtractionParams
    {
    public:
        SuperPixelSEEDSExtractionParams()
        {
            createWithDefaultValues();
        }
        virtual ~SuperPixelSEEDSExtractionParams() {}
        
        //It is SEEDS super pixels parameters class
        virtual SuperPixelType getSuperPixelType() { return SuperPixel_SEEDS; }
        
        int m_nNumberOfSuperpixels;             //  Desired number of superpixels. Note that the actual number may be smaller due to restrictions (depending on the image size and num_levels). Use getNumberOfSuperpixels() to get the actual number.
        int m_nNumberOfLevels;                  // Number of block levels. The more levels, the more accurate is the segmentation, but needs more memory and CPU time.
        int m_nPrior;                           // Enable 3x3 shape smoothing term if \>0. A larger value leads to smoother shapes. prior must be in the range [0, 5].
        int m_nNumberOfHistogramBins;           // Number of histogram bins.
        bool m_bEnableDoubleStep;               // If true, iterate each block level twice for higher accuracy.
        
    protected:
        virtual void createWithDefaultValues()
        {
            SuperPixelExtractionParams::createWithDefaultValues();
            m_nNumberOfSuperpixels = 4000;
            m_nNumberOfLevels = 5;
            m_nPrior = 5;
            m_nNumberOfHistogramBins = 5;
            m_bEnableDoubleStep = false;
        }
    };
    
    //SLIC super pixels extraction parameters
    class SuperPixelSLICExtractionParams : public SuperPixelExtractionParams
    {
    public:
        SuperPixelSLICExtractionParams()
        {
            createWithDefaultValues();
        }
        virtual ~SuperPixelSLICExtractionParams() {}
        
        //It is SLIC super pixels parameters class
        virtual SuperPixelType getSuperPixelType() { return SuperPixel_SLIC; }
        
        //Enum type of the SLIC algorithm
        enum SLICType
        {
            SLIC = 100,
            SLICO = 101
        };
        
        SLICType m_nAlgorithmType;
        int m_nRegionSize;                      // Chooses an average superpixel size measured in pixels
        float m_fRuler;                         // Chooses the enforcement of superpixel smoothness factor of superpixel
        bool m_bEnforceLabelConnectivity;       // Enforce label connectivity or not
        int m_nLabelConnectivityMinElemSize;    // min_element_size The minimum element size in percents that should be absorbed into a bigger superpixel. Given resulted average superpixel size valid value should be in 0-100 range.
        
    protected:
        virtual void createWithDefaultValues()
        {
            SuperPixelExtractionParams::createWithDefaultValues();
            m_nAlgorithmType = SLICType::SLICO;
            m_nRegionSize = 7;
            m_fRuler = 1.0f;
            m_bEnforceLabelConnectivity = true;
            m_nLabelConnectivityMinElemSize = 25;
        }
    };
    
    //LSC super pixels extraction parameters
    class SuperPixelLSCExtractionParams : public SuperPixelExtractionParams
    {
    public:
        SuperPixelLSCExtractionParams()
        {
            createWithDefaultValues();
        }
        virtual ~SuperPixelLSCExtractionParams() {}
        
        //It is LSC super pixels parameters class
        virtual SuperPixelType getSuperPixelType() { return SuperPixel_LSC; }
        
        int m_nRegionSize;                      // Chooses an average superpixel size measured in pixels
        float m_fRatio;                         // Chooses the enforcement of superpixel compactness factor of superpixel
        bool m_bEnforceLabelConnectivity;       // Enforce label connectivity.
        int m_nLabelConnectivityMinElemSize;    // The minimum element size in percents that should be absorbed into a bigger superpixel. Given resulted average superpixel size valid value should be in 0-100 range.
        
    protected:
        virtual void createWithDefaultValues()
        {
            SuperPixelExtractionParams::createWithDefaultValues();
            m_nRegionSize = 5;
            m_fRatio = 0.075f;
            m_bEnforceLabelConnectivity = false;
            m_nLabelConnectivityMinElemSize = 20;
        }
    };
    
    typedef std::vector<cv::Mat> SuperPixelExtractionResult;
    
    class SuperPixelProvider
    {
    public:
        //Extraction method of super pixels from an input image
        static void extractSuperPixels(const cv::Mat& inputImage, SuperPixelExtractionParams* params, bool visualize=false);
        
    private:
        
        //Extraction method for SEEDS super pixels
        static SuperPixelExtractionResult extractSEEDSSuperPixels(const cv::Mat& inputImage, SuperPixelSEEDSExtractionParams* params);
        
        //Extraction method for SLIC super pixels
        static SuperPixelExtractionResult extractSLICSuperPixels(const cv::Mat& inputImage, SuperPixelSLICExtractionParams* params);
        
        //Extraction method for LSC super pixels
        static SuperPixelExtractionResult extractLSCSuperPixels(const cv::Mat& inputImage, SuperPixelLSCExtractionParams* params);
    };
}

#endif /* SuperPixelProvider_hpp */
