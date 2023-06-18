#ifndef FLEXCLOG_H__
#define FLEXCLOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} fc_color_t;

extern fc_color_t fc_hex_to_clr(uint32_t hex);
extern void fc_clr_to_hex(char* hex, fc_color_t* color);

typedef struct {
    const char* file;
    const char* func;
    size_t line;
} fc_code_loc_t;

typedef enum {
    FC_LVL_TRC = 0,
    FC_LVL_DBG = 1,
    FC_LVL_INF = 2,
    FC_LVL_WRN = 3,
    FC_LVL_SVR = 4,
    FC_LVL_CRT = 5,
    FC_LVL_UNK = 6,
} fc_lvl_enum_t;

typedef struct {
    fc_lvl_enum_t lvl;
    const char* name;
} fc_lvl_t;

typedef struct {
    fc_lvl_enum_t lvl;
    fc_color_t bg_clr;
    fc_color_t fg_clr;
} fc_lvl_fmt_t;

typedef struct {
    fc_lvl_fmt_t* fmt;
} fc_thm_t;

typedef struct {
    fc_lvl_t lvl;
    const char* txt;
    fc_code_loc_t loc;
} fc_msg_t;

typedef size_t (*fc_fm_func_t)(char* buff, size_t buffsz, char spec, va_list args);
typedef struct {
    char* spec;
    fc_fm_func_t fmt_func;
} fc_fmt_t;

typedef struct {
    fc_lvl_enum_t min_lvl;
    char* lvl_fmt;
    fc_fmt_t** fmts;
    char pref;
    fc_thm_t* thm;
    size_t lvls;
} fc_conf_t;

char* fc_conf_get_lvl_fmt();
void fc_conf_set_lvl_fmt(const char* lvl_fmt);

fc_fmt_t** fc_conf_get_fmts();
void fc_conf_set_fmts(fc_fmt_t** fmts);

char fc_conf_get_prefix();
void fc_conf_set_prefix(char pref);

fc_thm_t* fc_conf_get_thm();
void fc_conf_set_thm(fc_thm_t* thm);

void fc_conf_add_fmt(fc_fmt_t* fmt);

extern void fc_thm_from_hex(uint32_t* bg_hex, uint32_t* fg_hex);

extern size_t fc_fmt(char* buff, size_t buffsz, const char* fmt, ...);
extern size_t fc_fmt_p(char* buff, size_t buffsz, const char* fmt, ...);

extern void fc_log(fc_msg_t* txt, ...);

extern void fc_init();

#define BUFFER_SIZE 4096
#define CODE_LOC (fc_code_loc_t) { __FILE__, __func__, __LINE__ }

#define ESC_SEQ_BG "\x1b[48;2;%d;%d;%dm"
#define ESC_SEQ_FG "\x1b[38;2;%d;%d;%dm"
#define ESC_SEQ_RST "\x1b[0m"

#define TRACE (fc_lvl_t) { .lvl = FC_LVL_TRC, .name = "TRACE" }
#define DEBUG (fc_lvl_t) { .lvl = FC_LVL_DBG, .name = "DEBUG" }
#define INFO (fc_lvl_t) { .lvl = FC_LVL_INF, .name = "INFO" }
#define WARN (fc_lvl_t) { .lvl = FC_LVL_WRN, .name = "WARN" }
#define SEVERE (fc_lvl_t) { .lvl = FC_LVL_SVR, .name = "SEVERE" }
#define CRITICAL (fc_lvl_t) { .lvl = FC_LVL_CRT, .name = "CRITICAL" }
#define UNKNOWN (fc_lvl_t) { .lvl = FC_LVL_UNK, .name = "???" }

#define trace(msg_fmt, ...) fc_log(&(fc_msg_t) { TRACE, .txt = msg_fmt, .loc = CODE_LOC }, TRACE, CODE_LOC, ##__VA_ARGS__)
#define debug(msg_fmt, ...) fc_log(&(fc_msg_t) { DEBUG, .txt = msg_fmt, .loc = CODE_LOC }, DEBUG, CODE_LOC, ##__VA_ARGS__)
#define info(msg_fmt, ...) fc_log(&(fc_msg_t) { INFO, .txt = msg_fmt, .loc = CODE_LOC }, INFO, CODE_LOC, ##__VA_ARGS__)
#define warn(msg_fmt, ...) fc_log(&(fc_msg_t) { WARN, .txt = msg_fmt, .loc = CODE_LOC }, WARN, CODE_LOC, ##__VA_ARGS__)
#define severe(msg_fmt, ...) fc_log(&(fc_msg_t) { SEVERE, .txt = msg_fmt, .loc = CODE_LOC }, SEVERE, CODE_LOC, ##__VA_ARGS__)
#define critical(msg_fmt, ...) fc_log(&(fc_msg_t) { CRITICAL, .txt = msg_fmt, .loc = CODE_LOC }, CRITICAL, CODE_LOC, ##__VA_ARGS__)
#define unknown(msg_fmt, ...) fc_log(&(fc_msg_t) { UNKNOWN, .txt = msg_fmt, .loc = CODE_LOC }, UNKNOWN, CODE_LOC, ##__VA_ARGS__)

