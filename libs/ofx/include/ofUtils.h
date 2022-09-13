#pragma once

#include "ofConstants.h"
#include <bitset> // For ofToBinary.
#include <chrono>
#include <iomanip>  //for setprecision
#include <algorithm>
#include <sstream>

/// \section Precision Time
/// \brief Get the precision time in microseconds
/// \returns the microseconds of the precision timer
uint64_t ofPrecisionTime();

/// \section Vectors
/// \brief Randomly reorder the values in a vector.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to modify.
/// \sa http://www.cplusplus.com/reference/algorithm/random_shuffle/
template<class T>
void ofRandomize(std::vector<T>& values) {
	random_shuffle(values.begin(), values.end());
}

/// \brief Conditionally remove values from a vector.
///
/// Values are removed if, when passed to the BoolFunction, the BoolFunction
/// function returns true.  The given BoolFunction can be a custom function
/// or a built-in function.
///
/// Example of a custom function to remove odd numbers from a std::vector<int>
/// of integers:
///
/// ~~~~{.cpp}
/// bool IsOdd(int i) {
///     return ((i % 2) == 1);
/// }
/// ~~~~
///
/// To call the function, one might use:
///
/// ~~~~{.cpp}
/// std::vector<int> myInts;
///
/// // Fill the vector with integers.
/// for (int i = 0; i < 10; ++i)
/// {
///     myInts.push_back(i);
/// }
///
/// ofRemove(myInts, IsOdd);
/// ~~~~
///
/// The resulting vector will contain the following values:
///
///    0, 2, 4, 6, 8
///
/// \tparam T the type contained by the vector.
/// \tparam BoolFunction the boolean function used to erase.
/// \param values The vector of values to modify.
/// \param shouldErase A boolean function as described above.
/// \sa http://www.cplusplus.com/reference/algorithm/remove_if/
template<class T, class BoolFunction>
void ofRemove(std::vector<T>& values, BoolFunction shouldErase) {
	values.erase(remove_if(values.begin(), values.end(), shouldErase), values.end());
}

/// \brief Sort a vector of values into ascending order.
///
/// The elements are compared using operator< for the first version.
/// Equivalent elements are not guaranteed to keep their original relative
/// order.
///
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be sorted.
/// \sa http://www.cplusplus.com/reference/algorithm/sort/
template<class T>
void ofSort(std::vector<T>& values) {
	sort(values.begin(), values.end());
}

/// \brief Sort a vector of values into an order defined by a comparator.
///
/// Example of a custom function to sort descending of integers:
///
/// ~~~~{.cpp}
/// bool sortDescending(int i, int j)
/// {
///     return (j < i);
/// }
/// ~~~~
///
/// This binary function must accept two elements in the range as arguments and
/// return a value convertible to bool. The value returned indicates whether the
/// element passed as first argument is considered to go before the second in
/// the specific strict weak ordering it defines.  The function shall not modify
/// any of its arguments.  This can either be a function pointer or a function
/// object.
///
/// To call the function, one might use:
///
/// ~~~~{.cpp}
/// std::vector<int> myInts;
///
/// // Fill the vector with integers.
/// for (int i = 0; i < 10; ++i)
/// {
///     myInts.push_back(i);
/// }
///
/// ofSort(myInts, sortDescending);
/// ~~~~
///
/// The resulting vector will contain the following values:
///
///    9, 8, 7, 6, 5, 4, 3, 2, 1, 0.
///
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be sorted.
/// \param compare The comparison function.
/// \sa http://www.cplusplus.com/reference/algorithm/sort/
template<class T, class BoolFunction>
void ofSort(std::vector<T>& values, BoolFunction compare) {
	std::sort(values.begin(), values.end(), compare);
}

/// \brief Search for a target value in a vector of values.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be searched.
/// \param target The target value to be found.
/// \returns true the index of the first target value found.
/// \sa http://www.cplusplus.com/reference/iterator/distance/
template <class T>
std::size_t ofFind(const std::vector<T>& values, const T& target) {
	return std::distance(values.begin(), find(values.begin(), values.end(), target));
}

