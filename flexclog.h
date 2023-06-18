/**
 * @file flexclog.h
 * @brief A flexible logging system with customizable log levels, color formatting and more.
 */

#ifndef FLEXCLOG_H__
#define FLEXCLOG_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

/**
 * @struct fc_color_t
 * @brief A structure representing an RGB color.
 */
typedef struct {
    uint8_t r;  /**< Red component of the color. */
    uint8_t g;  /**< Green component of the color. */
    uint8_t b;  /**< Blue component of the color. */
} fc_color_t;

/**
 * @fn extern fc_color_t fc_hex_to_clr(uint32_t hex)
 * @brief Converts a hexadecimal color value to an RGB color.
 * @param hex The hexadecimal color value to be converted.
 * @return The RGB color.
 */
extern fc_color_t fc_hex_to_clr(uint32_t hex);

/**
 * @fn extern void fc_clr_to_hex(char* hex, fc_color_t* color)
 * @brief Converts an RGB color to a hexadecimal color value.
 * @param hex The output hexadecimal color value.
 * @param color The RGB color to be converted.
 */
extern void fc_clr_to_hex(char* hex, fc_color_t* color);

/**
 * @struct fc_code_loc_t
 * @brief Represents a location in the code (file, function, and line).
 */
typedef struct {
    const char* file;  /**< File name. */
    const char* func;  /**< Function name. */
    size_t line;  /**< Line number. */
} fc_code_loc_t;

/**
 * @enum fc_lvl_enum_t
 * @brief Enumerates logging levels.
 */
typedef enum {
    FC_LVL_TRC = 0,  /**< Trace level logging. */
    FC_LVL_DBG = 1,  /**< Debug level logging. */
    FC_LVL_INF = 2,  /**< Info level logging. */
    FC_LVL_WRN = 3,  /**< Warning level logging. */
    FC_LVL_SVR = 4,  /**< Severe level logging. */
    FC_LVL_CRT = 5,  /**< Critical level logging. */
    FC_LVL_UNK = 6,  /**< Unknown level logging. */
} fc_lvl_enum_t;

/**
 * @struct fc_lvl_t
 * @brief Represents a logging level and its string representation.
 */
typedef struct {
    fc_lvl_enum_t lvl;  /**< Log level. */
    const char* name;  /**< Log level name. */
} fc_lvl_t;

/**
 * @struct fc_lvl_fmt_t
 * @brief Represents a logging level, foreground and background colors for that level.
 */
typedef struct {
    fc_lvl_enum_t lvl;  /**< Log level. */
    fc_color_t bg_clr;  /**< Background color for this level. */
    fc_color_t fg_clr;  /**< Foreground color for this level. */
} fc_lvl_fmt_t;

/**
 * @struct fc_thm_t
 * @brief Represents a theme for log levels.
 */
typedef struct {
    fc_lvl_fmt_t* fmt;  /**< Format for the level. */
} fc_thm_t;

/**
 * @struct fc_msg_t
 * @brief Represents a log message with level, text, and location.
 */
typedef struct {
    fc_lvl_t lvl;  /**< Log level. */
    const char* txt;  /**< Log message. */
    fc_code_loc_t loc;  /**< Code location of the log message. */
} fc_msg_t;

/**
 * @typedef size_t (*fc_fm_func_t)(char* buff, size_t buffsz, char spec, va_list args)
 * @brief Represents a function that formats a log message.
 */
typedef size_t (*fc_fm_func_t)(char* buff, size_t buffsz, char spec, va_list args);

/**
 * @struct fc_fmt_t
 * @brief Represents a log message format.
 */
typedef struct {
    char* spec;  /**< Format specifier. */
    fc_fm_func_t fmt_func;  /**< Function to format the log message. */
} fc_fmt_t;

/**
 * @struct fc_conf_t
 * @brief Represents a logging configuration.
 */
typedef struct {
    fc_lvl_enum_t min_lvl;  /**< Minimum log level. */
    char* lvl_fmt;  /**< Level format string. */
    fc_fmt_t** fmts;  /**< Log message formats. */
    char pref;  /**< Prefix character. */
    fc_thm_t* thm;  /**< Logging theme. */
    size_t lvls;  /**< Number of log levels. */
} fc_conf_t;

/**
 * @fn fc_lvl_enum_t fc_conf_get_minimum_lvl()
 * @brief Retrieves the minimum log level from the logging configuration.
 * @return The minimum log level.
 */
fc_lvl_enum_t fc_conf_get_minimum_lvl();

/**
 * @fn void conf_set_minimum_lvl(fc_lvl_enum_t min_lvl)
 * @brief Sets the minimum log level in the logging configuration.
 * @param min_lvl The new minimum log level.
 */
void conf_set_minimum_lvl(fc_lvl_enum_t min_lvl);

/**
 * @fn char* fc_conf_get_lvl_fmt()
 * @brief Retrieves the current level format string from the logging configuration.
 * @return A pointer to the level format string.
 */
char* fc_conf_get_lvl_fmt();

/**
 * @fn void fc_conf_set_lvl_fmt(const char* lvl_fmt)
 * @brief Sets the level format string in the logging configuration.
 * @param lvl_fmt The new level format string.
 */
void fc_conf_set_lvl_fmt(const char* lvl_fmt);

/**
 * @fn fc_fmt_t** fc_conf_get_fmts()
 * @brief Retrieves the log message formats from the logging configuration.
 * @return A pointer to the array of log message formats.
 */
fc_fmt_t** fc_conf_get_fmts();

