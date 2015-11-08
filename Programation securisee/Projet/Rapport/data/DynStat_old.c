/* --------------------------------------------------------- */
/* Objectif : Corriger les erreurs statiques et dynamiques   */
/*            de ce programme                                */
/* --------------------------------------------------------- */

int main(int argc, char *argv[])
{
	/* On ne touche pas à la variable "size" */
	int size = 16;
	int length = 0;
	char * buf;
	
	printf(argv[1]);
	
	buf = (char*)malloc(sizeof(char)*size + 1);
	
	strcpy_s(buf, sizeof(buf), argv[2]);
	printf("buffer : %s\n", buf);
	
	/* ----------------------------------------------------- */
	/* On ne touche pas au contenu "testdubuffer"            */
	/* mais on peut créer une variable reprenant ce contenu  */
	/* ----------------------------------------------------- */
	
	strcpy_s(buf, sizeof(buf), "testdubuffer");
	
	length = strlen(buf);
	if(length != 0)
		printf("buffer : %s\n", buf);
	else
		free(buf);
		
	return 0;
}