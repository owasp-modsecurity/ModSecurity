Design notes for source code
==
This file give some explanations and guidelines regarding ModSecurity v2 source code.
The goal is to discuss topics that are not related to a specific location in the code, so that cannot be best explained by comments.
The goal is not to replace comments where it is probably better.
It's quite short for the moment, but the goal is to extend it from time to time.

## Null pointer check
The default behaviour is to check for null pointer dereference everywhere it may be needed.
In case a pointer cannot be null, it has to be explained with a comment at the beginning of the function of when dereferencing the pointer.
On top of that, an explicit check should be done when compiling in debug mode with the following code:
```
    assert(mypointer);
```
In case a pointer that cannot be null is used at several locations (say more than 3 times),
the explanation could be given globally in this file.

### Pointers never null
The following pointers can never be null:

#### msr

msr is assigned at the following places:
- mod_security2.c (14 x): initialization
In all the above calls, and all calling functions, it immediately returns (with an error code) in case msr is null, up to a place where no mod_security2 processing at all occurs.
In subsequent calls, there's thus no possibility to have msr null.
- apache2_io.c (2 x): assign a previously initialized msr
- msc_json (9 x):     assign a previously initialized msr
- msc_lua.c (4 x):    assign a previously initialized msr
