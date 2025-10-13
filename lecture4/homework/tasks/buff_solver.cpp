#include "buff_solver.hpp"
#include <opencv2/core/eigen.hpp>
#include <iostream>
#include "buff_type.hpp"
#include "yolo11_buff.hpp"

namespace auto_buff
{
void Buff_Solver::setImagePointsFromFanBlade(const std::vector<FanBlade>& fanblades)
{
    if (fanblades.empty())
    {
        image_points.clear();
        return;
    }

    const FanBlade& fanblade = fanblades[0];
    const std::vector<cv::Point2f>& fanblade_points = fanblade.points;

    if (fanblade_points.size() != 6)
    {
        image_points.clear();
        return;
    }

    image_points.clear();
    for (int i = 0; i < 4; ++i)
    {
        image_points.emplace_back(
            static_cast<double>(fanblade_points[i].x), 
            static_cast<double>(fanblade_points[i].y)
        );
    }
}

void Buff_Solver::solvePnP()
{
    if (image_points.empty())
    {
        return;
    }

    bool solve_success = cv::solvePnP(
        world_points,
        image_points,
        camera_matrix,
        dist_coeffs,
        rvec,
        tvec,
        false,
        cv::SOLVEPNP_ITERATIVE
    );

    cv::cv2eigen(tvec, inner_circle_center);
}

void Buff_Solver::calculateRotationCenter()
{
    const double L = 0.7;

    double x_inner = inner_circle_center.x();
    double y_inner = inner_circle_center.y();
    double z_inner = inner_circle_center.z();

    double inner_origin_dist = sqrt(x_inner * x_inner + y_inner * y_inner);
    if (inner_origin_dist < 0.01)
    {
        return;
    }

    double x_rot = x_inner - (y_inner / inner_origin_dist) * L;
    double y_rot = y_inner + (x_inner / inner_origin_dist) * L;
    double z_rot = z_inner;                                      

    static Eigen::Vector3d prev_rot_center(0.0, 0.0, 0.0);
    static int frame_cnt = 0;
    frame_cnt++;

    if (frame_cnt <= 5)
    {
        rotation_center = (prev_rot_center * (frame_cnt - 1) + Eigen::Vector3d(x_rot, y_rot, z_rot)) / frame_cnt;
    }
    else
    {
        rotation_center = Eigen::Vector3d(x_rot, y_rot, z_rot) * 0.7 + prev_rot_center * 0.3;
    }

    prev_rot_center = rotation_center;
}

Eigen::Vector3d Buff_Solver::getInnerCircleCenter() const
{
    return inner_circle_center;
}

Eigen::Vector3d Buff_Solver::getRotationCenter() const
{
    return rotation_center;
}

} // namespace auto_buff