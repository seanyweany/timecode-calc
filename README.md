Okay, truthfully, I just wanted to try out Anthropic's Opus 4.6 by asking it to write this.<br>
Nearly everything was written by Opus. What do you think?

## Usage
### Calculate the sum of or difference between two timecodes!<br>
```
tccalc timecode [+ | -] timecode [-f | --framerate] fps

  timecode                 Up to four semicolon-delimited fields: [hh;mm;ss;ff].
                           Fewer fields may be given and are read right-to-left
                           (e.g., 2;09 is ss;ff, not hh;mm).

  + | -                    Arithmetic operator. Default (no operator) is
                           subtraction. The operator may be glued to either
                           operand or separated by spaces.

  -f, --framerate          Frames per second. Default is 60.

Examples:
  tccalc 42;11 29;09       prints  13;02
  tccalc 2;14;59;03 33;19  prints  02;14;25;44
  tccalc 29;10;02 + 15;29  prints  29;25;31
  tccalc 2;09-15           prints  01;54
  tccalc 04;29 1;14 -f 30  prints  03;15
```