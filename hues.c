/**
 * @file hues.h
 * @brief hues library for flexible and colorful logging
 */

#include "hues.h"

/**
 * @fn static void hues_log_message_v(hues_message* message, va_list list)
 * @brief Logs a formatted message.
 * @param message A pointer to the message to log.
 * @param list A list of arguments to use in the to_format string.
 */
static void hues_log_message_v(hues_message* message, va_list list);

/**
 * @fn static size_t hues_format_pv_core(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list)
 * @brief Formats a log message with a pointer argument using a va_list.
 * @param buffer A buffer to store the formatted log message.
 * @param buffer_size The size of the buffer.
 * @param prefix The prefix character for the to_format string.
 * @param formats A pointer to the array of log message formats.
 * @param to_format The to_format string for the log message.
 * @param list A list of arguments to use in the to_format string.
 * @return The number of characters in the formatted log message.
 */
static size_t hues_format_pv_core(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list);

/**
 * @fn static size_t hues_format_cv_core(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list)
 * @brief Formats a log message using a va_list.
 * @param buffer A buffer to store the formatted log message.
 * @param buffer_size The size of the buffer.
 * @param prefix The prefix character for the to_format string.
 * @param formats A pointer to the array of log message formats.
 * @param to_format The to_format string for the log message.
 * @param list A list of arguments to use in the to_format string.
 * @return The number of characters in the formatted log message.
 */
static size_t hues_format_cv_core(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list);

static hues_configuration hues_glob_configuration = { 
    .minimum_level = HUES_LEVEL_DEBUG,
    .header_format = "#t/#d #v\t",
    .prefix = '#',
    .theme = NULL,
    .levels_count = HUES_LEVEL_UNKNOWN + 1,
    .formats = NULL
};

char* hues_configuration_get_level_format() {
    return hues_glob_configuration.header_format;
}

void hues_configuration_set_level_format(const char* header_format) {
    hues_glob_configuration.header_format = strdup(header_format);
}

hues_level_enum hues_configuration_get_minimum_level() {
    return hues_glob_configuration.minimum_level;
}

void hues_configuration_set_minimum_level(hues_level_enum minimum_level) {
    hues_glob_configuration.minimum_level = minimum_level;
}

char hues_configuration_get_prefix() {
    return hues_glob_configuration.prefix;
}

void hues_configuration_set_prefix(char prefix) {
    hues_glob_configuration.prefix = prefix;
}

hues_theme* hues_configuration_get_theme() {
    return hues_glob_configuration.theme;
}

void hues_configuration_set_theme(hues_theme* theme) {
    hues_glob_configuration.theme = theme;
}

hues_format** hues_configuration_get_formats() {
    return hues_glob_configuration.formats;
}

void hues_configuration_set_formats(hues_format** formats) {
    hues_glob_configuration.formats = formats;
}

void hues_configuration_add_format(hues_format* format) {
    if (hues_glob_configuration.formats == NULL) {
        hues_glob_configuration.formats = malloc(sizeof(hues_format*) * 2);
        hues_glob_configuration.formats[0] = format;
        hues_glob_configuration.formats[1] = NULL;
    } else {
        size_t formats_count = 0;
        for (size_t i = 0; hues_glob_configuration.formats[i] != NULL; i++) {
            formats_count++;
        }
        hues_glob_configuration.formats = realloc(hues_glob_configuration.formats, sizeof(hues_format*) * (formats_count + 2));
        hues_glob_configuration.formats[formats_count] = format;
        hues_glob_configuration.formats[formats_count + 1] = NULL;
    }
}

const hues_color hues_hex_to_color(uint32_t hex) {
    hues_color clr;
    clr.r = (hex >> 16) & 0xFF;
    clr.g = (hex >> 8) & 0xFF;
    clr.b = hex & 0xFF;
    return clr;
}

void hues_color_to_hex(char* hex, hues_color* clr) {
    sprintf(hex, "#%02x%02x%02x", clr->r, clr->g, clr->b);
}

void hues_theme_from_hex(uint32_t* bg_hex, uint32_t* fg_hex) {
    hues_glob_configuration.theme = malloc(sizeof(hues_theme));
    hues_glob_configuration.theme->format = malloc(sizeof(hues_level_format) * hues_glob_configuration.levels_count);
    for (size_t i = 0; i < HUES_LEVEL_UNKNOWN + 1; i++) {
        hues_glob_configuration.theme->format[i].level = i;
        hues_glob_configuration.theme->format[i].background_color = hues_hex_to_color(bg_hex[i]);
        hues_glob_configuration.theme->format[i].foreground_color = hues_hex_to_color(fg_hex[i]);
    }
}

