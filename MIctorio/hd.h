#pragma once

#ifdef _WIN32
#define PROG_CLR "cls"
#define PROG_RM "del"
#define PROG_RMDIR "rmdir"
#else
#define PROG_CLR "clear"
#define PROG_RM "rm"
#define PROG_RMDIR "rm"
#endif