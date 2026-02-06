#ifndef TCCALC_H
#define TCCALC_H

int  streq(const char *a, const char *b);
int  parse_int(const char *s);
int  parse_field(char **p);
int  count_fields(char *s);
int  tc_to_frames(char *s, int nfields, int fps);
int  last_field(const char *s);
void print_help(void);

#endif
