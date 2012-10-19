
How to build and run HelloWorld1 demo:
(1) emake               // (recursively) build target files in the directory
(2) pd @                // push current directory onto command stack,
                        // then jump to the target directory (for testing).
(3) HelloWorld1.exe     // execute the HelloWorld1.exe demo
(4) pd                  // pop command stack,
                        // i.e., jump back to the previously working directory.


Other useful commands:
(1) emake clean         // clean up the target files in the mirror directory
(2) pd ~                // push current directory onto command stack,
                        // then jump to the mirror directory (of built files).
(3) emake clobber       // clean up the whole project in the target directory

Things to remember:
(1) the symbol "@" resembles a "target" in the real world.
(2) the symbol "~" resembles a "distorting mirror" in the real world.

