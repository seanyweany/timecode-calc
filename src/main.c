#include <stdio.h>

static int parse_field(char **p) {
    int val = 0;
    while (**p >= '0' && **p <= '9') {
        val = val * 10 + (**p - '0');
        (*p)++;
    }
    if (**p == ';')
        (*p)++;
    return val;
}

static int count_fields(char *s) {
    int n = 1;
    while (*s) {
        if (*s == ';')
            n++;
        s++;
    }
    return n;
}

static int tc_to_frames(char *s, int nfields) {
    int vals[4] = {0, 0, 0, 0};
    int start = 4 - nfields;
    for (int i = start; i < 4; i++)
        vals[i] = parse_field(&s);
    return ((vals[0] * 60 + vals[1]) * 60 + vals[2]) * 30 + vals[3];
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: tccalc timecode [+ or -] timecode\n");
        return 1;
    }

    /* Concatenate all args into buf */
    char buf[256];
    char *dst = buf;
    for (int i = 1; i < argc; i++) {
        char *src = argv[i];
        while (*src)
            *dst++ = *src++;
    }
    *dst = '\0';

    /* Find operator (+ or -) */
    char *op = 0;
    for (char *p = buf; *p; p++) {
        if (*p == '+' || *p == '-') {
            op = p;
            break;
        }
    }

    char *left, *right;
    char op_char = '-';

    if (op) {
        op_char = *op;
        *op = '\0';
        left = buf;
        right = op + 1;
    } else if (argc == 3) {
        /* Old style: tccalc TC1 TC2 */
        left = argv[1];
        right = argv[2];
    } else {
        printf("Usage: tccalc timecode [+ or -] timecode\n");
        return 1;
    }

    int n1 = count_fields(left);
    int n2 = count_fields(right);
    int nmax = n1 > n2 ? n1 : n2;

    int a = tc_to_frames(left, n1);
    int b = tc_to_frames(right, n2);

    int result;
    if (op_char == '+')
        result = a + b;
    else
        result = a - b;
    if (result < 0)
        result = -result;

    int ff = result % 30;
    result /= 30;
    int ss = result % 60;
    result /= 60;
    int mm = result % 60;
    int hh = result / 60;

    /* Expand output if result overflows input precision */
    if (hh > 0 && nmax < 4)
        nmax = 4;
    else if (mm > 0 && nmax < 3)
        nmax = 3;
    else if (ss > 0 && nmax < 2)
        nmax = 2;

    if (nmax >= 4)
        printf("%02d;%02d;%02d;%02d\n", hh, mm, ss, ff);
    else if (nmax >= 3)
        printf("%02d;%02d;%02d\n", mm, ss, ff);
    else if (nmax >= 2)
        printf("%02d;%02d\n", ss, ff);
    else
        printf("%02d\n", ff);

    return 0;
}
