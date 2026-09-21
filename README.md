# DLIB

DLIB stands for Dor's Library.
This is a personal C-Library Windows Platform support ONLY.

The word "personal" means that i do not take any responsabilty for your usage but you are welcome to use how ever you like.

NOTES: 
- this library is IN-PROCESS it has some cool things but nothing is finished yet.

- This library is inspired by nothings/stb style and i enjoy header only copy-paste-use so i did it for myself. Althogh there are some dependecies in those library so you might need to use multiple files while actually need one. This is unfortenate and i plan to take care of this in the future.

## HOW TO USE
usage is by copieng and pasting the header to your repo (and all the headers include in it).

Then you need to create a source file [dlib.c] with this content
```c
    // This define include implementation it should be defined only once in your entire application. 
    #define NAME_OF_HEADER_IMPLEMENTATION
    #include "name_of_header.h"
```
and in any other source files where you need it you use it as simple include ```#include "some_header_file.h"``` no defined implementation.

That's it Enjoy!.


For more detailed info you can read:
- 

- [`dbuild.h`: *Build tool for Windows-C-Applications*](wiki/DBUILD.MD)
- [`dstring.h`: *String handeling Utils*](wiki/DSTRING.MD)
- [`darena.h`: *Memory Arena*](wiki/DARENA.MD)
- [`darray.h`: *Very-Fast Dynamic Array*](wiki/DARRAY.MD)
- [`dfs.h`: *File-System handeling for Windows-C-Applications*](wiki/DFS.MD)
- [`dhash_table.h`: *Hash-Table*](wiki/DHASH_TABLE.MD)
- [`dqueue.h`: *Fast Dynamic-Ring-Queue*](wiki/DQUEUE.MD)
