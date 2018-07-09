/*
 * log.h
 * Copyright (C) 2018 lilin <lilin@lilin-VB>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef LOG_H
#define LOG_H
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _AUTH "lilin"
#define _GITHUB "https://github.com/lilin5819/log_h"

#ifndef DEBUG

#define LOG_DEF(...)
#define log_tag(...)
#define log_base(...)
#define logs(...)
#define log_(...)
#define log_err(...)
#define log_e(...)
#define log_d(...)
#define log_u(...)
#define log_ld(...)
#define log_lu(...)
#define log_f(...)
#define log_s(...)
#define log_p(...)
#define log_mem(...)
#define log_malloc(...)
#define log_free(...)

#define set_log_name(...)
#define set_log_file(...)
#define set_log_mode(...)
#define ok(...) __VA_ARGS__

#else /* DEBUG */

#define _RED_STR "\033[1;31m"
#define _GRE_STR "\033[1;32m"
#define _YEL_STR "\033[1;33m"
#define _FLASH_STR "\033[5m"
#define _COLOR_END "\033[0m"

#define _BLANK_ERR "blank"
#define _NULL_ERR "NULL"
#define _ASSERT_ERR "assert failed !"

#define _COLOR_MASK 0x03
#define _FLASH_MASK 0x04

typedef enum _COLOR_T { _GRE, _RED, _YEL, _END, _FLASH } COLOR_T;
static char _COLOR[5][16] = {_GRE_STR, _RED_STR, _YEL_STR, _COLOR_END,
                             _FLASH_STR};
extern char *_log_app_name;
extern int _log_verbose_mode;
extern FILE *_log_fp;
extern void set_log_name(char *appname);
extern void set_log_file(char *file);
extern void set_log_mode(int is_verbose);

#define LOG_DEF()                                                       \
  char *_log_app_name = "";                                             \
  int _log_verbose_mode = 1;                                            \
  FILE *_log_fp = NULL;                                                 \
  void set_log_name(char *appname) {                                    \
    if (!appname) return;                                               \
    _log_app_name = appname;                                            \
    logs(_GITHUB);                                                         \
    logs("start log app <%s>", appname);                                \
  }                                                                     \
  void set_log_mode(int is_verbose) { _log_verbose_mode = is_verbose; } \
  void set_log_file(char *file) {                                       \
    if (_log_fp) {                                                      \
      fclose(_log_fp);                                                  \
      _log_fp = NULL;                                                   \
    }                                                                   \
    _log_fp = fopen(file, "w+");                                        \
  }

static inline void log_base(const char flag, const char *tag, const char *file,
                            const char *fun, const int line, const char *fmt,
                            ...) {
  static char fmtbuf[1024];
  static char buf[2048];
  if (!_log_verbose_mode && !_log_fp) return;
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(fmtbuf, sizeof(fmtbuf), fmt, ap);
  va_end(ap);
  snprintf(buf, 2048, "[%s%s%s]", _COLOR[_YEL], _log_app_name, _COLOR[_END]);
  snprintf(buf + strlen(buf), 2048 - strlen(buf), "[%-10s > %-15s > %3d]", file,
           fun, line);
  if (tag)
    snprintf(buf + strlen(buf), 2048 - strlen(buf), "[%s%s%s%s]",
             _COLOR[flag & _FLASH], _COLOR[flag & _COLOR_MASK], tag,
             _COLOR[_END]);
  snprintf(buf + strlen(buf), 2048 - strlen(buf), "[%s%s%s%s]\n",
           _COLOR[flag & _FLASH], _COLOR[flag & _COLOR_MASK], fmtbuf,
           _COLOR[_END]);

  if (_log_verbose_mode) {
    fputs(buf, stdout);
    fflush(stdout);
  }
  if (_log_fp) {
    fputs(buf, _log_fp);
    fflush(_log_fp);
  }
}

#define log_line(flag, tag, ...)                                        \
  do {                                                                  \
    log_base(flag, tag, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
  } while (0)

#define logs(...) log_line(_GRE, NULL, __VA_ARGS__)
#define log_() log_line(_GRE, NULL, "line")
#define log_tag(tag, ...) log_line(_GRE, tag, __VA_ARGS__)
#define log_err(MSG, ...) log_line(_RED | _FLASH, MSG, __VA_ARGS__)
#define log_e(...) log_err("ERROR", __VA_ARGS__)
#define log_d(N) log_tag("int","%s=%d", #N, N)
#define log_u(N) log_tag("uint","%s=%u", #N, N)
#define log_ld(N) log_tag("long","%s=%ld", #N, N)
#define log_lu(N) log_tag("ulong","%s=%lu", #N, N)
#define log_f(N) log_tag("float","%s=%f", #N, N)

#define log_p(N)                          \
  do {                                    \
    if (!N)                               \
      log_err(_NULL_ERR, "%s", #N);       \
    else                                  \
      log_tag("pointer", "%s=%p", #N, N); \
  } while (0)
//     string
#define log_s(STR)                               \
  do {                                           \
    if (!STR)                                    \
      log_err(_NULL_ERR, "%s", #STR);            \
    else if (!((char *)(STR))[0])                \
      log_err(_BLANK_ERR, "%s=\"\"", #STR);      \
    else                                         \
      log_tag("string", "%s=\"%s\"", #STR, STR); \
  } while (0)

#define log_mem(P, LEN)                                     \
  do {                                                      \
    char hexbuf[512]; \
    for (int i = 0; i < LEN; i++) {                         \
      snprintf(hexbuf+strlen(hexbuf),512-strlen(hexbuf), "%02X", ((char *)P)[i] & 0xFF);      \
    }                                                       \
    if (!P)                                                 \
      log_err(_NULL_ERR, "%s", #P);                         \
    else                                                    \
      log_tag("MEMORY", "p:%s addr:%p len:%d HEX:%s", #P, P, LEN,hexbuf); \
  } while (0)

//     assert
#define ok(expr)                                    \
  do {                                              \
    if (!(expr)) log_err(_ASSERT_ERR,"assert msg: \"" "%s" "\"", #expr); \
  } while (0)

#define log_malloc(_VAR, _SIZE)                         \
  do {                                                  \
    log_tag("malloc", "%s %p %lu", #_VAR, _VAR, _SIZE); \
  } while (0)

#define log_free(_VAR, _SIZE)                         \
  do {                                                \
    log_tag("free", "%s %p %lu", #_VAR, _VAR, _SIZE); \
  } while (0)
#endif /* DEBUG */

#define log_size log_lu
#define log_int log_d
#define log_long log_ld
#define log_str log_s
#define log_string log_s

#endif /* !LOG_H */
