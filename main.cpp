#include <filesystem>
#include <string>

#include <opencv2/opencv.hpp>

#include "KabukaDetector.h"
#include "Graph.h"

std::vector<std::string> getDirectoryPathList(const char* dirPath);

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
	std::vector<int> kabukaList;
	std::vector<char[3]> dateList;

	for(auto p : pathList) {
		cv::Mat src = cv::imread(p);
		if (src.cols != 1280) {
			std::cout << "‰ð‘œ“x‚ª³‚µ‚­‚ ‚è‚Ü‚¹‚ñ(1280x720 only)" << std::endl;
			getchar();
			exit(0);
		}
		int kabuka = kd.pickupKabuka(src);

		kabukaList.push_back(kabuka);
		std::cout << kabuka << std::endl;
	}

	Graph gp;

	gp.setData(&kabukaList, &dateList);

	cv::imshow("image", gp.draw(800, 600));
	cv::waitKey();

	return 0;
}