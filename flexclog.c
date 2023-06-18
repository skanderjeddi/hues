#include "flexclog.h"

static void fc_log_msg_v(fc_msg_t* msg, va_list args);
static size_t fc_fmt_pvc(char* buff, size_t buffsz, char pref, fc_fmt_t** fmts, const char* fmt, va_list args);
static size_t fc_fmt_vc(char* buff, size_t buffsz, char pref, fc_fmt_t** fmts, const char* fmt, va_list args);

static fc_conf_t fc_glob_conf = { 
    .min_lvl = FC_LVL_DBG,
    .lvl_fmt = "#t/#d #v\t",
    .pref = '#',
    .thm = NULL,
    .lvls = FC_LVL_UNK + 1,
    .fmts = NULL
};

char* fc_conf_get_lvl_fmt() {
    return fc_glob_conf.lvl_fmt;
}

void fc_conf_set_lvl_fmt(const char* lvl_fmt) {
    fc_glob_conf.lvl_fmt = strdup(lvl_fmt);
}

fc_lvl_enum_t fc_conf_get_minimum_lvl() {
    return fc_glob_conf.min_lvl;
}

void conf_set_minimum_lvl(fc_lvl_enum_t min_lvl) {
    fc_glob_conf.min_lvl = min_lvl;
}

char fc_conf_get_prefix() {
    return fc_glob_conf.pref;
}

void fc_conf_set_prefix(char pref) {
    fc_glob_conf.pref = pref;
}

fc_thm_t* fc_conf_get_thm() {
    return fc_glob_conf.thm;
}

void fc_conf_set_thm(fc_thm_t* thm) {
    fc_glob_conf.thm = thm;
}

fc_fmt_t** fc_conf_get_fmts() {
    return fc_glob_conf.fmts;
}

void fc_conf_set_fmts(fc_fmt_t** fmts) {
    fc_glob_conf.fmts = fmts;
}

void fc_conf_add_fmt(fc_fmt_t* fmt) {
    if (fc_glob_conf.fmts == NULL) {
        fc_glob_conf.fmts = malloc(sizeof(fc_fmt_t*) * 2);
        fc_glob_conf.fmts[0] = fmt;
        fc_glob_conf.fmts[1] = NULL;
    } else {
        size_t fmts_cnt = 0;
        for (size_t i = 0; fc_glob_conf.fmts[i] != NULL; i++) {
            fmts_cnt++;
        }
        fc_glob_conf.fmts = realloc(fc_glob_conf.fmts, sizeof(fc_fmt_t*) * (fmts_cnt + 2));
        fc_glob_conf.fmts[fmts_cnt] = fmt;
        fc_glob_conf.fmts[fmts_cnt + 1] = NULL;
    }
}

const fc_color_t fc_hex_to_clr(uint32_t hex) {
    fc_color_t clr;
    clr.r = (hex >> 16) & 0xFF;
    clr.g = (hex >> 8) & 0xFF;
    clr.b = hex & 0xFF;
    return clr;
}

void fc_clr_to_hex(char* hex, fc_color_t* clr) {
    sprintf(hex, "#%02x%02x%02x", clr->r, clr->g, clr->b);
}

void fc_thm_from_hex(uint32_t* bg_hex, uint32_t* fg_hex) {
    fc_glob_conf.thm = malloc(sizeof(fc_thm_t));
    fc_glob_conf.thm->fmt = malloc(sizeof(fc_lvl_fmt_t) * fc_glob_conf.lvls);
    for (size_t i = 0; i < FC_LVL_UNK + 1; i++) {
        fc_glob_conf.thm->fmt[i].lvl = i;
        fc_glob_conf.thm->fmt[i].bg_clr = fc_hex_to_clr(bg_hex[i]);
        fc_glob_conf.thm->fmt[i].fg_clr = fc_hex_to_clr(fg_hex[i]);
    }
}

