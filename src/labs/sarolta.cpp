// SAJÁT
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "file-util.hh"
#include "image-util.hh"

#define MAX_PATH 260
#define PI 3.141592653589793238462643383279502884L
// SAJÁT

//#include "stdafx.h"
//#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <queue>
#include <stack>
#include <random>
#include <fstream>

using namespace std;
using namespace cv;

wchar_t* projectPath;

// SAJÁT
bool openFileDlg(char* fname)
{
  string abs_file_path = utcn::ip::FileUtil::getSingleFileAbsPath();
  if (abs_file_path.empty()) return false;
  strcpy(fname, abs_file_path.c_str());
  return true;
}

void resizeImg(const cv::Mat& src, cv::Mat &dst, int maxSize,
               bool interpolate) {
  utcn::ip::ImageUtil::resizeImg(src, dst, maxSize, interpolate);
}
// SAJÁT

void testOpenImage()
{
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src;
    src = imread(fname);
    cv::imshow("image", src);
    cv::waitKey();
  }
}

void testOpenImagesFld()
{
  /*char folderName[MAX_PATH];
  if (openFolderDlg(folderName) == 0)
    return;
  char fname[MAX_PATH];
  FileGetter fg(folderName, "bmp");
  while (fg.getNextAbsFile(fname))
  {
    Mat src;
    src = imread(fname);
    cv::imshow(fg.getFoundFileName(), src);
    if (cv::waitKey() == 27) //ESC pressed
      break;
  }*/
}

void testImageOpenAndSave()
{
//  _wchdir(projectPath);

  Mat src, dst;

  src = imread("Images/Lena_24bits.bmp", IMREAD_COLOR);	// Read the image

  if (!src.data)	// Check for invalid input
  {
    printf("Could not open or find the image\n");
    return;
  }

  // Get the image resolution
  Size src_size = Size(src.cols, src.rows);

  // Display window
  const char* WIN_SRC = "Src"; //window for the source image
  namedWindow(WIN_SRC, WINDOW_AUTOSIZE);
  moveWindow(WIN_SRC, 0, 0);

  const char* WIN_DST = "Dst"; //window for the destination (processed) image
  namedWindow(WIN_DST, WINDOW_AUTOSIZE);
  moveWindow(WIN_DST, src_size.width + 10, 0);

  cvtColor(src, dst, COLOR_BGR2GRAY); //converts the source image to a grayscale one

  imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

  cv::imshow(WIN_SRC, src);
  cv::imshow(WIN_DST, dst);

  cv::waitKey(0);
}

void testNegativeImage()
{
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    double t = (double)getTickCount(); // Get the current time [s]

    Mat src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    Mat dst = Mat(height, width, CV_8UC1);
    // Accessing individual pixels in an 8 bits/pixel image
    // Inefficient way -> slow
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        uchar val = src.at<uchar>(i, j);
        uchar neg = 255 - val;
        dst.at<uchar>(i, j) = neg;
      }
    }

    // Get the current time again and compute the time difference [s]
    t = ((double)getTickCount() - t) / getTickFrequency();
    // Print (in the console window) the processing time in [ms]
    printf("Time = %.3f [ms]\n", t * 1000);

    cv::imshow("input image", src);
    cv::imshow("negative image", dst);
    cv::waitKey();
  }
}

void testNegativeImageFast()
{
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    Mat dst = src.clone();

    // The fastest approach of accessing the pixels -> using pointers
    uchar* lpSrc = src.data;
    uchar* lpDst = dst.data;
    int w = (int)src.step; // no dword alignment is found !!!
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++) {
        uchar val = lpSrc[i * w + j];
        lpDst[i * w + j] = 255 - val;
      }
    cv::imshow("input image", src);
    cv::imshow("negative image", dst);
    cv::waitKey();
  }
}

void testColor2Gray()
{
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname);

    int height = src.rows;
    int width = src.cols;

    Mat dst = Mat(height, width, CV_8UC1);

    // Accessing individual pixels in a RGB 24 bits/pixel image
    // Inefficient way -> slow
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        Vec3b v3 = src.at<Vec3b>(i, j);
        uchar b = v3[0];
        uchar g = v3[1];
        uchar r = v3[2];
        dst.at<uchar>(i, j) = (r + g + b) / 3;
      }
    }

    cv::imshow("input image", src);
    cv::imshow("gray image", dst);
    cv::waitKey();
  }
}

void testBGR2HSV()
{
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname);
    int height = src.rows;
    int width = src.cols;

    // HSV components
    Mat H = Mat(height, width, CV_8UC1);
    Mat S = Mat(height, width, CV_8UC1);
    Mat V = Mat(height, width, CV_8UC1);

    // Defining pointers to each matrix (8 bits/pixels) of the individual components H, S, V
    uchar* lpH = H.data;
    uchar* lpS = S.data;
    uchar* lpV = V.data;

    Mat hsvImg;
    cvtColor(src, hsvImg, COLOR_BGR2HSV);

    // Defining the pointer to the HSV image matrix (24 bits/pixel)
    uchar* hsvDataPtr = hsvImg.data;

    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        int hi = i * width * 3 + j * 3;
        int gi = i * width + j;

        lpH[gi] = hsvDataPtr[hi] * 510 / 360;	// lpH = 0 .. 255
        lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
        lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
      }
    }

    cv::imshow("input image", src);
    cv::imshow("H", H);
    cv::imshow("S", S);
    cv::imshow("V", V);

    cv::waitKey();
  }
}

void testResize()
{
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src;
    src = imread(fname);
    Mat dst1, dst2;
    //without interpolation
    resizeImg(src, dst1, 320, false);
    //with interpolation
    resizeImg(src, dst2, 320, true);
    cv::imshow("input image", src);
    cv::imshow("resized image (without interpolation)", dst1);
    cv::imshow("resized image (with interpolation)", dst2);
    cv::waitKey();
  }
}

