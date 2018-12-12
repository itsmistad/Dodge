#pragma once
#include <string>
#include <sstream>

using namespace std;

template <typename T> string to_string_with_precision(T input, int precision = 1)
{
	ostringstream stream;
	stream.precision(precision);
	stream << fixed << input;
	return stream.str();
}

template <typename T> wstring to_wstring_with_precision(T input, int precision = 1)
{
	ostringstream stream;
	stream.precision(precision);
	stream << fixed << input;
	auto str = stream.str();
	return wstring(str.begin(), str.end());
}