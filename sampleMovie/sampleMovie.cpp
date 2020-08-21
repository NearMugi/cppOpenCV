#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

using namespace std;

template <typename ... Args>
std::string format(const std::string& fmt, Args ... args )
{
    size_t len = std::snprintf( nullptr, 0, fmt.c_str(), args ... );
    std::vector<char> buf(len + 1);
    std::snprintf(&buf[0], len + 1, fmt.c_str(), args ... );
    return std::string(&buf[0], &buf[0] + len);
}

int main(void)
{   
	// 時間計測
	chrono::system_clock::time_point loopTime, processStart, processEnd;
	double processTime, waitTime;

    std::string filepath = "sampleTimeCode.mp4";
	cv::VideoCapture cap(filepath);
	if (cap.isOpened() == false) {
		return -1;
	}
	double fps=cap.get(cv::CAP_PROP_FPS);
	std::cout << format("FPS:%f",fps) << std::endl;

	std::string windowName = "Video";
    cv::Mat image;
	loopTime = chrono::system_clock::now();
	while (1) {
		processStart = chrono::system_clock::now();
		
		// 1Loopの時間を取得
		double tmpLoopTime = static_cast<double>(chrono::duration_cast<chrono::microseconds>(processStart - loopTime).count() / 1000.0);
		loopTime = chrono::system_clock::now();
		
		// videoからimageへ1フレームを取り込む
		if(!cap.read(image)){
			// LOOP
			cap.set(cv::CAP_PROP_POS_FRAMES, 0);
			cap.read(image);
		};
		cv::putText(image, format("Target FPS:%f",fps), cv::Point(10,30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
		cv::putText(image, format(" Loop Time:%f",tmpLoopTime), cv::Point(10,60), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
		cv::imshow(windowName, image);

 		//ESCを押すと終了		
		if (cv::waitKey(1) == 27) break;
		
		
		// 処理時間を考慮して待つ
		processEnd = chrono::system_clock::now();
		processTime = static_cast<double>(chrono::duration_cast<chrono::microseconds>(processEnd - processStart).count() / 1000.0);
		waitTime = fps - processTime;
		if(waitTime >= 1){
			cv::waitKey(waitTime);
		}
	}

	cv::destroyWindow(windowName);
    return 0;
}