static size_t fc_fmt_v(char* buff, size_t buffsz, char pref, fc_fmt_t** fmts, const char* fmt, va_list args) {
    return fc_fmt_vc(buff, buffsz, pref, fmts, fmt, args);
}

size_t fc_fmt(char* buff, size_t buffsz, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    size_t written = fc_fmt_v(buff, buffsz, fc_glob_conf.pref, fc_glob_conf.fmts, fmt, args);
    va_end(args);
    return written;
}

static size_t fc_fmt_vc(char* buff, size_t buffsz, char pref, fc_fmt_t** fmts, const char* fmt, va_list args) {
    char* buffptr = buff;
    char* buffend = buff + buffsz;
    const char* fmt_end = fmt + strlen(fmt);
    size_t max_spec_len = 3;
    while (fmt < fmt_end) {
        if (*fmt == pref) {
            fc_fmt_t* fmt_spec = NULL;
            size_t spec_len = 1;
            for (size_t length = max_spec_len; length >= 1; length--) {
                for (size_t i = 0; fmts[i] != NULL; i++) {
                    if (strncmp(fmt + 1, fmts[i]->spec, length) == 0) {
                        fmt_spec = fmts[i];
                        spec_len = length;
                        break;
                    }
                }
                if (fmt_spec != NULL)
                    break;
            }
            if (fmt_spec == NULL) {
                for (size_t i = 0; i < spec_len; i++) {
                    if (buffptr < buffend) {
                        *buffptr = fmt[i];
                        buffptr++;
                    }
                }
            } else {
                buffptr += fmt_spec->fmt_func(buffptr, buffend - buffptr, fmt[1], args);
            }
            fmt += spec_len + 1;
        } else {
            if (buffptr < buffend) {
                *buffptr = *fmt;
                buffptr++;
            }
            fmt++;
        }
    }
    if (buffptr < buffend) {
        *buffptr = '\0';
    }
    return buffptr - buff;
}

static size_t fc_fmt_pv(char* buff, size_t buffsz, char pref, fc_fmt_t** fmts, const char* fmt, va_list args) {
    return fc_fmt_pvc(buff, buffsz, pref, fmts, fmt, args);
}

size_t fc_fmt_p(char* buff, size_t buffsz, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    size_t written = fc_fmt_pv(buff, buffsz, fc_glob_conf.pref, fc_glob_conf.fmts, fmt, args);
    va_end(args);
    return written;
}

static size_t fc_fmt_pvc(char* buff, size_t buffsz, char pref, fc_fmt_t** fmts, const char* fmt, va_list args) {
    char* buffptr = buff;
    char* buffend = buff + buffsz - 1; // Reserve space for null terminator
    const char* fmt_ptr = fmt;
    const char* fmt_end = fmt + strlen(fmt);
    size_t max_spec_len = 3;
    while (fmt_ptr < fmt_end) {
        if (*fmt_ptr == pref) {
            fc_fmt_t* fmt_spec = NULL;
            size_t spec_len = 0;
            for (size_t length = max_spec_len; length > 0; length--) {
                for (size_t i = 0; fmts[i] != NULL; i++) {
                    if (strncmp(fmt_ptr + 1, fmts[i]->spec, length) == 0) {
                        fmt_spec = fmts[i];
                        spec_len = length;
                        break;
                    }
                }
                if (fmt_spec != NULL)
                    break;
            }
            if (fmt_spec != NULL) {
                buffptr += fmt_spec->fmt_func(buffptr, buffend - buffptr, fmt_ptr[1], args);
                fmt_ptr += spec_len + 1;  // Skip over specifiers
            } else {  // If not a specifier, just copy the character
                if (buffptr < buffend) {
                    *buffptr = *fmt_ptr;
                    buffptr++;
                }
                fmt_ptr++;
            }
        } else if (*fmt_ptr == '%') {
            size_t spec_len = 0;
            size_t fmt_result_size = 0;
            size_t spec_ok = 1;
            char fmt_result[BUFFER_SIZE];
            char tempbuff[5];
            va_list argscp[3];
            va_copy(argscp[0], args);
            va_copy(argscp[1], args);
            va_copy(argscp[2], args);
            for (size_t length = 1; length <= max_spec_len; length++) {
                memset(tempbuff, 0, sizeof(tempbuff));
                memset(fmt_result, 0, sizeof(fmt_result));
                strncpy(tempbuff, fmt_ptr, length + 1);  // Include space for null terminator
                int result = vsnprintf(fmt_result, BUFFER_SIZE, tempbuff, argscp[length - 1]);
                va_end(argscp[length - 1]);
                if (result >= 0 && (size_t)result < sizeof(fmt_result) && strcmp(fmt_result, tempbuff) != 0) {
                    spec_ok = 1;
                    spec_len = length;
                    fmt_result_size = result;
                    va_arg(args, void*);
                    break;
                } else {
                    spec_ok = 0;
                }
            }
            if (spec_ok) {
                for (size_t i = 0; i < fmt_result_size && buffptr < buffend; i++) {
                    *buffptr++ = fmt_result[i];
                }
                fmt_ptr += spec_len + 1;  // Skip over specifiers
            } else {
                for (size_t i = 0; i <= spec_len && buffptr < buffend; i++) {
                    *buffptr++ = fmt_ptr[i];
                }
                fmt_ptr += spec_len + 1;  // Skip over specifiers
            }
        } else {
            if (buffptr < buffend) {
                *buffptr = *fmt_ptr;
                buffptr++;
            }
            fmt_ptr++;
        }
    }
    *buffptr = '\0';  // Null terminator
    return buffptr - buff;
}

