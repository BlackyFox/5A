#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "error.h"
#include "jpeg_stegano.h"

/*********************** init_jpeg_img() ********************************
Memory allocation and initialisation of a JPEGimg structr
Returns a pointersto the struct if correctly allocated,
        NULL in case of error

ARGS: (void)
*************************************************************************/
JPEGimg *
init_jpeg_img ( void )
{
	JPEGimg * img = NULL;
	if ( (img = (JPEGimg*) malloc (sizeof(JPEGimg)) ) == NULL)
	{
		print_err ("init_jpeg_img", "img", ERR_MEM);
		return NULL;
	}
	img->dctCoeffs = NULL;
	img->virtCoeffs = NULL;
	
	return img;
}

/************************* free_jpeg_img() *****************************
Free memory
Returns EXIT_SUCCESS
        a negative value in case of failure

ARGS: JPEGimg *img: structure to free
************************************************************************/
int free_jpeg_img ( JPEGimg *img )
{
	// Checkargs
	if (!img)
		return ERR_ARG;
	
	// Free memory
	if (img->dctCoeffs)
	{
		free (img->dctCoeffs);
		img->dctCoeffs = NULL;
	}
	
	jpeg_destroy_decompress(&(img->cinfo));
	free(img);
	img = NULL;
	
	return EXIT_SUCCESS;
}


/************************* jpeg_read() **********************************
Reads a jpeg image, store information and DCT coefficients
Returns a pointer to the structure created,
        NULL in case of failure
Retourne un code d'erreur

ARGS: char *path:       Path of the image
*********************************************************************/
JPEGimg *
jpeg_read (char *path)
{
	int comp;
  struct jpeg_error_mgr jerr; // Handle errors
  FILE *infile = NULL;
  JPEGimg *img = NULL;
	
	// Check args
	if (!path)
	{
		print_err ("jpeg_read()", "path", ERR_ARG);
		return NULL;
	}
	
	// Open path
	if ((infile = fopen(path, "rb") ) == NULL)
	{
		print_err ("jpeg_read()", path, ERR_FOPEN);
		return NULL;
	}
	
	// Memory allocation for img
	if ((img = init_jpeg_img()) == NULL)
	{
		fclose (infile);
		return NULL;
	}
	
	/* Initialize the JPEG decompression object with default error handling. */
  img->cinfo.err = jpeg_std_error (&jerr);
  jpeg_create_decompress (&(img->cinfo));
  
  /* Specify data source for decompression */
  jpeg_stdio_src (&(img->cinfo), infile);
  
  // Read header
  (void) jpeg_read_header (&(img->cinfo), TRUE);
  
	/* Get DCT coefficients
	 * dct_coeffs is a virtual array of the components Y, Cb, Cr
	 * access to the physical array with the function
	 * (cinfo->mem -> access_virt_barray)*/
	img->virtCoeffs = jpeg_read_coefficients (&(img->cinfo));
	
	// Structure allocation
	img->dctCoeffs = (JBLOCKARRAY*) malloc (sizeof(JBLOCKARRAY) * img->cinfo.num_components );
	if (img->dctCoeffs == NULL)
	{
		print_err ("jpeg_read()", "img->dctCoeffs", ERR_MEM);
		fclose (infile);
		return NULL;
	}
  
  // Loop on the components of the virtual array to get DCT coefficients
	for(comp = 0; comp < img->cinfo.num_components; comp++)
	{
  	img->dctCoeffs[comp] = (img->cinfo.mem -> access_virt_barray)((j_common_ptr) &(img->cinfo),
       img->virtCoeffs[comp], 0, 1, TRUE);
  	printf("%d\n", &img->virtCoeffs[comp]);
	}
  
  // free and close
  fclose (infile);
                        
	return img;
}

int rst_dct_block (JPEGimg *img, int block)
{
	int i,j;

	if (block > img->cinfo.num_components-1)
	{
		char s[1];
		sprintf(s, "%d", block);
		print_err ("rst_dct_block()", s, ERR_RST_DCTB);
		return 1;
	}
	else{
		for (i = 0; i < img->cinfo.comp_info[block].height_in_blocks; i++)
		{
			for (j = 0; j < img->cinfo.comp_info[block].width_in_blocks; j++)
			{
				img->dctCoeffs[block][i][j][0]=0;
			}
		}
		//printf("Okay\n");
		return 0;
	}
}

/******************** jpeg_write_from_coeffs() **************************
Write a jpeg image from its DCT coeffs
Returns EXIT_SUCCESS, 
        a negative value in case of error

ARGS: char *outfile: Path of the new image
      JPEGimg *img:  Structure containing DCT coeffs and infos of the image
************************************************************************/
int 
jpeg_write_from_coeffs (char *outfile, JPEGimg *img)
{
	struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
	FILE *output = NULL;

	// Open file
	if ((output = fopen (outfile, "wb")) == NULL)
	{
		print_err( "jpeg_write_from_coeffs()", outfile, ERR_FOPEN);
		return ERR_FOPEN;
	}
	
	/* Initialize the JPEG compression object with default error handling. */
  cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	
	/*telling, where to put jpeg data*/
	jpeg_stdio_dest(&cinfo, output);

	/* Applying parameters from source jpeg */
	jpeg_copy_critical_parameters(&(img->cinfo), &cinfo);

	/* copying DCT */
	jpeg_write_coefficients(&cinfo, img->virtCoeffs);

	/*clean-up*/
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose (output);
	
	/*Done!*/
	return EXIT_SUCCESS;
}




/***************************** main() ***********************************
Main function (see help for more details)
************************************************************************/
int main (int argc, char ** argv)
{
	int i; // Loop variable
	int return_value;
	JPEGimg *img = NULL;
	
	// Check number of arguments of the command line
	if (argc < 3)
	{
		printf ("%s: Reads a jpeg image and write it in a new file\n", argv[0]);
		printf ("Not enough arguments for %s\n", argv[0]);
		printf ("Usage: %s <cover.jpg> <copy.jpg>\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	srand((int)time(NULL));
	// read image and ...
	img = jpeg_read (argv[1]);
	// ...print some informations and finally ...
	printf ("\nNumber of components: %d\n\n", img->cinfo.num_components);
	for (i=0; i<img->cinfo.num_components; i++)
	{
		printf("%d\n", &img->virtCoeffs[i]);
		printf("%d\n", &img->cinfo.comp_info[i]);
		printf ("Component %d:\n   %d lines, %d columns\n\n", i, 
		  img->cinfo.comp_info[i].height_in_blocks, img->cinfo.comp_info[i].width_in_blocks);
	}
	//img->virtCoeffs[2]=0;
	//img->dctCoeffs[1][0][0][2]=0;
	//img->dctCoeffs[2]=0;

	if (rst_dct_block(img, 2) == 1)
	{
		return EXIT_FAILURE;
	}

	// ... write it in a new file
	return_value = jpeg_write_from_coeffs (argv[2],img);
	if (return_value == EXIT_SUCCESS)
		printf ("Image written in %s\n", argv[2]);	
	
	free_jpeg_img ( img );
	
	return EXIT_SUCCESS;
}
