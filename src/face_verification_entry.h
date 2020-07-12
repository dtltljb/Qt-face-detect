#ifndef FACE_VERIFICATION_TEST_H
#define FACE_VERIFICATION_TEST_H

int face_verification_entry(void * argv) ;

/**
 * Convert RGB24 file to BMP file
 * @param rgb24path    Location of input RGB file.
 * @param width        Width of input RGB file.
 * @param height       Height of input RGB file.
 * @param url_out      Location of Output BMP file.
 * @note eg:
 *  simplest_rgb24_to_bmp("lena_256x256_rgb24.rgb",256,256,"output_lena.bmp");
 */
int simplest_rgb24_to_bmp(const char *rgb24path,int width,int height,const char *bmppath);

#endif // FACE_VERIFICATION_TEST_H
