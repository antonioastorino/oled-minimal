# imager.c

This tool is written to create a binary black-and-white image from a text file. The input text file format is N lines (rows) by M characters (cols) per line. The allowed characters are `0` and `.`. The file will interpret the character `0` as a white pixel and the `.` as a black pixel.

The output text can be copy-pasted into your c/cpp code to initialize the 2D that represents the image.

Build by running `./build.sh` from the tools folder.
