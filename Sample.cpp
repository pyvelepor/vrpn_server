/******************************************************************************\
* Copyright (C) 2012-2014 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include <Windows.h>
#include <iostream>
#include "Leap.h"
#include "vrpn_Connection.h"
#include "VRPNServer.h"

#include "quat.h"

using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);
	SampleListener();

private:
	LeapTracker * server;
	vrpn_Connection* m_Connection;
};

SampleListener::SampleListener(){
	
	m_Connection = vrpn_create_server_connection();

	server = new LeapTracker(m_Connection);
}

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
  controller.setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();

  if (frame.hands().count() == 1){

	  const Hand hand = frame.hands().frontmost();
	  Vector position = hand.stabilizedPalmPosition();

	  float roll = hand.palmNormal().roll();
	  float yaw = hand.direction().yaw();
	  float pitch = hand.direction().pitch();

	  q_type quaternion;
	  //Quaternion calculations
	  //http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToQuaternion/index.htm

	  /*float c1, c2, c3;
	  float s1, s2, s3;
	  float quaternion[4];

	  c1 = cos(yaw / 2);
	  c2 = cos(pitch / 2);
	  c3 = cos(roll / 2);

	  s1 = sin(yaw / 2);
	  s2 = sin(pitch / 2);
	  s3 = sin(roll / 2);

	  quaternion[0] = s1 * s2 * c3 + c1 * c2 * s3;
	  quaternion[1] = s1 * c2 * c3 + c1 * s2 * s3;
	  quaternion[2] = c1 * s2 * c3 - s1 * c2 * s3;
	  quaternion[3] = c1 * c2 * c3 - s1 * s2 * s3;*/

	  q_from_euler(quaternion, yaw, pitch, roll);

	  double pos[] = { position.x, position.y, position.z };

	  //std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;

	  server->sendData(pos, quaternion);
	  server->mainloop();
	  m_Connection->mainloop();

	  //Sleep(0);
	  
  }
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main() {
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  // Keep this process running until Enter is pressed
  //std::cout << "Press Enter to quit..." << std::endl;
  //std::cin.get();

  while (true);

  // Remove the sample listener when done
  //controller.removeListener(listener);

  //return 0;
}
