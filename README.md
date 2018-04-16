Ok so the only things you need to worry about here are:

the include folder,

the src folder,

dragon.cc which is the main method,

and CMakeLists.txt which controls the cmake command.




Everything else I've done for you by modifying CMakeLists so that it copies
all the other files into the -Build directory. They're only here because they
need to be copied over.


Note: occasionally after compiling an executable, the makefile will "link" the
documents from the source folder. This means it will actually delete the files
in the -Build directory and copy over the files from the source directory (DRAGON/).
When this happens, all changes to vis.mac, run.mac, etc will be lost if they were
only saved in the -Build directory. That's why I recommend leaving a version of
the files in the source directory so, if it does the linking, you'll have the most
up-to-date version of the file copied to the -Build directory.

What this means is that your CMakeLists.txt has to be ready to copy almost everything
over to the -Build directory. I've already editted so that it does that for you.
Just make a note that's why there's so many files in the Source directory that you
don't really need to care about - they're only here because of this "linking" issue.
