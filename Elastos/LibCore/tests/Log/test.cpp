#include <Logger.h>
#include <Slogger.h>

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace Elastos::Util::Logging;

int create_dir(const char * name)
{
	if (access(name, F_OK) == 0) return 0;
	return (mkdir(name, 0755) < 0) ? -1 : 0;
}

int create_file(const char * name)
{
	if (access(name, F_OK) == 0) return 0;
	return (open(name, O_CREAT | O_WRONLY | O_TRUNC) < 0) ? -1 : 0;
}

int create_log_files()
{
	if (create_dir("/elastos/log") < 0) return -1;
	if (create_file("/elastos/log/main") < 0) return -1;
	if (create_file("/elastos/log/radio") < 0) return -1;
	if (create_file("/elastos/log/events") < 0) return -1;
	if (create_file("/elastos/log/system") < 0) return -1;
	return 0;
}

int main()
{
	if (create_log_files() < 0)	{
		printf("<err> create log files failed.\n");
		return -1;
	}
	
	// Test Logger
	Logger::D("Logger::D", "debug message.\n");
	Logger::E("Logger::E", "error message.\n");
	Logger::I("Logger::I", "infomation message.\n");
	Logger::V("Logger::V", "vebose message.\n");
	Logger::W("Logger::W", "warning mesage.\n");

	Slogger::D("Slogger::D", "system debug message.\n");
	Slogger::E("Slogger::E", "system error message.\n");
	Slogger::I("Slogger::I", "system infomation message.\n");
	Slogger::V("Slogger::V", "system vebose message.\n");
	Slogger::W("Slogger::W", "system warning mesage.\n");

	printf(">>>>> Test Logger ok.\n");
	return 0;
}
