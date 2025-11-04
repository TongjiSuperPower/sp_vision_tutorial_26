#include <iostream>
#include <opencv2/opencv.hpp>
#include "io/my_camera.hpp"
#include "tasks/yolo.hpp"
#include "tasks/armor.hpp"
#include "tools/img_tools.hpp"

int main() {
    myCamera camera;
    auto_aim::YOLO yolo("./configs/yolo.yaml");
    cv::Mat frame;
    std::list<auto_aim::Armor> armors;
    int frame_count = 0;

    while (true) {
        if (!camera.read(frame)) {
            cv::waitKey(1000);
            continue;
        }

        armors = yolo.detect(frame, frame_count);
        frame_count++;

        for (const auto& armor : armors) {
            std::vector<cv::Point> armor_points;
            for (const auto& pt : armor.points) {
                armor_points.emplace_back(cv::Point(static_cast<int>(pt.x), static_cast<int>(pt.y)));
            }
            tools::draw_points(frame, armor_points, cv::Scalar(0, 0, 255), 2);
        }

        cv::imshow("Camera-YOLO Armor Detection", frame);

        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
