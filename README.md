Okay, truthfully, I just wanted to try out Anthropic's Opus 4.6 by asking it to write this.<br>
What do you think? I thought it was impressive...

## Usage
### Calculate the sum of or difference between two timecodes @ 30fps!<br>
``tccalc [hh;mm;ss;ff] [|+ | -] [hh;mm;ss;ff]``<br>
(e.g., ``tccalc 42;11 29;09`` prints ``13;02``, ``tccalc 2;14;59;03 33;19`` prints ``02;14;25;14``, ``tccalc 29;10;02 + 15;29`` prints ``29;26;01``, even ``tccalc 2;09-15`` prints ``01;24``.)
Default (no operator) equates to subtraction.