void testCanny()
{
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src, dst, gauss;
    src = imread(fname, IMREAD_GRAYSCALE);
    double k = 0.4;
    int pH = 50;
    int pL = (int)k * pH;
    GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
    Canny(gauss, dst, pL, pH, 3);
    cv::imshow("input image", src);
    cv::imshow("canny", dst);
    cv::waitKey();
  }
}

void testVideoSequence()
{
//  _wchdir(projectPath);

  VideoCapture cap("Videos/rubic.avi"); // off-line video from file
  //VideoCapture cap(0);	// live video from web cam
  if (!cap.isOpened()) {
    printf("Cannot open video capture device.\n");
    cv::waitKey(0);
    return;
  }

  Mat edges;
  Mat frame;
  char c;

  while (cap.read(frame))
  {
    Mat grayFrame;
    cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
    Canny(grayFrame, edges, 40, 100, 3);
    cv::imshow("source", frame);
    cv::imshow("gray", grayFrame);
    cv::imshow("edges", edges);
    c = cv::waitKey(100);  // waits 100ms and advances to the next frame
    if (c == 27) {
      // press ESC to exit
      printf("ESC pressed - capture finished\n");
      break;  //ESC pressed
    };
  }
}


void testSnap()
{
//  _wchdir(projectPath);

  VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
  if (!cap.isOpened()) // openenig the video device failed
  {
    printf("Cannot open video capture device.\n");
    return;
  }

  Mat frame;
  char numberStr[256];
  char fileName[256];

  // video resolution
  Size capS = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),
                   (int)cap.get(CAP_PROP_FRAME_HEIGHT));

  // Display window
  const char* WIN_SRC = "Src"; //window for the source frame
  namedWindow(WIN_SRC, WINDOW_AUTOSIZE);
  moveWindow(WIN_SRC, 0, 0);

  const char* WIN_DST = "Snapped"; //window for showing the snapped frame
  namedWindow(WIN_DST, WINDOW_AUTOSIZE);
  moveWindow(WIN_DST, capS.width + 10, 0);

  char c;
  int frameNum = -1;
  int frameCount = 0;

  for (;;)
  {
    cap >> frame; // get a new frame from camera
    if (frame.empty())
    {
      printf("End of the video file\n");
      break;
    }

    ++frameNum;

    cv::imshow(WIN_SRC, frame);

    c = cv::waitKey(10);  // waits a key press to advance to the next frame
    if (c == 27) {
      // press ESC to exit
      printf("ESC pressed - capture finished");
      break;  //ESC pressed
    }
    if (c == 115) { //'s' pressed - snap the image to a file
      frameCount++;
      fileName[0] = NULL;
      sprintf(numberStr, "%d", frameCount);
      strcat(fileName, "Images/A");
      strcat(fileName, numberStr);
      strcat(fileName, ".bmp");
      bool bSuccess = imwrite(fileName, frame);
      if (!bSuccess)
      {
        printf("Error writing the snapped image\n");
      }
      else
        cv::imshow(WIN_DST, frame);
    }
  }

}

/**
 * LAB 4
 */

// Vec3b pass by reference pass by value helyett
int calculateArea(const Mat& src, Vec3b color) {
  int area = 0;
  const int height = src.rows;
  const int width = src.cols;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++)
      if (color == src.at<Vec3b>(i, j))
        area++;
  }
  return area;
}

// Vec3b pass by reference pass by value helyett
Point calculateCenterOfMass(const Mat& src, Vec3b color) {
  int area = calculateArea(src, color);

  const int height = src.rows;
  const int width = src.cols;

  int row = 0;
  int column = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++)
      if (color == src.at<Vec3b>(i, j)) {
        row += i;
        column += j;
      }
  }
  Point center((row / area), (column / area));
  //cout << "Row: " << row << endl;
  //cout << "Column: " << column << endl;
  //cout << "Area: " << area << endl;
  //cout << "Center x: " << center.x << endl;
  //cout << "Center y: " << center.y << endl;
  return center;
}

// Vec3b pass by reference pass by value helyett
float calculateAxisOfElongation(const Mat& src, Vec3b color) {
  const int height = src.rows;
  const int width = src.cols;

  Point center = calculateCenterOfMass(src, color);
  int row = center.x;
  int column = center.y;
  int num = 0;
  int denom1 = 0;
  int denom2 = 0;
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      if (color == src.at<Vec3b>(i, j)) {
        num += (i - row) * (j - column);
        denom1 += (j - column) * (j - column);
        denom2 = (i - row) * (i - row);
      }
  num = 2 * num;
  float angle = 0;
  if (denom1 - denom2 != 0)
    angle = atan2(num, (denom1 - denom2)) / 2;
  return angle;
}

int calculatePerimeter(const Mat& src, Vec3b color) {
  const int height = src.rows;
  const int width = src.cols;
  int perimeter = 0;
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      if (color == src.at<Vec3b>(i, j) && ((color != src.at<Vec3b>(i - 1, j)) || (color != src.at<Vec3b>(i, j - 1)) || (color != src.at<Vec3b>(i + 1, j)) || (color != src.at<Vec3b>(i, j + 1)))) {
        perimeter++;
      }
  return perimeter;
}

float calculateThinnessRatio(const Mat& src, Vec3b color) {
  int area = calculateArea(src, color);
  float perimeter = calculatePerimeter(src, color);
  float thinness = 4 * PI * (area / (perimeter * perimeter));
  return thinness;
}

