#ifndef COLORDETECTIONNODE_HPP
#define COLORDETECTIONNODE_HPP

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>

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
        int blur_size;
        int lines_col;
        int lines_row;
        float thresh_col;
        float thresh_row;

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



    //image processing
    //convert to RGB
    cv_bridge::CvImagePtr convertImage();

    //convert to rg_chromasity
    void rgb2rgChromasity(const cv::Mat& src, cv::Mat& output);


};

#endif // COLORDETECTIONNODE_HPP