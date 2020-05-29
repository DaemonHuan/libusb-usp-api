#pragma once
#ifndef __CDEBUG_LOG_H
#define __CDEBUG_LOG_H

#define _LOG_BUFFSIZE     (1024 * 1024 * 8)
#define _SYS_BUFFSIZE     (1024 * 1024 * 8)
#define	_LOG_PATH_LEN     512
#define _LOG_MODULE_LEN   1024

typedef  enum LogLevel {
	LL_DEBUG = 1,
	LL_INFO = 2,
	LL_NOTICE = 3,
	LL_WARN = 4,
	LL_ERROR = 5
}LogLevel;

//============basic===================
//
#define LOG_ERROR(log_fmt, log_arg ...) \
    do{ \
        _cDebug(LL_ERROR,   "[%s] " log_fmt "\n", __FUNCTION__, ##log_arg); \
    } while (0)

#define LOG_WARN(log_fmt, log_arg ...) \
    do{ \
        _cDebug(LL_WARN,   "[%s] " log_fmt "\n", __FUNCTION__, ##log_arg); \
    } while (0)

#define LOG_NOTICE(log_fmt, log_arg ...) \
    do{ \
        _cDebug(LL_NOTICE,   "[%s] " log_fmt "\n", __FUNCTION__, ##log_arg); \
    } while (0)

#define LOG_INFO(log_fmt, log_arg ...) \
    do{ \
        _cDebug(LL_INFO,   "[%s] " log_fmt "\n", __FUNCTION__, ##log_arg); \
    } while (0)
#define LOG_DEBUG(log_fmt, log_arg ...) \
    do{ \
        _cDebug(LL_DEBUG,   "[%s] " log_fmt "\n", __FUNCTION__, ##log_arg); \
    } while (0)
//============extend===================

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

#define MACRO_RET(condition, return_val) { if (condition) return return_val; }

	int cDebug_init(LogLevel lev, const char* p_modulename, const char* p_logdir, int t_append, int t_sync);
	int cDebug_close();

	extern int _cDebug(LogLevel l, const char* logformat, ...);

#ifdef __cplusplus
}
#endif /*__cplusplus */
#endif // CDEBUG_H
