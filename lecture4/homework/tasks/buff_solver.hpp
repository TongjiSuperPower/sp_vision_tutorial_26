#ifndef AUTO_BUFF__SOLVER_HPP
#define AUTO_BUFF__SOLVER_HPP

#include <vector>
#include <Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include "buff_type.hpp"
#include "buff_detector.hpp"

namespace auto_buff
{
class Buff_Solver
{
public:
    Buff_Solver()
    {
        camera_matrix = (cv::Mat_<double>(3, 3) <<  1286.307063384126 , 0                  , 645.34450819155256, 
                                                    0                 , 1288.1400736562441 , 483.6163720308021 , 
                                                    0                 , 0                  , 1                   );
        
        dist_coeffs = (cv::Mat_<double>(5, 1) << -0.47562935060124745, 0.21831745829617311, 0.0004957613589406044, -0.00034617769548693592, 0);
        
        world_points = {
            cv::Point3d(0.15, 0.00, 0.0),  // 关键点0：内环右
            cv::Point3d(0.00, 0.15, 0.0),  // 关键点1：内环上
            cv::Point3d(-0.15, 0.00, 0.0), // 关键点2：内环左
            cv::Point3d(0.00, -0.15, 0.0)  // 关键点3：内环下
        };
    }

    void setImagePointsFromFanBlade(const std::vector<FanBlade>& fanblades);

    void solvePnP();

    void calculateRotationCenter();

    Eigen::Vector3d getInnerCircleCenter() const;

    Eigen::Vector3d getRotationCenter() const;

private:
    std::vector<cv::Point2d> image_points; 
    std::vector<cv::Point3d> world_points;
    cv::Mat camera_matrix;                   
    cv::Mat dist_coeffs;                     

    cv::Mat rvec;
    cv::Mat tvec;
    Eigen::Vector3d inner_circle_center;
    Eigen::Vector3d rotation_center;
};
} // namespace auto_buff

#endif // AUTO_BUFF__SOLVER_HPP