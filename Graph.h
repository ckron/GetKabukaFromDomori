#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

class Graph
{
public:
	void setData(std::vector<int>* v, std::vector<char[3]>* d)
	{
		values = v;
		dates = d;
	}

	cv::Mat draw(int width, int height)
	{
		cv::Mat res(cv::Size(width, height), CV_8UC3, cv::Scalar::all(BACKGROUND_COLOR));
		
		if(res.empty()) { 
			std::cout << "•`‰æÝ’è‚ªŠÔˆá‚Á‚Ä‚¢‚Ü‚·(width / height)" << std::endl;
			return cv::Mat();
		}

		drawAxis(res);
		drawPoints(res);

		return res;
	}
private:
	std::vector<int>* values;
	std::vector<char[3]>* dates;

	// 10px margin.
	static const int GRAPH_MARGIN = 20;

	static const int BACKGROUND_COLOR = 230;
	static const int USE_BLACK_COLOR = 10;
	static const int DRAWAREA_COLOR = 250;

	void drawPoints(cv::Mat dst)
	{
		cv::Point xAxisStart(GRAPH_MARGIN, dst.rows - GRAPH_MARGIN);
		cv::Point xAxisEnd(dst.cols - GRAPH_MARGIN, xAxisStart.y);
		cv::Point yAxisStart(GRAPH_MARGIN, GRAPH_MARGIN);
		cv::Point yAxisEnd(yAxisStart.x, dst.rows - GRAPH_MARGIN);
		
		for(int i=0; i<values->size(); i++) {
			int x = xAxisStart.x + 100 + 10 * i;
			int y = yAxisEnd.y - (*values)[i];

			cv::circle(dst, cv::Point(x, y), 10, cv::Scalar(0, 0, 255));
		}

		for (int i=0; i<values->size()-1; i++) {
			int x = xAxisStart.x + 100 + 10 * i;
			int y = yAxisEnd.y - (*values)[i];
			int nx = xAxisStart.x + 100 + 10 * (i+1);
			int ny = yAxisEnd.y - (*values)[i+1];

			cv::line(dst, cv::Point(x, y), cv::Point(nx, ny),
								cv::Scalar(0, 0, 255));
		}
	}

	void drawAxis(cv::Mat dst)
	{
		cv::Point xAxisStart(GRAPH_MARGIN, dst.rows - GRAPH_MARGIN);
		cv::Point xAxisEnd(dst.cols - GRAPH_MARGIN, xAxisStart.y); 
		cv::Point yAxisStart(GRAPH_MARGIN, GRAPH_MARGIN);
		cv::Point yAxisEnd(yAxisStart.x, dst.rows - GRAPH_MARGIN);

		// draw X Axis.
		line(dst, xAxisStart, xAxisEnd, cv::Scalar::all(USE_BLACK_COLOR));
		// draw Y Axis.
		line(dst, yAxisStart, yAxisEnd, cv::Scalar::all(USE_BLACK_COLOR));
	}
};