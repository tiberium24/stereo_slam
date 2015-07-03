/**
 * @file
 * @brief Graph class
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <image_geometry/pinhole_camera_model.h>

#include <g2o/core/sparse_optimizer.h>
#include <g2o/core/block_solver.h>
#include <g2o/types/slam3d/edge_se3.h>
#include <g2o/solvers/cholmod/linear_solver_cholmod.h>
#include <g2o/core/optimization_algorithm_levenberg.h>

#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

#include "frame.h"
#include "loop_closing.h"

using namespace std;
using namespace boost;
namespace fs  = boost::filesystem;

namespace slam
{

class LoopClosing;

class Graph
{

public:

	/** \brief Class constructor
   * \param Loop closing object pointer
   */
  Graph(LoopClosing* loop_closing);

  /** \brief Initialize the graph
   */
  void init();

  /** \brief Starts graph
   */
  void run();

  /** \brief Add a frame to the queue of frames to be inserted into the graph as vertices
   * \param The frame to be inserted
   */
  void addFrameToQueue(Frame frame);

  /** \brief Add an edge to the graph
   * \param Index of vertex 1
   * \param Index of vertex 2
   * \param Transformation between vertices
   * \param Number of inliers between these vertices
   */
  void addEdge(int i, int j, tf::Transform edge, int inliers);

  /** \brief Optimize the graph
   */
  void update();

  /** \brief Get the closest neighbors by distance
   * \param The vertex id to retrieve its neighbors
   * \param The vertex where discard window will be centered.
   * \param Window size of discarded vertices.
   * \param Number of neighbors to be retrieved.
   * \param Will contain the list of best neighbors by distance.
   */
  void findClosestVertices(int vertex_id, int window_center, int window, int best_n, vector<int> &neighbors);

  /** \brief Retrieve the list of the vertices of a corresponding frame
   * \param The frame id
   * \param Will contain the list of vertices for this frame.
   */
  void getFrameVertices(int frame_id, vector<int> &vertices);

  /** \brief Set the transformation between camera and robot odometry frame
   * \param the transform
   */
  inline void setCamera2Odom(const tf::Transform& camera2odom){camera2odom_ = camera2odom;}

  /** \brief Set camera matrix
   * \param camera matrix
   */
  inline void setCameraMatrix(const cv::Mat& camera_matrix){camera_matrix_ = camera_matrix;}

  /** \brief Get camera matrix
   */
  inline cv::Mat getCameraMatrix() const {return camera_matrix_;}

  /** \brief Set camera model
   * \param camera matrix
   */
  inline void setCameraModel(const image_geometry::PinholeCameraModel& camera_model){camera_model_ = camera_model;}

  /** \brief Get camera model
   */
  inline image_geometry::PinholeCameraModel getCameraModel() const {return camera_model_;}

protected:

  /** \brief Check if there are frames in the queue to be inserted into the graph
   * @return true if frames queue is not empty.
   */
  bool checkNewFrameInQueue();

  /** \brief Converts the frame to a graph vertex and adds it to the graph
   */
  void processNewFrame();

  /** \brief Add a vertex to the graph
   * @return the vertex id
   * \param Vertex pose
   */
  int addVertex(Eigen::Vector4f pose);

  /** \brief Save the graph to file
   */
  void saveToFile();

private:

  g2o::SparseOptimizer graph_optimizer_; //!> G2O graph optimizer

  list<Frame> frame_queue_; //!> Frames queue to be inserted into the graph

  int frames_counter_; //!> Processed frames counter

  vector< pair< int,int > > cluster_frame_; //!> Stores the cluster/frame relation (cluster_id, frame_id)

  mutex mutex_graph_; //!> Mutex for the graph manipulation

  mutex mutex_frame_queue_; //!> Mutex for the insertion of new frames into the graph

  tf::Transform camera2odom_; //!> Transformation between camera and robot odometry frame

  LoopClosing* loop_closing_; //!> Loop closing

  cv::Mat camera_matrix_; //!> The camera matrix

  image_geometry::PinholeCameraModel camera_model_; //!> Pinhole left camera model
};

} // namespace

#endif // GRAPH_H