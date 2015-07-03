/**
 * @file
 * @brief The cluster class represents one cv::KeyPoints clustering of the camera frame.
 */

#ifndef CLUSTER_H
#define CLUSTER_H

#include <ros/ros.h>
#include <tf/transform_datatypes.h>

#include <opencv2/opencv.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;

namespace slam
{

class Cluster
{

public:

  /** \brief Class constructor
   */
  Cluster();

  /** \brief Class constructor
   */
  Cluster(int id, int frame_id, tf::Transform pose, vector<cv::KeyPoint> kp, cv::Mat ldb_desc, cv::Mat sift_desc, vector<cv::Point3f> points);

  /** \brief Computes and returns the 3D points in world coordinates
   * @return the 3D points in world coordinates
   */
  vector<cv::Point3f> getWorldPoints();

  /** \brief Get the cluster id
   */
  inline int getId() const {return id_;}

  /** \brief Get the frame id
   */
  inline int getFrameId() const {return frame_id_;}

  /** \brief Get cv::KeyPoints
   */
  inline vector<cv::KeyPoint> getKp() const {return kp_;}

  /** \brief Get ldb descriptors
   */
  inline cv::Mat getLdb() const {return ldb_desc_;}

  /** \brief Get sift descriptors
   */
  inline cv::Mat getSift() const {return sift_desc_;}

  /** \brief Get 3D camera points
   */
  inline vector<cv::Point3f> getPoints() const {return points_;}

  /** \brief Get camera pose
   */
  inline tf::Transform getPose() const {return pose_;}

private:


  int id_; //!> Cluster id

  int frame_id_; //!> Corresponding frame id

  tf::Transform pose_; //!> Camera world position

  vector<cv::KeyPoint> kp_; //!> cv::KeyPoints.

  cv::Mat ldb_desc_; //!> LDB descriptors
  cv::Mat sift_desc_; //!> Sift descriptors

  vector<cv::Point3f> points_; //!> Stereo 3D points in camera frame

};

} // namespace

#endif // CLUSTER_H