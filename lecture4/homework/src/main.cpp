#include "tasks/buff_detector.hpp"
#include "tasks/buff_solver.hpp"
#include "tools/plotter.hpp"
#include "io/camera.hpp"
#include <chrono>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

int main()
{
    io::Camera camera(2.5, 16.9, "2bdf:0001");
    std::chrono::steady_clock::time_point timestamp;  
    auto_buff::Buff_Detector detector;
    auto_buff::Buff_Solver solver;
    tools::Plotter plotter("127.0.0.1", 9870);  

    cv::VideoCapture cap("assets/test.avi");

    while(true)
    {
        cv::Mat img;
        // camera.read(img, timestamp);
        cap >> img;

        auto fanblades = detector.detect(img);
        if (fanblades.empty())
        {
            cv::imshow("Camera View", img);
            if (cv::waitKey(1) == 27) break;
            continue;
        }

        solver.setImagePointsFromFanBlade(fanblades);
        solver.solvePnP();
        Eigen::Vector3d inner_center = solver.getInnerCircleCenter();

        solver.calculateRotationCenter();
        Eigen::Vector3d rot_center = solver.getRotationCenter();

        nlohmann::json plot_json;
        auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()
        );
        plot_json["timestamp"] = static_cast<double>(now_ms.count()) / 1000.0;
        
        plot_json["inner_circle_center_x"] = inner_center.x();
        plot_json["inner_circle_center_y"] = inner_center.y();
        plot_json["inner_circle_center_z"] = inner_center.z();
        plot_json["rotation_center_x"] = rot_center.x();
        plot_json["rotation_center_y"] = rot_center.y();
        plot_json["rotation_center_z"] = rot_center.z();

        plotter.plot(plot_json);

        cv::imshow("Camera View", img);
        if (cv::waitKey(1) == 27)
        {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}