#include <thread>
#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "ros/subscribe_options.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>

namespace gazebo
{
  class RosPlugin : public ModelPlugin
  {
    public: RosPlugin() {}

    private: physics::ModelPtr model;
    private: physics::LinkPtr link;
    private: event::ConnectionPtr updateConnection;

    private: ros::Publisher ros_pub;
    private: ros::Subscriber ros_sub;

    private: std::unique_ptr<ros::NodeHandle> rosnode;
    private: ros::CallbackQueue rosQueue;
    private: std::thread rosQueueThread;

    private: double pose_z;
    private: double PD_force;

    public: void SetForceToZ(const double &_force)
    {
      this->link->SetForce(gazebo::math::Vector3(0, 0, _force));
    }

    public: void OnRosMsg(const std_msgs::Float64ConstPtr &_msg)
    {
      this->PD_force = _msg->data;
      SetForceToZ(PD_force);
    }

    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf)
    {
      this->model = _model;
      this->link = _model->GetLinks()[0];

      if(!ros::isInitialized()){
        int argc = 0;
        char** argv = NULL;
        ros::init(argc,argv,"ROS_plugin");
          
      }
      this->rosnode.reset(new ros::NodeHandle("ROS_plugin"));
      
      ros::SubscribeOptions so =
      ros::SubscribeOptions::create<std_msgs::Float64>("/"+ this->model->GetName() + "/PD_force",1,
        boost::bind(&RosPlugin::OnRosMsg, this, _1),
          ros::VoidPtr(), &this->rosQueue);

      this->ros_sub = this->rosnode->subscribe(so);	
      this->ros_pub = this->rosnode->advertise<std_msgs::Float64>("/empty_can/pose_z", 1);
      
      this->rosQueueThread =
      std::thread(std::bind(&RosPlugin::QueueThread, this));

      this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      std::bind(&RosPlugin::OnUpdate, this));

    }

    private: void QueueThread()
    {
      static const double timeout = 0.01;
      while (this->rosnode->ok())
      {
        this->rosQueue.callAvailable(ros::WallDuration(timeout));
      }
    }

    public: void OnUpdate()
    {
      gazebo::math::Pose pose;
      pose = this->model->GetWorldPose();
      pose_z = pose.pos.z;

      std_msgs::Float64 msg;
      msg.data = pose_z;

      this->ros_pub.publish(msg);
    }
  };
  GZ_REGISTER_MODEL_PLUGIN(RosPlugin)
}
#endif