/**
 * @fn static size_t hues_format_v(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list)
 * @brief Formats a log message using a va_list.
 * @param buffer A buffer to store the formatted log message.
 * @param buffer_size The size of the buffer.
 * @param prefix The prefix character for the to_format string.
 * @param formats A pointer to the array of log message formats.
 * @param to_format The to_format string for the log message.
 * @param list A list of arguments to use in the to_format string.
 * @return The number of characters in the formatted log message.
 */
static size_t hues_format_cv(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list) {
    return hues_format_cv_core(buffer, buffer_size, prefix, formats, to_format, list);
}

/**
 * @fn size_t hues_format(char* buffer, size_t buffer_size, const char* to_format, ...)
 * @brief Formats a log message with a variable number of arguments.
 * @param buffer A buffer to store the formatted log message.
 * @param buffer_size The size of the buffer.
 * @param to_format The to_format string for the log message.
 * @return The number of characters in the formatted log message.
 */
size_t hues_format_c(char* buffer, size_t buffer_size, const char* to_format, ...) {
    va_list list;
    va_start(list, to_format);
    size_t written = hues_format_cv(buffer, buffer_size, hues_glob_configuration.prefix, hues_glob_configuration.formats, to_format, list);
    va_end(list);
    return written;
}

/**
 * @fn static size_t hues_format_cv_core(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list)
 * @brief Formats a log message with a pointer argument using a va_list.
 * @param buffer A buffer to store the formatted log message.
 * @param buffer_size The size of the buffer.
 * @param prefix The prefix character for the to_format string.
 * @param formats A pointer to the array of log message formats.
 * @param to_format The to_format string for the log message.
 * @param list A list of arguments to use in the to_format string.
 * @return The number of characters in the formatted log message.
 */
static size_t hues_format_cv_core(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list) {
    char* buffptr = buffer;
    char* buffend = buffer + buffer_size;
    const char* format_end = to_format + strlen(to_format);
    size_t max_spec_len = 3;
    while (to_format < format_end) {
        if (*to_format == prefix) {
            hues_format* format_spec = NULL;
            size_t spec_len = 1;
            for (size_t length = max_spec_len; length >= 1; length--) {
                for (size_t i = 0; formats[i] != NULL; i++) {
                    if (strncmp(to_format + 1, formats[i]->specifier, length) == 0) {
                        format_spec = formats[i];
                        spec_len = length;
                        break;
                    }
                }
                if (format_spec != NULL)
                    break;
            }
            if (format_spec == NULL) {
                for (size_t i = 0; i < spec_len; i++) {
                    if (buffptr < buffend) {
                        *buffptr = to_format[i];
                        buffptr++;
                    }
                }
            } else {
                buffptr += format_spec->format_function(buffptr, buffend - buffptr, to_format[1], list);
            }
            to_format += spec_len + 1;
        } else {
            if (buffptr < buffend) {
                *buffptr = *to_format;
                buffptr++;
            }
            to_format++;
        }
    }
    if (buffptr < buffend) {
        *buffptr = '\0';
    }
    return buffptr - buffer;
}

/**
 * @fn static size_t hues_format_pv(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list)
 * @brief Formats a log message with a pointer argument using a va_list.
 * @param buffer A buffer to store the formatted log message.
 * @param buffer_size The size of the buffer.
 * @param prefix The prefix character for the to_format string.
 * @param formats A pointer to the array of log message formats.
 * @param to_format The to_format string for the log message.
 * @param list A list of arguments to use in the to_format string.
 * @return The number of characters in the formatted log message.
 */
static size_t hues_format_pv(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list) {
    return hues_format_pv_core(buffer, buffer_size, prefix, formats, to_format, list);
}

/**
 * @fn size_t hues_format_p(char* buffer, size_t buffer_size, const char* to_format, ...)
 * @brief Formats a log message with a variable number of arguments.
 * @param buffer A buffer to store the formatted log message.
 * @param buffer_size The size of the buffer.
 * @param to_format The to_format string for the log message.
 * @return The number of characters in the formatted log message.
 */
size_t hues_format_p(char* buffer, size_t buffer_size, const char* to_format, ...) {
    va_list list;
    va_start(list, to_format);
    size_t written = hues_format_pv(buffer, buffer_size, hues_glob_configuration.prefix, hues_glob_configuration.formats, to_format, list);
    va_end(list);
    return written;
}

/**
 * @fn static size_t hues_format_pv_core(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list)
 * @brief Formats a log message with a pointer argument using a va_list.
 * @param buffer A buffer to store the formatted log message.
 * @param buffer_size The size of the buffer.
 * @param prefix The prefix character for the to_format string.
 * @param formats A pointer to the array of log message formats.
 * @param to_format The to_format string for the log message.
 * @param list A list of arguments to use in the to_format string.
 * @return The number of characters in the formatted log message.
 */
