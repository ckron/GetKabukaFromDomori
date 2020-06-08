#include <filesystem>
#include <string>

#include <opencv2/opencv.hpp>

#include "KabukaDetector.h"

cv::Mat adjustImage900(cv::Mat src);
std::vector<std::string> getDirectoryPathList(const char* dirPath);

cv::Mat adjustImage900(cv::Mat src)
{
	cv::Mat dst;
	cv::resize(src, dst, cv::Size(900, 506), cv::INTER_LINEAR);

	return dst;
}

std::vector<std::string> getDirectoryPathList(const char* dirPath)
{
	std::vector<std::string> pathList;
	for(const std::filesystem::directory_entry& f
				: std::filesystem::directory_iterator(dirPath)) {
		pathList.push_back(f.path().string());
	}

	return pathList;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> pathList = getDirectoryPathList("img/sample");

	KabukaDetector kd;

	for(auto p : pathList) {
		cv::Mat src = cv::imread(p);
		if (src.cols != 900) {
			src = adjustImage900(src);
		}
		int kabuka = kd.pickupKabuka(src);

		std::cout << kabuka << std::endl;
		cv::imshow("image", src);
	}

	cv::waitKey();

	return 0;
}