#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>
//#include "labengine.h"


#define maska(i,j)  ((((j) + (i)) % 2) == 0)


typedef struct
{
	int pole, sluzh;
}pol;

void kvadrat(pol qrcode[21][21],int x,int y)
{
	//otrisovka poiskovogo uzora
	int i,j;
	//levo
	for (i = y+1; i < y+7; ++i)
	{
		qrcode[i][x].pole = 1;

	}
	//vverh
	for (j = x; j <x+ 7; ++j)
		qrcode[y][j].pole = 1;


	//kvadrat
	for (i = y+2; i < y+5; ++i)
		for (j = x+2; j < x+5; ++j)
			qrcode[i][j].pole = 1;

	//pravo
	for (i = y; i < y+7; ++i)
	{
		qrcode[i][x + 6].pole = 1;

	}
	//niz
	for (j = x; j < x+7; ++j)
		qrcode[y + 6][j].pole = 1;
}


int perevod_v_dv(int zh,int pole[152],int a,int d)
{
	int i, j;
	if (d==3)
		i = 10;
	else
		if (d==2)
			i = 7;
		else
			i = 4;
	 j = i;
	while (j > 0)
	{
		pole[zh + j - 1] = a % 2;
		a /= 2;
		j--;
	}
	zh += i;
	return zh;
}

int getstring(char *stroka)
{
  int length=0;
  char ch;
  	printf("Vvedite cifri <= 51\n");
	while ((ch = getc(stdin)) != '\n')
	{
		if (ch < 48 || ch > 58)
		{
			printf("ERROR NOT NUMBER\n");
			return -1;
		}
			
		if (length == 51)
		{
			printf("ERROR TOO MACH NUMBERS");
			return -1;
		}
		stroka[length] = (char) ch;
		++length;

	}
  return length;
}

int razbienie(int length,int pole[152],char *stroka,int zh)
{
  int i = 0;
	int k = 3;
  int a=0;
  int d;
	while (i != length)
	{
		d = 0;
		while ((i != length) && (i != k))
		{

			a = a * 10 + stroka[i] - 48;
			d++;
			i++;
			
		}
			
		k += 3;
		zh = perevod_v_dv(zh, pole, a,d);
		
		a = 0;
	}
  return zh;
}


int zapolnenie(int zh,int pole[152])
{
  int g=0;
  if (zh!=152)
	zh++; 
	//zapolnenie 0000 and 1101100 and 0010001
	if (zh!=152)
	while (zh % 8 != 0)
		++zh;
	
	while (zh < 152)
	{
		if (g % 2 == 0)
		{
			pole[zh] = 1;
			pole[zh + 1] = 1;
			pole[zh + 2] = 1;
			pole[zh + 3] = 0;
			pole[zh + 4] = 1;
			pole[zh + 5] = 1;
			pole[zh + 6] = 0;
			pole[zh + 7] = 0;


			++g;
			zh += 8;
		}
		else
		{
			pole[zh] = 0;
			pole[zh + 1] = 0;
			pole[zh + 2] = 0;
			pole[zh + 3] = 1;
			pole[zh + 4] = 0;
			pole[zh + 5] = 0;
			pole[zh + 6] = 0;
			pole[zh + 7] = 1;


			++g;
			zh += 8;
		}

	}
  return zh;
}

void perevod_v_des(int pole2[19],int pole[152])
{
  int i,ssh,j;
  for (i = 0; i < 19; ++i)
		pole2[i] = 0;
	ssh = 0;
	while (ssh != 152)
	{
		j = 0;
		for (i = ssh + 7; i >= ssh; --i)
		{

			pole2[(ssh) / 8] += pole[i] *(int) pow(2., j);
			j++;
		}
		ssh += 8;
	}
}




void bloki_korekcii(int pole2[19],int galua[256],int galua2[256],int mnogochlen[7])
{
  int i,k,j,x,y,z;
  for ( k = 0; k < 19; ++k)
	{
		x = pole2[0];
		for (j = 0; j < 18; ++j)
		{
			pole2[j] = pole2[j + 1];
		}
		pole2[18] = 0;

		if (x == 0)
			continue;
		y = galua2[x];
		for (i = 0; i < 7; ++i)
		{
			z = mnogochlen[i] + y;
			if (z>254)
				z %= 255;
			pole2[i] = pole2[i] ^ galua[z];
		}
		
	}
}


void perevod_blokov(int korec[56],int pole2[19])
{
  int zh = 0;
  int i,j,k;
	//perevod blokov v 2
	for (i = 0; i < 7; ++i)
	{
		
		j = 8;
		k = j;
		while (j > 0)
		{
			korec[zh + j - 1] = pole2[i] % 2;
			pole2[i] /= 2;
			j--;
		}
		zh += k;
	}
}