static size_t hues_format_pv_core(char* buffer, size_t buffer_size, char prefix, hues_format** formats, const char* to_format, va_list list) {
    char* buffptr = buffer;
    char* buffend = buffer + buffer_size - 1; // Reserve space for null terminator
    const char* format_ptr = to_format;
    const char* format_end = to_format + strlen(to_format);
    size_t max_spec_len = 3;
    while (format_ptr < format_end) {
        if (*format_ptr == prefix) {
            hues_format* format_spec = NULL;
            size_t spec_len = 0;
            for (size_t length = max_spec_len; length > 0; length--) {
                for (size_t i = 0; formats[i] != NULL; i++) {
                    if (strncmp(format_ptr + 1, formats[i]->specifier, length) == 0) {
                        format_spec = formats[i];
                        spec_len = length;
                        break;
                    }
                }
                if (format_spec != NULL)
                    break;
            }
            if (format_spec != NULL) {
                buffptr += format_spec->format_function(buffptr, buffend - buffptr, format_ptr[1], list);
                format_ptr += spec_len + 1;  // Skip over specifiers
            } else {  // If not a specifier, just copy the character
                if (buffptr < buffend) {
                    *buffptr = *format_ptr;
                    buffptr++;
                }
                format_ptr++;
            }
        } else if (*format_ptr == '%') {
            size_t spec_len = 0;
            size_t format_result_size = 0;
            size_t spec_ok = 1;
            char format_result[BUFFER_SIZE];
            char tempbuff[5];
            va_list argscp[3];
            va_copy(argscp[0], list);
            va_copy(argscp[1], list);
            va_copy(argscp[2], list);
            for (size_t length = 1; length <= max_spec_len; length++) {
                memset(tempbuff, 0, sizeof(tempbuff));
                memset(format_result, 0, sizeof(format_result));
                strncpy(tempbuff, format_ptr, length + 1);  // Include space for null terminator
                int result = vsnprintf(format_result, BUFFER_SIZE, tempbuff, argscp[length - 1]);
                va_end(argscp[length - 1]);
                if (result >= 0 && (size_t)result < sizeof(format_result) && strcmp(format_result, tempbuff) != 0) {
                    spec_ok = 1;
                    spec_len = length;
                    format_result_size = result;
                    va_arg(list, void*);
                    break;
                } else {
                    spec_ok = 0;
                }
            }
            if (spec_ok) {
                for (size_t i = 0; i < format_result_size && buffptr < buffend; i++) {
                    *buffptr++ = format_result[i];
                }
                format_ptr += spec_len + 1;  // Skip over specifiers
            } else {
                for (size_t i = 0; i <= spec_len && buffptr < buffend; i++) {
                    *buffptr++ = format_ptr[i];
                }
                format_ptr += spec_len + 1;  // Skip over specifiers
            }
        } else {
            if (buffptr < buffend) {
                *buffptr = *format_ptr;
                buffptr++;
            }
            format_ptr++;
        }
    }
    *buffptr = '\0';  // Null terminator
    return buffptr - buffer;
}

static size_t hues_function_format_pid(char* buffer, size_t buffer_size, char specifier, va_list list) {
    return snprintf(buffer, buffer_size, "%d", getpid());
}

static size_t hues_function_format_date(char* buffer, size_t buffer_size, char specifier, va_list list) {
    time_t now = time(NULL);
    struct tm* time_info = localtime(&now);
    return strftime(buffer, buffer_size, "%d/%m/%Y", time_info);
}

static size_t hues_function_format_time(char* buffer, size_t buffer_size, char specifier, va_list list) {
    time_t now = time(NULL);
    struct tm* time_info = localtime(&now);
    return strftime(buffer, buffer_size, "%H:%M:%S", time_info);
}

static size_t hues_function_format_level(char* buffer, size_t buffer_size, char specifier, va_list list) {
    hues_level level = va_arg(list, hues_level);
    return snprintf(buffer, buffer_size, "%s", level.name);
}

static size_t hues_function_format_function(char* buffer, size_t buffer_size, char specifier, va_list list) {
    hues_code_location location = va_arg(list, hues_code_location);
    return snprintf(buffer, buffer_size, "%s", strdup(location.method_name));
}

static size_t hues_function_format_file(char* buffer, size_t buffer_size, char specifier, va_list list) {
    hues_code_location location = va_arg(list, hues_code_location);
    return snprintf(buffer, buffer_size, "%s", strdup(location.file));
}

static size_t hues_function_format_line_number(char* buffer, size_t buffer_size, char specifier, va_list list) {
    hues_code_location location = va_arg(list, hues_code_location);
    return snprintf(buffer, buffer_size, "%ld", location.line);
}

