#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>

// Symbol defined by the linker script representing the top of RAM / stack start
extern uint8_t _estack; 

void * _sbrk(ptrdiff_t incr) {
    extern uint8_t _end; 
    static uint8_t *heap_end;
    uint8_t *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }

    prev_heap_end = heap_end;

    if (heap_end + incr > (uint8_t *)&_estack - 1024) { 
        // Set the thread-safe errno definition to out of memory
        #undef errno
        extern int errno;
        errno = ENOMEM;
        return (void *)-1;
    }

    heap_end += incr;
    return (void *)prev_heap_end;
}

/* --- Added Minimal System Call Stubs to satisfy Newlib C standard library --- */

int _read(int file, char *ptr, int len) {
    return 0; // Return 0 (EOF) since we aren't reading from standard input streams
}

int _write(int file, char *ptr, int len) {
    // If you ever want printf() to send data out via UART1, 
    // you would loop over 'ptr' and feed characters to USART1->DR here.
    return len; 
}

int _close(int file) {
    return -1;
}

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR; // Character device type flag
    return 0;
}

int _isatty(int file) {
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _getpid(void) {
    return 1;
}

int _kill(int pid, int sig) {
    #undef errno
    extern int errno;
    errno = EINVAL;
    return -1;
}

void _exit(int status) {
    while (1); // Trap inside an infinite loop if abort or exit is triggered
}