void vvod_dannih(pol qrcode[21][21],int fin[208])
{
  int tri = 0;
  int i = 20, j = 20;
  while (tri < 208)
  {
	  while (j > -1)
	  {
		  if (!qrcode[i][j].sluzh)
		  {
			  qrcode[i][j].pole = fin[tri];
			 if (maska(i, j))
				  qrcode[i][j].pole = !qrcode[i][j].pole;
			  tri++;
		  }
		  if (!qrcode[i-1][j].sluzh)
		  {
			  i--;
			  qrcode[i][j].pole = fin[tri];
			  if (maska(i, j))
				  qrcode[i][j].pole = !qrcode[i][j].pole;
			  tri++;
			  i++;
		  }
		  j--;
	  }
	  j++;
	  i -= 2;
	  if (i == 6)
		  i--;
	  while (j<21)
	  {
		  if (!qrcode[i][j].sluzh)
		  {
			  qrcode[i][j].pole = fin[tri];
			  if(maska(i, j))
				  qrcode[i][j].pole = !qrcode[i][j].pole;
			  tri++;
		  }
		  if (!qrcode[i-1][j].sluzh)
		  {
			  i--;
				  qrcode[i][j].pole = fin[tri];
			  if (maska(i, j))
				  qrcode[i][j].pole = !qrcode[i][j].pole;
			  tri++;
			  i++;
		  }
		  j++;
	  }
	  j--;
	  i -= 2;
	  if (i == 6)
		  i--;
  }
	
}



void grafika(pol qrcode[21][21])
{
  int i,j;
  printf("\n");
	//otrisovka qr code
for (i= 0; i < 23; ++i)
		{
      printf("%c",219);
}
  printf("\n");
	for (j = 0; j < 10; ++j)
	{
    printf("%c",219);

		for (i= 0; i < 21; ++i)
		{
			if ((qrcode[i][2 * j].pole) && (qrcode[i][2 * j + 1].pole))
        printf(" ");
      else
		  if ((!qrcode[i][2 * j].pole) && (qrcode[i][2 * j + 1].pole))
        printf("%c",223);
      else
		  if ((qrcode[i][2 * j].pole) && (!qrcode[i][2 * j + 1].pole))
        printf("%c",220);
      else
      printf("%c",219);
		}
printf("%c",219);

    printf("\n");
	}
printf("%c",219);
  for (i= 0; i < 21; ++i)
		{
	  if ((qrcode[i][20].pole))
        printf("%c",220);   
      else
         printf("%c",219);
        
  }
printf("%c",219);

printf("\n");
for (i= 0; i < 23; ++i)
		{
      printf("%c",223);
}
}

