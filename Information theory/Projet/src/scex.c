#undef DEBUG
#include "include.h"

#define POLY1 0xEF75L
#define POLY2 0x58C2FL
#define POLY3 0x739583L

#define MASK1 0x1FFFFL
#define MASK2 0x7FFFFL
#define MASK3 0x7FFFFFL

mot32  motpar( mot32 w)
 {
  w ^=(w>>16);
  w ^=(w>> 8);
  w ^=(w>> 4);
  w ^=(w>> 2);
  w ^=(w>> 1);
  
  return(w&01L);
 }

/***********************************************************/
/* Systeme de chiffrement par flot scex. Ce systeme est un */
/* exemple pour etudier l'attaque de Siegenthaler.         */
/* La clef fait 59 bits (8 octets)                         */
/*                                                         */
/* Usage scex d|e infile outfile "clef"                    */
/*                                                         */
/* CBA FILIOL Eric                                         */
/***********************************************************/
int main(int argc, char * argv[])
 {
  register mot32 reg1, reg2, reg3, reb; 
  mot32 i, j;
  FILE * fin, * fout;
  mot08 outblock, lettre, f[8] = {0,0,0,1,0,1,1,1}, x;

  /***************************************/
  /*     Ouverture des fichiers          */
  /***************************************/
  fin = fopen(argv[2],"r");
  fout = fopen(argv[3],"w");

  /***************************************/
  /* Mise a la clef des registres        */
  /***************************************/
  reg1 = (mot08)(argv[4][0]) | ((mot08)(argv[4][1]) << 8) | ((mot08)(argv[4][2]) << 16);
  reg1 &= MASK1;

  reg2 = (mot08)(argv[4][2]) | ((mot08)(argv[4][3]) << 8) | ((mot08)(argv[4][4]) << 16);
  reg2 >>= 1;
  reg2 &= MASK2; 

  reg3 = (mot08)(argv[4][4]) | ((mot08)(argv[4][5]) << 8) | ((mot08)(argv[4][6]) << 16) | ((mot08)(argv[4][7]) << 24);
  reg3 >>= 4;
  reg3 &= MASK3;

#ifdef DEBUG
  reg1 = 0x3130;
  reg2 = 0x21999;
  reg3 = 0x66C6A6;
  printf("Etat initiaux des registres : %lx %lx %lx\n", reg1, reg2, reg3);
#endif
  printf("Crypto %s - Etat initiaux des registres : %lx %lx %lx\n", argv[3], reg1, reg2, reg3);

  /***************************************/
  /* Generation de la suite chiffante    */
  /***************************************/
  j = 0L;
  while(fscanf(fin,(argv[1][0] == 'e')?"%c":"%02hhX",&lettre), !feof(fin))
   {
    j++;
    outblock = 0;
    for(i = 0;i < 8;i++)
     {
      x = f[(reg1 & 1) | ((reg2 & 1) << 1) | ((reg3 & 1) << 2)];
      outblock |= (x << i);
      
      reb = motpar(reg1 & POLY1);
      reg1 >>=1;
      reg1 |= reb?0x10000L:0L;
      /* Autre solution reg1 |= (reb << 16); */
      
      reb = motpar(reg2 & POLY2);
      reg2 >>=1;
      reg2 |= reb?0x40000L:0L;
      /* Autre solution reg2 |= (reb << 18); */

      reb = motpar(reg3 & POLY3);
      reg3 >>=1;
      reg3 |= reb?0x400000L:0L;
      /* Autre solution reg3 |= (reb << 22); */
     }
#ifdef DEBUG
    printf("%lx\n",outblock);
#endif
    if(argv[1][0] == 'e') fprintf(fout,"%02X",lettre^outblock);
    else fprintf(fout,"%c",lettre^outblock);
   }
  printf("\n");

#ifdef DEBUG
  printf("Nombre de lettres traitees : %d\n",j);
#endif
  fclose(fin);
  fclose(fout);
 }
