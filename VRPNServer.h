#include "vrpn_Tracker.h"
#include "quat.h"

class LeapTracker : public vrpn_Tracker {
public:
	LeapTracker(vrpn_Connection *c = 0);
	virtual ~LeapTracker() {};

	virtual void mainloop();
	void sendData(double position[3], q_type quaternion);

protected:
	struct timeval _timestamp;
	double last_position[3];
	q_type last_orientation;
};