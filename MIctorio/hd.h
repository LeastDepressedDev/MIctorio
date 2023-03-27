#pragma once

#ifndef WELCOME_MSG
#define WELCOME_MSG "######################################\n#       Mod Interface For Factorio   #\n#    by Sirtage(qigan)               #\n#          Made in Russia            #\n######################################\n Type >help to see for commands."
#endif 

#ifndef RECENT
#define RECENT "r:"
#endif 


#ifdef _WIN32
#define PROG_CLR "cls"
#define PROG_RM "del"
#define PROG_RMDIR "rmdir"
#else
#define PROG_CLR "clear"
#define PROG_RM "rm"
#define PROG_RMDIR "rm"
#endif