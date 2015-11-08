#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LEN 256

int desalloc(char* b1, char* b2){
	if (b2 != NULL)
	{
		free(b2);
		b2 = NULL;
		if (b1 != NULL)
		{
			free(b1);
			b1 = NULL;
		}
	}
	return 0;
}

int main(int argc, char **argv)
{

	/* Initialisation des variables */

	FILE *f = NULL;
	long fsize = 0;
	char* buffer = NULL;
	char* buffer2 = NULL;
	int i = 0;
	int fd = 0;

	/* Verification du nombre d'arguments */
	
	if (argc == 2)
	{
		/* Allocation d'un char* pour le nom du fichier */
	
		buffer2 = (char*)calloc(strnlen(argv[1], MAX_LEN)+1, sizeof(char));
	
		/* Ecriture du nom du fichier dans le buffer2 */

		snprintf(buffer2, strnlen(argv[1], MAX_LEN)+1, "%s", argv[1]);
		
		/* Ouverture du fichier en lecture uniquement */

		fd = open(buffer2, O_RDONLY);
		
		/* Si l'ouverture echoue, on arrete */

		if (fd < 0)
		{
			printf("Open call failed.\n");
			return 6;
		}else{
			/* Si l'ouverture reussi, on lie le fichier au flux */

			f = fdopen(fd, "r");
			
			
			if (f != NULL)
			{
				if (fseek(f, 0, SEEK_END) == 0)
				{
					fsize = ftell(f);
					/*rewind(f);*/
					if (fseek(f, 0, SEEK_SET) == 0)
					{
						buffer = (char*)calloc(fsize+1, sizeof(char));
						for (i = 0; i < fsize; ++i)
						{
							if (fread(&buffer[i], 1, 1, f) == 0)
							{
								printf("Error in reading.\n");
								return 5;
							}
						}
						printf("%s", buffer);
						
						desalloc(buffer, buffer2);

						i = 0;
						fsize = 0;
						
						fclose(f);
						f = NULL;
						
						printf("\n");
					}else{
						perror("Cannot seek start.");
						desalloc(buffer, buffer2);
						return 7;
					}
				}else{
					printf("Cannot seek end.\n");
					desalloc(buffer, buffer2);
					return 4;
				}
			}else{
				printf("File %s not openable\n", argv[1]);
				desalloc(buffer, buffer2);
				return 3;
			}
		}
	}else{
		desalloc(buffer, buffer2);
		if (argc > 2)
		{
			printf("Too many arguments.\n");
			desalloc(buffer, buffer2);
			return 1;
		}
		printf("Too few arguments\n");
		desalloc(buffer, buffer2);
		return 2;
	}
	return 0;
}