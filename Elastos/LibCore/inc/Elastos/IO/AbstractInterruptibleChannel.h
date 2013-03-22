#include <Elastos.IO_server.h>

class AbstractInterruptibleChannel
{
public:
	Boolean IsOpen();
protected:
	AbstractInterruptibleChannel() : mClosed(false),
									 mInterrupted(false) 
									 {}

	~AbstractInterruptibleChannel() {}

	virtual void ImplCloseChannel() = 0;

private:
	volatile Boolean mClosed;
	volatile Boolean mInterrupted;
};