static size_t hues_function_format_full_code_location(char* buffer, size_t buffer_size, char specifier, va_list list) {
    hues_code_location location = va_arg(list, hues_code_location);
    return snprintf(buffer, buffer_size, "%s @ %s:%ld", location.method_name, location.file, location.line);
}

/**
 * @fn void hues_log(hues_message* message, ...)
 * @brief Logs a message.
 * @param message The message to log.
 */
void hues_log(hues_message* message, ...) {
    va_list list;
    va_start(list, message);
    hues_log_message_v(message, list);
    va_end(list);
}

/**
 * @fn static void hues_log_message_v(hues_message* message, va_list list)
 * @brief Logs a message using a va_list.
 * @param message The message to log.
 * @param list A list of arguments to use in the to_format string.
 */
static void hues_log_message_v(hues_message* message, va_list list) {
    if (message->level.level < hues_glob_configuration.minimum_level) {
        return;
    }
    char buffer[BUFFER_SIZE];
    hues_level_format* theme_level;
    for (size_t i = 0; i < hues_glob_configuration.levels_count; i++) {
        if (hues_glob_configuration.theme->format[i].level == message->level.level) {
            theme_level = &hues_glob_configuration.theme->format[i];
            break;
        }
    }
    if (!theme_level) {
        fprintf(stderr, "No color configuration found for level %d\n", message->level.level);
        return;
    }
    size_t written = snprintf(buffer, sizeof(buffer), ESC_SEQ_BG, theme_level->background_color.r, theme_level->background_color.g, theme_level->background_color.b);
    written += snprintf(buffer + written, sizeof(buffer), ESC_SEQ_FG, theme_level->foreground_color.r, theme_level->foreground_color.g, theme_level->foreground_color.b);
    written += hues_format_pv_core(buffer + written, sizeof(buffer), hues_glob_configuration.prefix, hues_glob_configuration.formats, hues_glob_configuration.header_format, list);
    written += hues_format_pv_core(buffer + written, sizeof(buffer) - written, hues_glob_configuration.prefix, hues_glob_configuration.formats, message->contents, list);
    ssize_t index = 0;
    if (buffer[written - 1] == '\n') {
        index = -1;
    }
    written += snprintf(buffer + written + index, sizeof(buffer) - written - index, ESC_SEQ_RST);
    printf("%s%s", buffer, index == -1 ? "\n" : "");
}

static uint32_t hues_theme_light_foreground_colors[] = { 0x212121, 0x008000, 0x000000, 0x808000, 0xDC143C, 0xFFFFFF, 0x808080 };
static uint32_t hues_theme_light_background_colors[] = { 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFAE6, 0xFFF0F5, 0xFF0000, 0xFFFFFF };

static uint32_t hues_theme_dark_foreground_colors[] = { 0xFFFFFF, 0xFFDF00, 0x90EE90, 0xFFA500, 0xFF69B4, 0xFFFF00, 0xFFFFFF };
static uint32_t hues_theme_dark_foreground_colors[] = { 0x6161ED, 0x181818, 0x181818, 0x181818, 0x181818, 0xE60000, 0xE60000 };

static void hues_register_format_functions() {
    size_t levels_count = HUES_LEVEL_UNKNOWN + 1;
    hues_format** formats = malloc((levels_count + 1) * sizeof(hues_format*));
    size_t formats_count = 8;
    hues_format* format_array = malloc(formats_count * sizeof(hues_format));
    format_array[0] = (hues_format) { "d", hues_function_format_date };
    format_array[1] = (hues_format) { "t", hues_function_format_time };
    format_array[2] = (hues_format) { "L", hues_function_format_level };
    format_array[3] = (hues_format) { "f", hues_function_format_function };
    format_array[4] = (hues_format) { "F", hues_function_format_file };
    format_array[5] = (hues_format) { "l", hues_function_format_line_number };
    format_array[6] = (hues_format) { "c", hues_function_format_full_code_location };
    format_array[7] = (hues_format) { "p", hues_function_format_pid };
    for (size_t i = 0; i < formats_count; i++) {
        formats[i] = &(format_array[i]);
    }
    formats[formats_count] = NULL; // Terminate the array with NULL
    hues_glob_configuration.formats = formats;
}

void hues_theme_use_light() {
    hues_theme_from_hex(hues_theme_light_background_colors, hues_theme_light_foreground_colors);
}

void hues_theme_use_dark() {
    hues_theme_from_hex(hues_theme_dark_foreground_colors, hues_theme_dark_foreground_colors);
}

void hues_initialize() {
    hues_glob_configuration.minimum_level = HUES_LEVEL_TRACE;
    hues_glob_configuration.prefix = '#';
    hues_glob_configuration.header_format = "(#d-#t) [#L in #c]  ";
    hues_register_format_functions();
    hues_theme_use_dark();
}