/// \brief Search for a target value in a vector of values.
/// \tparam T the type contained by the vector.
/// \param values The vector of values to be searched.
/// \param target The target value to be found.
/// \returns true if at least one value equal to the target value is found.
template <class T>
bool ofContains(const std::vector<T>& values, const T& target) {
	return ofFind(values, target) != values.size();
}

/// \brief Replace all occurrences of a string with another string.
/// \note The input string is passed by reference, so it will be modified.
/// \param input The string to run the replacement on.
/// \param searchStr The string to be replaced.
/// \param replaceStr The string to put in place.
void ofStringReplace(std::string& input, const std::string& searchStr, const std::string& replaceStr);

/// \brief Check if string contains another string.
///
/// Example:
/// ~~~{.cpp}
///     std::string haystack = "foobar";
///     ofLog() << ofIsStringInString(haystack, "foo"); // Outputs 1.
///     ofLog() << ofIsStringInString(haystack,"bar"); // Outputs 1.
///     ofLog() << ofIsStringInString(haystack, "something else"); // Outputs 0.
/// ~~~
/// \param haystack The string to check for occurrence in.
/// \param needle The string to check for.
bool ofIsStringInString(const std::string& haystack, const std::string& needle);

/// \brief Check how many times a string contains another string.
/// \param haystack The string to check for occurrence in .
/// \param needle The string to check for.
std::size_t ofStringTimesInString(const std::string& haystack, const std::string& needle);

