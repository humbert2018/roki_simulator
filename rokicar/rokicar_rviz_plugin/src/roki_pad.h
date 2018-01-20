#ifndef ROKI_PAD_H
#define ROKI_PAD_H

# include <ros/ros.h>
# include <rviz/panel.h>
# include <rviz/viewport_mouse_event.h>
# include <geometry_msgs/PoseStamped.h>

class QLabel;
class QLineEdit;
class QPushButton;

namespace roki_plugin
{

class RokiPad: public rviz::Panel
{
// This class uses Qt slots and is a subclass of QObject, so it needs
// the Q_OBJECT macro.
Q_OBJECT
public:
  RokiPad( QWidget* parent = 0 );
  void OnPoseCallback(const geometry_msgs::PoseStamped::ConstPtr &pose);

public Q_SLOTS:
  void OnClick();

protected:
  // AI
  QLineEdit*  _ai_name_editor;
  // Simu
  QLineEdit*  _simu_name_editor;
  // Speed
  QLineEdit* _speed_editor;


  // push button
  QLabel*      _create_mode_label;
  QPushButton* _create_mode_button;
  bool         _create_open;

  // node
  ros::NodeHandle _node;
  ros::Subscriber _sub;
  ros::Publisher  _pub;
};

}

#endif