/**
 * @fn void fc_conf_set_fmts(fc_fmt_t** fmts)
 * @brief Sets the log message formats in the logging configuration.
 * @param fmts A pointer to the array of new log message formats.
 */
void fc_conf_set_fmts(fc_fmt_t** fmts);

/**
 * @fn char fc_conf_get_prefix()
 * @brief Retrieves the prefix character from the logging configuration.
 * @return The prefix character.
 */
char fc_conf_get_prefix();

/**
 * @fn void fc_conf_set_prefix(char pref)
 * @brief Sets the prefix character in the logging configuration.
 * @param pref The new prefix character.
 */
void fc_conf_set_prefix(char pref);

/**
 * @fn fc_thm_t* fc_conf_get_thm()
 * @brief Retrieves the logging theme from the logging configuration.
 * @return A pointer to the logging theme.
 */
fc_thm_t* fc_conf_get_thm();

/**
 * @fn void fc_conf_set_thm(fc_thm_t* thm)
 * @brief Sets the logging theme in the logging configuration.
 * @param thm A pointer to the new logging theme.
 */
void fc_conf_set_thm(fc_thm_t* thm);

/**
 * @fn void fc_conf_add_fmt(fc_fmt_t* fmt)
 * @brief Adds a log message format to the logging configuration.
 * @param fmt A pointer to the new log message format.
 */
void fc_conf_add_fmt(fc_fmt_t* fmt);

/**
 * @fn extern void fc_thm_from_hex(uint32_t* bg_hex, uint32_t* fg_hex)
 * @brief Converts hexadecimal color values to an RGB theme.
 * @param bg_hex A pointer to the background color's hexadecimal value.
 * @param fg_hex A pointer to the foreground color's hexadecimal value.
 */
extern void fc_thm_from_hex(uint32_t* bg_hex, uint32_t* fg_hex);

/**
 * @fn extern size_t fc_fmt(char* buff, size_t buffsz, const char* fmt, ...)
 * @brief Formats a log message.
 * @param buff A buffer to store the formatted log message.
 * @param buffsz The size of the buffer.
 * @param fmt The format string for the log message.
 * @param ... Additional arguments used with the format string.
 * @return The number of characters in the formatted log message.
 */
extern size_t fc_fmt(char* buff, size_t buffsz, const char* fmt, ...);

/**
 * @fn extern size_t fc_fmt_p(char* buff, size_t buffsz, const char* fmt, ...)
 * @brief Formats a log message with a pointer argument.
 * @param buff A buffer to store the formatted log message.
 * @param buffsz The size of the buffer.
 * @param fmt The format string for the log message.
 * @param ... Additional arguments used with the format string.
 * @return The number of characters in the formatted log message.
 */
extern size_t fc_fmt_p(char* buff, size_t buffsz, const char* fmt, ...);

/**
 * @fn extern void fc_log(fc_msg_t* txt, ...)
 * @brief Logs a message.
 * @param txt A pointer to the log message.
 * @param ... Additional arguments used with the log message.
 */
extern void fc_log(fc_msg_t* txt, ...);

/**
 * @fn extern void fc_init()
 * @brief Initializes the logging system.
 */
extern void fc_init();

/**
 * @def BUFFER_SIZE 4096
 * @brief Buffer size for logging messages.
 */
#define BUFFER_SIZE 4096

/**
 * @def CODE_LOC (fc_code_loc_t) { __FILE__, __func__, __LINE__ }
 * @brief Macro to generate a code location.
 */
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

/**
 * @def trace(msg_fmt, ...)
 * @brief Logs a message at the TRACE level.
 * @param msg_fmt Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define trace(msg_fmt, ...) fc_log(&(fc_msg_t) { TRACE, .txt = msg_fmt, .loc = CODE_LOC }, TRACE, CODE_LOC, ##__VA_ARGS__)

/**
 * @def debug(msg_fmt, ...)
 * @brief Logs a message at the DEBUG level.
 * @param msg_fmt Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define debug(msg_fmt, ...) fc_log(&(fc_msg_t) { DEBUG, .txt = msg_fmt, .loc = CODE_LOC }, DEBUG, CODE_LOC, ##__VA_ARGS__)

/**
 * @def info(msg_fmt, ...)
 * @brief Logs a message at the INFO level.
 * @param msg_fmt Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define info(msg_fmt, ...) fc_log(&(fc_msg_t) { INFO, .txt = msg_fmt, .loc = CODE_LOC }, INFO, CODE_LOC, ##__VA_ARGS__)

/**
 * @def warn(msg_fmt, ...)
 * @brief Logs a message at the WARN level.
 * @param msg_fmt Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define warn(msg_fmt, ...) fc_log(&(fc_msg_t) { WARN, .txt = msg_fmt, .loc = CODE_LOC }, WARN, CODE_LOC, ##__VA_ARGS__)

/**
 * @def severe(msg_fmt, ...)
 * @brief Logs a message at the SEVERE level.
 * @param msg_fmt Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define severe(msg_fmt, ...) fc_log(&(fc_msg_t) { SEVERE, .txt = msg_fmt, .loc = CODE_LOC }, SEVERE, CODE_LOC, ##__VA_ARGS__)

/**
 * @def critical(msg_fmt, ...)
 * @brief Logs a message at the CRITICAL level.
 * @param msg_fmt Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define critical(msg_fmt, ...) fc_log(&(fc_msg_t) { CRITICAL, .txt = msg_fmt, .loc = CODE_LOC }, CRITICAL, CODE_LOC, ##__VA_ARGS__)

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
