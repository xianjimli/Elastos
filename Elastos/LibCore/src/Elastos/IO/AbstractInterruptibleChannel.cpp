#include "AbstractInterruptibleChannel.h"

Boolean AbstractInterruptibleChannel::IsOpen()
{
	return !mClosed;
}


