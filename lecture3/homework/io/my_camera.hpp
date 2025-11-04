#ifndef MY_CAMERA_HPP
#define MY_CAMERA_HPP

#include "io/hikrobot/include/MvCameraControl.h"
#include <opencv2/opencv.hpp>
#include <unordered_map>

class myCamera {
public:
    myCamera();
    ~myCamera();
    bool read(cv::Mat& frame);
private:
    void* _handle_;
    MV_CC_DEVICE_INFO_LIST _device_list_;
    MV_FRAME_OUT _raw_frame_;
    unsigned int _wait_msec_;
    bool _is_grabbing_;
    bool _is_device_opened_;
    bool _is_handle_created_;
    const std::unordered_map<MvGvspPixelType, cv::ColorConversionCodes> _type_map_ = {
        {PixelType_Gvsp_BayerGR8, cv::COLOR_BayerGR2RGB},
        {PixelType_Gvsp_BayerRG8, cv::COLOR_BayerRG2RGB},
        {PixelType_Gvsp_BayerGB8, cv::COLOR_BayerGB2RGB},
        {PixelType_Gvsp_BayerBG8, cv::COLOR_BayerBG2RGB}
    };
    cv::Mat _transfer_frame_(MV_FRAME_OUT& raw);
};

#endif