float calculateAspeindatio(const Mat& src, Vec3b color) {
  float R = 0;
  const int height = src.rows;
  const int width = src.cols;

  int c_max = 0;
  int c_min = width;
  int r_max = 0;
  int r_min = height;
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      if (color == src.at<Vec3b>(i, j)) {
        if (i < r_min) {
          r_min = i;
        }
        else {
          if (i > r_max)
            r_max = i;
        }
        if (j < c_min) {
          c_min = j;
        }
        else {
          if (j > c_max) {
            c_max = j;
          }
        }

      }
  //cout << "C_min: " << c_min << endl;
  //cout << "C_max: " << c_max << endl;
  //cout << "R_min: " << r_min << endl;
  //cout << "R_max: " << r_max << endl;
  if (r_max - r_min + 1 != 0)
    R = (c_max - c_min + 1.0) / (r_max - r_min + 1.0);
  return R;
}

void drawContourPoints(const Mat& src, Vec3b color) {
  Vec3b contour_color = { 0,255,255 };
  const int height = src.rows;
  const int width = src.cols;
  Mat contour = src.clone();
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
      if (color == src.at<Vec3b>(i, j) && ((color != src.at<Vec3b>(i - 1, j)) || (color != src.at<Vec3b>(i, j - 1)) || (color != src.at<Vec3b>(i + 1, j)) || (color != src.at<Vec3b>(i, j + 1)))) {
        contour.at<Vec3b>(i, j) = contour_color;
      }
  cv::imshow("contour object image", contour);
}

void drawProjections(const Mat& src, Vec3b color) {
  const int height = src.rows;
  const int width = src.cols;
  Mat vertical = Mat(height, width, CV_8UC3);
  Mat horizontal = Mat(height, width, CV_8UC3);

  int row_index = height;
  int column_index = 0;
  for (int i = 0; i < height; i++) {
    column_index = 0;
    for (int j = 0; j < width; j++) {
      if (color == src.at<Vec3b>(i, j)) {
        horizontal.at<Vec3b>(i, column_index++) = color;
      }
      horizontal.at<Vec3b>(i, j) = { 255,255,255 };
    }
  }

  for (int j = 0; j < width; j++) {
    row_index = height - 1;
    for (int i = height - 1; i >= 0; i--) {
      if (color == src.at<Vec3b>(i, j)) {
        vertical.at<Vec3b>(row_index--, j) = color;
      }
      vertical.at<Vec3b>(i, j) = { 255,255,255 };
    }
  }
  cv::imshow("vertical image projection", vertical);
  cv::imshow("horizontal image projection", horizontal);
}

void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
  //More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
  Mat* src = (Mat*)param;
  if (event == EVENT_LBUTTONDOWN)
  {
    printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
           x, y,
           (int)(*src).at<Vec3b>(y, x)[2],
           (int)(*src).at<Vec3b>(y, x)[1],
           (int)(*src).at<Vec3b>(y, x)[0]);
  }
}
void onMouse(int event, int x, int y, int flags, void* param) {
  Mat* src = (Mat*)param;
  if (event == EVENT_LBUTTONDOWN) {
    Vec3b color = (*src).at<Vec3b>(y, x);
    //calculate area
    int area = calculateArea(*src, color);
    cout << "Area: " << area << endl;
    //calculate center of mass

    Point center = calculateCenterOfMass(*src, color);
    int center_x = center.x;
    int center_y = center.y;
    cout << "Center of mass: " << center_x << ", " << center_y << endl;

    //calculate axis of elongation
    float angle = calculateAxisOfElongation(*src, color);
    float angleDegrees = (angle * 180.0f) / PI;
    cout << "Axis of elongation " << angle << " which in degrees is " << angleDegrees << "\n";

    //calculate the perimeter
    int perimeter = calculatePerimeter(*src, color);
    cout << "Perimeter: " << perimeter << endl;

    //calculate thinness ratio
    float thinness = calculateThinnessRatio(*src, color);
    cout << "Thinness ratio: " << thinness << endl;

    //calculate aspect ratio
    float aspeindatio = calculateAspeindatio(*src, color);
    cout << "Aspect ratio: " << aspeindatio << endl;

    //draw contour points
    drawContourPoints(*src, color);

    //draw center of mass
    //drawCenterOfMass(center_x, center_y, *src);
    Mat centerimg = (*src).clone();
    centerimg.at<Vec3b>(center_x, center_y) = (255, 255, 255); //center of mass be RED
    cv::imshow("center of mass", centerimg);

    //draw axis of elongation
    Mat axis = (*src).clone();
    line(axis, center, Point(center_x + 150 * cos(angle), center_y + 150 * sin(angle)), Vec3b(0, 0, 0));
    cv::imshow("axis of elongation", axis);

    //projections of the binary object
    drawProjections(*src, color);
    cv::waitKey();
  }
}



void testMouseClick()
{
  Mat src;
  // Read image from file
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    src = imread(fname);
    //Create a window
    namedWindow("My Window", 1);

    //set the callback function for any mouse event
    setMouseCallback("My Window", onMouse, &src);

    //show the image
    cv::imshow("My Window", src);

    // Wait until user press some key
    cv::waitKey(0);
  }
}

void addGrayLevel() {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    Mat dst = Mat(height, width, CV_8UC1);
    // Accessing individual pixels in an 8 bits/pixel image
    // Inefficient way -> slow
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        uchar val = src.at<uchar>(i, j);
        uchar neg = val - 50;
        dst.at<uchar>(i, j) = neg;
      }
    }

    cv::imshow("input image", src);
    cv::imshow("negative image", dst);
    cv::waitKey();
  }
}

void multiplyGrayLevel() {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    Mat dst = Mat(height, width, CV_8UC1);
    // Accessing individual pixels in an 8 bits/pixel image
    // Inefficient way -> slow
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        uchar val = src.at<uchar>(i, j);
        uchar neg = val * 8;
        dst.at<uchar>(i, j) = neg;
      }
    }
    cv::imshow("input image", src);
    cv::imshow("negative image", dst);
    imwrite("C:\\Users\\andre\\Dropbox\\My PC (LAPTOP-AUV3VKT8)\\Documents\\Uni\\VI.Sem\\Image Processing\\Lab - Attila\\Lab1\\Images\\mult.png", dst);
    cv::waitKey();
  }
}