/// \section String Conversion
/// \brief Convert a value to a string.
///
/// ofToString does its best to convert any value to a string. If the data type
/// implements a stream << operator, then it will be converted.
///
/// Example:
/// ~~~~{.cpp}
///    std::string str = "framerate is ";
///    str += ofToString(ofGetFrameRate()) + " fps";
///    // The string now contains something like "framerate is 60 fps".
/// ~~~~
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \returns A string representing the value or an empty string on failure.
template <class T>
std::string ofToString(const T& value){
	std::ostringstream out;
	out << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific precision.
///
/// Like sprintf "%4f" format, in this example precision=4
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \param precision The precision to use when converting to a string.
/// \returns The string representation of the value.
template <class T>
std::string ofToString(const T& value, int precision){
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision) << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific width and fill
///
/// Like sprintf "% 4d" or "% 4f" format, in this example width=4, fill=' '
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \param width The width of the value to use when converting to a string.
/// \param fill The character to use when padding the converted string.
/// \returns The string representation of the value.
template <class T>
std::string ofToString(const T& value, int width, char fill ){
	std::ostringstream out;
	out << std::fixed << std::setfill(fill) << std::setw(width) << value;
	return out.str();
}

/// \brief Convert a value to a string with a specific precision, width and filll
///
/// Like sprintf "%04.2d" or "%04.2f" format, in this example precision=2, width=4, fill='0'
///
/// \tparam T The data type of the value to convert to a string.
/// \param value The value to convert to a string.
/// \param precision The precision to use when converting to a string.
/// \param width The width of the value to use when converting to a string.
/// \param fill The character to use when padding the converted string.
/// \returns The string representation of the value.
template <class T>
std::string ofToString(const T& value, int precision, int width, char fill ){
	std::ostringstream out;
	out << std::fixed << std::setfill(fill) << std::setw(width) << std::setprecision(precision) << value;
	return out.str();
}

/// \brief Convert a vector of values to a comma-delimited string.
///
/// This method will take any vector of values and output a list of the values
/// as a comma-delimited string.
///
/// \tparam T The data type held by the vector.
/// \param values The vector of values to be converted to a string.
/// \returns a comma-delimited string representation of the intput values.
template<class T>
std::string ofToString(const std::vector<T>& values) {
	std::stringstream out;
	int n = values.size();
	out << "{";
	if(n > 0) {
		for(int i = 0; i < n - 1; i++) {
			out << values[i] << ", ";
		}
		out << values[n - 1];
	}
	out << "}";
	return out.str();
}

/// \brief Convert a string represetnation to another type.
///
/// Often, this method will be used to convert a string to a numerical type.
///
/// \tparam T The target data type.
/// \param value The string value to convert to type T.
/// \returns the string converted to the target data type T.
template<class T>
T ofFromString(const std::string & value){
	T data;
	std::stringstream ss;
	ss << value;
	ss >> data;
	return data;
}

/// \brief Convert a string represetnation to another string.
/// \param value The string value to convert to another string.
/// \returns the string converted to another string.
template<>
std::string ofFromString(const std::string & value);

/// \brief Convert a string represetnation to another string.
///
/// Equivalent to calling the `.c_str()` on a std::string.
///
/// \param value The string value to convert to another string.
/// \returns the string converted to a c-style string.
template<>
const char * ofFromString(const std::string & value);

/// \brief Convert a string to a given data type.
/// \tparam T The return type.
/// \param value The string value to convert to a give type.
/// \returns the string converted to the type.
template<typename T>
T ofTo(const std::string & str){
	T x;
	std::istringstream cur(str);
	cur >> x;
	return x;
}

/// \section Number Conversion
/// \brief Convert a string to an integer.
///
/// Converts a `std::string` representation of an int (e.g., `"3"`) to an actual
/// `int`.
///
/// \param intString The string representation of the integer.
/// \returns the integer represented by the string or 0 on failure.
int ofToInt(const std::string& intString);

/// \brief Convert a string to a int64_t.
///
/// Converts a `std::string` representation of a long integer
/// (e.g., `"9223372036854775807"`) to an actual `int64_t`.
///
/// \param intString The string representation of the long integer.
/// \returns the long integer represented by the string or 0 on failure.
int64_t ofToInt64(const std::string& intString);

/// \brief Convert a string to a float.
///
/// Converts a std::string representation of a float (e.g., `"3.14"`) to an
/// actual `float`.
///
/// \param floatString string representation of the float.
/// \returns the float represented by the string or 0 on failure.
float ofToFloat(const std::string& floatString);

/// \brief Convert a string to a double.
///
/// Converts a std::string representation of a double (e.g., `"3.14"`) to an
/// actual `double`.
///
/// \param doubleString The string representation of the double.
/// \returns the double represented by the string or 0 on failure.
double ofToDouble(const std::string& doubleString);

/// \brief Convert a string to a boolean.
///
/// Converts a std::string representation of a boolean (e.g., `"TRUE"`) to an
/// actual `bool` using a case-insensitive comparison against the words `"true"`
/// and `"false"`.
///
/// \param boolString The string representation of the boolean.
/// \returns the boolean represented by the string or 0 on failure.
bool ofToBool(const std::string& boolString);

/// \brief Converts any value to its equivalent hexadecimal representation.
///
/// The hexadecimal representation corresponds to the way a number is stored in
/// memory.
///
/// \tparam T The data type of the value to convert to a hexadecimal string.
/// \param value The value to convert to a hexadecimal string.
/// \returns the hexadecimal string representation of the value.
template <class T>
std::string ofToHex(const T& value) {
	std::ostringstream out;
	// pretend that the value is a bunch of bytes
	unsigned char* valuePtr = (unsigned char*) &value;
	// the number of bytes is determined by the datatype
	int numBytes = sizeof(T);
	// the bytes are stored backwards (least significant first)
	for(int i = numBytes - 1; i >= 0; i--) {
		// print each byte out as a 2-character wide hex value
		out << std::setfill('0') << std::setw(2) << std::hex << (int) valuePtr[i];
	}
	return out.str();
}

/// \brief Convert a string to a hexadecimal string.
///
/// Converts a string (e.g., `"abc"`) to its equivalent hexadecimal
/// representation (e.g., `"616263"`).
///
/// \param value The value to convert to a hexadecimal string.
/// \returns a hexadecimal string.
template <>
std::string ofToHex(const std::string& value);

/// \brief Convert a c-style string to a hexadecimal string.
///
/// Converts a string (e.g., `"abc"`) to its equivalent hexadecimal
/// representation (e.g., `"616263"`).
///
/// \param value The value to convert to a hexadecimal string.
/// \returns a hexadecimal string.
std::string ofToHex(const char* value);

/// \brief Convert a string representing an integer in hexadecimal to a string.
///
/// Converts a hexadecimal representation of an int (little-endian, 32-bit,
/// e.g., `"0xbadf00d"` or `"badf00d"`) to an actual `int`.
///
/// \param intHexString The string representing an integer in hexadecimal.
/// \returns the integer represented by the string.
int ofHexToInt(const std::string& intHexString);

/// \brief Convert a string representing an char in hexadecimal to a char.
///
/// Converts a hexadecimal representation of an char (e.g., `"61"`) to an actual
/// `char` (e.g., `a`).
///
/// \param charHexString The string representing an char in hexadecimal.
/// \returns the char represented by the string.
char ofHexToChar(const std::string& charHexString);

/// \brief Convert a string representing an float in hexadecimal to a float.
///
/// Converts a hexadecimal representation of an float (little-endian, 32-bit
/// IEEE 754, e.g., `"43000000"`) to an actual float (e.g., `128.f`).
///
/// \param floatHexString The string representing an float in hexadecimal.
/// \returns the float represented by the string.
float ofHexToFloat(const std::string& floatHexString);

/// \brief Convert a string representing an string in hexadecimal to a string.
///
/// Converts a hexadecimal representation of an string (e.g.,
/// `"61626364656667"`) to an actual string (`"abcdefg"`).
///
/// \param stringHexString The string representing an string in hexadecimal.
/// \returns the string represented by the string.
std::string ofHexToString(const std::string& stringHexString);

/// \brief Convert a string representation of a char to a actual char.
///
/// Converts a string representation of a single char (e.g., `"c"`) to an
/// actual `char`.
///
/// \note Multi-byte chars and multi-char strings are not supported.
///
/// \param charString The char string to convert.
/// \returns The string as a char or 0 on failure.
char ofToChar(const std::string& charString);

/// \brief Converts any datatype value to a string of only 1s and 0s.
///
/// The corresponding string of 1s and 0s corresponds to the way value is stored
/// in memory.
///
/// \tparam T The data type of the value to convert to a binary string.
/// \param value The data to convert to a binary string.
/// \returns a binary string.
template <class T>
std::string ofToBinary(const T& value) {
	return std::bitset<8 * sizeof(T)>(*reinterpret_cast<const uint64_t*>(&value)).to_string();
}

/// \brief Converts a string value to a string of only 1s and 0s.
///
/// The corresponding string of 1s and 0s corresponds to the way value is stored
/// in memory.
///
/// \param value The string to convert to a binary string.
/// \returns a binary string.
template <>
std::string ofToBinary(const std::string& value);

/// \brief Converts a c-style string to a string of only 1s and 0s.
///
/// The corresponding string of 1s and 0s corresponds to the way value is stored
/// in memory.
///
/// \param value The c-style string to convert to a binary string.
/// \returns a binary string.
std::string ofToBinary(const char* value);

/// \brief Convert a binary string to an int.
///
/// Interprets a string consisting only of 1s and 0s as an int (little-endian,
/// 32-bit), and returns the corresponding int value.
///
/// \value The binary string.
/// \returns the integer represented by the string or 0 on failure.
int ofBinaryToInt(const std::string& value);

/// \brief Convert a binary string to an char.
///
/// Interprets a string consisting only of 1s and 0s as an char and returns
/// the corresponding int value.
///
/// \value The binary string.
/// \returns the char represented by the string or 0 on failure.
char ofBinaryToChar(const std::string& value);

/// \brief Convert a binary string to a float.
///
/// Interprets a string consisting only of 1s and 0s as an a float
/// (little-endian, 32-bit IEEE 754), and returns the corresponding float value.
///
/// \value The binary string.
/// \returns the float represented by the string or 0 on failure.
float ofBinaryToFloat(const std::string& value);

/// \brief Convert a binary string to ASCII characters.
///
/// Interprets a string consisting only of 1s and 0s as 8-bit ASCII characters
/// and returns the corresponding string.
///
/// \value The binary string.
/// \returns the ASCII string represented by the string.
std::string ofBinaryToString(const std::string& value);

template <typename T>
T ofMax(const T& a, const T& b) {
    return a > b ? a : b;
}

template <typename T>
T ofMin(const T& a, const T& b) {
    return a < b ? a : b;
}

template <typename T>
T ofClamp(const T& n, const T& lower, const T& upper) {
	return ofMax<T>(lower, ofMin(n, upper));
}


