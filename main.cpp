#include <filesystem>
#include <string>

#include <opencv2/opencv.hpp>

#include "KabukaDetector.h"
#include "Graph.h"

std::vector<std::string> split(std::string str, char delim);
std::vector<std::string> getDirectoryPathList(const char* dirPath);

std::vector<std::string> split(std::string str, char delim)
{
	std::vector<std::string> ret;
	std::string temp;
	for (const auto& c : str) {
		if (c == delim) {
			ret.push_back(temp);
			temp = "";
		}
		else {
			temp.push_back(c);
		}
	}
	ret.push_back(temp);
	return ret;
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
	std::vector<int> kabukaList;
	std::vector<std::vector<int>> dateList;

	for(auto p : pathList) {
		cv::Mat src = cv::imread(p);
		std::filesystem::path fpa = p;
		std::vector<std::string> date = split(fpa.stem().string(), '_');
		std::vector<int> d;
		for(int i=0; i<3; i++) {
			if(date[i] == std::string("a")) {
				d.push_back(0);
			} else if(date[i] == std::string("p")) {
				d.push_back(1);
			} else {
				d.push_back(std::stoi(date[i]));
			}
		}
		if (src.cols != 1280) {
			std::cout << "‰ð‘œ“x‚ª³‚µ‚­‚ ‚è‚Ü‚¹‚ñ(1280x720 only)" << std::endl;
			getchar();
			exit(0);
		}
		int kabuka = kd.pickupKabuka(src);

		kabukaList.push_back(kabuka);
		dateList.push_back(d);
		std::cout << kabuka << std::endl;
	}

	Graph gp;

	gp.setData(&kabukaList, &dateList);

	cv::imshow("image", gp.draw(800, 600));
	cv::waitKey();

	return 0;
}