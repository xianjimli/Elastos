
#ifndef __ZLELTIME_H__
#define __ZLELTIME_H__

#include <ZLTimeManager.h>

class ZLElTimeManager : public ZLTimeManager {

private:
	ZLTime currentTime() const;
	short hoursBySeconds(long seconds) const;
	short minutesBySeconds(long seconds) const;

	short yearBySeconds(long seconds) const;
	short monthBySeconds(long seconds) const;
	short dayOfMonthBySeconds(long seconds) const;
};

#endif /* __ZLELTIME_H__ */
