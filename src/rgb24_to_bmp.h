#ifndef RGB24_TO_BMP_H
#define RGB24_TO_BMP_H

/**
 * Convert RGB24 file to BMP file
 * @param rgb24_buffer input RGB buffer
 * @param width        Width of input RGB buffer.
 * @param height       Height of input RGB buffer.
 * @param url_out      Location of Output BMP file.
 */
int rgb24_to_bmp( char *rgb24_buffer,int width,int height,const char *bmppath);

#endif // RGB24_TO_BMP_H
