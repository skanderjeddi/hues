/**
 * @file hues.h
 * @brief A flexible logging system with customizable log levels, color formatting and more.
 */

#ifndef HUES_H__
#define HUES_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

/**
 * @struct hues_color
 * @brief A structure representing an RGB color.
 */
typedef struct {
    uint8_t r;  /**< Red component of the color. */
    uint8_t g;  /**< Green component of the color. */
    uint8_t b;  /**< Blue component of the color. */
} hues_color;

/**
 * @fn extern hues_color hues_hex_to_color(uint32_t hex)
 * @brief Converts a hexadecimal color value to an RGB color.
 * @param hex The hexadecimal color value to be converted.
 * @return The RGB color.
 */
extern hues_color hues_hex_to_color(uint32_t hex);

/**
 * @fn extern void hues_color_to_hex(char* hex, hues_color* color)
 * @brief Converts an RGB color to a hexadecimal color value.
 * @param hex The output hexadecimal color value.
 * @param color The RGB color to be converted.
 */
extern void hues_color_to_hex(char* hex, hues_color* color);

/**
 * @struct hues_code_location
 * @brief Represents a location in the code (file, function, and line).
 */
typedef struct {
    const char* file;  /**< File name. */
    const char* method_name;  /**< Function name. */
    size_t line;  /**< Line number. */
} hues_code_location;

/**
 * @enum hues_level_enum
 * @brief Enumerates logging levels.
 */
typedef enum {
    HUES_LEVEL_TRACE = 0,  /**< Trace level logging. */
    HUES_LEVEL_DEBUG = 1,  /**< Debug level logging. */
    HUES_LEVEL_INFO = 2,  /**< Info level logging. */
    HUES_LEVEL_WARN = 3,  /**< Warning level logging. */
    HUES_LEVEL_SEVERE = 4,  /**< Severe level logging. */
    HUES_LEVEL_CRITICAL = 5,  /**< Critical level logging. */
    HUES_LEVEL_UNKNOWN = 6,  /**< Unknown level logging. */
} hues_level_enum;

/**
 * @struct hues_level
 * @brief Represents a logging level and its string representation.
 */
typedef struct {
    hues_level_enum level;  /**< Log level. */
    const char* name;  /**< Log level name. */
} hues_level;

/**
 * @struct hues_level_format
 * @brief Represents a logging level, foreground and background colors for that level.
 */
typedef struct {
    hues_level_enum level;  /**< Log level. */
    hues_color background_color;  /**< Background color for this level. */
    hues_color foreground_color;  /**< Foreground color for this level. */
} hues_level_format;

/**
 * @struct hues_theme
 * @brief Represents a theme for log levels.
 */
typedef struct {
    hues_level_format* format;  /**< Format for the level. */
} hues_theme;

/**
 * @struct hues_message
 * @brief Represents a log message with level, text, and location.
 */
typedef struct {
    hues_level level;  /**< Log level. */
    const char* contents;  /**< Log message. */
    hues_code_location location;  /**< Code location of the log message. */
} hues_message;

/**
 * @typedef size_t (*hues_format_function)(char* buff, size_t buffsz, char specifier, va_list args)
 * @brief Represents a function that formats a log message.
 */
typedef size_t (*hues_format_function)(char* buff, size_t buffsz, char specifier, va_list args);

/**
 * @struct hues_format
 * @brief Represents a log message format.
 */
typedef struct {
    char* specifier;  /**< Format specifier. */
    hues_format_function format_function;  /**< Function to format the log message. */
} hues_format;

/**
 * @struct hues_configuration
 * @brief Represents a logging configuration.
 */
typedef struct {
    hues_level_enum minimum_level;  /**< Minimum log level. */
    char* header_format;  /**< Level format string. */
    hues_format** formats;  /**< Log message formats. */
    char prefix;  /**< Prefix character. */
    hues_theme* theme;  /**< Logging theme. */
    size_t levels_count;  /**< Number of log levels. */
} hues_configuration;

/**
 * @fn hues_level_enum hues_configuration_get_minimum_level()
 * @brief Retrieves the minimum log level from the logging configuration.
 * @return The minimum log level.
 */
hues_level_enum hues_configuration_get_minimum_level();

/**
 * @fn void hues_configuration_set_minimum_level(hues_level_enum minimum_level)
 * @brief Sets the minimum log level in the logging configuration.
 * @param minimum_level The new minimum log level.
 */
void hues_configuration_set_minimum_level(hues_level_enum minimum_level);

/**
 * @fn char* hues_conf_get_level_format()
 * @brief Retrieves the current level format string from the logging configuration.
 * @return A pointer to the level format string.
 */
char* hues_configuration_get_level_format();

