# imager.c

This tool is written to create a binary black-and-white image from a text file.
The input text file format is N lines (rows) by M characters (cols) per line.
The allowed characters are `0` and `.`. The file will interpret the character `0` as a white pixel and the `.` as a black pixel.
For this specific display, the cursor size is 8x8 pixels. Therefore, M should be 8. However, the number of rows can be a multiple of 8 (see below); 

The output text can be copy-pasted into your c/cpp code to initialize the 2D that represents the image.

```bash
$ ./build.sh                                            # build imager.c
$ ./build/imager assets/right_rhombus_16x8.txt 16 8     # create binaries for a 16x8 image 
```
The output will be
```
Read: 144 bytes
{
{0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00, },
{0x7f, 0x3f, 0x1f, 0x1f, 0x0f, 0x07, 0x03, 0x01, },
}
```
This output is used as it is in `oled.cpp`, mapped to the character 'R' and used in the example sketch `oled.ino`.