void colorImage() {
  Mat pic = Mat(256, 256, CV_8UC3); //color image
  for (int i = 0; i < pic.rows; i++)
  {
    for (int j = 0; j < pic.cols; j++)
    {
      Vec3b pixel;
      if (i <= 127 && j <= 127) { //white
        pixel[0] = 255;//B
        pixel[1] = 255;//G
        pixel[2] = 255;//R
        pic.at<Vec3b>(i, j) = pixel;
      }
      else if (i <= 127) { //red
        pixel[0] = 0;//B
        pixel[1] = 0;//G
        pixel[2] = 255;//R
        pic.at<Vec3b>(i, j) = pixel;
      }
      else if (j <= 127) { //green
        pixel[0] = 0;//B
        pixel[1] = 255;//G
        pixel[2] = 0;//R
        pic.at<Vec3b>(i, j) = pixel;
      }
      else {	//yellow
        pixel[0] = 0;//B
        pixel[1] = 255;//G
        pixel[2] = 255;//R
        pic.at<Vec3b>(i, j) = pixel;
      }
    }
  }
  cv::imshow("generated image", pic);
  cv::waitKey();
}

void floatMatrix() {
  float vals[4] = { 1, 2, 3, 4 };
  Mat M(2, 2, CV_32FC1, vals); //4 parameter constructor
  Mat Inv = M.inv();
  for (int i = 0; i < Inv.rows; i++) {
    for (int j = 0; j < Inv.cols; j++)
      cout << Inv.at<float>(i, j) << " ";
    cout << endl;
  }
}

/**
 * LAB 2
 */

void separateRGBChannels() {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname);
    int height = src.rows;
    int width = src.cols;
    Mat Rimg = Mat(height, width, CV_8UC1); //greyscale
    Mat Gimg = Mat(height, width, CV_8UC1); //greyscale
    Mat Bimg = Mat(height, width, CV_8UC1); //greyscale
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        Vec3b val = src.at<Vec3b>(i, j);
        Rimg.at<uchar>(i, j) = val[2];
        Gimg.at<uchar>(i, j) = val[1];
        Bimg.at<uchar>(i, j) = val[0];
      }
    }
    cv::imshow("input image", src);
    cv::imshow("R channel image", Rimg);
    cv::imshow("B channel image", Bimg);
    cv::imshow("G channel image", Gimg);
    cv::waitKey();
  }

}

void RGB24toGray() {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname);
    int height = src.rows;
    int width = src.cols;

    Mat Gimg = Mat(height, width, CV_8UC1); //greyscale

    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        Vec3b val = src.at<Vec3b>(i, j);

        Gimg.at<uchar>(i, j) = (val[0] + val[1] + val[2]) / 3;
      }
    }
    cv::imshow("input image", src);
    cv::imshow("greyscale image", Gimg);
    cv::waitKey();
  }
}

void grayToBW() {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;

    Mat bwimg = Mat(height, width, CV_8UC1); //greyscale
    int threshold;
    cout << "Introduce the threshold:";
    cin >> threshold;
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        if (src.at<uchar>(i, j) < threshold)
          bwimg.at<uchar>(i, j) = 0;
        else
          bwimg.at<uchar>(i, j) = 255;
      }
    }
    cv::imshow("input image", src);
    cv::imshow("black and white image", bwimg);
    cv::waitKey();
  }
}

void RGBtoHSV() {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname);
    int height = src.rows;
    int width = src.cols;

    Mat Himg = Mat(height, width, CV_8UC1); //greyscale
    Mat Simg = Mat(height, width, CV_8UC1); //greyscale
    Mat Vimg = Mat(height, width, CV_8UC1); //greyscale

    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        Vec3b value = src.at<Vec3b>(i, j);
        float r = value[2] / 255.0;
        float g = value[1] / 255.0;
        float b = value[0] / 255.0;
        float M = max(max(r, g), b);
        float m = min(min(r, g), b);
        float C = M - m;
        // convert value
        float v_temp = M;
        // convert saturation
        float s_temp;
        if (v_temp) {
          s_temp = C / v_temp;
        }
        else {
          s_temp = 0;
        }
        // convert hue
        float h_temp;
        if (C) {
          if (M == r) h_temp = 60 * (g - b) / C;
          if (M == g) h_temp = 120 + 60 * (b - r) / C;
          if (M == b) h_temp = 240 + 60 * (r - g) / C;
        }
        else  // grayscale
          h_temp = 0;
        if (h_temp < 0)
          h_temp += 360;
        // normalize for display
        Himg.at<uchar>(i, j) = h_temp * 255 / 360;
        Simg.at<uchar>(i, j) = s_temp * 255;
        Vimg.at<uchar>(i, j) = v_temp * 255;
      }
    }
    cv::imshow("input image", src);
    cv::imshow("hue image", Himg);
    cv::imshow("saturation image", Simg);
    cv::imshow("value image", Vimg);
    cv::waitKey();
  }
}

bool isInside(Mat img, int i, int j) {
  if (img.cols > j && j >= 0 && img.rows > i && i >= 0)
    return true;
  return false;
}

/* Histogram display function - display a histogram using bars (simlilar to L3 / Image Processing)
Input:
name - destination (output) window name
hist - pointer to the vector containing the histogram values
hist_cols - no. of bins (elements) in the histogram = histogram image width
hist_height - height of the histogram image
Call example:
showHistogram ("MyHist", hist_dir, 255, 200);
*/
void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
  Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

  //computes histogram maximum
  int max_hist = 0;
  for (int i = 0; i < hist_cols; i++)
    if (hist[i] > max_hist)
      max_hist = hist[i];
  double scale = 1.0;
  scale = (double)hist_height / max_hist;
  int baseline = hist_height - 1;

  for (int x = 0; x < hist_cols; x++) {
    Point p1 = Point(x, baseline);
    Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
    line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins colored in magenta
  }

  cv::imshow(name, imgHist);
}

