#include "Channels.h"

IReadableByteChannel* Channels::NewChannel( 
	IInputStream* inputStream)
{   
	IReadableByteChannel* inputChannel = (IReadableByteChannel*)
										 new ReadableByteChannelImpl(
										 inputStream);
	return inputChannel;
}

IWritableByteChannel* Channels::NewChannel(
	IOutputStream* outputStream)
{
	IWritableByteChannel* outputChannel;
	outputChannel = (IWritableByteChannel*)
					new WritableByteChannelImpl(
					outputStream);
	
	return outputChannel;
}

