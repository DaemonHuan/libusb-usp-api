//#include "stdafx.h"
#include "cdebug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <time.h>

#include <io.h>
#include <direct.h>
//#include <unistd.h>
//#include <pthread.h>

#include <sys/stat.h>
#include <sys/timeb.h>
//#include <sys/file.h>

#define TRUE    (1)
#define FALSE   (0)

enum LogLevel lm_system_level;
static int lm_isAppend = 0;
static int lm_isSync = 0;
char lm_filelocation[_LOG_PATH_LEN] = "\0";
//pthread_mutex_t lm_mutex = PTHREAD_MUTEX_INITIALIZER;
FILE * lm_file_fp = NULL;

int _log_init(LogLevel l, const char *filelocation, int append, int issync)
{
	MACRO_RET(NULL != lm_file_fp, FALSE);
	lm_system_level = l;
	lm_isAppend = append;
	lm_isSync = issync;
	if (strlen(filelocation) >= (sizeof(lm_filelocation) - 1))
	{
		fprintf(stderr, "the path of log file is too long:%d limit:%d\n", strlen(filelocation), sizeof(lm_filelocation) - 1);
		exit(0);
	}
	//本地存储filelocation  以防止在栈上的非法调用调用
	strncpy(lm_filelocation, filelocation, sizeof(lm_filelocation));
	lm_filelocation[sizeof(lm_filelocation) - 1] = '\0';

	if ('\0' == lm_filelocation[0])
	{
		lm_file_fp = stdout;
		fprintf(stderr, "now all the running-information are going to put to stderr\n");
		return TRUE;
	}

	lm_file_fp = fopen(lm_filelocation, append ? "a" : "w");
	if (lm_file_fp == NULL)
	{
		fprintf(stderr, "cannot open log file,file location is %s\n", lm_filelocation);
		exit(0);
	}
	char buff[1024] = "\0";
	//setvbuf (fp, io_cached_buf, _IOLBF, sizeof(io_cached_buf)); //buf set _IONBF  _IOLBF  _IOFBF
	setvbuf(lm_file_fp, buff, _IOFBF, sizeof(buff));
	//setvbuf(lm_file_fp, (char *)NULL, _IOLBF, 0);
	fprintf(stderr, "now all the running-information are going to the file %s\n", lm_filelocation);
	return TRUE;
}

const char* _log_getlevelstring(LogLevel lev)
{
	switch (lev) {
	case LL_DEBUG:
		return "DEBUG";
	case LL_INFO:
		return "INFO";
	case LL_NOTICE:
		return "NOTICE";
	case LL_WARN:
		return "WARN";
	case LL_ERROR:
		return "ERROR";
	default:
		return "UNKNOWN";
	}
}

int _log_checklevel(LogLevel lev)
{
	int iResult = (lev >= lm_system_level) ? TRUE : FALSE;
	return iResult;
}

int _log_maketitle(char* m_buffer, LogLevel lev)
{
	struct timeb time_cur;
	ftime(&time_cur);

	struct tm vtm;
	localtime_s(&vtm, &time_cur.time);

	return snprintf(m_buffer, _LOG_BUFFSIZE, "[%04d-%02d-%02d %02d:%02d:%02d.%03d][\t%s\t]", vtm.tm_year + 1900, vtm.tm_mon + 1, vtm.tm_mday,
		vtm.tm_hour, vtm.tm_min, vtm.tm_sec, time_cur.millitm, _log_getlevelstring(lev));
}

int _log_Write(char *_pbuffer, int len)
{
	//    if(0 != access(lm_filelocation, W_OK))
	//    {
	//        pthread_mutex_lock(&lm_mutex);
	//锁内校验 access 看是否在等待锁过程中被其他线程loginit了  避免多线程多次close 和init
	//        if(0 != access(lm_filelocation, W_OK))
	//        {
	//            cDebug_close();
	//            _log_init(lm_system_level, lm_filelocation, lm_isAppend, lm_isSync);
	//        }
	//        pthread_mutex_unlock(&lm_mutex);
	//    }

	if (0 != _access(lm_filelocation, 2))
	{
		cDebug_close();
		_log_init(lm_system_level, lm_filelocation, lm_isAppend, lm_isSync);
	}

	if (1 == fwrite(_pbuffer, len, 1, lm_file_fp)) //only write 1 item
	{
		if (lm_isSync) fflush(lm_file_fp);
		*_pbuffer = '\0';
	}
	else
	{
		int x = errno;
		fprintf(stderr, "Failed to write to logfile. errno:%s    message:%s", strerror(x), _pbuffer);
		return FALSE;
	}
	return TRUE;
}

//
int cDebug_init(LogLevel lev, const char* p_modulename, const char* p_logdir, int t_append, int t_sync)
{
	//如果路径存在文件夹，则判断是否存在
	if (_access(p_logdir, 0) == -1)
	{
		if (_mkdir(p_logdir) < 0) fprintf(stderr, "create folder failed\n");
	}

	/*
	*     snprintf(_location_str, _LOG_PATH_LEN, "%s/%s.access", p_logdir, p_modulename);
	//    INFO_W.loginit(l, _location_str);
	loginit(l, _location_str);
	snprintf(_location_str, _LOG_PATH_LEN, "%s/%s.error", p_logdir, p_modulename);
	//warning级别以上日志去WARN_W  去向由宏决定的 请见macro_define.h
	if(l > LL_WARN) loginit(l, _location_str);
	//        WARN_W.loginit(l, _location_str);

	else loginit(LL_WARN, _location_str);
	//        WARN_W.loginit(LL_WARNING, _location_str);
	*/
	char _location_str[_LOG_PATH_LEN];
	snprintf(_location_str, _LOG_PATH_LEN, "%s/%s.clog", p_logdir, p_modulename);
	_log_init(lev, _location_str, t_append, t_sync);

	return TRUE;
}

int cDebug_close()
{
	if (lm_file_fp == NULL) return FALSE;

	fflush(lm_file_fp);
	fclose(lm_file_fp);
	lm_file_fp = NULL;

	return TRUE;
}

int _cDebug(LogLevel lev, const char* logformat, ...)
{
	MACRO_RET(!_log_checklevel(lev), FALSE);
	int _size;
	int prestrlen = 0;

	//    static __thread char lm_buffer[_LOG_BUFFSIZE] = "\0";
	static char lm_buffer[_LOG_BUFFSIZE] = "\0";
	char * star = lm_buffer;
	prestrlen = _log_maketitle(star, lev);
	star += prestrlen;

	va_list args;
	va_start(args, logformat);
	_size = vsnprintf(star, _LOG_BUFFSIZE - prestrlen, logformat, args);
	va_end(args);

	//    if(NULL == lm_file_fp) fprintf(stderr, "%s", lm_buffer);
	//    else _logWrite(lm_buffer, prestrlen + _size);
	if (NULL != lm_file_fp) _log_Write(lm_buffer, prestrlen + _size);

	return TRUE;
}
