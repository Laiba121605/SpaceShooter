#include "Logger.h"

Logger& Logger::getinstance()
{
	static Logger obj;
	return obj;
}

void Logger::initialize(const string& filename)
{
	cout << "Logger::initialize called with: " << filename << endl;

	file.open(filename, ios::out | ios::app);

	if (!file.is_open())
	{
		cerr << "Failed to open log file: " << filename << endl;
	}
	else
	{
		file << "Log file initialized: " << filename << endl;
	}
}

void Logger::cleanup(const string& filename)
{
	if (file.is_open())
	{
		file.close();
	}
}

void Logger::writeerror(const string& error_message)
{
	if (!file.is_open())
	{
		cerr << "Log file is not open. Cannot write error message." << endl;
		return;
	}

	time_t t = time(nullptr);
	tm tm_buf;
	localtime_s(&tm_buf, &t);
	char arr[32];
	strftime(arr, sizeof(arr), "%Y-%m-%d %H:%M:%S", &tm_buf);
	file << "[" << arr << "] " << error_message << endl;
}