/**
 * @fn void hues_configuration_set_level_format(const char* header_format)
 * @brief Sets the level format string in the logging configurationiguration.
 * @param header_format The new level format string.
 */
void hues_configuration_set_level_format(const char* header_format);

/**
 * @fn hues_format** hues_configuration_get_formats()
 * @brief Retrieves the log message formats from the logging configurationiguration.
 * @return A pointer to the array of log message formats.
 */
hues_format** hues_configuration_get_formats();

/**
 * @fn void hues_configuration_set_formats(hues_format** formats)
 * @brief Sets the log message formats in the logging configurationiguration.
 * @param formats A pointer to the array of new log message formats.
 */
void hues_configuration_set_formats(hues_format** formats);

/**
 * @fn char hues_configuration_get_prefix()
 * @brief Retrieves the prefix character from the logging configurationiguration.
 * @return The prefix character.
 */
char hues_configuration_get_prefix();

/**
 * @fn void hues_configuration_set_prefix(char prefix)
 * @brief Sets the prefix character in the logging configurationiguration.
 * @param prefix The new prefix character.
 */
void hues_configuration_set_prefix(char prefix);

/**
 * @fn hues_theme* hues_configuration_get_theme()
 * @brief Retrieves the logging theme from the logging configurationiguration.
 * @return A pointer to the logging theme.
 */
hues_theme* hues_configuration_get_theme();

/**
 * @fn void hues_configuration_set_theme(hues_theme* theme)
 * @brief Sets the logging theme in the logging configurationiguration.
 * @param theme A pointer to the new logging theme.
 */
void hues_configuration_set_theme(hues_theme* theme);

/**
 * @fn void hues_configuration_add_format(hues_format* format)
 * @brief Adds a log message format to the logging configurationiguration.
 * @param format A pointer to the new log message format.
 */
void hues_configuration_add_format(hues_format* format);

/**
 * @fn extern void hues_theme_from_hex(uint32_t* bg_hex, uint32_t* fg_hex)
 * @brief Converts hexadecimal color values to an RGB theme.
 * @param bg_hex A pointer to the background color's hexadecimal value.
 * @param fg_hex A pointer to the foreground color's hexadecimal value.
 */
extern void hues_theme_from_hex(uint32_t* bg_hex, uint32_t* fg_hex);

/**
 * @fn extern size_t hues_format(char* buff, size_t buffsz, const char* format, ...)
 * @brief Formats a log message.
 * @param buff A buffer to store the formatted log message.
 * @param buffsz The size of the buffer.
 * @param format The format string for the log message.
 * @param ... Additional arguments used with the format string.
 * @return The number of characters in the formatted log message.
 */
extern size_t hues_format_c(char* buff, size_t buffsz, const char* format, ...);

/**
 * @fn extern size_t hues_format_p(char* buff, size_t buffsz, const char* format, ...)
 * @brief Formats a log message with a pointer argument.
 * @param buff A buffer to store the formatted log message.
 * @param buffsz The size of the buffer.
 * @param format The format string for the log message.
 * @param ... Additional arguments used with the format string.
 * @return The number of characters in the formatted log message.
 */
extern size_t hues_format_p(char* buff, size_t buffsz, const char* format, ...);

/**
 * @fn extern void hues_log(hues_message* contents, ...)
 * @brief Logs a message.
 * @param contents A pointer to the log message.
 * @param ... Additional arguments used with the log message.
 */
extern void hues_log(hues_message* contents, ...);

/**
 * @fn extern void hues_initialize()
 * @brief Initializes the logging system.
 */
extern void hues_initialize();

/**
 * @def BUFFER_SIZE 4096
 * @brief Buffer size for logging messages.
 */
#define BUFFER_SIZE 4096

/**
 * @def CODE_LOC (hues_code_location) { __FILE__, __func__, __LINE__ }
 * @brief Macro to generate a code location.
 */
#define CODE_LOC (hues_code_location) { __FILE__, __func__, __LINE__ }

#define ESC_SEQ_BG "\x1b[48;2;%d;%d;%dm"
#define ESC_SEQ_FG "\x1b[38;2;%d;%d;%dm"
#define ESC_SEQ_RST "\x1b[0m"

#define TRACE (hues_level) { .level = HUES_LEVEL_TRACE, .name = "TRACE" }
#define DEBUG (hues_level) { .level = HUES_LEVEL_DEBUG, .name = "DEBUG" }
#define INFO (hues_level) { .level = HUES_LEVEL_INFO, .name = "INFO" }
#define WARN (hues_level) { .level = HUES_LEVEL_WARN, .name = "WARN" }
#define SEVERE (hues_level) { .level = HUES_LEVEL_SEVERE, .name = "SEVERE" }
#define CRITICAL (hues_level) { .level = HUES_LEVEL_CRITICAL, .name = "CRITICAL" }
#define UNKNOWN (hues_level) { .level = HUES_LEVEL_UNKNOWN, .name = "???" }