/**
 * LAB3
 */

void computeHistogramWithBins() {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    int bins;
    cout << "No of bins:";
    cin >> bins;
    int values[255] = { 0 };

    Mat src = imread(fname);

    int height = src.rows;
    int width = src.cols;
    //Calculate the histogram values
    for (int i = 0; i < height; i++)
      for (int j = 0; j < width; j++) {
        int noBin = src.at<uchar>(i, j) * bins / 255;
        values[noBin]++;
      }
    //Print the histogram
    showHistogram("Greyscale histogram", values, bins, 255);
    cv::waitKey();
  }
}

void histogram(Mat_<uchar> img, int* hist) {
  for (int i = 0; i < 256; i++) {
    hist[i] = 0;
  }

  for (int i = 0; i < img.rows; i++)
    for (int j = 0; j < img.cols; j++) {
      hist[img.at<uchar>(i, j)]++;
    }
}

void PDF(Mat_<uchar> img, int* hist, float* pdf) {
  int M = img.rows * img.cols;

  histogram(img, hist);
  //showHistogram("myhist", hist, 255, 200);

  for (int i = 0; i < 256; i++) {
    pdf[i] = 0;
  }

  for (int i = 0; i < 256; i++) {
    pdf[i] = (float)hist[i] / M;
  }
}
void histogramPrint() {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname);
    int hist[256] = { 0 };
    histogram(src, hist);
    //Print the histogram
    showHistogram("Greyscale histogram", hist, 255, 255);
    cv::waitKey();
  }
}
vector<int> maxima(Mat_<uchar> img, int WH, float TH) {
  int hist[256];
  float pdf[256];
  vector<int> maximarr;
  //insert 0 at the beginning of the maxima position list
  maximarr.push_back(0);
  //compute the PDF function
  PDF(img, hist, pdf);
  //compute the average v of the normalized histogram values
  for (int k = WH; k <= 255 - WH; k++) {
    float v = 0;

    for (int i = k - WH; i <= k + WH; i++) {
      v += pdf[i];
    }

    v /= 2 * WH + 1;
    //search for the histogram maximum
    if (pdf[k] > v + TH) {
      int i;
      for (i = k - WH; i <= k + WH; i++) {
        if (pdf[k] < pdf[i]) {
          break;
        }
      }
      //insert the maxima position in the maxima position list
      if (i == k + WH + 1)
        maximarr.push_back(k);
    }

  }
  //insert 255 at the end of the maxima position list
  maximarr.push_back(255);
  return maximarr;
}
int nearestMax(int pixel, std::vector<int> maximarr) {
  int nearest_max = 0, min_dist = 255, dist;

  for (int maxima : maximarr) {
    dist = abs(pixel - maxima);
    if (dist <= min_dist) {
      min_dist = dist; // we found another max which is closer to our pixel
      nearest_max = maxima; // the nearest max becomes current maxima
    }
  }
  return nearest_max;
}

void multilevelTresholding(int WH, int TH) {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    Mat dst = src.clone();

    std::vector<int> maximarr = maxima(src, WH, TH);
    //thresholding
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        //assign the pixel the value of the nearest historgam maxima from the list
        dst.at<uchar>(i, j) = nearestMax(src.at<uchar>(i, j), maximarr);
      }
    }
    cv::imshow("Original image", src);
    cv::imshow("Tresholded image", dst);
    cv::waitKey(0);
  }
}
void floydSteinbergAlg(int WH, int TH) {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    Mat dst = src.clone();

    std::vector<int> maximarr = maxima(src, WH, TH);

    for (int i = 0; i < dst.rows; i++) {
      for (int j = 0; j < dst.cols; j++) {
        int oldpixel = dst.at<uchar>(i, j);
        int newpixel = nearestMax(oldpixel, maximarr);
        dst.at<uchar>(i, j) = newpixel;
        //calculate the quantization error
        int error = oldpixel - newpixel;
        //spread the error to the neighbouring pixels
        if (isInside(src, i, j + 1)) {
          dst.at<uchar>(i, j + 1) = src.at<uchar>(i, j + 1) + 7 * error / 16;
        }
        else if (isInside(src, i + 1, j - 1)) {
          dst.at<uchar>(i + 1, j - 1) = src.at<uchar>(i + 1, j - 1) + 3 * error / 16;
        }
        else if (isInside(src, i + 1, j)) {
          dst.at<uchar>(i + 1, j) = src.at<uchar>(i + 1, j) + 5 * error / 16;
        }
        else if (isInside(src, i + 1, j + 1)) {
          dst.at<uchar>(i + 1, j + 1) = src.at<uchar>(i + 1, j + 1) + error / 16;
        }
      }
    }
    cv::imshow("Original image", src);
    cv::imshow("Dithered image", dst);
    cv::waitKey(0);
  }
}

/**
 * LAB 5
 */

void assignRandomColorsToLabels(Mat src, int label) {
  int height = src.rows;
  int width = src.cols;
  Mat dst(height, width, CV_8UC3, CV_RGB(255,255,255));

  default_random_engine gen;
  uniform_int_distribution<int> d(0, 255);

  Vec3b* colors = new Vec3b[label]();
  colors[0] = Vec3b(255, 255, 255);
  for (int i = 1; i <= label; i++) {
    colors[i] = Vec3b(rand() % 256, rand() % 256, rand() % 256);
  }

  for (int i = 0; i <= height - 1; i++)
    for (int j = 0; j <= width - 1; j++) {
      //if(src.at<short>(i,j) != 0)
      dst.at<Vec3b>(i, j) = colors[src.at<short>(i, j)];
    }
  cout << "No of objects: " << label;

  cv::imshow("Colored labels", dst);
  cv::waitKey(0);
}

