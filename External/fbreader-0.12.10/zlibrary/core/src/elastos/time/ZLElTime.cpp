
#include <sys/timeb.h>
#include <time.h>

#include <ZLTime.h>

#include "ZLElTime.h"

ZLTime ZLElTimeManager::currentTime() const {
	struct timeb timeB;
	ftime(&timeB);
	return ZLTime(timeB.time, timeB.millitm);
}

short ZLElTimeManager::hoursBySeconds(long seconds) const {
	return localtime(&seconds)->tm_hour;
}

short ZLElTimeManager::minutesBySeconds(long seconds) const {
	return localtime(&seconds)->tm_min;
}

short ZLElTimeManager::yearBySeconds(long seconds) const {
	return localtime(&seconds)->tm_year + 1900;
}

short ZLElTimeManager::monthBySeconds(long seconds) const {
	return localtime(&seconds)->tm_mon + 1;
}

short ZLElTimeManager::dayOfMonthBySeconds(long seconds) const {
	return localtime(&seconds)->tm_mday;
}
