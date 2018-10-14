"# GEMasist" 

GEM Assist                                            16-Dec-2017     P. Slegg
==========


Background
----------
I am not a fan of having to use a command line on the Atari, it is just
wrong to use a shell on a system that had one of the earliest WIMP
environments.

Two things were frustrating me.
1. At the time the PurePascal shell didn't work with Freemint/XaAES
   and compiling from the .ttp was a chore.
2. I was also using CVS from the command line.

Initially I planned to create a simple graphical interface to use common
CVS functions but then the thought occurred to me to create an application
that could be used to wrap all sorts of simple command line utilities.

I use such utilities infrequently and when I come to use them again I
have forgotten the exact switches I need. By setting up GEM Assist I
don't need to remember the command sequence, that was always the attraction
of a decent user interface.

If you are a committed cli user then you might not have a need for
GEM Assist.


Keep It Simple Stupid
---------------------
Just to be clear this wasn't meant to create the perfect, all-singing,
all-dancing graphical interface to a command line utility. Only a
custom interface can do that but what do you do if there isn't one ?

The idea was to allow users to be able to create a simple interface
relatively quickly, adding the commands that they need at the time and
making it easy to add other commands later.

These files can be shared amongst users, each adding more functions
and opening up usage of cl apps to others.


Structure
---------
When using a command line application that you are not familiar with it is
quite common to have to refer to the --help or the man pages. These provide
lists of switches and their meaning. My idea was that this information could
be used by GEM Assist to automatically generate a simple dialogue box.

XML was chosen as the file format because it lends itself nicely to storing
simple information like the command switch that will be sent to the
utility and a label to show in the dialogue box.

Various types of switch are needed, some are optional (check boxes) and some
exclusive (radio buttons - which have to be grouped) and of course input
and output fields for filenames are needed.

In this version of GEM Assist the XML file does not contain any information
about formatting of the dialogue box content. The layout is automatic
but very simple, so might not produce the most elegant result[1]. This is
part of the KISS design.

It should be remembered that graphical interfaces can reduce the flexibility
offered by some command line utilities. Also, there are only a few restrictions
on which command switches can be used, so it will be possible to send meaningless
commands to the utility, this is a little bit better and no worse than when using
the command line. At least the syntax will be correct even if the logic isn't.


Usage
-----
If you all ready have an XML file for your command line app then just
drag&drop it onto the GEM Assist application.

Alternatively, can create a file association in your desktop and launch
it that way or add the app to your desktop config and pass the
path to the xml file.

Run the GEM Assist as a GTP and you can open any suitable xml file.

When launched, GEM Assist opens a dialogue, the content will depend on the
XML file but there will be two buttons. One to cancel and the other to
launch the command line utility with the settings you have chosen.

If you are creating an xml file and need help, then adding the debug
option (-d) when starting GEM Assist will print various messages to guide
you.


Examples
--------
1. ls
Let us look at a simple example which is about as useful as Hello World.
Suppose we want to use GEM Assist to control the ls command with its
switches  -a  -l.




2. jpegtran
A utility you might not be familiar with. It can be used to manipulate jpeg
images with switches for rotate, greyscale, etc.

  -rotate 90|180|270
  -grayscale
  -flip horizontal|vertical

From this information I can see that the dialogue box should have a
3-button  radio button group for rotate, a checkbox for grayscale and
a 2-button radio group for flip.

An input filename is needed as well as an output one.

jpegtran does not have a switch for the output file, it just dumps its
output to stdout. If the output is prefixed with a > then it can be
sent to a suitable file.




Other uses I can think of: dhcp, samba, route, ifconfig, netstat,
archive utils like tar and bzip...



Config Files
------------

There is another function to GEM Assist that has not been developed very far.

When development started I quickly realised that most of the functionality
would lend itself to setting the options in a config file. Clearly this would
depend on the size and scope of the config file and the complexity of the
options in it but simple on/off values, filenames, ip addresses could easily
be managed through a GEM Assist dialogue.

Currently GEM Assist is unable load a config and parse it to set the dialogue
box values. Ideally it would do this and then save the config file, changing
only those values that is has been asked to do.



Future
------
[1] Layout, there are two ways forward:
      a) add lots of x-y positions into the XML
      b) improve the automation of the layout with the aid of group or
         column instructions in the XML

    My preference is for the latter with the intention of keeping the XML
    as simple as possible.


