# dde-run2
from https://code.google.com/archive/p/dde-run2/

fork of DDE_run, Win32 command line program to execute commands on a local server via DDE

dde_run2.exe is a Win32 command line program to execute commands on a local server via DDE (Dynamic Data Exchange). It tries to connect to a specified server with a specified topic. If it can't, it can run the specified program and then try again. If connected, up to 5 commands can be executed.

Usage:
-s Specify service name
-t Specify topic
-p Specify the path name to program if connection fails
-r Specify request to execute on server
-c Specify command to execute on server. Up to 5 allowed
I use it for old programs which support only DDE.
My case:
Perl x32 has own DDE modules, but i can't find any module for Perl x64. So my perl program runs DDE_run2, DDE_run2 calls DDE API functions.

History:
Perl x64 does not work with DDE 32bit now, so i decided use dde_run.
DDE_run does not work for me on win7 x64.
I choose DDE_run. It's simple.
I sent letter to Michael Polyakov and ask him about using.

"You can use the code in whatever way you like. I'm not very familiar with all intricacies of different licenses and this was such a long time ago (skipped). Please use whatever code is on that site in any way, I would only be happy if it's still useful for anyone. If you include it in some project and don't mind sticking my name in the credits somewhere it would already be much more than I initially envisioned for that code. (skipped) initially I needed it to play music from Total Commander but that was a long time ago."

So i forked it, added some code and recompiled it on ms vc 2010.
DDE_run is here http://www.angelfire.com/linux/myp/DDE_run/dde_run.html'>http://www.angelfire.com/linux/myp/DDE_run/dde_run.html

I'm not familiar to licenses also.
So i go to https://tldrlegal.com/licenses/browse'>https://tldrlegal.com/licenses/browse But it's simple program, probably you can write it by yourself
So you can use the code in whatever way you like.