void bfsLabeling(int neighbor) {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;
    int label = 0;
    Mat labels(height, width, CV_16SC1, Scalar(0));
    for (int i = 0; i <= height - 1; i++)
      for (int j = 0; j <= width - 1; j++)
        if (src(i, j) == 0 && labels.at<short>(i, j) == 0) {
          label++;
          queue<Point2i> Q;
          labels.at<short>(i, j) = label;
          Q.push({ i,j });
          while (!Q.empty()) {
            Point2i p = Q.front();
            Q.pop();
            int di[8] = { -1, 0, +1, 0, -1, -1, +1, +1 };
            int dj[8] = { 0, -1, 0, +1, -1, +1, -1, +1 };
            for (int k = 0; k < neighbor; k++) {
              if (isInside(src, p.x + di[k], p.y + dj[k]) && src.at<uchar>(p.x + di[k], p.y + dj[k]) == 0 && labels.at<short>(p.x + di[k], p.y + dj[k]) == 0) {
                labels.at<short>(p.x + di[k], p.y + dj[k]) = label;
                Q.push({ p.x + di[k], p.y + dj[k] });
              }
            }
          }
        }
    //assignRandomColorsToLabels(labels, label);
    cout << label;
  }
}

void twoPassLabeling(int neighbor) {
  char fname[MAX_PATH];
  while (openFileDlg(fname))
  {
    Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
    int height = src.rows;
    int width = src.cols;

    int label = 0;
    vector<vector<int>> edges;
    Mat labels(height, width, CV_16SC1, Scalar(0)); //height x width matrix with 0s
    for (int i = 0; i <= height - 1; i++)
      for (int j = 0; j <= width - 1; j++) {
        if (src(i, j) == 0 && labels.at<short>(i, j) == 0) {
          vector<int> L;
          int di[8] = { -1, 0, +1, 0, -1, -1, +1, +1 };
          int dj[8] = { 0, -1, 0, +1, -1, +1, -1, +1 };
          for (int k = 0; k < neighbor; k++) {
            if (isInside(src, i + di[k], j + dj[k]) && labels.at<short>(i + di[k], j + dj[k]) > 0) {
              L.push_back(labels.at<short>(i + di[k], j + dj[k]));
            }
          }
          //no previous neighbor has a label
          if (L.size() == 0) {
            label++;
            labels.at<short>(i, j) = label;
          }
          else {
            //take the smallest label from neighbors
            int x = L[0];
            for (int j = 1; j < L.size(); j++) {
              if (L[j] < x)
                x = L[j];
            }
            //assign this label to the current pixel
            labels.at<short>(i, j) = x;
            for (int y : L) {
              if (x != y) {
                //mark each neighboring label y as equivalent to x
                //add edges that indicates the equivalence relations
                edges.resize(label + 1);
                edges[x].push_back(y);
                edges[y].push_back(x);
              }
            }
          }
        }
      }
    //labelled image after the first pass
    //assignRandomColorsToLabels(labels, label);
    //second pass to relabel
    int newlabel = 0;
    int* newlabels = new int[label + 1] {0};

    for (int i = 1; i <= label; i++) {
      if (newlabels[i] == 0) {
        newlabel++;
        queue<int> Q;
        newlabels[i] = newlabel;
        Q.push(i);
        while (!Q.empty()) {
          int x = Q.front();
          Q.pop();
          //iterate through all edges connected to x
          for (int y : edges.at(x)) {
            //if they have not been labeled with the new label, then assign them a new label
            if (newlabels[y] == 0) {
              newlabels[y] = newlabel;
              Q.push(y);
            }
          }
        }
      }
    }
    //update all labels with the new label values assigned
    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        labels.at<short>(i, j) = newlabels[labels.at<short>(i, j)];
      }
    }
    //labelled image after the second pass
    //assignRandomColorsToLabels(labels, newlabel);
    cout << label << endl;
  }
}

int dfsLabeling(Mat image, int neighborhoodType) {
  int label = 0;
  int stepCount = 0;

  Mat labels(image.rows, image.cols, CV_16SC1, Scalar(0));

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      /*if (stepCount % 1000 == 0) { //print the resulted image after every 1000 steps
              assignRandomColorsToLabels(labels, label);
      }*/
      stepCount++;
      if (image.at<uchar>(i, j) == 0 && labels.at<short>(i,j) == 0) {
        label++;
        stack<Point2i> stack;
        labels.at<short>(i,j) = label;
        stack.push({i, j});
        while (!stack.empty()) {
          Point p = stack.top();
          stack.pop();
          int di[8] = { -1, 0, +1, 0, -1, -1, +1, +1 };
          int dj[8] = { 0, -1, 0, +1, -1, +1, -1, +1 };
          for (int k = 0; k < neighborhoodType; k++) {
            if (isInside(image, p.x + di[k], p.y + dj[k])) {
              if (image.at < uchar >(p.x + di[k], p.y + dj[k]) == 0 && labels.at<short>(p.x + di[k], p.y + dj[k]) == 0) {
                labels.at<short>(p.x + di[k], p.y + dj[k]) = label;
                stack.push({ p.x + di[k], p.y + dj[k] });
              }
            }
          }
        }
      }
    }
  }
  //assignRandomColorsToLabels(labels, label);
  return label;
}

