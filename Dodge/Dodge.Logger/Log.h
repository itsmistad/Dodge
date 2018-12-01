#pragma once

#include <string>

using namespace std;

class Log
{
private:
	static string currentFile;

	static void Write(string tag, string message);

public:
	static void Initialize();
	static void Debug(string format, ...);
	static void Info(string format, ...);
	static void Warn(string format, ...);
	static void Error(string format, ...);
};

