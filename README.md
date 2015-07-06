# PCL People-Tracker Timing Test #

This repository contains a test suite to evaluate the execution time behaviour of PCL's people library. The primary intention of this test suite is to measure the impact of certain parameters on both the execution time and accuracy of results. These findings are determined to estimate behaviour in peak-load conditions, which is a common issue to real-time image processing.

## Test Suite Setup ##

To run the test suit, following files and folders must be present in your test directory. Files and folders written in italic must be labeled as given below.

- your\_point\_cloud\_stream.bag
- *ReferFiles*
- *TestConfig.txt*
- *TestExe*
- your\_SVM\_input\_for\_people\_detection.yaml

**your\_point\_cloud\_stream.bag**  
This file contains your image stream in ROS's *.bag* file format. It must provide point-cloud image data of *sensor_msgs/PointCloud2.msg* type.

***ReferFiles***  
This folder accommodates plain text files referred to as *reference files* that contain ground truth data of people in associated *bag* files. These files may be designated arbitrarily. Following format applies to these files:

    Frame_Count 3
    Frame_Number 1
    X_1 Z_1
    X_2 Z_2
    Frame_Number 2
    X_1 Z_1
    X_2 Z_2
    X_3 Z_3
    ...

The first line indicates the total number of frames. The following lines provide the gound coordinates with regard to the camera position. Each line after a *Frame_Number* counter represents the position of a person.

***TestConfig.txt***  
This file provides configurable settings. A sample is in the */res* folder of this repository. Following parameters must be set.

*topic :*  
This parameter defines the topic name that identifies the pointcloud stored within the input bag file.

*rgb\_intrinsics\_matrix :*  
This parameter defines the coefficients of the intrinsic camera matrix that indicate its optical, geometric and digital characteristics.

*svm\_filename :*  
This parameter defines the file name needed for the support-vector-machine classifier.

*groundCoeffs :*  
This parameter defines the coefficients of the plane equation needed to detect and remove the ground plane.

*maxDeviation :*  
This parameter defines a threshold in meters that limits the accepted deviation from a position according to the reference file. In case of exceedance, a detected object is not considered a match.

*minConfidence :*  
This parameter defines a threshold for the HOG confidence, i.e., clusters are only considered a match that exhibit a confidence value greater than minConfidence.

***TestExe***  
This file is the executable of this test suite.

