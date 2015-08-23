Text Editor
===========

I learned ncurses and figured why not.
Posting this now (9/30/15) because school is starting
so I don't plan on working on it anymore.

Probably the most well-developed program I made this summer.

##How to use:

    usage: text [ file ]

* F4 quits (after prompt)
* F5 saves the current file (untitled.txt is the default name)
* F6 saves the current file and allows you to specify the filename.
(WARNING: no confirmation screen or error checking yet).

##Current issues:

x-y coordinates in ncurses correspond to rows and columns in
the text document.  This means tabs are an issue and text
wrapping is impossible.  Solution: separate screen logic
from document logic.

Can only save files under the name save.txt.  Solution: create
a prompt window (from scratch...) that retuns a string to the
caller. (Done!)
