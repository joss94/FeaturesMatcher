#pragma once

#include <string>
#include "opencv2/opencv.hpp"
#include "opencv2/calib3d.hpp"

class ImagesMatcher
{

public:

	/// <summary>
	/// Constructor
	/// </summary>
	ImagesMatcher();

	/// <summary>
	/// Computes the homography (transformation matrix) to go from an image to another
	/// </summary>
	/// <param name="imgRef">Reference image</param>
	/// <param name="imgObs">New observation image</param>
	/// <param name="showMatches">If set to true, an image showing the matching key points is displayed</param>
	/// <returns>Homogrpahy matrix</returns>
	cv::Mat getHomography(cv::Mat imgRef, cv::Mat imgObs, bool showMatches = false);

	/// <summary>
	/// Transform a series of points using the homogrpahy matrix computed
	/// </summary>
	/// <param name="points">Series of points with coordinates in the reference image</param>
	/// <returns>Corresponding series of points with coordinates in the new observation image</returns>
	std::vector<cv::Point2f> transformPoints(const std::vector<cv::Point2f>& points);

private:

	/// <summary>
	/// Displays an image showing the key points of the 2 images, and the matches between them
	/// </summary>
	/// <param name="img1">Reference image</param>
	/// <param name="keyPoints1">Reference image keypoints</param>
	/// <param name="img2">Observation image</param>
	/// <param name="keyPoints2">Observation image keypoints</param>
	/// <param name="matches">List of matches between the keypoints</param>
	/// <param name="nMatchesToDisplay">Number of matches to display</param>
	void displayMatches(cv::Mat img1, std::vector<cv::KeyPoint>& keyPoints1, cv::Mat img2, std::vector<cv::KeyPoint>& keyPoints2, std::vector<cv::DMatch> matches, int nMatchesToDisplay);

private:

	cv::Ptr<cv::SIFT> m_pDetector;
	cv::Ptr<cv::FlannBasedMatcher> m_pMatcher;
	
	cv::Mat m_H;
};