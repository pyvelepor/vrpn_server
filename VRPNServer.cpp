#include <iostream>
#include "vrpn_Tracker.h"
#include "VRPNServer.h"

	LeapTracker::LeapTracker(vrpn_Connection *c /*= 0 */) :
		vrpn_Tracker("Tracker0", c)
	{
	}

	void
		LeapTracker::mainloop()
	{
			server_mainloop();
			vrpn_gettimeofday(&_timestamp, NULL);

			vrpn_Tracker::timestamp = _timestamp;

			// We will just put a fake data in the position of our tracker
			//static float angle = 0; angle += 0.001f;

			// the pos array contains the position value of the tracker
			// XXX Set your values here
			pos[0] = LeapTracker::last_position[0];
			pos[1] = LeapTracker::last_position[1];
			pos[2] = LeapTracker::last_position[2];
			

			// the d_quat array contains the orientation value of the tracker, stored as a quaternion
			// XXX Set your values here
			d_quat[0] = LeapTracker::last_orientation[0];
			d_quat[1] = LeapTracker::last_orientation[1];
			d_quat[2] = LeapTracker::last_orientation[2];
			d_quat[3] = LeapTracker::last_orientation[3];


			char msgbuf[1000];

			d_sensor = 0;

			int  len = vrpn_Tracker::encode_to(msgbuf);
			
			//vrpn_Tracker::report_pose(0, _timestamp, pos, d_quat);


			if (d_connection->pack_message(len, _timestamp, position_m_id, d_sender_id, msgbuf,
				vrpn_CONNECTION_LOW_LATENCY))
			{
				fprintf(stderr, "can't write message: tossing\n");
			}

			else
			{
				//std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
			}

			
		}

	void LeapTracker::sendData(double position[3], q_type quaternion){
		last_position[0] = position[0];
		last_position[1] = position[1];
		last_position[2] = position[2];

		last_orientation[0] = quaternion[0];
		last_orientation[1] = quaternion[1];
		last_orientation[2] = quaternion[2];
		last_orientation[3] = quaternion[3];

		//mainloop();
	}