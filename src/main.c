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
    if (argc != 3) {
        printf("Usage: tcdiff timecode timecode\n");
        return 1;
    }

    int n1 = count_fields(argv[1]);
    int n2 = count_fields(argv[2]);
    int nmax = n1 > n2 ? n1 : n2;

    int a = tc_to_frames(argv[1], n1);
    int b = tc_to_frames(argv[2], n2);
    int diff = a - b;
    if (diff < 0)
        diff = -diff;

    int ff = diff % 30;
    diff /= 30;
    int ss = diff % 60;
    diff /= 60;
    int mm = diff % 60;
    int hh = diff / 60;

    if (nmax == 4)
        printf("%02d;%02d;%02d;%02d\n", hh, mm, ss, ff);
    else if (nmax == 3)
        printf("%02d;%02d;%02d\n", mm, ss, ff);
    else if (nmax == 2)
        printf("%02d;%02d\n", ss, ff);
    else
        printf("%02d\n", ff);

    return 0;
}