// Define the macro for hooking funcs with no args and no return value
#define HOOK_FUNCTION_0_ARG_VOID(funcname)                           \
    typedef void (*funcname##_type)();                               \
    funcname##_type original_##funcname = (funcname##_type)funcname; \
    void hooked_##funcname(fc_code_loc_t loc)                      \
    {                                                                \
        trace("'" #funcname "' called at #c\n", loc);         \
        /* Additional hook logic here */                             \
        original_##funcname();                                       \
    }                                                                \
    void funcname()                                                  \
    {                                                                \
        hooked_##funcname(CODE_LOC);                            \
    }
// Define the macro for hooking funcs with no args and a return value
#define HOOK_FUNCTION_0_ARG(funcname, ret_type)                      \
    typedef ret_type (*funcname##_type)();                           \
    funcname##_type original_##funcname = (funcname##_type)funcname; \
    ret_type hooked_##funcname(fc_code_loc_t loc)                  \
    {                                                                \
        trace("'" #funcname "' called at #c\n", loc);         \
        /* Additional hook logic here */                             \
        return original_##funcname();                                \
    }                                                                \
    ret_type funcname()                                              \
    {                                                                \
        return hooked_##funcname(CODE_LOC);                     \
    }

// Define the macro for hooking funcs with one argument and no return value
#define HOOK_FUNCTION_1_ARG_VOID(funcname, arg_type)                 \
    typedef void (*funcname##_type)(arg_type);                       \
    funcname##_type original_##funcname = (funcname##_type)funcname; \
    void hooked_##funcname(arg_type arg, fc_code_loc_t loc)        \
    {                                                                \
        trace("'" #funcname "' called at #c\n", loc);         \
        /* Additional hook logic here */                             \
        original_##funcname(arg);                                    \
    }                                                                \
    void funcname(arg_type arg)                                      \
    {                                                                \
        hooked_##funcname(arg, CODE_LOC);                       \
    }
// Define the macro for hooking funcs with one argument and a return value
#define HOOK_FUNCTION_1_ARG(funcname, ret_type, arg_type)            \
    typedef ret_type (*funcname##_type)(arg_type);                   \
    funcname##_type original_##funcname = (funcname##_type)funcname; \
    inline ret_type hooked_##funcname(arg_type arg, fc_code_loc_t loc)    \
    {                                                                \
        trace("'" #funcname "' called at #c\n", loc);              \
        /* Additional hook logic here */                             \
        return original_##funcname(arg);                             \
    }                                                                \
    ret_type funcname(arg_type arg)                                  \
    {                                                                \
        return hooked_##funcname(arg, CODE_LOC);                \
    }

// Define the macro for hooking funcs with void return type and 2 args
#define HOOK_FUNCTION_2_ARG_VOID(funcname, arg_type1, arg_type2)                \
    typedef void (*funcname##_type)(arg_type1, arg_type2);                      \
    funcname##_type original_##funcname = (funcname##_type)funcname;            \
    void hooked_##funcname(arg_type1 arg1, arg_type2 arg2, fc_code_loc_t loc) \
    {                                                                           \
        trace("'" #funcname "' called at #c\n", loc);                    \
        /* Additional hook logic here */                                        \
        original_##funcname(arg1, arg2);                                        \
    }                                                                           \
    void funcname(arg_type1 arg1, arg_type2 arg2)                               \
    {                                                                           \
        hooked_##funcname(arg1, arg2, CODE_LOC);                           \
    }
// Define the macro for hooking funcs with non-void return type and 2 args
#define HOOK_FUNCTION_2_ARG(funcname, ret_type, arg_type1, arg_type2)               \
    typedef ret_type (*funcname##_type)(arg_type1, arg_type2);                      \
    funcname##_type original_##funcname = (funcname##_type)funcname;                \
    ret_type hooked_##funcname(arg_type1 arg1, arg_type2 arg2, fc_code_loc_t loc) \
    {                                                                               \
        trace("'" #funcname "' called at #c\n", loc);                        \
        /* Additional hook logic here */                                            \
        return original_##funcname(arg1, arg2);                                     \
    }                                                                               \
    ret_type funcname(arg_type1 arg1, arg_type2 arg2)                               \
    {                                                                               \
        return hooked_##funcname(arg1, arg2, CODE_LOC);                        \
    }

// Define the macro for hooking funcs with void return type and 3 args
#define HOOK_FUNCTION_3_ARG_VOID(funcname, arg_type1, arg_type2, arg_type3)                     \
    typedef void (*funcname##_type)(arg_type1, arg_type2, arg_type3);                           \
    funcname##_type original_##funcname = (funcname##_type)funcname;                            \
    void hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, fc_code_loc_t loc) \
    {                                                                                           \
        trace("'" #funcname "' called at #c\n", loc);                                    \
        /* Additional hook logic here */                                                        \
        original_##funcname(arg1, arg2, arg3);                                                  \
    }                                                                                           \
    void funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3)                               \
    {                                                                                           \
        hooked_##funcname(arg1, arg2, arg3, CODE_LOC);                                     \
    }
// Define the macro for hooking funcs with non-void return type and 3 args
#define HOOK_FUNCTION_3_ARG(funcname, ret_type, arg_type1, arg_type2, arg_type3)                    \
    typedef ret_type (*funcname##_type)(arg_type1, arg_type2, arg_type3);                           \
    funcname##_type original_##funcname = (funcname##_type)funcname;                                \
    ret_type hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, fc_code_loc_t loc) \
    {                                                                                               \
        trace("'" #funcname "' called at #c\n", loc);                                        \
        /* Additional hook logic here */                                                            \
        return original_##funcname(arg1, arg2, arg3);                                               \
    }                                                                                               \
    ret_type funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3)                               \
    {                                                                                               \
        return hooked_##funcname(arg1, arg2, arg3, CODE_LOC);                                  \
    }

// Define the macro for hooking funcs with void return type and 4 args
#define HOOK_FUNCTION_4_ARG_VOID(funcname, arg_type1, arg_type2, arg_type3, arg_type4)                          \
    typedef void (*funcname##_type)(arg_type1, arg_type2, arg_type3, arg_type4);                                \
    funcname##_type original_##funcname = (funcname##_type)funcname;                                            \
    void hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, fc_code_loc_t loc) \
    {                                                                                                           \
        trace("'" #funcname "' called at #c\n", loc);                                                    \
        /* Additional hook logic here */                                                                        \
        original_##funcname(arg1, arg2, arg3, arg4);                                                            \
    }                                                                                                           \
    void funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4)                               \
    {                                                                                                           \
        hooked_##funcname(arg1, arg2, arg3, arg4, CODE_LOC);                                               \
    }
// Define the macro for hooking funcs with non-void return type and 4 args
#define HOOK_FUNCTION_4_ARG(funcname, ret_type, arg_type1, arg_type2, arg_type3, arg_type4)                         \
    typedef ret_type (*funcname##_type)(arg_type1, arg_type2, arg_type3, arg_type4);                                \
    funcname##_type original_##funcname = (funcname##_type)funcname;                                                \
    ret_type hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, fc_code_loc_t loc) \
    {                                                                                                               \
        trace("'" #funcname "' called at #c\n", loc);                                                        \
        /* Additional hook logic here */                                                                            \
        return original_##funcname(arg1, arg2, arg3, arg4);                                                         \
    }                                                                                                               \
    ret_type funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4)                               \
    {                                                                                                               \
        return hooked_##funcname(arg1, arg2, arg3, arg4, CODE_LOC);                                            \
    }

// Define the macro for hooking funcs with void return type and 5 args
#define HOOK_FUNCTION_5_ARG_VOID(funcname, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5)                               \
    typedef void (*funcname##_type)(arg_type1, arg_type2, arg_type3, arg_type4, arg_type5);                                     \
    funcname##_type original_##funcname = (funcname##_type)funcname;                                                            \
    void hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, arg_type5 arg5, fc_code_loc_t loc) \
    {                                                                                                                           \
        trace("'" #funcname "' called at #c\n", loc);                                                                    \
        /* Additional hook logic here */                                                                                        \
        original_##funcname(arg1, arg2, arg3, arg4, arg5);                                                                      \
    }                                                                                                                           \
    void funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, arg_type5 arg5)                               \
    {                                                                                                                           \
        hooked_##funcname(arg1, arg2, arg3, arg4, arg5, CODE_LOC);                                                         \
    }
// Define the macro for hooking funcs with non-void return type and 5 args
#define HOOK_FUNCTION_5_ARG(funcname, ret_type, arg_type1, arg_type2, arg_type3, arg_type4, arg_type5)                              \
    typedef ret_type (*funcname##_type)(arg_type1, arg_type2, arg_type3, arg_type4, arg_type5);                                     \
    funcname##_type original_##funcname = (funcname##_type)funcname;                                                                \
    ret_type hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, arg_type5 arg5, fc_code_loc_t loc) \
    {                                                                                                                               \
        trace("'" #funcname "' called at #c\n", loc);                                                                        \
        /* Additional hook logic here */                                                                                            \
        return original_##funcname(arg1, arg2, arg3, arg4, arg5);                                                                   \
    }                                                                                                                               \
    ret_type funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, arg_type5 arg5)                               \
    {                                                                                                                               \
        return hooked_##funcname(arg1, arg2, arg3, arg4, arg5, CODE_LOC);                                                      \
    }

#endif // LOG_H__
