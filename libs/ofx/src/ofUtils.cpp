#include "ofUtils.h"
#include "ofAppBaseWindow.h"
#include "ofMainLoop.h"
#include "ofAppRunner.h"
#include "ofEvents.h"
#include "ofMath.h"

#include <chrono>
#include <numeric>
#include <locale>
#include <cstdarg>

using namespace std;

//--------------------------------------
uint64_t ofTime::getAsMilliseconds() const{
	auto seconds = std::chrono::seconds(this->seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds);
	return (std::chrono::duration_cast<std::chrono::milliseconds>(seconds) +
			std::chrono::duration_cast<std::chrono::milliseconds>(nanoseconds)).count();
}

//--------------------------------------
uint64_t ofTime::getAsMicroseconds() const{
	auto seconds = std::chrono::seconds(this->seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds);
	return (std::chrono::duration_cast<std::chrono::microseconds>(seconds) +
			std::chrono::duration_cast<std::chrono::microseconds>(nanoseconds)).count();
}

//--------------------------------------
uint64_t ofTime::getAsNanoseconds() const{
	auto seconds = std::chrono::seconds(this->seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds);
	return (std::chrono::duration_cast<std::chrono::nanoseconds>(seconds) + nanoseconds).count();
}

//--------------------------------------
double ofTime::getAsSeconds() const{
	return seconds + nanoseconds / 1000000000.;
}

//--------------------------------------
std::chrono::time_point<std::chrono::nanoseconds> ofTime::getAsTimePoint() const{
	auto seconds = std::chrono::seconds(this->seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds);
	return std::chrono::time_point<std::chrono::nanoseconds>(
				std::chrono::duration_cast<std::chrono::nanoseconds>(seconds) + nanoseconds);
}

//--------------------------------------
std::chrono::nanoseconds ofTime::operator-(const ofTime& other) const{
	auto seconds = std::chrono::seconds(this->seconds) - std::chrono::seconds(other.seconds);
	auto nanoseconds = std::chrono::nanoseconds(this->nanoseconds) - std::chrono::nanoseconds(other.nanoseconds);
	return std::chrono::duration_cast<std::chrono::nanoseconds>(seconds) + nanoseconds;
}

//--------------------------------------
bool ofTime::operator<(const ofTime & other) const{
	return seconds < other.seconds || (seconds == other.seconds && nanoseconds < other.nanoseconds);
}

//--------------------------------------
bool ofTime::operator>(const ofTime & other) const{
	return seconds > other.seconds || (seconds == other.seconds && nanoseconds > other.nanoseconds);
}

//--------------------------------------
bool ofTime::operator<=(const ofTime & other) const{
	return seconds <= other.seconds || (seconds == other.seconds && nanoseconds <= other.nanoseconds);
}

//--------------------------------------
bool ofTime::operator>=(const ofTime & other) const{
	return seconds >= other.seconds || (seconds == other.seconds && nanoseconds >= other.nanoseconds);
}
//--------------------------------------------------
#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

uint64_t ofPrecisionTime() {
#ifdef _WIN32
    LARGE_INTEGER ticks;
    LARGE_INTEGER time;

    QueryPerformanceFrequency(&ticks);
    QueryPerformanceCounter(&time);

    static LONGLONG old_time = time.QuadPart;

    return (uint64_t)(((time.QuadPart - old_time) * 1000000) / ticks.QuadPart);
#else
    struct timeval    tp;
    gettimeofday(&tp, 0);

    static uint32_t old_tv_sec_2 = tp.tv_sec;
    return ((tp.tv_sec - old_tv_sec_2) * 1000000 + tp.tv_usec);
#endif
}
//--------------------------------------------------
int ofGetSeconds(){
	time_t 	curr;
	tm 		local;
	time(&curr);
	local	=*(localtime(&curr));
	return local.tm_sec;
}

//--------------------------------------------------
int ofGetMinutes(){
	time_t 	curr;
	tm 		local;
	time(&curr);
	local	=*(localtime(&curr));
	return local.tm_min;
}

//--------------------------------------------------
int ofGetHours(){
	time_t 	curr;
	tm 		local;
	time(&curr);
	local	=*(localtime(&curr));
	return local.tm_hour;
}

//--------------------------------------------------
int ofGetYear(){
	time_t    curr;
	tm       local;
	time(&curr);
	local   =*(localtime(&curr));
	int year = local.tm_year + 1900;
	return year;
}

//--------------------------------------------------
int ofGetMonth(){
	time_t    curr;
	tm       local;
	time(&curr);
	local   =*(localtime(&curr));
	int month = local.tm_mon + 1;
	return month;
}

//--------------------------------------------------
int ofGetDay(){
	time_t    curr;
	tm       local;
	time(&curr);
	local   =*(localtime(&curr));
	return local.tm_mday;
}

//--------------------------------------------------
int ofGetWeekday(){
	time_t    curr;
	tm       local;
	time(&curr);
	local   =*(localtime(&curr));
	return local.tm_wday;
}

//----------------------------------------
template<>
string ofFromString(const string& value){
	return value;
}

//----------------------------------------
template<>
const char * ofFromString(const string& value){
	return value.c_str();
}

