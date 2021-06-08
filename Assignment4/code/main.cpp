#include <chrono>
#include <iostream>
#include <opencv2/opencv.hpp>

std::vector<cv::Point2f> control_points;

int pointsize = 4;

void mouse_handler(int event, int x, int y, int flags, void *userdata) 
{
    if (event == cv::EVENT_LBUTTONDOWN && control_points.size() < pointsize) 
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", "
        << y << ")" << '\n';
        control_points.emplace_back(x, y);
    }     
}

void naive_bezier(const std::vector<cv::Point2f> &points, cv::Mat &window) 
{
    auto &p_0 = points[0];
    auto &p_1 = points[1];
    auto &p_2 = points[2];
    auto &p_3 = points[3];

    for (double t = 0.0; t <= 1.0; t += 0.001) 
    {
        auto point = std::pow(1 - t, 3) * p_0 + 3 * t * std::pow(1 - t, 2) * p_1 +
                 3 * std::pow(t, 2) * (1 - t) * p_2 + std::pow(t, 3) * p_3;

        window.at<cv::Vec3b>(point.y, point.x)[2] = 255;
    }
}

cv::Point2f lerp_v2f(const cv::Point2f& a, const cv::Point2f& b, float t)
{
    return a + (b - a) * t;
}


/**
 * 贝塞尔曲线递归算法，计算每个t对一个的曲线轨迹点
 * 
 * control_points:控制贝塞尔曲线的点
 * t：插值比例/权重
 */
cv::Point2f recursive_bezier(const std::vector<cv::Point2f> &control_points, float t) 
{

    if (control_points.size() == 1) {
        return control_points[0];
    }

    std::vector<cv::Point2f> lerp_points;
    for (size_t i = 1; i < control_points.size(); i++)
    {
        lerp_points.push_back(lerp_v2f(control_points[i - 1], control_points[i], t));
    }
    
    return recursive_bezier(lerp_points, t);

}

/**
 * 触发贝塞尔曲线插值算法，获取所有曲线点
 * 
 * control_points:控制贝塞尔曲线的点
 * window:绘制曲线/点的窗口
 */
void bezier(const std::vector<cv::Point2f> &control_points, cv::Mat &window) 
{

    // 每次迭代的幅度为0.001，设计足够小的步幅，使曲线更连贯，否则可能会有断点
    for (double i = 0.0; i < 1.0; i+=0.001)
    {
        // 每一次for循环迭代，都计算出曲线上的一个轨迹点
        auto point = recursive_bezier(control_points, i);
        // 点的颜色为绿色，注意通道为BGR
        window.at<cv::Vec3b>(point.y, point.x)[1] = 255;
    }
    

}

int main() 
{
    cv::Mat window = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0));
    cv::cvtColor(window, window, cv::COLOR_BGR2RGB);
    cv::namedWindow("Bezier Curve", cv::WINDOW_AUTOSIZE);

    cv::setMouseCallback("Bezier Curve", mouse_handler, nullptr);

    int key = -1;
    while (key != 27) 
    {
        for (auto &point : control_points) 
        {
            cv::circle(window, point, 3, {255, 255, 255}, 3);
        }

        if (control_points.size() == pointsize) 
        {
            naive_bezier(control_points, window);
            bezier(control_points, window);

            cv::imshow("Bezier Curve", window);
            cv::imwrite("my_bezier_curve.png", window);
            key = cv::waitKey(0);

            return 0;
        }

        cv::imshow("Bezier Curve", window);
        key = cv::waitKey(20);
    }

return 0;
}
