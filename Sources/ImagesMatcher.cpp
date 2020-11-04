#pragma once

#include "ImagesMatcher.h"
#include <algorithm>

ImagesMatcher::ImagesMatcher()
{
	// Create features detector
	m_pDetector = cv::SIFT::create();

	// Create keypoints matcher
	m_pMatcher = cv::FlannBasedMatcher::create();
}

cv::Mat ImagesMatcher::getHomography(cv::Mat img1, cv::Mat img2, bool showMatches)
{
	clock_t c = clock();



	// Detect keypoints in image 1
	c = clock();
	printf("Detecting keypoints on ref image... ");

	std::vector<cv::KeyPoint> keyPoints1;
	cv::Mat descriptors1;
	m_pDetector->detectAndCompute(img1, cv::Mat(), keyPoints1, descriptors1);

	printf("took %.2f sec\n", double(clock() - c) / CLOCKS_PER_SEC);



	// Detect keypoints in image 2
	c = clock();
	printf("Detecting keypoints on obs image... ");

	std::vector<cv::KeyPoint> keyPoints2;
	cv::Mat descriptors2;
	m_pDetector->detectAndCompute(img2, cv::Mat(), keyPoints2, descriptors2);

	printf("took %.2f sec\n", double(clock() - c) / CLOCKS_PER_SEC);



	// Match keypoints
	c = clock();
	printf("Matching keypoints... ");

	std::vector<cv::DMatch> matches;
	m_pMatcher->match(descriptors1, descriptors2, matches);

	printf("took %.2f sec\n", double(clock() - c) / CLOCKS_PER_SEC);



	// Display the 50 best matches if required
	if (showMatches)
	{
		displayMatches(img1, keyPoints1, img2, keyPoints2, matches, 50);
	}



	// Build vectors of corresponding points according to matches
	c = clock();
	printf("Computing homography... ");

	int nKeyPointsToUse = std::min(std::min(matches.size(), keyPoints1.size()), keyPoints2.size());
	std::vector<cv::Point2f> srcPoints;
	std::vector<cv::Point2f> dstPoints;

	for (int i = 0; i < nKeyPointsToUse; i++)
	{
		const auto& m = matches[i];
		srcPoints.push_back(keyPoints1[m.queryIdx].pt);
		dstPoints.push_back(keyPoints2[m.trainIdx].pt);
	}

	// Find the homography from the corresponding sets of points
	m_H = findHomography(srcPoints, dstPoints, cv::RANSAC, 10.0);

	printf("took %.2f sec\n", double(clock() - c) / CLOCKS_PER_SEC);



	// Print homography matrix
	printf("\nHomography matrix : \n");
	for (int r = 0; r < m_H.rows; r++)
	{
		printf("[");
		for (int c = 0; c < m_H.cols; c++)
		{
			printf("%.3f ", m_H.at<double>(r, c));
		}
		printf("]\n");
	}
	printf("\n");



	return m_H;
}

std::vector<cv::Point2f> ImagesMatcher::transformPoints(const std::vector<cv::Point2f>& points)
{
	std::vector<cv::Point2f> dstCoords;
	cv::perspectiveTransform(points, dstCoords, m_H);
	return dstCoords;
}

void ImagesMatcher::displayMatches(cv::Mat img1, std::vector<cv::KeyPoint>& keyPoints1, cv::Mat img2, std::vector<cv::KeyPoint>& keyPoints2, std::vector<cv::DMatch> matches, int nMatchesToDisplay)
{
	// Sort matches in order to get the best ones first
	std::sort(matches.begin(), matches.end(), [&](const cv::DMatch& a, const cv::DMatch& b) -> bool
	{
		return a.distance < b.distance;
	});

	// Draw matches on images
	cv::Mat result;
	cv::drawMatches(
		img1, keyPoints1, 
		img2, keyPoints2, 
		std::vector<cv::DMatch>(matches.begin(), matches.begin() + std::min(nMatchesToDisplay, (int)matches.size())), result);

	// Display results
	cv::namedWindow("Result", cv::WINDOW_NORMAL);
	cv::imshow("Result", result);
	cv::waitKey(0);
}