#include "Gestures.h"

Gestures::Gestures(void){
	setGesture(99999);
}


Gestures::~Gestures(void){
	
}


int Gestures::getGesture(void){
	return gesture;

}

void Gestures::setGesture(int g){
	gesture = g;

}

int aproximateVal(int val){
	if(val > -0.5 && val <0.5){
		return 0;
	}else{
		return 1;
	}
}
int Gestures::decodeGesture(float rX, float rY, float rZ, float lX, float lY, float lZ, float sCX, float sCY, float sCZ){
	

	//Set hand to undefined
	handLeftX = handLeftY = handLeftZ = handRightX = handRightY = handRightZ = HAND_UNDEFINED;
	
	
	if(rX > sCX && rX > 0.4 ){
		handRightX = HAND_RIGHT;
	}
	if(lX < sCX && lX < -0.4){
		handLeftX = HAND_LEFT;
	}
	if(lX > sCX){
		handLeftX = HAND_RIGHT;
	}
	if(rX < sCX){
		handRightX = HAND_LEFT;
	}
	if(rY > 0.5){
		handRightY = HAND_UP;
	}
	if(lY > 0.5){
		handLeftY = HAND_UP;
	}
	if(rY <  -0.2){
		handRightY = HAND_DOWN;
	}
	if(lY < -0.2){
		handLeftY = HAND_DOWN;
	}



	if(handRightX == HAND_RIGHT && handLeftX == HAND_LEFT){
		return HORIZONTAL_HANDS;
	}else if(handRightX == HAND_RIGHT && handLeftX == HAND_RIGHT){
		return EQUAL_RIGHT_HANDS;
	}else if(handRightX == HAND_LEFT && handLeftX == HAND_LEFT){
		return EQUAL_LEFT_HANDS;
	}else if(handRightY == HAND_UP && handLeftY == HAND_UP){
		return HANDS_UP;
	}else if(handRightY == HAND_DOWN && handLeftY == HAND_DOWN){
		return HANDS_DOWN;
	}else if(handRightX == HAND_LEFT && handLeftX == HAND_RIGHT){
		return CROSS_HANDS;
	}else if(handRightX == HAND_RIGHT && handLeftY == HAND_DOWN){
		return DOWN_L_RIGHT_HANDS;
	}else if(handLeftX == HAND_LEFT && handRightY == HAND_DOWN){
		return DOWN_L_LEFT_HANDS;
	
	}else{
		return 99;
	}
}