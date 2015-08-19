Text Editor
===========

I learned ncurses and figured why not.
Posting this now (9/30/15) because school is starting
so I don't plan on working on it anymore.

Probably the most well-developed program I made this summer.

How to use:

F4 quits (no prompt)
F5 saves the current file as save.txt

Current issues:

x-y coordinates in ncurses correspond to rows and columns in
the text document.  This means tabs are an issue and text
wrapping is impossible.  Solution: separate screen logic
from document logic.

Can only save files under the name save.txt.  Solution: create
a prompt window (from scratch...) that retuns a string to the
caller.
