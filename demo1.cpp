#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 读取本地视频文件，替换为你的文件路径
    VideoCapture cap("/home/shijian/桌面/vscode/build/resources/video_move_red.avi");
    if (!cap.isOpened()) {
        cout << "无法打开视频文件" << endl;
        return -1;
    }

    Mat frame;
    while (true) {
        cap >> frame; // 捕捉一帧
        if (frame.empty()) {
            cout << "视频播放完毕" << endl;
            break;
        }
        string ColorType = "Unknown";
        Mat hsv;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        Scalar lowerGreen(29,52,184); // 绿色下限
        Scalar upperGreen(97, 255, 255); // 绿色上限
        Scalar lowerRed(160,28,202); // 红色下限
        Scalar upperRed(179, 255, 255); // 红色上限
        Scalar lowerYellow(20, 100, 100); // 黄色下限
        Scalar upperYellow(30, 255, 255); // 黄色上限
        Mat mask1, mask2,mask3;
        inRange(hsv, lowerGreen, upperGreen, mask1);
        inRange(hsv, lowerRed, upperRed, mask2);
        inRange(hsv, lowerYellow, upperYellow, mask3);
        vector<vector<Point>> contours1, contours2, contours3;
        findContours(mask1, contours1, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        findContours(mask2, contours2, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        findContours(mask3, contours3, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        for (size_t i = 0; i < contours1.size(); i++) {
            double area = contourArea(contours1[i]);
            if (area > 150) {
            Rect boundingBox = boundingRect(contours1[i]);
            rectangle(frame, boundingBox, Scalar(0, 255, 0), 2);
            putText(frame, "Green Light", Point(boundingBox.x, boundingBox.y - 10),
            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
            ColorType = "Green";
            }
            if(area <= 150){
                break;
            }
        }
        for (size_t i = 0; i < contours2.size(); i++) {
            double area = contourArea(contours2[i]);
            if (area > 800) {
            Rect boundingBox = boundingRect(contours2[i]);
            rectangle(frame, boundingBox, Scalar(0, 255, 0), 2);
            putText(frame, "RED Light", Point(boundingBox.x, boundingBox.y - 10),
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1);
            ColorType = "Red";
            }
        }
        for (size_t i = 0; i < contours3.size(); i++) {
            double area = contourArea(contours3[i]);
            if (area > 250) {
            Rect boundingBox = boundingRect(contours3[i]);
            rectangle(frame, boundingBox, Scalar(0, 255, 255), 2);
            putText(frame, "Yellow Light", Point(boundingBox.x, boundingBox.y - 10),
            FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1);
            ColorType = "Yellow";
            }
            if(area <= 250){
                break;
            }
        }
        putText(frame, "Detected Color: " + ColorType, Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);
        imshow("Detected Light", frame);

        // 按 'q' 键退出
        if (waitKey(30) == 'q') {
            break;
        }
    }

    // 释放视频对象并关闭所有窗口
    cap.release();
    destroyAllWindows();
    return 0;
}