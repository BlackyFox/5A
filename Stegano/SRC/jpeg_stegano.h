#ifndef JPEG_STEGANO_H_
#define JPEG_STEGANO_H_

//#include <jpeglib.h>
#include "jpeg-8/cdjpeg.h"		/* Common decls for cjpeg/djpeg applications */
#include "jpeg-8/jversion.h"		/* for version message */

#define MAX_DCT_VALUE (255*32)
#define MIN_DCT_VALUE (-255*32)
#define NB_DCT_COEFFS (MAX_DCT_VALUE - MIN_DCT_VALUE + 1)

//typedef struct jpeg_decompress_struct sjdec;

/* Structure of main informations of a JPEG image */
typedef struct JPEGimg_s
{
	JBLOCKARRAY * dctCoeffs; // This is a 4-dimansion array. A coeff value is available in <structName>.dctCoeffs[comp][lin][col][pos]
	struct jpeg_decompress_struct cinfo; // Image informations such as number of components, size etc.
	jvirt_barray_ptr * virtCoeffs; // /!\ Do not modify this pointer of content /!\.
} JPEGimg;

/******************** FONCTIONS of jpeg_stegano.c ****************************/

JPEGimg *init_jpeg_img ( void );
int free_jpeg_img ( JPEGimg *img );

JPEGimg * jpeg_read (char *path);

int jpeg_write_from_coeffs (char *outfile, JPEGimg *img);
                            
#endif /*JPEG_STEGANO_H_*/
