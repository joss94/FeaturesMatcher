#include "ImagesMatcher.h"

/// <summary>
/// Shows help in console
/// </summary>
void showHelp()
{
	printf(
		"\n" \
		"To use this features matching tool : \n" \
		"FlyinstinctFeaturesMatcher.exe [path_to_ref_image] [path_to_obs_image] [line_start_x] [line_start_y] [line_end_x] [line_end_y]\n"
	);

	system("pause");
	exit(-1);
}

/// <summary>
/// Main entry
/// </summary>
/// <param name="argc"> Nb of args </param>
/// <param name="argv"> List of args : [path_to_ref_image] [path_to_obs_image] [line_start_x] [line_start_y] [line_end_x] [line_end_y]</param>
/// <returns></returns>
int main(int argc, char* argv[])
{
	// Check number of input args
	if (argc != 7)
	{
		showHelp();
	}

	// Start algo : create matcher object
	clock_t c = clock();
	ImagesMatcher matcher;

	// Read images and check files validity
	auto img1 = cv::imread(argv[1]);
	auto img2 = cv::imread(argv[2]);

	if (img1.rows == 0 || img2.rows == 0)
	{
		printf("Could not open images properly\n");
		showHelp();
	}

	// Compute transformation and transform line coordinates
	matcher.getHomography(img1, img2);

	std::vector<cv::Point2f> srcCoords;

	srcCoords.push_back(cv::Point2f(std::stoi(argv[3]), std::stoi(argv[4])));
	srcCoords.push_back(cv::Point2f(std::stoi(argv[5]), std::stoi(argv[6])));

	std::vector<cv::Point2f> dstCoords = matcher.transformPoints(srcCoords);

	// Plot lines in ref and obs images
	cv::Mat srcWithLine = img1.clone();
	cv::line(srcWithLine, srcCoords[0], srcCoords[1], cv::Scalar(0), 3);
	cv::Mat dstWithLine = img2.clone();
	cv::line(dstWithLine, dstCoords[0], dstCoords[1], cv::Scalar(0), 3);

	// Concat images for final display
	std::vector<cv::Mat> finalImages = { srcWithLine, dstWithLine };
	cv::Mat finalImage;
	cv::hconcat(finalImages, finalImage);

	cv::namedWindow("Result", cv::WINDOW_KEEPRATIO);
	cv::imshow("Result", finalImage);

	printf("Done ! \n");
	printf("Total time : %.2f sec\n", double((int)clock() - c) / CLOCKS_PER_SEC);
	cv::waitKey(0);
}