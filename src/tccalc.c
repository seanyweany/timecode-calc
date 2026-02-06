#include <stdio.h>
#include "tccalc.h"

int streq(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *a == '\0' && *b == '\0';
}

int parse_int(const char *s) {
    int val = 0;
    while (*s >= '0' && *s <= '9') {
        val = val * 10 + (*s - '0');
        s++;
    }
    return val;
}

int parse_field(char **p) {
    int val = 0;
    while (**p >= '0' && **p <= '9') {
        val = val * 10 + (**p - '0');
        (*p)++;
    }
    if (**p == ';')
        (*p)++;
    return val;
}

int count_fields(char *s) {
    int n = 1;
    while (*s) {
        if (*s == ';')
            n++;
        s++;
    }
    return n;
}

int tc_to_frames(char *s, int nfields, int fps) {
    int vals[4] = {0, 0, 0, 0};
    int start = 4 - nfields;
    for (int i = start; i < 4; i++)
        vals[i] = parse_field(&s);
    return ((vals[0] * 60 + vals[1]) * 60 + vals[2]) * fps + vals[3];
}

int last_field(const char *s) {
    const char *last = s;
    while (*s) {
        if (*s == ';')
            last = s + 1;
        s++;
    }
    return parse_int(last);
}

void print_help(void) {
    printf(
        "Calculates the sum or difference between two timecodes!\n"
        "\n"
        "tccalc timecode [+ | -] timecode [-f | --framerate] fps\n"
        "\n"
        "  timecode                 Up to four semicolon-delimited fields: [hh;mm;ss;ff].\n"
        "                           Fewer fields may be given and are read right-to-left\n"
        "                           (e.g., 2;09 is ss;ff, not hh;mm).\n"
        "\n"
        "  + | -                    Arithmetic operator. Default (no operator) is\n"
        "                           subtraction. The operator may be glued to either\n"
        "                           operand or separated by spaces.\n"
        "\n"
        "  -f, --framerate          Frames per second. Default is 60.\n"
        "\n"
        "Examples:\n"
        "  tccalc 42;11 29;09       prints  13;02\n"
        "  tccalc 2;14;59;03 33;19  prints  02;14;25;44\n"
        "  tccalc 29;10;02 + 15;29  prints  29;25;31\n"
        "  tccalc 2;09-15           prints  01;54\n"
        "  tccalc 04;29 1;14 -f 30  prints  03;15\n"
    );
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: tccalc timecode [+ or -] timecode [-f fps]\n");
        return 1;
    }

    /* Check for help flag before anything else */
    for (int i = 1; i < argc; i++) {
        if (streq(argv[i], "-h") || streq(argv[i], "--help")
                || streq(argv[i], "/?")) {
            print_help();
            return 0;
        }
    }

    int fps = 60;

    /* Extract flags and concatenate timecode args into buf */
    char buf[256];
    char *dst = buf;
    char *tc_argv[2] = {0, 0};
    int tc_argc = 0;

    for (int i = 1; i < argc; i++) {
        if ((streq(argv[i], "-f") || streq(argv[i], "--framerate"))
                && i + 1 < argc) {
            fps = parse_int(argv[++i]);
            continue;
        }
        if (argv[i][0] == '-' && argv[i][1] == 'f'
                && argv[i][2] >= '0' && argv[i][2] <= '9') {
            fps = parse_int(argv[i] + 2);
            continue;
        }
        if (tc_argc < 2)
            tc_argv[tc_argc] = argv[i];
        tc_argc++;
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
    } else if (tc_argc == 2) {
        /* Old style: tccalc TC1 TC2 */
        left = tc_argv[0];
        right = tc_argv[1];
    } else {
        printf("Usage: tccalc timecode [+ or -] timecode [-f fps]\n");
        return 1;
    }

    int n1 = count_fields(left);
    int n2 = count_fields(right);
    int nmax = n1 > n2 ? n1 : n2;

    int a = tc_to_frames(left, n1, fps);
    int b = tc_to_frames(right, n2, fps);

    int result;
    if (op_char == '+')
        result = a + b;
    else
        result = a - b;
    if (result < 0)
        result = -result;

    int ff = result % fps;
    result /= fps;
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

    if (last_field(left) >= fps || last_field(right) >= fps)
        printf("Timecode doesn't match selected framerate.\n");

    return 0;
}
