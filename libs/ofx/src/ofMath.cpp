#include "ofUtils.h"
#include "ofMath.h"
#include "float.h"

using namespace std;

//--------------------------------------------------
int ofNextPow2(int a){
	// from nehe.gamedev.net lesson 43
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

//--------------------------------------------------
void ofSeedRandom(int val) {
	srand((long) val);
}

//--------------------------------------------------
float ofRandom(float max) {
	return (max * rand() / float(RAND_MAX)) * (1.0f - std::numeric_limits<float>::epsilon());
}

//--------------------------------------------------
float ofRandom(float x, float y) {
	float high = ofMax(x, y);
	float low = ofMin(x, y);
	return ofMax(low, (low + ((high - low) * rand() / float(RAND_MAX))) * (1.0f - std::numeric_limits<float>::epsilon()));
}

//--------------------------------------------------
float ofRandomf() {
	return -1.0f + (2.0f * rand() / float(RAND_MAX)) * (1.0f - std::numeric_limits<float>::epsilon());
}

//--------------------------------------------------
float ofRandomuf() {
	return (rand() / float(RAND_MAX)) * (1.0f - std::numeric_limits<float>::epsilon());
}

//---- new to 006
//from the forums http://www.openframeworks.cc/forum/viewtopic.php?t=1413

//--------------------------------------------------
float ofNormalize(float value, float min, float max){
	return ofClamp( (value - min) / (max - min), 0, 1);
}

//check for division by zero???
//--------------------------------------------------
float ofMap(float value, float inputMin, float inputMax, float outputMin, float outputMax, bool clamp) {

	if (fabs(inputMin - inputMax) < FLT_EPSILON){
		return outputMin;
	} else {
		float outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
	
		if( clamp ){
			if(outputMax < outputMin){
				if( outVal < outputMax )outVal = outputMax;
				else if( outVal > outputMin )outVal = outputMin;
			}else{
				if( outVal > outputMax )outVal = outputMax;
				else if( outVal < outputMin )outVal = outputMin;
			}
		}
		return outVal;
	}

}

//--------------------------------------------------
float ofDist(float x1, float y1, float x2, float y2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//--------------------------------------------------
float ofDist(float x1, float y1, float z1, float x2, float y2, float z2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

//--------------------------------------------------
float ofDistSquared(float x1, float y1, float x2, float y2) {
	return ( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}

//--------------------------------------------------
float ofDistSquared(float x1, float y1, float z1, float x2, float y2, float z2) {
	return ( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2) );
}

//--------------------------------------------------
float ofClamp(float value, float min, float max) {
	return value < min ? min : value > max ? max : value;
}

//--------------------------------------------------
int ofSign(float n) {
	if( n > 0 ) return 1;
	else if(n < 0) return -1;
	else return 0;
}

//--------------------------------------------------
bool ofInRange(float t, float min, float max) {
	return t>=min && t<=max;
}

//--------------------------------------------------
float ofRadToDeg(float radians) {
	return radians * RAD_TO_DEG;
}

//--------------------------------------------------
float ofDegToRad(float degrees) {
    return degrees * DEG_TO_RAD;
}

//--------------------------------------------------
float ofLerp(float start, float stop, float amt) {
	return start + (stop-start) * amt;
}

float ofWrap(float value, float from, float to){
	// algorithm from http://stackoverflow.com/a/5852628/599884
	if(from > to){
		swap(from, to);
	}
	float cycle = to - from;
	if(ofIsFloatEqual(cycle, 0.0f)){
		return to;
	}
	return value - cycle * floor((value - from) / cycle);
}

//--------------------------------------------------
float ofWrapRadians(float angle, float from, float to){
	return ofWrap(angle, from, to);
}

float ofWrapDegrees(float angle, float from, float to){
	return ofWrap(angle, from, to);
}

//--------------------------------------------------
float ofLerpDegrees(float currentAngle, float targetAngle, float pct) {
    return currentAngle + ofAngleDifferenceDegrees(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float ofLerpRadians(float currentAngle, float targetAngle, float pct) {
	return currentAngle + ofAngleDifferenceRadians(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float ofAngleDifferenceDegrees(float currentAngle, float targetAngle) {
	return ofWrapDegrees(targetAngle - currentAngle);
}

//--------------------------------------------------
float ofAngleDifferenceRadians(float currentAngle, float targetAngle) {
	return  ofWrapRadians(targetAngle - currentAngle);
}


