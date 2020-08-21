#include <opencv2/opencv.hpp>
#include <iostream>

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
    std::string filepath = "sampleTimeCode.mp4";
	cv::VideoCapture cap(filepath);
	if (cap.isOpened() == false) {
		return -1;
	}
	float fps=cap.get(cv::CAP_PROP_FPS);
	std::cout << format("FPS:%f",fps) << std::endl;

	std::string windowName = "Video";
    cv::Mat image;
	while (1) {
		// videoからimageへ1フレームを取り込む
		if(!cap.read(image)){
			// LOOP
			cap.set(cv::CAP_PROP_POS_FRAMES, 0);
			cap.read(image);
		};
		
		cv::imshow(windowName, image);
 		//ESCを押すと終了		
		if (cv::waitKey(1) == 27) break;

		cv::waitKey(fps);
	}
	cv::destroyWindow(windowName);
    return 0;
}