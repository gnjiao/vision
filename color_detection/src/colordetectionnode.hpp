#ifndef COLORDETECTIONNODE_HPP
#define COLORDETECTIONNODE_HPP

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

//own message
#include <color_detection/colors_detected.h>

//std
#include <vector>
#include <string>
#include <cmath>

//image conversion
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv/highgui.h>

//util
#include <visionutil/visionmodels.hpp>
#include <rosutil/rosutil.hpp>

class ColorDetectionNode {
public:
    ColorDetectionNode(int argc, char* argv[]);


private:
    struct color_alg_params {
        VisionModels::color_model_vardim<double> color_model;


        //object detection algorithm parameters
        int min_contour_size;
        float gauss_thresh;

        //these are helper variables that are derived from color model
        //call the corresponding member function to calculate them.
        std::vector<std::vector<double> > sigma_inv;
        double gauss_constant;


        /*IMPORTANT: initialize color_model before calling this!
        Calculates and sets gauss_constant and sigma_inv.*/
        void precalc_vars() {
            //http://en.wikipedia.org/wiki/Multivariate_normal_distribution
            double sigma_det = (color_model.sigma[0][0]*color_model.sigma[1][1]) -
                               (color_model.sigma[0][1]*color_model.sigma[1][0]);
            gauss_constant = 1.0d / std::sqrt(std::pow(2*M_PI,2)*sigma_det);

            sigma_inv = std::vector<std::vector<double> >(2);
            sigma_inv[0] = std::vector<double>(2);
            sigma_inv[1] = std::vector<double>(2);
            sigma_inv[0][0] = color_model.sigma[1][1] / sigma_det;
            sigma_inv[0][1] = -(color_model.sigma[0][1]) / sigma_det;
            sigma_inv[1][0] = -(color_model.sigma[1][0]) / sigma_det;
            sigma_inv[1][1] = color_model.sigma[0][0] / sigma_det;
        }


    };

    //main stuff, mostly ROS
    ros::Subscriber rgb_subscriber;
    ros::Publisher classifier_publisher;
    ros::Time t_rgb;
    sensor_msgs::Image::ConstPtr camera_img_raw;

    void rgbCallback(const sensor_msgs::Image::ConstPtr &msg);
    ros::NodeHandle nodeSetup(int argc, char* argv[]);
    void runNode(ros::NodeHandle handle);
    void update();

    //models
    std::vector<color_alg_params> models;
    void readModel(ros::NodeHandle n, std::string color_model_name, color_alg_params &cap);

    //general algorithm parameters
    int blur_size;
    void readParameters(ros::NodeHandle n);

    //image processing
    //convert to RGB
    cv_bridge::CvImagePtr convertImage();

    //convert to rg_chromasity
    void rgb2rgChromasity(const cv::Mat& src, cv::Mat& output);

    //calculate gaussian
    void multiGaussian(const cv::Mat& src, cv::Mat& output, const color_alg_params &model);

    //sorting contours
    //note: has to be static. Else we get 200 lines of compile errors. Gotta love c++ compilers...
    static bool contourSort(std::vector<cv::Point> c1, std::vector<cv::Point> c2);


};

#endif // COLORDETECTIONNODE_HPP
