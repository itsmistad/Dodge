#include "Log.h"
#include <stdarg.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <direct.h>
#include <windows.h>

string Log::currentFile = "";

void Log::Initialize()
{
	// Define the name of the current file as the timestamp + ".txt".
	// If the file does not exist, create it.
	auto currentTime = time(NULL);
	tm localTime;
	localtime_s(&localTime, &currentTime);
	ostringstream stream;
	stream << put_time(&localTime, "%Y-%m-%dT%H-%M-%S");

	currentFile = stream.str() + ".txt";

	_mkdir("Logs");
}

void Log::Write(string tag, string message)
{
	// Log it to the current log file.
	// 1. Open a stream to the file at "/Logs/" + file name variable (what happens if the file does not exist?)
	fstream logFileStream;
	logFileStream.open("Logs/" + currentFile, fstream::out | fstream::app);
	// 2. Append "<timestamp>\t<tag>\t<message>\n" to the end of the file stream.
	auto currentTime = time(NULL);
	tm localTime;
	localtime_s(&localTime, &currentTime);
	logFileStream << put_time(&localTime, "%Y-%m-%dT%H:%M:%S") << "\t" << tag << "\t" << message << endl;
	// 3. Close the file stream.
	logFileStream.close();
}

void Log::Debug(string format, ...)
{
#ifdef _DEBUG
	char message[1024];
	va_list args;

	va_start(args, format);
	_vsnprintf_s(message, sizeof(message), format.c_str(), args);
	va_end(args);

	Write("[DEBUG]", message);
#endif
}

void Log::Info(string format, ...)
{
	char message[1024];
	va_list args;

	va_start(args, format);
	_vsnprintf_s(message, sizeof(message), format.c_str(), args);
	va_end(args);

	Write("[INFO]", message);
}

void Log::Warn(string format, ...)
{
	char message[1024];
	va_list args;

	va_start(args, format);
	_vsnprintf_s(message, sizeof(message), format.c_str(), args);
	va_end(args);

	Write("[WARN]", message);
}

void Log::Error(string format, ...)
{
	char message[1024];
	va_list args;

	va_start(args, format);
	_vsnprintf_s(message, sizeof(message), format.c_str(), args);
	va_end(args);

	Write("[ERROR]", message);
}