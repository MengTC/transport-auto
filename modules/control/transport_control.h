#pragma once
#include <iostream>
#include <vector>
#include <cstdio>
#include <fstream>
#include <map>
#include <string>

#include "cyber/component/component.h"
#include "cyber/component/timer_component.h"
#include "cyber/cyber.h"

#include "modules/drivers/gps/GPSproto.h"
#include "modules/drivers/gps/proto/gps.pb.h"
#include "modules/control/proto/control_command.pb.h"
#include "modules/control/proto/control_setting_conf.pb.h"

using apollo::drivers::Gps;
using apollo::control::ControlCommand;
using apollo::cyber::Component;
using apollo::cyber::ComponentBase;
using apollo::cyber::Writer;
using apollo::cyber::Reader;

class transport_Control : public apollo::cyber::Component<Gps> {
 public:
  bool Init() override;
  bool Proc(const std::shared_ptr<Gps>& msg0) override;
  void Clear() override;

 private:
  std::shared_ptr<Writer<ControlCommand>> writer;
  ControlCommand controlcmd;
  double Stanley(double k, double v, int& ValidCheck);
  double CaculateSteer(const std::shared_ptr<Gps>& msg0);
  double CaculateAcc(const std::shared_ptr<Gps>& msg0);
  void ReadTraj();
  void ReadConfig();
  void UpdateTraj(const std::shared_ptr<Gps>& msg0);
  int FindLookahead(double totaldis);
  vector<double> trajinfo[6];
  vector<double> rel_loc[3];

  apollo::control::ControlSettingConf control_setting_conf_;
  fstream TrajFile;
  fstream traj_record_file;
  int TrajIndex;
  std::shared_ptr<apollo::cyber::Reader<Gps>> gps_reader_;
  int frame;
  struct ConfigInfo {
    double look_ahead_dis;
    double stanley_k;
    double stanley_prop;
    double desired_speed;
    int speed_mode;
    double speed_k;
  } configinfo;
};
CYBER_REGISTER_COMPONENT(transport_Control)
