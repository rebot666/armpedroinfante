#ifndef GESTURES_H
#define GESTURES_H

//Gestures
#define HANDS_UP 0
#define HANDS_DOWN 1
#define HORIZONTAL_HANDS 2
#define VERTICAL_HANDS 3
#define EQUAL_RIGHT_HANDS 4
#define EQUAL_LEFT_HANDS 5
#define CROSS_HANDS 6
#define DOWN_L_RIGHT_HANDS 7
#define DOWN_L_LEFT_HANDS 8

//Positions of hands in quadrants of Kinect space
//Left represents 0 to -1 quadrant
//Right represents 0 to 1 quadrant
#define HAND_UP 0
#define HAND_DOWN 1
#define HAND_LEFT 2
#define HAND_RIGHT 3
#define HAND_UNDEFINED 99

class Gestures
{
public:
	Gestures(void);
	~Gestures(void);
	int getGesture(void);
	void setGesture(int);
	int decodeGesture(float, float, float, float, float, float, float, float, float);

private:
	int aproximateVal(int val);
	int gesture;
	int handLeftX, handLeftY, handLeftZ;
	int handRightX, handRightY,handRightZ;
};

#endif // !GESTURES_H
