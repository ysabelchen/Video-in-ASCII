#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>
#include <iostream>
using namespace std;
using namespace cv;

string pixelToASCII(int pixel_intensity) {
    // the ascii character depends on lightness/darkness of that pixel
    // the brighter the pixel, the closer to the "%" and "@" characters the ascii char will be (I prefer this)
        // emphasizes the characters/objects in motion
    const string ASCII_CHARS = "  ._-=+*!#%@";
    // reverse the ascii chars for a different result. the brighter the pixel, the closer to the "." and " " characters the ascii char will be
        // emphasizes the background
    // const string ASCII_CHARS = "@%#!*+=-_.  ";
    string s = string(1, ASCII_CHARS[pixel_intensity * ASCII_CHARS.length() / 256]);
    return s;
}

int main() {
    // vid1.mp4: https://www.pexels.com/video/a-dog-fights-with-his-reflection-in-the-mirror-3042473/
    // vid2.mp4: https://www.pexels.com/video/falcon-flying-and-land-on-a-wooden-perch-7668765/
    string video_path = "/Users/22ysabelc/Desktop/Personal Projects/C++ Video to ASCII/vid1.mp4";
    VideoCapture capture(video_path);

    double fps = capture.get(CAP_PROP_FPS);
    // cout << fps << endl;

    int frame_duration_ms = 1000 / fps;

    int width = 200;
    int height = 40;

    // gets video resolution
    int frame_width = capture.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = capture.get(CAP_PROP_FRAME_HEIGHT);
    //cout << frame_width << " " << frame_height << endl;

    // adjust height to be proportional to width depending on the resolution of the video
    height = (width * frame_height / frame_width) * 0.4194;

    Mat frame, gray_frame, resized_frame;

    while(true) {
        capture >> frame;
        if (frame.empty())
            break;

        // change to grayscale
        cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
        resize(gray_frame, resized_frame, Size(width, height), 0, 0, INTER_LINEAR);

        // creates each frame of the video
        string ascii_frame;
        // for each row, the code will traverse through each column element and replace the pixel there with an ascii char
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                ascii_frame += pixelToASCII(resized_frame.at<uchar>(i, j));
            }
            ascii_frame += "\n"; // so the frames don't blend together
        }
        system("clear"); // clears console
        cout << ascii_frame;
        // adds a slight delay
        std::this_thread::sleep_for(std::chrono::milliseconds(frame_duration_ms - 10));
    }

    return 0;
}
