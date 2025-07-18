#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef MODE_DEBUG
#define NEED_DEBUG 1
#else
#define NEED_DEBUG 0
#endif

#define printde(fmt, ...) do { if (NEED_DEBUG) {fprintf(stderr, "%s:%s():%d: " fmt, __FILE__,  __func__,__LINE__, __VA_ARGS__); fflush(stderr);} } while (0)
#define putsde(str) do { if (NEED_DEBUG) {fprintf(stderr, "%s:%s():%d: %s",__FILE__,  __func__,__LINE__, str); fflush(stderr);} } while (0)
#define perrord(str) do { if (NEED_DEBUG) {fprintf(stderr, "%s:%s():%d: ",__FILE__,  __func__,__LINE__);perror(str); fflush(stderr);} } while (0)
#define printdo(fmt, ...) do { if (NEED_DEBUG) {fprintf(stdout, fmt, __VA_ARGS__); fflush(stdout);} } while (0)
#define putsdo(str) do { if (NEED_DEBUG) {fputs(str,stdout); fflush(stdout);} } while (0)
#define perrorl(str) {fprintf(stderr, "%s:%s():%d: ",__FILE__,  __func__,__LINE__);perror(str); fflush(stderr);}
#define putsel(str) {fprintf(stderr, "%s:%s():%d: %s",__FILE__,  __func__,__LINE__, str); fflush(stderr);}
#endif 

