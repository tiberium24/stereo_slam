#include "cluster.h"
#include "tools.h"

using namespace tools;

namespace slam
{
  Cluster::Cluster() : id_(-1){}

  Cluster::Cluster(int id, int frame_id, tf::Transform pose, vector<cv::KeyPoint> kp, cv::Mat ldb_desc, cv::Mat sift_desc, vector<cv::Point3f> points) :
                  id_(id), frame_id_(frame_id), pose_(pose), kp_(kp), ldb_desc_(ldb_desc), sift_desc_(sift_desc), points_(points){}

  vector<cv::Point3f> Cluster::getWorldPoints()
  {
    vector<cv::Point3f> out;
    for (uint i=0; i<points_.size(); i++)
    {
      cv::Point3f p = Tools::transformPoint(points_[i], pose_);
      out.push_back(p);
    }

    return out;
  }
}