void ex_1() {
  char fname[MAX_PATH];
  while (openFileDlg(fname)) {
    Mat src = imread(fname);
    int height = src.rows;
    int width = src.cols;
    Mat gimg = Mat(height, width, CV_8UC1);
    //cv::imshow("Source image", src);
    //cv::waitKey(0);

    //convert the image into greayscale, then black and white
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        Vec3b val = src.at<Vec3b>(i, j);

        gimg.at<uchar>(i, j) = (val[0] + val[1] + val[2]) / 3;
      }
    }

    Mat bwimg = Mat(height, width, CV_8UC1); //greyscale
    int threshold = 250;
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j++)
      {
        if (gimg.at<uchar>(i, j) < threshold)
          bwimg.at<uchar>(i, j) = 0;
        else
          bwimg.at<uchar>(i, j) = 255;
      }
    }

    int neighbor = 8;
    int label = 0;
    Mat labels(height, width, CV_16SC1, Scalar(0));
    for (int i = 0; i <= height - 1; i++)
      for (int j = 0; j <= width - 1; j++)
        if (bwimg.at<uchar>(i, j) == 0 && labels.at<short>(i, j) == 0) {
          label++;
          queue<Point2i> Q;
          labels.at<short>(i, j) = label;
          Q.push({ i,j });
          while (!Q.empty()) {
            Point2i p = Q.front();
            Q.pop();
            int di[8] = { -1, 0, +1, 0, -1, -1, +1, +1 };
            int dj[8] = { 0, -1, 0, +1, -1, +1, -1, +1 };
            for (int k = 0; k < neighbor; k++) {
              if (isInside(bwimg, p.x + di[k], p.y + dj[k]) && bwimg.at<uchar>(p.x + di[k], p.y + dj[k]) == 0 && labels.at<short>(p.x + di[k], p.y + dj[k]) == 0) {
                labels.at<short>(p.x + di[k], p.y + dj[k]) = label;
                Q.push({ p.x + di[k], p.y + dj[k] });
              }
            }
          }
        }

    cv::imshow("BW image", bwimg);
    cv::waitKey(0);

    //Newlabel shows all the obejcts but 1 color represents the background. So we do not count that
    cout << "The number of objects in the picture: " << label - 1 << endl;

    assignRandomColorsToLabels(labels, label);

    //3. List all the colors
    cout << "The different colors in the picture are: " << endl;
    //Vec3b* colors = new Vec3b(newlabel-1);
    /*int index = 0;
    for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                    for (int x = 0; x < newlabel - 1; x++) {
                            if (src.at<uchar>(i, j) != (255, 255, 255)) { //background color
                                    int already_printed = 0;
                                    Vec3b color = src.at<uchar>(i, j);
                                    if (color == colors[x]) {
                                            already_printed = 1;
                                    }
                                    if (!already_printed) {
                                            cout << color[0] << " " << color[1] << " " << color[2] << endl;
                                            colors[index] = color;
                                            index++;
                                            if (index == newlabel - 1) {
                                                    break;
                                            }
                                    }
                            }
                    }
            }
    }
    */
  }
}

/**
 * LAB 6
 */

void borderTracing4() {
  char fname[MAX_PATH];
  while (openFileDlg(fname)) {
    Mat src = imread(fname, IMREAD_GRAYSCALE);
    vector<int> chainCode;

    bool found = false;

    int height = src.rows;
    int width = src.cols;

    int di[] = { 0,-1,0,1 };
    int dj[] = { 1,0,-1,0 };

    Point2i border[10000];

    int ind = 0;
    Point2i point;

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (src.at<uchar>(i, j) == 0) {
          point = Point2i(j, i);
          border[ind] = point;
          ind++;
          found = true;
          break;
        }
      }

      if (found) {
        break;
      }
    }


    int dir = 0;
    while (!(border[0] == border[ind - 2] && border[1] == border[ind - 1]) || (ind <= 2)) {
      dir = (dir + 3) % 4;
      for (int k = dir; k < dir + 4; k++) {
        uchar neighbor = src.at<uchar>(point.y + di[k % 4], point.x + dj[k % 4]);
        if (neighbor == 0) {
          border[ind] = Point2i(point.x + dj[k % 4], point.y + di[k % 4]);
          ind++;
          point = Point2i(point.x + dj[k % 4], point.y + di[k % 4]);
          chainCode.push_back(dir);
          dir = k % 4;
          break;
        }
      }
    }


    Mat result;
    cv::cvtColor(src, result, COLOR_GRAY2BGR);
    for (int i = 0; i < ind; i++) {
      circle(result, border[i], 1, Scalar(0, 0, 255), -1);
    }
    cv::imshow("Source image", src);
    cv::imshow("Border tracing result", result);
    cv::waitKey(0);

    cout << endl << "Chain code:" << endl;
    for (int i = 0; i < chainCode.size(); i++) {
      cout << chainCode[i];
    }

    cout << endl << "Derivative chain code:" << endl;
    for (int i = 1; i < chainCode.size(); i++) {
      int deriv = (chainCode[i] - chainCode[i - 1] + 4) % 4;
      cout << deriv;
    }
  }
}

void borderTracing8() {
  char fname[MAX_PATH];
  while (openFileDlg(fname)) {
    Mat src = imread(fname, IMREAD_GRAYSCALE);
    vector<int> chainCode;

    bool found = false;

    int height = src.rows;
    int width = src.cols;

    int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
    int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

    Point2i border[10000];

    int ind = 0;
    Point2i point;

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        if (src.at<uchar>(i, j) == 0) {
          point = Point2i(j, i);
          border[ind] = point;
          ind++;
          found = true;
          break;
        }
      }

      if (found) {
        break;
      }
    }


    int dir = 7;
    while (!(border[0] == border[ind - 2] && border[1] == border[ind - 1]) || (ind <= 2)) {
      if (dir % 2 == 0) {
        dir = (dir + 7) % 8;
      }
      else {
        dir = (dir + 6) % 8;
      }

      for (int k = dir; k < dir + 8; k++) {
        uchar neighbor = src.at<uchar>(point.y + di[k % 8], point.x + dj[k % 8]);
        if (neighbor == 0) {
          border[ind] = Point2i(point.x + dj[k % 8], point.y + di[k % 8]);
          ind++;
          point = Point2i(point.x + dj[k % 8], point.y + di[k % 8]);
          chainCode.push_back(dir);
          dir = k % 8;
          break;
        }
      }
    }


    Mat result;
    cv::cvtColor(src, result, COLOR_GRAY2BGR);
    for (int i = 0; i < ind; i++) {
      circle(result, border[i], 1, Scalar(0, 0, 255), -1);
    }
    cv::imshow("Source image", src);
    cv::imshow("Border tracing result", result);
    cv::waitKey(0);

    cout << endl << "Chain code:" << endl;
    for (int i = 0; i < chainCode.size(); i++) {
      cout << chainCode[i];
    }

    cout << endl << "Derivative chain code:" << endl;
    for (int i = 1; i < chainCode.size(); i++) {
      int deriv = (chainCode[i] - chainCode[i - 1] + 8) % 8;
      cout << deriv;
    }
  }
}

