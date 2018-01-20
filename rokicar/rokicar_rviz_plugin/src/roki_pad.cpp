#include <stdio.h>

#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QDoubleValidator>

#include "roki_pad.h"
#include "rokicar/RokiCreatorMsg.h"

namespace roki_plugin
{

RokiPad::RokiPad( QWidget* parent )
  : rviz::Panel( parent )
{
  // layout
  QHBoxLayout* ai_layout = new QHBoxLayout;
  ai_layout->addWidget( new QLabel( "rokicar AI:" ));
  _ai_name_editor = new QLineEdit;
  ai_layout->addWidget( _ai_name_editor );

  QHBoxLayout* simu_layout = new QHBoxLayout;
  simu_layout->addWidget( new QLabel( "rokicar Simu:" ));
  _simu_name_editor = new QLineEdit;
  simu_layout->addWidget( _simu_name_editor );

  QHBoxLayout* speed_layout = new QHBoxLayout;
  speed_layout->addWidget( new QLabel( "rokicar speed:" ));
  _speed_editor = new QLineEdit;

  QDoubleValidator* validator = new QDoubleValidator;
  validator->setRange(-999999, 999999);
  _speed_editor->setValidator(validator);
  speed_layout->addWidget( _speed_editor );

  QHBoxLayout* create_layout = new QHBoxLayout;
  _create_mode_button = new QPushButton;
  _create_mode_label = new QLabel("关闭状态");
  create_layout->addWidget(_create_mode_label);
  create_layout->addWidget(_create_mode_button);

  // main layout
  QVBoxLayout* layout = new QVBoxLayout;
  layout->addLayout(ai_layout);
  layout->addLayout(simu_layout);
  layout->addLayout(speed_layout);
  layout->addLayout(create_layout);
  setLayout(layout);

  // init
  _ai_name_editor->setText("");
  _simu_name_editor->setText("simple_simu");
  _create_mode_button->setText("启动");
  _speed_editor->setText("10");
  _create_open = false;

  // op event
  connect(_create_mode_button, SIGNAL(clicked(bool)), this, SLOT(OnClick()));

  // topic
	_sub = _node.subscribe("/move_base_simple/goal", 1, &RokiPad::OnPoseCallback, this);
  _pub = _node.advertise<rokicar::RokiCreatorMsg>("/rokicar/create", 1);
}

void RokiPad::OnClick() {
  // swap
  if(!_create_open) {
    _create_mode_button->setText("关闭");
    _create_mode_label->setText("2D Nav Goal创建roki");
    _create_open = true;
  }
  else{
    _create_mode_button->setText("启动");
    _create_mode_label->setText("关闭状态");
    _create_open = false;
  }
}

void RokiPad::OnPoseCallback(const geometry_msgs::PoseStamped::ConstPtr &pose){
  if(!_create_open) return; // 启动中才创建
  rokicar::RokiCreatorMsg msg;
  msg.ai_name = _ai_name_editor->text().toStdString();
  msg.simu_name = _simu_name_editor->text().toStdString();
  msg.box.x = 2;
  msg.box.y = 2;
  msg.box.z = 2;
  msg.pos = pose->pose.position;
  msg.quat = pose->pose.orientation;
  msg.speed = _speed_editor->text().toDouble();
  _pub.publish(msg);
}

} 

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(roki_plugin::RokiPad, rviz::Panel)