//----------------------------------------
template <>
string ofToHex(const string& value) {
	ostringstream out;
	// how many bytes are in the string
	std::size_t numBytes = value.size();
	for(std::size_t i = 0; i < numBytes; i++) {
		// print each byte as a 2-character wide hex value
		out << setfill('0') << std::setw(2) << std::hex << (unsigned int) ((unsigned char)value[i]);
	}
	return out.str();
}

//----------------------------------------
string ofToHex(const char* value) {
	// this function is necessary if you want to print a string
	// using a syntax like ofToHex("test")
	return ofToHex((string) value);
}

//----------------------------------------
int ofToInt(const string& intString) {
	return ofTo<int>(intString);
}

//----------------------------------------
int ofHexToInt(const string& intHexString) {
	int x = 0;
	istringstream cur(intHexString);
	cur >> std::hex >> x;
	return x;
}

//----------------------------------------
char ofHexToChar(const string& charHexString) {
	int x = 0;
	istringstream cur(charHexString);
	cur >> std::hex >> x;
	return (char) x;
}

//----------------------------------------
float ofHexToFloat(const string& floatHexString) {
	union intFloatUnion {
		uint32_t i;
		float f;
	} myUnion;
	myUnion.i = 0;
	istringstream cur(floatHexString);
	cur >> std::hex >> myUnion.i;
	return myUnion.f;
}

//----------------------------------------
string ofHexToString(const string& stringHexString) {
	stringstream out;
	stringstream stream(stringHexString);
	// a hex string has two characters per byte
	std::size_t numBytes = stringHexString.size() / 2;
	for(std::size_t i = 0; i < numBytes; i++) {
		string curByte;
		// grab two characters from the hex string
		stream >> setw(2) >> curByte;
		// prepare to parse the two characters
		stringstream curByteStream(curByte);
		int cur = 0;
		// parse the two characters as a hex-encoded int
		curByteStream >> std::hex >> cur;
		// add the int as a char to our output stream
		out << (char) cur;
	}
	return out.str();
}

//----------------------------------------
float ofToFloat(const string& floatString) {
	return ofTo<float>(floatString);
}

//----------------------------------------
double ofToDouble(const string& doubleString) {
	return ofTo<double>(doubleString);
}

//----------------------------------------
int64_t ofToInt64(const string& intString) {
	return ofTo<int64_t>(intString);
}

//----------------------------------------
bool ofToBool(const string& boolString) {
	if(boolString == "true") {
		return true;
	}
	if(boolString == "false") {
		return false;
	}
	bool x = false;
	istringstream cur(boolString);
	cur >> x;
	return x;
}

//----------------------------------------
char ofToChar(const string& charString) {
	return ofTo<char>(charString);
}

//----------------------------------------
template <> string ofToBinary(const string& value) {
	stringstream out;
	std::size_t numBytes = value.size();
	for(std::size_t i = 0; i < numBytes; i++) {
		std::bitset<8> bitBuffer(value[i]);
		out << bitBuffer;
	}
	return out.str();
}

//----------------------------------------
string ofToBinary(const char* value) {
	// this function is necessary if you want to print a string
	// using a syntax like ofToBinary("test")
	return ofToBinary((string) value);
}

//----------------------------------------
int ofBinaryToInt(const string& value) {
	const int intSize = sizeof(int) * 8;
	std::bitset<intSize> binaryString(value);
	return (int) binaryString.to_ulong();
}

//----------------------------------------
char ofBinaryToChar(const string& value) {
	const int charSize = sizeof(char) * 8;
	std::bitset<charSize> binaryString(value);
	return (char) binaryString.to_ulong();
}

//----------------------------------------
float ofBinaryToFloat(const string& value) {
	const int floatSize = sizeof(float) * 8;
	std::bitset<floatSize> binaryString(value);
	union ulongFloatUnion {
			unsigned long result;
			float f;
	} myUFUnion;
	myUFUnion.result = binaryString.to_ulong();
	return myUFUnion.f;
}
//----------------------------------------
string ofBinaryToString(const string& value) {
	ostringstream out;
	stringstream stream(value);
	std::bitset<8> byteString;
	std::size_t numBytes = value.size() / 8;
	for(std::size_t i = 0; i < numBytes; i++) {
		stream >> byteString;
		out << (char) byteString.to_ulong();
	}
	return out.str();
}

//--------------------------------------------------
void ofStringReplace(string& input, const string& searchStr, const string& replaceStr){
	auto pos = input.find(searchStr);
	while(pos != std::string::npos){
		input.replace(pos, searchStr.size(), replaceStr);
		pos += replaceStr.size();
		std::string nextfind(input.begin() + pos, input.end());
		auto nextpos = nextfind.find(searchStr);
		if(nextpos==std::string::npos){
			break;
		}
		pos += nextpos;
	}
}

//--------------------------------------------------
bool ofIsStringInString(const string& haystack, const string& needle){
    return haystack.find(needle) != std::string::npos;
}

//--------------------------------------------------
std::size_t ofStringTimesInString(const string& haystack, const string& needle){
	const size_t step = needle.size();

	size_t count(0);
	size_t pos(0) ;

	while( (pos=haystack.find(needle, pos)) != std::string::npos) {
		pos +=step;
		++count ;
	}

	return count;
}