void reconstruct() {
  // Mat src = imread("Images\\border_tracing\\gray_background.bmp");
  Mat src = imread("/home/atis/Projects/CPP/opencv-proba/assets/Images/border_trace/gray_background.bmp");  
  // ifstream file("Images\\reconstruct.txt");
  ifstream file("/home/atis/Projects/CPP/opencv-proba/assets/Text/reconstruct.txt");
  Point2i p;
  file >> p.y >> p.x;
  int no_chain_codes = -1, dir;
  file >> no_chain_codes;
  //cout << "Starting point:" << p.x << " " << p.y << endl;
  //cout << "No of chain codes:" << no_chain_codes << endl;

  circle(src, p, 1, Scalar(0, 0, 255), -1);
  for (int i = 0; i < no_chain_codes; i++) {
    file >> dir;
    switch (dir) {
      case 0:
        p.x += 1;
        break;
      case 1:
        p.y -= 1;
        p.x += 1;
        break;
      case 2:
        p.y -= 1;
        break;
      case 3:
        p.y -= 1;
        p.x -= 1;
        break;
      case 4:
        p.x -= 1;
        break;
      case 5:
        p.y += 1;
        p.x -= 1;
        break;
      case 6:
        p.y += 1;
        break;
      case 7:
        p.y += 1;
        p.x += 1;
        break;
      default:
        break;
    }
    circle(src, p, 1, Scalar(0, 0, 255), -1);
  }

  file.close();
  cv::imshow("Reconstructed image", src);
  cv::waitKey(0);
}

int main()
{
  cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_FATAL);
//  projectPath = _wgetcwd(0, 0);

  int op;
  do
  {
    system("cls");
    destroyAllWindows();
    printf("Menu:\n");
    printf(" 1 - Open image\n");
    printf(" 2 - Open BMP images from folder\n");
    printf(" 3 - Image negative\n");
    printf(" 4 - Image negative (fast)\n");
    printf(" 5 - BGR->Gray\n");
    printf(" 6 - BGR->Gray (fast, save result to disk) \n");
    printf(" 7 - BGR->HSV\n");
    printf(" 8 - Resize image\n");
    printf(" 9 - Canny edge detection\n");
    printf(" 10 - Edges in a video sequence\n");
    printf(" 11 - Snap frame from live video\n");
    printf(" 12 - Mouse callback demo\n");
    printf(" 13 - Increase grey level by a constant\n");
    printf(" 14 - Multiply grey level by a constant\n");
    printf(" 15 - Color image generation\n");
    printf(" 16 - Print a matrix and its inverse\n");
    printf(" 17 - Separate the R,G,B channels of a colored image\n");
    printf(" 18 - Convert RGB to Greyscale manually\n");
    printf(" 19 - Greyscale image to black and white\n");
    printf(" 20 - Colored image converted into H,S,V channels\n");
    printf(" 21 - Histogram for greyscale image\n");
    printf(" 22 - Histogram with variable no of bins\n");
    printf(" 23 - Multilevel thresholding\n");
    printf(" 24 - Floyd Steinberg\n");
    printf(" 25 - Bfs labeling\n");
    printf(" 26 - Two pass labeling\n");
    printf(" 27 - Dfs labeling\n");
    printf(" 28 - Test\n");
    printf(" 29 - Border tracing with 4 neighbors\n");
    printf(" 30 - Border tracing with 8 neighbors\n");
    printf(" 31 - Reconstruct border\n");
    printf(" 0 - Exit\n\n");
    printf("Option: ");
    scanf("%d", &op);
    switch (op)
    {
      case 1:
        testOpenImage();
        break;
      case 2:
        testOpenImagesFld();
        break;
      case 3:
        testNegativeImage();
        break;
      case 4:
        testNegativeImageFast();
        break;
      case 5:
        testColor2Gray();
        break;
      case 6:
        testImageOpenAndSave();
        break;
      case 7:
        testBGR2HSV();
        break;
      case 8:
        testResize();
        break;
      case 9:
        testCanny();
        break;
      case 10:
        testVideoSequence();
        break;
      case 11:
        testSnap();
        break;
      case 12:
        testMouseClick();
        break;
      case 13:
        addGrayLevel();
        break;
      case 14:
        multiplyGrayLevel();
        break;
      case 15:
        colorImage();
        break;
      case 16:
        floatMatrix();
        break;
      case 17:
        separateRGBChannels();
        break;
      case 18:
        RGB24toGray();
        break;
      case 19:
        grayToBW();
        break;
      case 20:
        RGBtoHSV();
        break;
      case 21:
        histogramPrint();
        break;
      case 22:
        computeHistogramWithBins();
        break;
      case 23:
        multilevelTresholding(5, 0.0003);
        break;
      case 24:
        floydSteinbergAlg(5, 0.0003);
        break;
      case 25:
        bfsLabeling(8);
        break;
      case 26:
        twoPassLabeling(8);
        break;
      case 27:
        //dfsLabeling(8);
        break;
      case 28:
        ex_1();
        break;
      case 29:
        borderTracing4();
        break;
      case 30:
        borderTracing8();
        break;
      case 31:
        reconstruct();
        break;
    }
  } while (op != 0);
  return 0;
}