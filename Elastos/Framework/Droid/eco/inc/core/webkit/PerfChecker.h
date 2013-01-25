#ifndef __PERFCHECKER_H__
#define __PERFCHECKER_H__

class PerfChecker
{
public:
	PerfChecker();

    /**
     * @param what log string
     * Logs given string if mResponseThreshold time passed between either
     * instantiation or previous responseAlert call
     */
	virtual CARAPI_(void) ResponseAlert(
		/* [in] */ CString what);

private:
	Int64 mTime;

	static const Int64 mResponseThreshold = 2000;    // 2s
};

#endif //__PERFCHECKER_H__