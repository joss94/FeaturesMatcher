# FeaturesMatcher

Compile with CMake (Opencv required)

Release 1.0 is already compiled and available on repo directory

Profiling analysis:  
Heaviest function is the keypoints detection (opencv detecteAndCompute with the SIFT detector): responsible for > 90% ellapsed time  
  
Optimization suggestions:  
	- For a series of photos, keypoint detection could be done only once on the first ref image
	- For video processing : use the last image as the new ref image, re-using available keypoints