static size_t func_fmt_pid(char* buff, size_t buffsz, char specifier, va_list args) {
    return snprintf(buff, buffsz, "%d", getpid());
}

static size_t func_fmt_date(char* buff, size_t buffsz, char specifier, va_list args) {
    time_t now = time(NULL);
    struct tm* time_info = localtime(&now);
    return strftime(buff, buffsz, "%d/%m/%Y", time_info);
}

static size_t func_fmt_time(char* buff, size_t buffsz, char specifier, va_list args) {
    time_t now = time(NULL);
    struct tm* time_info = localtime(&now);
    return strftime(buff, buffsz, "%H:%M:%S", time_info);
}

static size_t func_fmt_lvl(char* buff, size_t buffsz, char specifier, va_list args) {
    fc_lvl_t lvl = va_arg(args, fc_lvl_t);
    return snprintf(buff, buffsz, "%s", lvl.name);
}

static size_t func_fmt_func(char* buff, size_t buffsz, char specifier, va_list args) {
    fc_code_loc_t loc = va_arg(args, fc_code_loc_t);
    return snprintf(buff, buffsz, "%s", strdup(loc.func));
}

static size_t func_fmt_file(char* buff, size_t buffsz, char specifier, va_list args) {
    fc_code_loc_t loc = va_arg(args, fc_code_loc_t);
    return snprintf(buff, buffsz, "%s", strdup(loc.file));
}

static size_t func_fmt_line_number(char* buff, size_t buffsz, char specifier, va_list args) {
    fc_code_loc_t loc = va_arg(args, fc_code_loc_t);
    return snprintf(buff, buffsz, "%ld", loc.line);
}

static size_t func_fmt_full_code_location(char* buff, size_t buffsz, char specifier, va_list args) {
    fc_code_loc_t loc = va_arg(args, fc_code_loc_t);
    return snprintf(buff, buffsz, "%s @ %s:%ld", loc.func, loc.file, loc.line);
}

void fc_log(fc_msg_t* msg, ...) {
    va_list args;
    va_start(args, msg);
    fc_log_msg_v(msg, args);
    va_end(args);
}