**your\_SVM\_input\_for\_people\_detection.yaml**  
This file provides calibration data needed to recognize human shapes. The */res* folder of this repository contains a proven one that is also provided on [https://github.com/PointCloudLibrary/pcl/tree/master/people/data](https://github.com/PointCloudLibrary/pcl/tree/master/people/data).


## Preconditions ##

Please note that a test session can only be executed with root permissions.


## Test Execution Manual ##
### Synopsis ###

    ./Test_Exe <file_1> <file_2> [parameter list] <policy> <tag>

### Options ###
- *<file_1\>* name of the .bag file including file extension [mandatory]

- *<file_2\>* name of reference file including file extension [mandatory]

-  *-minh <minimum height\>* parameter setting the minimum height limit as float [mandatory]

- *-maxh <maximum height\>* parameter setting the maximum height limit as float [mandatory]

- *-minw <minimum width\>* parameter setting the minimum width as float [mandatory]

- *-maxw <maximum width\>* parameter setting the maximum width as float [mandatory]

- *-vs <voxel size\>* parameter setting the voxel size as float [mandatory]

- *-sf <sampling factor\>* parameter setting the down sampling factor as float or integer (note: a float value is converted to an integer one by truncating the fractional part) [mandatory]

- <policy\> sets the scheduling policy. You can choose from RR | FIFO | NO_RT [mandatory]

- <tag\> used to tag an output files [optional]

### Sample Call ###

    ./Test_Exe scene_1.bag ref_scene_1.txt -minc -1.5 -minh 1.3 -maxh 2.3 -minw 0.9 -maxw 1.4 -vs 0.06 -sf 1 RR s1_3

### Built-in Error Messages


- *InputFile not found.*  
No file found corresponding to *<file\_1>* parameter in the test directory.

- *No reference file found.*  
No file found corresponding to *<file\_2>* parameter in the ReferFiles folder of the test directory.

- *Test parameter not set.*  
Some of the parameter list items have not been set.

- *Incorrect inputs.*  
Test command does not match required pattern.

- *Settings in TestConfig.txt missing.*  
Some settings of TestConfig.txt have not been set.

- *No TestConfig.txt found in <path/to/root>.*  
There is no TestConfig.txt file in the test directory.

- *Wrong input for scheduling policy. Available options : FIFO | RR | NO_RT*  
Wrong parameter assigned to the policy parameter.

- *Failed to set policy.*  
Setting of scheduling policy using sched_setscheduler system call failed.

- *mlockall failed*  
Locking of the calling process's virtual address space into RAM using mlockall system call failed.

## Test Results ##
After every test run, two files are generated. These files can be found in the *Results* folder of your test directory. If such a folder does not exist a new one is created.

The first file is the *report file*. Its name consists of the *<file\_1\>* parameter, the file classifier *Report*, an optional tag and the file extension `txt`. Given a *road_scene.bag* input file that was tagged with `1`, the associated report file is *road\_scene\_Report\_1.txt*.  
The content of this file exhibits the following structure:

    Fri Jun 5 13:32:50 2015
    -- Frame : 0
    ---- Time : 0.261413
    ---- Cluster : 0
    ------ B_X : 2.18725
    ------ B_Y : 0.87723
    ------ B_Z : 5.28796
    ------ HOG_C : -1.10829
    ---- Cluster : 1
    ------ B_X : 0.404633
    ------ B_Y : 0.878143
    ------ B_Z : 5.5867
    ------ HOG_C : -0.944061
    ...

The first line contains the date and time of test execution. The subsequent lines contain the detection results separated by frame count. Each frame includes information about its execution time and clusters that are expected to represent a person. Each person cluster is identified by its bottom position relative to the camera position (according to the right-hand-rule whereas Z points into the scene and Y to the ground) and the confidence value HOG_C that is based on its histogram of oriented gradients (HOG).

The second file associated with a particular test run is the *result file*. The file's name is subject to the same pattern as the *report file*. Hence, referring to the example from above the name of the result file would be *road\_scene\_Result\_1.txt*.  
The content of this file exhibits the following structure:

    # Fri Jun 5 13:32:50 2015
    # --- Parameterlist ---
    # minc : -1.5
    # minh : 1.3
    # maxh : 2.3
    # minw : 0.4
    # maxw : 1.4
    # vs   : 0.06
    # sf   : 1
    # -- Characteristics --
    # meanTime : 1.07173
    # stdDevT  : 0.0336307
    # minTime  : 1.00204
    # maxTime  : 1.15381
    # ---- Detection ----
    # detRate : 0.944
    # maxFaSe : 1
    # meanHOG : -1
    # ---------------------
    #FRAME EXE_TIME FP FN MA MA_DEV
    0      1.0645    1  0  2   0.72
    1      1.1136    3  0  2   0.71
    2      1.077     2  0  2   0.67
    3      1.0595    0  0  2   0.73
    ...

The first line contains the date and time of test execution. The first section *Parameterlist* outlines all input parameters that characterise this test run. The second section *Characteristics* lists time-specific classifications, such as the mean execution time of all frames (*meanTme*), the standard deviation (*stdDevT*) of the execution time and its minimum (*minTime*) and maximum (*maxTime*) peak values.

The subsequent section *Detection* contains detection-specific results. The *detRate* value is a ratio of the number of frames where all reference positions (see *ReferFiles*) have been detected with a specified maximal deviation (see *maxDeviation*) to the number of all frames of a given stream. The *maxFaSeq* value is the maximum false sequence, i.e., the longest sequence of frames that did not match their associated references. This figure is of interest in conjunction with *detRate* since a result can be considered more valuable than another having a smaller *maxFaSeq*, even if *detRate* is equal. The *meanHOG* value is the mean HOG confidence of clusters associated with frames that contribute to the denominator of *detRate*. This value is a measure for the accuracy of the detection.

The last section contains six columns. The first one indicates the frame count and the second one its associated execution time. The *FP* column shows the false positive count that indicates the number of spurious object. That is, objects that have a HOG confidence greater than *minConfidence* but no ground truth reference. The *FN* column shows the false negative count that indicates the number of referenced objects being not recognized. The *MA* column shows the number of objects that match a reference of the respective frame. The last column *MA_DEV* shows a ratio of the mean deviation of matching objects in the respective frame and to the *maxDeviation* parameter.

