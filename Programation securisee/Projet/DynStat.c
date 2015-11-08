#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* --------------------------------------------------------- */
/* Objectif : Corriger les erreurs statiques et dynamiques   */
/*            de ce programme                                */
/* --------------------------------------------------------- */

/* Correction made by Antoine Puissant */


int main(int argc, char *argv[])
{
	/* Creation of the variables and initialization */
	int size = 16;
    int length = 0;
	char* buf1 = NULL;
	char* buf2 = NULL;
	char* testdubuffer = NULL;
	int ret = 0;

	/* Checking if we have only two arguments given */
    if (argc == 3)
    {
    	/* Memory allocation for the two args given */
    	/* Allocation for the size of the argument (max = size = 16) */
    	buf1 = (char*)calloc(strnlen(argv[1], size)+1, sizeof(char));
    	buf2 = (char*)calloc(strnlen(argv[2], size)+1, sizeof(char));

    	/* Memory allocation for the "testdubuffer" variable */
    	testdubuffer = (char*)calloc(strnlen("testdubuffer", 12)+1, sizeof(char));

    	/* Writing argv[1] in buf1, argv[2] in buf2 and "testdubuffer" in testdubuffer */
		snprintf(buf1, strnlen(argv[1], size)+1, "%s", argv[1]);
    	snprintf(buf2, strnlen(argv[2], size)+1, "%s", argv[2]);
    	snprintf(testdubuffer, strnlen("testdubuffer", 12)+1, "%s", "testdubuffer");

    	/* Outputing buf1, then buf2 */
		printf("%s\n", buf1);
		printf("buffer : %s\n", buf2);
		
		/* Memory liberation of buf2. Then reallocation for the right size */		
		free(buf2);
		buf2 = NULL;
		buf2 = (char*)calloc(strnlen(testdubuffer, 12)+1, sizeof(char));

		/* Wrinting testdubuffer in buf2 */
		snprintf(buf2, strnlen(testdubuffer, 12)+1, "%s", testdubuffer);

		/* Getting size of buf2 */
		length = strnlen(buf2, 12)+1;

		/* If writing worked (size>0), outputing buf2 and free memory */
	    if(length > 0){
	        printf("buffer : %s\n", buf2);
	    /* If writing failed, free memory and exit with error code 3 */
	    }else{
	    	perror("Copy of testdubuffer to buf2 failed");
	    	ret = 3;
		}
    	free(buf2);
    	free(buf1);
    	free(testdubuffer);
    	buf2 = NULL;
    	buf1 = NULL;
    	testdubuffer = NULL;
    	size = 0;
    	length = 0;
    	return ret;
    }else{
    	if (argc < 3)
    	{
    		/* If there are less than two arguments given, exiting with error code 1 */
    		perror("Too few arguments");
    		return 1;
    	}else{
    		/* If there are more than two arguments given, exiting with error code 2 */
    		perror("Too many arguments");
    		return 2;
    	}
    }
}