#include "mpx_supt.h"

void print(char *s) {
    int s_size = (int) sizeof(*s);
    sys_req(WRITE, DEFAULT_DEVICE, s, &s_size);
}

unsigned char dectobcd(int dec) {
    int tens = (dec / 10) % 10;
    int ones = dec % 10;
    unsigned char bcd = 16 * tens + ones;
    return bcd;
}