int main()
{
	int i, j, k=0, zh = 4, length = 0,a=0;
	pol qrcode[21][21];
	int pole[152] = { 0, 0, 0, 1 };
	char *stroka =(char*) malloc(51 * sizeof(char));
	int pole2[19];
	int mnogochlen[7] = { 87, 229, 146, 149, 238, 102, 21 };
	int korec[56];
	int fin[208];

	int galua[256] = { 1, 2, 4, 8, 16, 32, 64, 128, 29, 58, 116, 232, 205, 135, 19, 38,
		76, 152, 45, 90, 180, 117, 234, 201, 143, 3, 6, 12, 24, 48, 96, 192,
		157, 39, 78, 156, 37, 74, 148, 53, 106, 212, 181, 119, 238, 193, 159, 35,
		70, 140, 5, 10, 20, 40, 80, 160, 93, 186, 105, 210, 185, 111, 222, 161,
		95, 190, 97, 194, 153, 47, 94, 188, 101, 202, 137, 15, 30, 60, 120, 240,
		253, 231, 211, 187, 107, 214, 177, 127, 254, 225, 223, 163, 91, 182, 113, 226,
		217, 175, 67, 134, 17, 34, 68, 136, 13, 26, 52, 104, 208, 189, 103, 206,
		129, 31, 62, 124, 248, 237, 199, 147, 59, 118, 236, 197, 151, 51, 102, 204,
		133, 23, 46, 92, 184, 109, 218, 169, 79, 158, 33, 66, 132, 21, 42, 84,
		168, 77, 154, 41, 82, 164, 85, 170, 73, 146, 57, 114, 228, 213, 183, 115,
		230, 209, 191, 99, 198, 145, 63, 126, 252, 229, 215, 179, 123, 246, 241, 255,
		227, 219, 171, 75, 150, 49, 98, 196, 149, 55, 110, 220, 165, 87, 174, 65,
		130, 25, 50, 100, 200, 141, 7, 14, 28, 56, 112, 224, 221, 167, 83, 166,
		81, 162, 89, 178, 121, 242, 249, 239, 195, 155, 43, 86, 172, 69, 138, 9,
		18, 36, 72, 144, 61, 122, 244, 245, 247, 243, 251, 235, 203, 139, 11, 22,
		44, 88, 176, 125, 250, 233, 207, 131, 27, 54, 108, 216, 173, 71, 142, 1 };
	int galua2[256] = { 0, 0, 1, 25, 2, 50, 26, 198, 3, 223, 51, 238, 27, 104, 199, 75,
		4, 100, 224, 14, 52, 141, 239, 129, 28, 193, 105, 248, 200, 8, 76, 113,
		5, 138, 101, 47, 225, 36, 15, 33, 53, 147, 142, 218, 240, 18, 130, 69,
		29, 181, 194, 125, 106, 39, 249, 185, 201, 154, 9, 120, 77, 228, 114, 166,
		6, 191, 139, 98, 102, 221, 48, 253, 226, 152, 37, 179, 16, 145, 34, 136,
		54, 208, 148, 206, 143, 150, 219, 189, 241, 210, 19, 92, 131, 56, 70, 64,
		30, 66, 182, 163, 195, 72, 126, 110, 107, 58, 40, 84, 250, 133, 186, 61,
		202, 94, 155, 159, 10, 21, 121, 43, 78, 212, 229, 172, 115, 243, 167, 87,
		7, 112, 192, 247, 140, 128, 99, 13, 103, 74, 222, 237, 49, 197, 254, 24,
		227, 165, 153, 119, 38, 184, 180, 124, 17, 68, 146, 217, 35, 32, 137, 46,
		55, 63, 209, 91, 149, 188, 207, 205, 144, 135, 151, 178, 220, 252, 190, 97,
		242, 86, 211, 171, 20, 42, 93, 158, 132, 60, 57, 83, 71, 109, 65, 162,
		31, 45, 67, 216, 183, 123, 164, 118, 196, 23, 73, 236, 127, 12, 111, 246,
		108, 161, 59, 82, 41, 157, 85, 170, 251, 96, 134, 177, 187, 204, 62, 90,
		203, 89, 95, 176, 156, 169, 160, 81, 11, 245, 22, 235, 122, 117, 44, 215,
		79, 174, 213, 233, 230, 231, 173, 232, 116, 214, 244, 234, 168, 80, 88, 175 };
	char bukv[45]={'0',	'1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' ','$','%','*','+','-','.','/',':'};
 
	//obnulenie
	for (i = zh; i < 152; ++i)
		pole[i] = 0;

	//input numbers
	if ((length = getstring(stroka)) == -1)
		return 0;


		  
  k = length;
	//dlina stroki
	{
		i = 10;
		j = i;
		while (j > 0)
		{
			pole[zh + j - 1] = k % 2;
			k /= 2;
			j--;
		}
		zh += i;
	}




	zh=razbienie(length,pole,stroka,zh);

  zh=zapolnenie(zh,pole);
	

	//perevod obratno v 10 
	perevod_v_des(pole2,pole);



	//zapis vichislennih bitov v finalnii massiv
	for (i = 0; i < 152; ++i)
		fin[i] = pole[i];



	//vichislenie bloka korecsii
	bloki_korekcii(pole2,galua,galua2,mnogochlen);

	
	perevod_blokov(korec,pole2);

	for (i = 152; i < 208; ++i)
		fin[i] = korec[i - 152];

	

	//clean
	for (i = 0; i < 21; ++i)
		for (j = 0; j < 21; ++j)
		{
			qrcode[i][j].pole = 0;
			qrcode[i][j].sluzh = 0;

		}
			


	//uglovie kvadrati 
	kvadrat(qrcode,0,0);
	kvadrat(qrcode, 14, 0);
	kvadrat(qrcode,0, 14 );



	//polosochki
	for (i = 8; i < 13; i ++)
	{
		if (i % 2 == 0)
		{
			qrcode[i][6].pole = 1;
			qrcode[6][i].pole = 1;
		}
		qrcode[i][6].sluzh = 1;
		qrcode[6][i].sluzh = 1;
	}
		
			

//zapolnenie bitov maski i urovnya koreckii
  qrcode[0][8].pole = 1;
  qrcode[1][8].pole = 1;
  qrcode[2][8].pole = 1;
  qrcode[4][8].pole = 1;
  qrcode[5][8].pole = 1;

  qrcode[8][8].pole = 1;
  qrcode[7][8].pole = 1;
  qrcode[8][7].pole = 1;
  qrcode[8][2].pole = 1;




  qrcode[8][20].pole = 1;
  qrcode[8][19].pole = 1;
  qrcode[8][18].pole = 1;
  qrcode[8][16].pole = 1;
  qrcode[8][14].pole = 1;
  qrcode[8][15].pole = 1;

  qrcode[8][13].pole = 1;



  qrcode[18][8].pole = 1;
  qrcode[14][8].pole = 1;
  qrcode[13][8].pole = 1;


  for (i = 0; i < 9; i++)
	  for (j = 0; j < 9; j++)
		  qrcode[i][j].sluzh = 1;
  for (i = 0; i < 9; i++)
	  for (j = 13; j < 21; j++)
	  {
		  qrcode[i][j].sluzh = 1;
		  qrcode[j][i].sluzh = 1;
	  }
		  


  //vvod dannih
  vvod_dannih(qrcode, fin);

//zapusk okna grafiki 
  grafika(qrcode);
	//zavershenie otrisovki , oshidanie nagatia klavishi
	_getch();
	return 0;
}


