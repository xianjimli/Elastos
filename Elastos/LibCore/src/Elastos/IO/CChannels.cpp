
#include "CChannels.h"
#include "Channels.h"
CChannels::CChannels() {}

CChannels::~CChannels() {}

ECode CChannels::constructor()
{
	return NOERROR;
}

ECode CChannels::NewChannel(
	/* [in] */ IInputStream* inputStream,
 	/* [out] */ IReadableByteChannel** inputChannel)
{
	*inputChannel = Channels::NewChannel(inputStream);
	return NOERROR;
}

ECode CChannels::NewChannelEx(
	/* [in] */ IOutputStream* outputStream,
	/* [out] */ IWritableByteChannel** outputChannel)
{
	*outputChannel = Channels::NewChannel(outputStream);
	return NOERROR;
}

ECode CChannels::NewInputStream(
	/* [in] */ IReadableByteChannel * pChannel,
	/* [out] */ IInputStream ** ppInputStream)
{
	return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewOutputStream(
	/* [in] */ IWritableByteChannel * pChannel,
	/* [out] */ IOutputStream ** ppOutputStream)
{
	return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewReader(
	/* [in] */ IReadableByteChannel * pChannel,
	/* [in] */ ICharsetDecoder * pDecoder,
	/* [in] */ Int32 minBufferCapacity,
 	/* [out] */ IReader ** ppReader)
{
	return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewReaderEx(
	/* [in] */ IReadableByteChannel * pChannel,
	/* [in] */ const String& charsetName,
	/* [out] */ IReader ** ppReader)
{
	return E_NOT_IMPLEMENTED;
}

ECode CChannels::NewWriter(
	/* [in] */ IWritableByteChannel * pChannel,
	/* [in] */ ICharsetEncoder * pEncoder,
	/* [in] */ Int32 minBufferCapacity,
	/* [out] */ IWriter ** ppWriter)
{
	return E_NOT_IMPLEMENTED;	
}

ECode CChannels::NewWriterEx(
	/* [in] */ IWritableByteChannel * pChannel,
	/* [in] */ const String& charsetName,
 	/* [out] */ IWriter ** ppWriter)
{ 
	return E_NOT_IMPLEMENTED;
}