static void fc_log_msg_v(fc_msg_t* msg, va_list args) {
    if (msg->lvl.lvl < fc_glob_conf.min_lvl) {
        return;
    }
    char buff[BUFFER_SIZE];
    fc_lvl_fmt_t* thm_lvl;
    for (size_t i = 0; i < fc_glob_conf.lvls; i++) {
        if (fc_glob_conf.thm->fmt[i].lvl == msg->lvl.lvl) {
            thm_lvl = &fc_glob_conf.thm->fmt[i];
            break;
        }
    }
    if (!thm_lvl) {
        fprintf(stderr, "No clr conf found for lvl %d\n", msg->lvl.lvl);
        return;
    }
    size_t written = snprintf(buff, sizeof(buff), ESC_SEQ_BG, thm_lvl->bg_clr.r, thm_lvl->bg_clr.g, thm_lvl->bg_clr.b);
    written += snprintf(buff + written, sizeof(buff), ESC_SEQ_FG, thm_lvl->fg_clr.r, thm_lvl->fg_clr.g, thm_lvl->fg_clr.b);
    written += fc_fmt_pvc(buff + written, sizeof(buff), fc_glob_conf.pref, fc_glob_conf.fmts, fc_glob_conf.lvl_fmt, args);
    written += fc_fmt_pvc(buff + written, sizeof(buff) - written, fc_glob_conf.pref, fc_glob_conf.fmts, msg->txt, args);
    ssize_t index = 0;
    if (buff[written - 1] == '\n') {
        index = -1;
    }
    written += snprintf(buff + written + index, sizeof(buff) - written - index, ESC_SEQ_RST);
    printf("%s%s", buff, index == -1 ? "\n" : "");
}

static uint32_t thm_l_fg[] = { 0x212121, 0x008000, 0x000000, 0x808000, 0xDC143C, 0xFFFFFF, 0x808080 };
static uint32_t thm_l_bg[] = { 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFAE6, 0xFFF0F5, 0xFF0000, 0xFFFFFF };

static uint32_t thm_d_fg[] = { 0xFFFFFF, 0xFFDF00, 0x90EE90, 0xFFA500, 0xFF69B4, 0xFFFF00, 0xFFFFFF };
static uint32_t thm_d_bg[] = { 0x6161ed, 0x181818, 0x181818, 0x181818, 0x181818, 0xE60000, 0xE60000 };

static void fc_reg_fmt_funcs() {
    size_t lvls = FC_LVL_UNK + 1;
    fc_fmt_t** fmts = malloc((lvls + 1) * sizeof(fc_fmt_t*));
    size_t fmts_cnt = 8;
    fc_fmt_t* fmt_array = malloc(fmts_cnt * sizeof(fc_fmt_t));
    fmt_array[0] = (fc_fmt_t) { "d", func_fmt_date };
    fmt_array[1] = (fc_fmt_t) { "t", func_fmt_time };
    fmt_array[2] = (fc_fmt_t) { "L", func_fmt_lvl };
    fmt_array[3] = (fc_fmt_t) { "f", func_fmt_func };
    fmt_array[4] = (fc_fmt_t) { "F", func_fmt_file };
    fmt_array[5] = (fc_fmt_t) { "l", func_fmt_line_number };
    fmt_array[6] = (fc_fmt_t) { "c", func_fmt_full_code_location };
    fmt_array[7] = (fc_fmt_t) { "p", func_fmt_pid };
    for (size_t i = 0; i < fmts_cnt; i++) {
        fmts[i] = &(fmt_array[i]);
    }
    fmts[fmts_cnt] = NULL; // Terminate the array with NULL
    fc_glob_conf.fmts = fmts;
}

void fc_thm_use_lgt() {
    fc_thm_from_hex(thm_l_bg, thm_l_fg);
}

void fc_thm_use_drk() {
    fc_thm_from_hex(thm_d_bg, thm_d_fg);
}

void fc_init() {
    fc_glob_conf.min_lvl = FC_LVL_TRC;
    fc_glob_conf.pref = '#';
    fc_glob_conf.lvl_fmt = "(#d-#t) [#L in #c]  ";
    fc_reg_fmt_funcs();
    fc_thm_use_drk();
}