/**
 * @def trace(message_format, ...)
 * @brief Logs a message at the TRACE level.
 * @param message_format Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define trace(message_format, ...) hues_log(&(hues_message) { TRACE, .contents = message_format, .location = CODE_LOC }, TRACE, CODE_LOC, ##__VA_ARGS__)

/**
 * @def debug(message_format, ...)
 * @brief Logs a message at the DEBUG level.
 * @param message_format Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define debug(message_format, ...) hues_log(&(hues_message) { DEBUG, .contents = message_format, .location = CODE_LOC }, DEBUG, CODE_LOC, ##__VA_ARGS__)

/**
 * @def info(message_format, ...)
 * @brief Logs a message at the INFO level.
 * @param message_format Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define info(message_format, ...) hues_log(&(hues_message) { INFO, .contents = message_format, .location = CODE_LOC }, INFO, CODE_LOC, ##__VA_ARGS__)

/**
 * @def warn(message_format, ...)
 * @brief Logs a message at the WARN level.
 * @param message_format Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define warn(message_format, ...) hues_log(&(hues_message) { WARN, .contents = message_format, .location = CODE_LOC }, WARN, CODE_LOC, ##__VA_ARGS__)

/**
 * @def severe(message_format, ...)
 * @brief Logs a message at the SEVERE level.
 * @param message_format Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define severe(message_format, ...) hues_log(&(hues_message) { SEVERE, .contents = message_format, .location = CODE_LOC }, SEVERE, CODE_LOC, ##__VA_ARGS__)

/**
 * @def critical(message_format, ...)
 * @brief Logs a message at the CRITICAL level.
 * @param message_format Format string for the log message.
 * @param ... Additional arguments used with the format string.
 */
#define critical(message_format, ...) hues_log(&(hues_message) { CRITICAL, .contents = message_format, .location = CODE_LOC }, CRITICAL, CODE_LOC, ##__VA_ARGS__)

// Define the macro for hooking funcs with no args and no return value
#define HOOK_FUNCTION_0_ARG_VOID(funcname)                           \
    typedef void (*funcname##_type)();                               \
    funcname##_type original_##funcname = (funcname##_type)funcname; \
    void hooked_##funcname(hues_code_location location)                      \
    {                                                                \
        trace("'" #funcname "' called at #c\n", location);         \
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
    ret_type hooked_##funcname(hues_code_location location)                  \
    {                                                                \
        trace("'" #funcname "' called at #c\n", location);         \
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
    void hooked_##funcname(arg_type arg, hues_code_location location)        \
    {                                                                \
        trace("'" #funcname "' called at #c\n", location);         \
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
    inline ret_type hooked_##funcname(arg_type arg, hues_code_location location)    \
    {                                                                \
        trace("'" #funcname "' called at #c\n", location);              \
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
    void hooked_##funcname(arg_type1 arg1, arg_type2 arg2, hues_code_location location) \
    {                                                                           \
        trace("'" #funcname "' called at #c\n", location);                    \
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
    ret_type hooked_##funcname(arg_type1 arg1, arg_type2 arg2, hues_code_location location) \
    {                                                                               \
        trace("'" #funcname "' called at #c\n", location);                        \
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
    void hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, hues_code_location location) \
    {                                                                                           \
        trace("'" #funcname "' called at #c\n", location);                                    \
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
    ret_type hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, hues_code_location location) \
    {                                                                                               \
        trace("'" #funcname "' called at #c\n", location);                                        \
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
    void hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, hues_code_location location) \
    {                                                                                                           \
        trace("'" #funcname "' called at #c\n", location);                                                    \
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
    ret_type hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, hues_code_location location) \
    {                                                                                                               \
        trace("'" #funcname "' called at #c\n", location);                                                        \
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
    void hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, arg_type5 arg5, hues_code_location location) \
    {                                                                                                                           \
        trace("'" #funcname "' called at #c\n", location);                                                                    \
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
    ret_type hooked_##funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, arg_type5 arg5, hues_code_location location) \
    {                                                                                                                               \
        trace("'" #funcname "' called at #c\n", location);                                                                        \
        /* Additional hook logic here */                                                                                            \
        return original_##funcname(arg1, arg2, arg3, arg4, arg5);                                                                   \
    }                                                                                                                               \
    ret_type funcname(arg_type1 arg1, arg_type2 arg2, arg_type3 arg3, arg_type4 arg4, arg_type5 arg5)                               \
    {                                                                                                                               \
        return hooked_##funcname(arg1, arg2, arg3, arg4, arg5, CODE_LOC);                                                      \
    }

#endif // LOG_H__
