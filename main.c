//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out test1.bmp test1_grey.bmp


//cd C:\Users\schle\penumbral_eclipse\CS_Project
//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe test1.bmp test1_grey.bmp

//To run (win): main.exe 2MEDIUM.bmp 2MEDIUM_grey.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

void fillCopy(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH]);
void capture(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH]);
void checkInnerFrame(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial);
int checkOuterFrame(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial);
void paintBlackSquare(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial);
void finalImage(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],int x, int y);

//Declaring the array to store the image (unsigned char = unsigned 8 bit)

unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char test_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char capturedCoord[BMP_WIDTH][BMP_HEIGTH];


#define innerFrameSize 12
int nbJumps = ((2 * ((BMP_WIDTH - 2)/innerFrameSize)) - 1);
int jumpSize = (innerFrameSize/2);
int outerFrameSize = (innerFrameSize + 2);

int jumps = BMP_HEIGTH/innerFrameSize;






void toGreyScale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      erosion_image[x][y] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;

    }
  }
}

void toBinary(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH])
{
  int color;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {

      if (erosion_image[x][y] <= 90)
      {
        color = 0; // black
      }
      else
      {
        color = 255; // white
      }
      erosion_image[x][y] = color;

    }
  }
}

void eroder(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH], int timer)
{
    for (int x = 0; x < BMP_WIDTH; x++)
    {

      for (int y = 0; y < BMP_HEIGTH; y++)
      {

        if (copy_image[x][y] == 255)
        {

          int erode = 0;

          if (x - 1 < 0 || copy_image[x - 1][y] == 0)
          {

            erode = 1;
          }

          else if (x + 1 >= BMP_WIDTH || copy_image[x + 1][y] == 0)
          {

            erode = 1;
          }

          else if (y - 1 < 0 || copy_image[x][y - 1] == 0)
          {

            erode = 1;
          }

          else if (y + 1 >= BMP_HEIGTH || copy_image[x][y + 1] == 0)
          {

            erode = 1;
          }

          if (erode == 1)
          {

            erosion_image[x][y] = 0;
          }

          
          erode = 0;

        }
      }
    }

    fillCopy(erosion_image, copy_image);
    timer--;

    capture(erosion_image);
    
    if (timer > 0) {

      eroder(erosion_image, copy_image, timer);

    }
  
}

void capture(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH]) {

    for (int i = 0; i < BMP_WIDTH-12; i++) {
      for (int j = 0; j < BMP_HEIGTH-12; j++) {

        if (checkOuterFrame(erosion_image, i, j)  == 1) {
            checkInnerFrame(erosion_image, i + 1, j + 1);
        }
      }
    }
}


// i and j initial are top left corner of the outer rectangle to check
int checkOuterFrame(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial) {

  for (int i = iInitial; i < iInitial + innerFrameSize + 2; i++) {
    if (erosion_image[i][jInitial] == 255) {
      return 0;
    }

    if (erosion_image[i][jInitial + innerFrameSize + 1]== 255) {
      return 0;
    }
  }

  for (int j = jInitial; j < iInitial + innerFrameSize + 2; j++) {
    if (erosion_image[iInitial][j] == 255) {
      return 0;
    }

    if (erosion_image[iInitial + innerFrameSize + 1][j] == 255) {
      return 0;
    }
  }
  return 1; //everything is black
}

void checkInnerFrame(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial) {
  unsigned char whiteExist=0;
    for (int i = iInitial; i < iInitial + innerFrameSize; i++) {
      for (int j = jInitial; j < jInitial + innerFrameSize; j++) {
        if (erosion_image[i][j] == 255) {
          erosion_image[iInitial][jInitial]=0;
          whiteExist=1;
          /* counter ++;
          paintBlackSquare(erosion_image, iInitial, jInitial); */
        }
      }
    }
    if(whiteExist){
      /* capturedCoord[iInitial+(innerFrameSize/2)][jInitial+(innerFrameSize/2)]=1; */
      finalImage(input_image,iInitial+(innerFrameSize/2),jInitial+(innerFrameSize/2));
    }
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!! change la photo a colorier !!!!!!!!!!!!!!!!
/* void paintBlackSquare(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial) {
    
    for (int i = iInitial; i < iInitial + innerFrameSize; i++) {
      for (int j = jInitial; j < jInitial + innerFrameSize; j++) {

        erosion_image[i][j]= 0;
        
        

      }
    }

    for (int i = iInitial; i < iInitial + innerFrameSize; i++) {
      black_image[i][jInitial + (innerFrameSize/2)][0] = 255;
      black_image[i][jInitial + (innerFrameSize/2)][1] = 0;
      black_image[i][jInitial + (innerFrameSize/2)][2] = 0;
    }

    for (int j = jInitial; j < jInitial + innerFrameSize; j++) {
      black_image[iInitial + (innerFrameSize/2)][j][0] = 255;
      black_image[iInitial + (innerFrameSize/2)][j][1] = 0;
      black_image[iInitial + (innerFrameSize/2)][j][2] = 0;
    }

}
 */
void fillCopy(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH]){

      for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
          copy_image[x][y] = erosion_image[x][y];
        }
      }
}


/* void fillBlack(unsigned char black_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {

  for (int i = 0; i < BMP_WIDTH; i++) {
    for (int j = 0; j < BMP_HEIGTH; j++) {

      black_image[i][j][0] = 0;
      black_image[i][j][1] = 0;
      black_image[i][j][2] = 0;

    }
  }
} */

/* void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
      erosion_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
} */

void finalImage(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS],int x, int y){
  for(int k = 0 ; k < innerFrameSize ; k++) {
    input_image[(x - (innerFrameSize/2)) + k][y][0] = 255;
    input_image[(x - (innerFrameSize/2)) + k][y][1] = 0;
    input_image[(x - (innerFrameSize/2)) + k][y][2] = 0;

    input_image[x][(y - (innerFrameSize/2)) + k][0] = 255;
    input_image[x][(y - (innerFrameSize/2)) + k][1] = 0;
    input_image[x][(y - (innerFrameSize/2)) + k][2] = 0;
  }
}

void binaryToBMP(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH]){
  for (int i = 0; i < BMP_WIDTH; i++) {
    for (int j = 0;  j < BMP_HEIGTH; j++) {
      if (erosion_image[i][j] == 0) {
        test_image[i][j][0] = 0;
        test_image[i][j][1] = 0;
        test_image[i][j][2] = 0;

      } else {
        test_image[i][j][0] = 255;
        test_image[i][j][1] = 255;
        test_image[i][j][2] = 255;        
      }
    }
  }
}

//Main function
int main(int argc, char **argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
    exit(1);
  }

  printf("Example program - 02132 - A1\n");

  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run inversion
  toGreyScale(input_image, erosion_image);

  toBinary(erosion_image);


  fillCopy(erosion_image, copy_image);
  eroder(erosion_image, copy_image, 8);


  
  //binaryToBMP(erosion_image);
  //Save image to file
  write_bitmap(input_image, argv[2]);

  printf("Done!\n");
  return 0;
}
