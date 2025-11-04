#include "my_camera.hpp"
#include <iostream>

myCamera::myCamera() 
    : _handle_(nullptr),
      _wait_msec_(100),
      _is_grabbing_(false),
      _is_device_opened_(false),
      _is_handle_created_(false) {
    int ret = MV_CC_EnumDevices(MV_USB_DEVICE, &_device_list_);
}

myCamera::~myCamera() {
    if (_is_grabbing_) {
        int ret = MV_CC_StopGrabbing(_handle_);
        if (ret == MV_OK) {
            _is_grabbing_ = false;
        }
    }

    if (_is_device_opened_) {
        int ret = MV_CC_CloseDevice(_handle_);
        if (ret == MV_OK){
            _is_device_opened_ = false;
        }
    }

    if (_is_handle_created_) {
        int ret = MV_CC_DestroyHandle(_handle_);
        if (ret == MV_OK){
            _handle_ = nullptr;
            _is_handle_created_ = false;
        }
    }
}

cv::Mat myCamera::_transfer_frame_(MV_FRAME_OUT& raw) {
    cv::Mat img(cv::Size(raw.stFrameInfo.nWidth, raw.stFrameInfo.nHeight), CV_8U, raw.pBufAddr);
    MV_CC_PIXEL_CONVERT_PARAM cvt_param;
    cvt_param.nWidth = raw.stFrameInfo.nWidth;
    cvt_param.nHeight = raw.stFrameInfo.nHeight;
    cvt_param.pSrcData = raw.pBufAddr;
    cvt_param.nSrcDataLen = raw.stFrameInfo.nFrameLen;
    cvt_param.enSrcPixelType = raw.stFrameInfo.enPixelType;
    cvt_param.pDstBuffer = img.data;
    cvt_param.nDstBufferSize = img.total() * img.elemSize();
    cvt_param.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
    auto pixel_type = raw.stFrameInfo.enPixelType;
    cv::cvtColor(img, img, _type_map_.at(pixel_type));
    return img;
}

bool myCamera::read(cv::Mat& frame) {
    if (_device_list_.nDeviceNum == 0) {
        return false;
    }

    if (!_is_handle_created_) {
        int ret = MV_CC_CreateHandle(&_handle_, _device_list_.pDeviceInfo[0]);
        if (ret != MV_OK) {
            return false;
        }
        _is_handle_created_ = true;
    }

    if (!_is_device_opened_) {
        int ret = MV_CC_OpenDevice(_handle_);
        if (ret != MV_OK) {
            return false;
        }
        _is_device_opened_ = true;
        int ret_param = 0;
        ret_param = MV_CC_SetEnumValue(_handle_, "BalanceWhiteAuto", MV_BALANCEWHITE_AUTO_CONTINUOUS);
        ret_param = MV_CC_SetEnumValue(_handle_, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
        ret_param = MV_CC_SetEnumValue(_handle_, "GainAuto", MV_GAIN_MODE_OFF);
        ret_param = MV_CC_SetFloatValue(_handle_, "ExposureTime", 10000);
        ret_param = MV_CC_SetFloatValue(_handle_, "Gain", 20);
        ret_param = MV_CC_SetFrameRate(_handle_, 60);
    }

    if (!_is_grabbing_) {
        int ret = MV_CC_StartGrabbing(_handle_);
        if (ret != MV_OK) {
            return false;
        }
        _is_grabbing_ = true;
    }
    int ret = MV_CC_GetImageBuffer(_handle_, &_raw_frame_, _wait_msec_);
    if (ret != MV_OK) {
        return false;
    }
    frame = _transfer_frame_(_raw_frame_);
    ret = MV_CC_FreeImageBuffer(_handle_, &_raw_frame_);
    return true;
}