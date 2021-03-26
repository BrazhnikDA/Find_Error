#pragma once
#include <string>
#include <fstream>

#define PATH_LOG_FILE L"D:\\1\\LOG\\Log_Find_Error.log"

class Log
{
public:
	Log();
	~Log();

	std::wstring GetDate();
	std::wstring GetTime();
	void OutputFile(std::wstring outText, bool Date);
private:
};

Log::Log()
{
}

Log::~Log()
{
}

inline std::wstring Log::GetDate()
{
	time_t t;
	struct tm* tm;

	t = time(NULL);
	tm = localtime(&t);

	std::wstring month;
	if (++tm->tm_mon < 10)
	{
		month = L"0";
		month += std::to_wstring(tm->tm_mon);
	}
	else { month = std::to_wstring(tm->tm_mon); }

	std::wstring res = std::to_wstring((tm->tm_year % 100) + 2000) + L"/" + month + L"/" + std::to_wstring(tm->tm_mday);

	return res;
}

inline std::wstring Log::GetTime()
{
    time_t t;
    struct tm* tm;

    t = time(NULL);
    tm = localtime(&t);

	std::wstring min;
	if (tm->tm_min < 10)
		min = L"0" + std::to_wstring(tm->tm_min);
	else min = std::to_wstring(tm->tm_min);

	std::wstring sec;
	if (tm->tm_sec < 10)
		sec = L"0" + std::to_wstring(tm->tm_sec);
	else sec = std::to_wstring(tm->tm_sec);

	std::wstring res = std::to_wstring(tm->tm_hour) + L":" + min + L":" + sec;

	return res;
}

inline void Log::OutputFile(std::wstring outText, bool Date)
{
	std::wofstream ofs(PATH_LOG_FILE, std::ios::out | std::ios::app); //создать

	if (Date) { ofs << GetTime(); }

	for (int i = 0; i < outText.size(); i++)
	{
		ofs << outText[i];
	}
	ofs.close();
}

