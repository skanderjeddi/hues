#include "flexclog.h"

#include <pthread.h>

size_t flexlog_format_mutex_lock(char* buffer, size_t buffer_size, char specifier, va_list args) {
    pthread_mutex_t* mtx = va_arg(args, pthread_mutex_t*);
    return snprintf(buffer, buffer_size, "mutex %p locked by %d", mtx, getpid());
}

int main(int argc, char** argv) {
    fc_init();
    fc_fmt_t format_mutex_lock;
    format_mutex_lock.spec = "ptl";
    format_mutex_lock.fmt_func = flexlog_format_mutex_lock;
    fc_conf_add_fmt(&format_mutex_lock);

    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);
    pthread_mutex_lock(&mtx);
    info("ptl\n", &mtx);
    pthread_mutex_unlock(&mtx);
    info("ptl\n", &mtx);
    pthread_mutex_destroy(&mtx);

    return 0;
}
