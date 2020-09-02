.ie n \{\
.ds `` ""
.ds '' ""\}
.el \{\
.ds `` \(lq
.ds '' \(rq\}
.de Rt
.if n \{\
.na
.nh \}
..
.de SE	\" sentence
..
.RP
.ds LH STDIO Reimplementation
.ds CH
.ds RH Page %
.ds CF \\*(DA
.TL
A Reimplementation of the Standard I/O Package
.AU
Steve Summit
.AB
.PP
.Rt
This paper describes a new implementation of the traditional
C/\c
.UX
stdio package.
Although retaining the efficiency and
(to the extent possible)
the compactness of the original,
this implementation contains several significant improvements,
including
user-definable underlying I/O functions,
improved error handling,
new routines for \*(``string\*('' I/O,
and
efficient unbuffered I/O.
This package is intended to be compatible with the proposed
.SM
ANSI
.LG
C standard, although it contains new functions not mentioned in
that standard.
.sp .5i
.LP
Copyright 1989 by Steve Summit.
.br
All Rights Reserved.
.br
Noncommercial redistribution permitted;
commercial, for-profit use will require permission of the author.
.AE
.SH
Introduction
.PP
.Rt
The \*(``Standard I/O\*('' (stdio) package traditionally associated with
the C language (as part of its run-time library) has proven to be
an effective and valuable subsystem.
This paper describes a new implementation of the stdio package.
This implementation has been undertaken for several reasons:
.IP 1. 6 2
To provide a testbed for extensions and additions to the the
traditional stdio feature set,
notably \*(``user-defined\*('' I/O processing;
.IP 2.
To provide
.SM
ANSI
.LG
C functionality on systems which are not yet standard-conforming;
.IP 3.
To provide a package, in source form, (relatively) free from
license and copyright restrictions; and
.IP 4.
Because it was there.
.LP
The following sections describe the principal improvements
present in this package,
as well as a few implementation, installation, and maintenance issues.
.SH
User-Definable I/O Functions
.PP
It is frequently desirable to have stdio-like high-level behavior
(printf, etc.) which results in something other than calls to the low-level
.I read
and
.I write
functions.
An alternative frequently suggested,
and implemented in this package,
is to allow functions other than the standard ones to be
assigned, on a per-stream basis.
For example, a windowing system might have a low-level call,
.I wwrite ,
for writing text into a window.
If
.I wwrite
is installed as the underlying write function for a stream,
the full flexibility of fprintf
(and fputs and fputc, if desired)
becomes available for window output.
No parallel reimplementation
(\*(``wprintf\*('' or the like)
is required.
.PP
The definable low-level functions are a read function, a write
function, a seek function, and a close function.
These functions, whatever their implementation, are assumed to
take the same arguments, and return the same values, as the
.UX
system calls
.I read ,
.I write ,
.I lseek ,
and
.I close .
However, it is possible for the underlying \*(``handle\*('' to be a
pointer (\fCchar\ *\fP, or
.SM
ANSI
.LG
\fCvoid\ *\fP)
rather than an integer descriptor.
.PP
When the characters being read or written are not mapped to an
I/O-like character stream at all (for instance when a stdio
stream is \*(``writing\*('' to a dynamically allocated in-memory buffer)
it may be desirable to gain more control over stdio's buffering
process.
For this reason, the internal stdio buffering functions
.I _filbuf
and
.I _flsbuf
are also redefinable.
.SH
Error Handling
.PP
C programs are notoriously poor at handling I/O errors.
On input, an error is not immediately distinguishable from EOF,
and few programs bother to check (using
.I feof
or
.I ferror ).
On output,
although an error code is potentially returned by each call to
printf, it is the very rare program that checks every return
value, and it is arguably inappropriate to do so.
(The source and object code bloating resulting from exhaustive
printf return value checking would be intolerable for many programs.)
.SE
Yet the underlying stdio implementation has complete error
information available, since it does (as it must) check for
errors resulting from any low-level I/O function.
(These checks are not inefficient in time or space, since they
are made in only a few centralized routines, and on a per-bufferful
rather than per-character basis.)
.PP
This implementation of stdio can have an error handling function
associated with each stream.
A stream's error handler is called if any low-level I/O error
occurs.
The default error handler prints an error message (to stderr)
including the file name (if known),
the kind of error
(read, write, etc.)
and the perror text.
In the case of write errors, the default error handler aborts the
program.
For read errors, the normal EOF is returned.
.PP
Aborting a program due to a write error is certainly a harsh
penalty.
The decision to do so was made based on the following
observations:
.IP 1. 6 2
Very, very few programs check for write errors.
.IP 2.
The most common write error (in my experience) is \*(``file system
full\*('' (\s-2ENOSPC\s0).
Runaway programs continuing to write to a full filesystem (often
/tmp) are extremely unpleasant and can bring a system to its knees.
.PP
To be sure, there are programs
(notably text editors, databases, and revision control systems)
for which write errors should most certainly
.I not
cause an abort.
Such programs, if linked against this stdio package, can use one
of several mechanisms to replace the default error handler with a
more appropriate one.
These programs are fairly easy to identify, as they have
typically been recognized as requiring more-careful-than-usual
write return value checking.
(It is unfortunate that those few programs which are likely to
be penalized by the behavior of the default error handler are the
same ones that have been responsible enough to check for write errors
already.)
.PP
If the automatic abort due to a write error is felt to be
completely inappropriate, it can be disabled (at library compile
time) by #defining \s-2WRITEERRNOTFATAL\s0.
.SH
Unbuffered I/O Efficiency
.PP
An unbuffered stream does not (by definition) have an associated
buffer, from which contiguous characters can be passed to
.I write .
Traditionally, an unbuffered stream performed low-level
.I read s
and
.I write s
one character at a time,
resulting in a potentially devastating performance degradation due to
system call overhead.
However, in many cases (particularly for output streams) the
calling program's stdio calls do refer to contiguous chunks of characters,
making the character-at-a-time low-level performance particularly
unnecessary.
.PP
This package can perform unbuffered I/O of longer strings of
characters when the characters are detectably contiguous in the
calling program, obviating the need for a stdio buffer.
This optimization is performed in the following cases:
.IP \(bu 6 2
fread
.IP \(bu
fwrite
.IP \(bu
fputs
.IP \(bu
puts
.IP \(bu
fprintf and printf,
for constant text (between %'s) in the format string,
for strings printed with %s,
and for formatted numbers printed with %[doxuefg].
.PP
This option does of course increase the code size slightly.
The optimization is conditional on the preprocessor macro \s-2FASTNBF\s0.
.PP
The
.SM
FASTNBF
.LG
implementation does
.I not
simply assign a temporary buffer during calls to printf and fprintf, 
as is done by some other versions.
Rather, special cases in the lowest-level output routines check
for the _IONBF flag in situations when multiple adjacent
characters are likely to be available.
This technique is less desirable from a code cleanliness standpoint
(special cases are always ugly), but it is vastly preferable from
a performance standpoint:
temporary buffer assignment within fprintf is nonreentrant and unsafe.
.PP
The existence of the
.SM
FASTNBF
.LG
option should remove any temptation
to make stderr buffered.
(A few programs,
notably tar with the v option,
and to a lesser extent RCS,
print significant quantities of text to stderr,
which is noticeably slow on loaded systems or over network connections,
and which occasionally lead to suggestions to buffer stderr,
which would be a horrible idea.)
.SH
New \*(``String I/O\*('' Functions
.PP
Stdio has always contained the \*(``string I/O\*('' functions
.I sscanf
and
.I sprintf ,
which read/write not to a file but an in-memory string.
These functions are convenient and surprisingly easy to
implement, but they suffer from annoying and unnecessary
limitations.
In particular, it is in general impossible to guarantee that
.I sprintf
will not overflow its output string, overwriting other
memory with potentially dire effects.
.PP
This package adds the following new \*(``string I/O\*('' functions:
.IP snprintf 10
like
.I sprintf ,
but allows a maximum output string length to be
specified.
.IP saprintf
like
.I sprintf ,
but returns a pointer to dynamically-allocated
memory (obtained with
.I malloc )
large enough to contain the formatted string.
.IP strnopen
Returns a general-purpose stream, \*(``opened\*('' on a string,
in either \*(``r\*('', \*(``w\*('' or \*(``a\*('' mode
(the modes have a similar interpretation as for
.I fopen ).
The string length can be specified, to avoid overwriting on
output,
or to limit the portion of the string read on input
(i.e. stopping before to the default '\e0').
.IP
.I strnopen
allows multiple, arbitrary I/O calls to be performed on a single
string
(\fIsprintf\fP essentially allows just one
.I printf ).
.IP stropen
like
.I strnopen ,
but without the explicit string length argument,
and therefore potentially unsafe for output.
.IP straopen
like
.I stropen ,
but builds dynamically-allocated output buffer \*(``on the fly\*('' (like
.I saprintf ).
.SH
Interface With Other Libraries
.PP
Occasionally, the fact that stdio traditionally makes direct
calls to other library routines
(such as
.I malloc )
is troublesome.
For instance, a program might have its own \*(``wrapper\*('' functions
around
.I malloc
and
.I free ,
that perform consistency checking or
storage reclamation.

The only routines called
by this implementation of stdio are:
.DS I
.ta \w'write'u+3m
open	malloc
read	realloc
write	free
lseek	fstat
close	isatty
.DE
Of these, all but
.I fstat
and
.I isatty
can be redefined either at
library compilation time or at run time.
.I fstat
and
.I isatty
can only be redefined at compile time.
(\fIfstat\fP and
.I isatty
are only used for buffering decisions on
\*(``normal\*('' streams.
.I fstat
is only used on systems, such as 4.2bsd, which have a
st_blksize field in the stat structure.)
.PP
.I popen
and
.I pclose
additionally call
.I pipe ,
.I fork ,
.I exec ,
and
.I wait ,
which are not currently redefinable.
.PP
The fact that the
.I read ,
.I write ,
.I lseek ,
and
.I close
functions are
redefinable (this refers to the default functions, in addition to
the fact that they are also redefinable on a per-stream basis)
can be exploited to
.IP
build an application on top of a user-mode distributed file
system, which uses special
.I open ,
.I read ,
and
.I write
calls
.IP
engender true
.SM
ANSI
.LG
compliance, by using reserved symbols
(e.g.
.I __read ,
.I __write ,
etc.)
.SH
Miscellaneous Extensions
.PP
This package contains the usual host of gratuitous creeping
features.
The following new functions, described briefly here, are
documented more fully in separate \*(``man\*('' pages.
Though nonstandard, they should be \*(``safe:\*(''
they are not relied upon by other parts of the library,
so user-defined routines which happen to have these names should
not cause problems (other than of course that the overridden
routines are unavailable).
.IP fabort 10
Discard buffered characters, without flushing them.
.IP fdclose
Close a stream, without closing the low-level descriptor.
.IP flushall
Call
.I fflush
on all open streams.
.IP saprintf
Formatted print to dynamically-allocated string.
.IP scanfiles
Call some function for each open stream.
.IP seterrfn
Install an error handler for a stream,
or set the default error handler for all streams.
.IP setfuncs
Install the four low-level I/O functions for a stream,
or set the default functions for all streams.
.IP snprintf
Formatted print to string, specified length not to be exceeded.
.IP stropen
\*(``open\*('' string for stream-like I/O.
.IP straopen
Open dynamically-allocating string stream.
.IP straptr
Return current output pointer for \fIstraopen\fPed stream.
.IP straclose
\*(``Close\*('' \fIstraopen\fPed stream and return final output pointer.
.IP strnopen
\*(``Open\*('' length-specified string for stream-like I/O.
.IP vfscanf
Like
.I fscanf ,
but takes variable argument list pointer (va_list)
rather than explicit variable arguments.
(Analogous to
.I vfsprintf ,
for input)
.IP vscanf
Like
.I vfscanf ,
but assumes stdin.
.IP vsscanf
Like
.I vfscanf ,
but \*(``reads\*('' a string.
.IP vsaprintf
Like
.I saprintf ,
but takes a single va_list
(encapsulating a variable number of arguments)
rather than the variable arguments themselves.
.PP
The internal function
.I _cleanup ,
called by
.I exit
to clean up open streams,
flushes them (\fIfflush\fP) rather than closing them (\fIfclose\fP).
The traditional close occasionally causes problems, to wit:
.IP \(bu
when a user-definable function called during the cleanup process
attempts to print a debugging or diagnostic message, but stdout
and stderr are already closed; or
.IP \(bu
when the program calling stdio is operating under the auspices of
an interpreter or in-process debugger, since it may run multiple
times within the same process, and must keep at least the three
standard file descriptors open.
.PP
.I printf
(and, by extension,
.I fprintf ,
.I sprintf ,
and the others)
accept a few new format specifiers.
These accept the standard field width and other modifiers as well.
.IP %b 8
Binary.
%#b prints a leading \*(``0b\*(''.
.IP %r,%R
Roman numerals.
%r uses lower-case letters; %V upper-case.
.PP
A
.SM
NULL
.LG
pointer passed to %s prints as \*(``(null)\*(''.
.PP
I was about to implement a way for the application to register
new format characters for user-defined conversion,
when I heard that this had already been conceived and implemented
(in 8th edition
.UX
, I believe).
I will implement this function once I learn the specifications
for the existing implementation so I can emulate it compatibly.
.PP
I am investigating the possibility of allowing
.I setvbuf
and
related functions to be called at any time.
(Traditionally, setting the buffer is allowed only immediately
after a stream is opened, before any I/O is performed.)
.SE
Although this extension is theoretically possible, it is proving
to be somewhat expensive in code space, with limited payoff since
no portable application will make use of it.
(In fact, I am not sure that nonportable applications would have
much use for this functionality, either.
The attempt was mainly undertaken on the general principle that
arbitrary limitations should be avoided.)
.SH
Limitations
.PP
Although this package has been conscientiously written,
and will be carefully evaluated,
the breadth of the stdio specification is sufficient that there
may well be areas in which this implementation is inadequate.
.PP
One area in which inadequate implementation is likely is the
simultaneous read/write mode (\s-2_IORW\s0; the \*(``r+\*('',
\*(``w+\*('', and \*(``a+\*('' arguments to
.I fopen ).
The author
is not particularly enthralled with
the use of these modes, and
consequently is unaware of their full implications.
On the other hand,
a version of RCS linked against this implementation
has been performing flawlessly for over a year,
so the
.SM
_IORW
.LG
implementation is not completely without merit.
(It was during a port of RCS to the author's V7
.SM
PDP11
.LG
without
.SM
_IORW
.LG
in the libc.a stdio that the code here was first added.
RCS not only uses \*(``w+\*('' but is also a critical program which
cannot tolerate library malfunction without corrupting files,
so it is a fairly good \*(``torture test\*('' for at least those
aspects of the library which it does exercise.)
.PP
The author is similarly ambivalent about the use of the
.I scanf
family.
The scanf code here presented is a straightforward but uninspired
implementation.
It has been superficially evaluated,
and although it does fix at least one bug
which is present in some of the more standard implementations,
it is likely that it contains a few bugs
pertaining to the more obscure cases.
.SH
Building
.PP
The library is built using the
.UX
utility
.I make ;
the usual Makefile is enclosed.
A number of conditional compilation switches exist; some have
already been mentioned here.
The following list (and the equivalent one in a comment in the
Makefile) attempts to recapitulate all of the available
conditional compilation options.
(Since new ones are continually added, however, only the source
code can be considered definitive in this regard.)
.IP bsd4_2 \w'WRITEERRNOTFATAL'u+2m
The code is being compiled under a 4.2bsd or later or derived system.
The distinction is whether the stat structure contains a "natural
block size" field st_blksize.
.IP CLEANUPCLOSE
The
_cleanup
function, called from
.I exit ,
should close each open stream, rather than simply flushing it.
(The standard stdio implementations typically close files during
.I _cleanup ;
the decision to merely to flush them is peculiar to this package.)
.IP FASTFRDWR
The code for
.I fread
and
.I fwrite
transfers characters between the calling program and the stdio buffer
(and directly to or from the operating system,
bypassing the buffer, if possible)
in large chunks.
.IP FASTNBF
Special cases throughout the code recognize the opportunity to
call the underlying write function with a string of contiguous
characters, rather than mindlessly one at a time, when a stream
is doing unbuffered (_IONBF) output.
.IP FORCECLEANUP
A dummy external reference in a central object file forces the
module in this library containing the
.I _cleanup
routine to be loaded.
Since
.I cleanup
is typically only called by
.I exit ,
and since
.I exit
is loaded from the standard library (/lib/libc.a or the like),
the copy of
.I _cleanup
from the standard library would otherwise typically be loaded.
.IP PUTCLBUF
The line-buffered check for newline characters ('\en') is
performed directly by the
.I putc
macro, rather than being buried in
.I _flsbuf .
The tradeoff is the classic one between execution speed and code
size: the macro version can bloat the object code considerably,
while the non-macro version requires a function call per
character output.
.IP READWRITE
The (quasi) simultaneous read/write mode
(_IORW, fopen "r+", "w+", and "a+")
is enabled.
.IP SAFEREALLOC
The
.I realloc
implementation being used checks its first argument and,
if NULL, essentially performs a
.I malloc .
(SAFEREALLOC is automatically turned on if __STDC__ is defined.)
.IP SETBUFANYTIME
The various
.I setbuf
functions are permitted at any time during the life of a stream,
not necessarily just after opening and before doing any I/O.
(SETBUFANYTIME is not really implemented yet, and may never be.)
.IP STDINFLUSH
When a read is pending on stdin, stdout is flushed.
(This is a historically early form of "line buffering," and is
superceded by true line buffering if it is in effect.)
.IP STICKYEOF
The EOF condition is not only remembered but also checked on
subsequent calls without necessarily bothering to try another read.
The primary consequence is that when a program receives EOF from
a terminal (normally triggered by control-D under
.UX
), but wishes to continue, it must explicitly call
.I clearerr .
.IP
"Sticky" EOF breaks lots of programs.
It was introduced in 4.2bsd, and is (conditionally) included here
for compatibility.
.IP THREEARGOPEN
The system's
.I open
takes an O_CREAT flag and an optional third argument,
thereby superceding the
.I creat
call.
(The THREEARGOPEN distinction isn't really finished yet.)
.IP WRITEERRNOTFATAL
The default error handling function should
.I not
abort the program on write errors.
.LP
A few source files have additional conditional switches which are
enabled or disbled by editing the particular source file, rather
than setting them through the global CFLAGS macro in the Makefile.
The file doprnt.c
(the common code for the
.I printf
family)
has several:
.IP BINARY
The %b extension is enabled.
.IP FLOATING
The floating-point formats (%e, %f, and %g) are enabled.
.IP NOUNSLONG
The compiler does not support the \fCunsigned long\fP type,
so it must be simulated.
.IP NULLPTR
A NULL (0) pointer passed to the %s format is printed as "(null)"
(or whatever string the NULLPTR macro is #defined to).
.IP ROMAN
The Roman numeral formats (%v and %V) are enabled.
.SH
Implementation
.PP
This package has been written from scratch; it contains no
proprietary code.
Some care has been taken to make it compatible with the
\*(``standard\*('' implementations, on more than just a public
interface level.
(By \*(``standard\*('' I refer to the V7 stdio, from which BSD
stdio and\(emI believe\(emSystemIII/V stdio are descended.)
.SE
In particular, the _iobuf structure defined in stdio.h contains
fields with the same names, and in many cases with the same offsets.
A few internal routines, notably
.I _filbuf ,
.I _flsbuf ,
.I _doprnt ,
and
.I _doscan ,
retain the same names and calling sequences.
This consistency should make it easy for those familiar with the
\*(``standard\*('' implementations to understand this version,
and also admits the possibility for object file compatibility,
and the possibility that applications which made nonportable
assumptions about stdio internals will continue to work.
These possibilities are discussed further in the next section.
.PP
The new features implemented by this package primarily involve
new fields in the _iobuf structure; these new fields are
described here.
.PP
The _readfunc, _writefunc, _seekfunc, and _closefunc are pointers
to functions returning int, int, long int, and int, respectively.
The functions take the same arguments as do the corresponding
.UX
system calls read, write, lseek, and close.
Any of the function pointers may contain the value NULL;
the corresponding function is then assumed not to exist.
(When an attempt is made to access a nonexistent function,
the effect is to disallow the requesting operation.
For example, streams without read functions can essentially not,
that is not usefully, be fopened mode \*(``r.\*('')
.PP
By setting all or some of the function pointers to some other
function than the default, and setting an appropriate _file
(\*(``file descriptor\*('') value, it can be arranged that the \*(``I/O\*(''
that underlies a buffered stream involve almost anything.
Examples might include I/O to windows, encryption or decryption,
normal file-descriptor-based I/O with special cleanup functions
on close, etc.
.PP
In case the \*(``handle\*('' for the special I/O functions being used is
a structure pointer (rather than the usual small integer descriptor)
a pointer-sized field is available in the _iobuf struct for this
purpose.
The
.SM
_IOFPTR
.LG
bit in the _flag field indicates that the pointer
field (_fptr) should be passed as the first argument to I/O
functions, rather than _file.
.PP
When the special processing which is to be performed \*(``underneath\*(''
a user-defined stdio stream cannot conveniently be encapsulated
in \*(``read\*('' and \*(``write\*('' functions, it is possible to gain control
at a higher level.
A stream can also have its own _filbuf and/or _flsbuf function
installed, which will gain control whenever the buffer
under/overflows.
The _filbuf and _flsbuf functions (and their replacements) have
access to the complete _iobuf structure, and can therefore
interact with the buffer in arbitrary ways.
The primary example is the saprintf function, which dynamically
grows the buffer, in-memory, to keep it big enough for the string
being formatted.
.PP
Customized _filbuf and _flsbuf functions are potentially difficult to write,
require access to the normally private fields of the _iobuf
structure (therefore requiring recompilation if not rewriting
when that structure changes), are nonportable, and are
potentially short-lived as other internals of this package change.
(The only current use of a such a customized function is the
special _flsbuf function which arranges for the dynamic growth
of the outbut buffers for
.I saprintf ,
.I vsaprintf ,
and
.I straopen .)
.PP
Not surprisingly, this library contains a number of \*(``internal\*(''
routines, not to be accessed by calling programs, but which
nonetheless have names in global scope.
The naming conventions for such routines are often problematical.
.PP
Under the old rules, leading underscores were informally reserved
to \*(``the implementation,\*('' so library writers typically named
internal routines with leading underscores and crossed their
fingers and hoped that there were no name clashes between
libraries.
(This is relatively easy when there is exactly one standard C
library containing multiple facilities.)
.PP
ANSI C has, in this area as in so many others, clarified and
strengthened the rules quite a bit.
Names with a single leading underscore have been abandoned to the
application; internal library routines must now begin with two
underscores or with an underscore followed by an uppercase letter.
.PP
A standalone library implementation such as this one falls into a
grey area, however.
It should not use names beginning with two underscores, since
those are now officially reserved to the implementor of the C
library delivered with the compiler.
On the other hand, it should not use names beginning with one
underscore, because those are now available to the application.
.PP
Presumably, the application level is now allowed to use names
beginning with a single underscore so that it may usurp the
\*(``leading underscores are internal library routines\*('' convention
for use by its own, project-level libraries.
Since, from the point of view of the compiler, this standalone
stdio library is more like a project library, and since single
underscores were used by the historical implementations with
which (for now) I am attempting to maintain object-level
backwards compatibility (see below), this library currently
uses single underscores (followed by lower-case letters) for
its internal symbols.
.PP
It is expected that the names of these internal symbols may
change as the proper role and implementation strategies for
standalone C sublibrary replacements become better defined.
.SH
Object-Level Compatibility
.PP
Consistency at the object file level means that, under certain
circumstances, object files and libraries which were compiled
against the standard <stdio.h>, and intended to be linked against
the standard library, may be linked against this package without
recompiling.
Additionally, source files that (antisocially) inspect or modify
the contents of an _iobuf structure may compile and function
correctly with this version.
.PP
Object-level compatibility obviously cannot be guaranteed.
Here are a few guidelines outlining the likelihood for success in
such an endeavor:
.IP 1.
Object files which reference only the _ptr, _cnt, and _base
fields are almost guaranteed to work, provided they do so in
\*(``normal\*('' ways, calling _filbuf or _flsbuf as appropriate
when the buffer is empty or full.
.IP 2.
Object files which reference stdout or stderr are guaranteed
.I not
to work, since the size of a struct _iobuf is considerably larger
in this version, and &_iob[1] therefore has a different offset.
This means, for example, that:
.RS
.IP
an old fprintf.o will work
.IP
an old printf.o will not work
.IP
an object file that calls only printf will work (provided it is
linked against an updated printf.o)
.IP
an object file that calls fflush(stdout)
or fprintf(stderr, ...)
will not work.
.RE
.IP 3.
Source files which reference the standard fields, and use the
standard values for the _flag field, are likely to work, although
there are certainly source files out there which make assumptions
so dangerous that they will only work with the exact version of
stdio which they were written against.
.IP
In particular, I expect that many printf lookalikes (varargs
functions that call _doprnt themselves) will continue to work,
although of course they would be well-advised to convert to
vprintf or vfprintf.
scanf lookalikes (how common are they?) that call _doscan should
work as well (they have, as yet, no portable vscanf to call).
sprintf emulators, which handcraft a stack-allocated
_iobuf, may also work, but will probably crash if they overflow
the buffer, since an uninitialized _flsbuf function will likely
be called even before
.SM
_IOSTRG
.LG
is checked.
sscanf emulators are almost guaranteed to fail at \*(``EOF\*('' for this
reason.
