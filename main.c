//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out test1.bmp test1_grey.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe test1.bmp test1_grey.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

void fillCopy(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]);
void capture(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int innerFrameSize);
void checkInnerFrame(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int innerFrameSize, int iInitial, int jInitial);
int checkOuterFrame(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int innerFrameSize, int iInitial, int jInitial);
void paintBlackSquare(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int innerFrameSize, int iInitial, int jInitial);
//Declaring the array to store the image (unsigned char = unsigned 8 bit)

unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char black_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

int counter = 0;











void toGreyScale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      output_image[x][y][0] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
      output_image[x][y][1] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
      output_image[x][y][2] = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;
    }
  }
}

void toBinary(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
  int color;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {

      if (output_image[x][y][0] <= 90)
      {
        color = 0; // black
      }
      else
      {
        color = 255; // white
      }
      output_image[x][y][0] = color;
      output_image[x][y][1] = color;
      output_image[x][y][2] = color;
    }
  }
}

void eroder(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int timer)
{



    for (int x = 0; x < BMP_WIDTH; x++)
    {

      for (int y = 0; y < BMP_HEIGTH; y++)
      {

        if (copy_image[x][y][0] == 255)
        {

          int erode = 0;

          if (x - 1 < 0 || copy_image[x - 1][y][0] == 0)
          {

            erode = 1;
          }

          else if (x + 1 > BMP_WIDTH || copy_image[x + 1][y][0] == 0)
          {

            erode = 1;
          }

          else if (y - 1 < 0 || copy_image[x][y - 1][0] == 0)
          {

            erode = 1;
          }

          else if (y + 1 > BMP_HEIGTH || copy_image[x][y + 1][0] == 0)
          {

            erode = 1;
          }

          if (erode == 1)
          {

            output_image[x][y][0] = 0;
            output_image[x][y][1] = 0;
            output_image[x][y][2] = 0;
          }

          
          erode = 0;

        }
      }
    }

    fillCopy(output_image, copy_image);
    timer--;

    capture(output_image, 12);
    
    if (timer > 0) {

      eroder(output_image, copy_image, timer);

    }
  
}

void capture(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int innerFrameSize) {

    int nbJumps = (2 * ((BMP_WIDTH - 2)/innerFrameSize)) - 1;
    int jumpSize = innerFrameSize/2;
    int outerFrameSize = innerFrameSize + 2;

    for (int i = 0; i < nbJumps; i++) {
      for (int j = 0; j < nbJumps; j++) {

        if (checkOuterFrame(output_image, innerFrameSize, i*jumpSize, j*jumpSize)  == 1) {
            checkInnerFrame(output_image, innerFrameSize, (i*jumpSize) + 1, (j*jumpSize) + 1);
        }
        
      }
    }


}


// i and j initial are top left corner of the outer rectangle to check
int checkOuterFrame(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int innerFrameSize, int iInitial, int jInitial) {

  for (int i = iInitial; i < iInitial + innerFrameSize + 2; i++) {

    if (output_image[i][jInitial][0] == 255) {
      return 0;
    }

    if (output_image[i][jInitial + innerFrameSize + 1][0] == 255) {
      return 0;
    }

  }

  for (int j = jInitial; j < iInitial + innerFrameSize + 2; j++) {

    if (output_image[iInitial][j][0] == 255) {
      return 0;
    }

    if (output_image[iInitial + innerFrameSize + 1][j][0] == 255) {
      return 0;
    }

  }

  return 1; //everything is black

}

void checkInnerFrame(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int innerFrameSize, int iInitial, int jInitial) {

    for (int i = iInitial; i < iInitial + innerFrameSize; i++) {
      for (int j = jInitial; j < jInitial + innerFrameSize; j++) {

        if (output_image[i][j][0] == 255) {
          counter ++;
          paintBlackSquare(output_image, innerFrameSize, iInitial, jInitial);
        }

      }
    }


}

// !!!!!!!!!!!!!!!!!!!!!!!!!!! change la photo a colorier !!!!!!!!!!!!!!!!
void paintBlackSquare(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int innerFrameSize, int iInitial, int jInitial) {
    
    for (int i = iInitial; i < iInitial + innerFrameSize; i++) {
      for (int j = jInitial; j < jInitial + innerFrameSize; j++) {

        output_image[i][j][0] = 0;
        output_image[i][j][1] = 0;
        output_image[i][j][2] = 0;
        

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

void fillCopy(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){

      for (int x = 0; x < BMP_WIDTH; x++) {
        for (int y = 0; y < BMP_HEIGTH; y++) {
          copy_image[x][y][0] = output_image[x][y][0];
          copy_image[x][y][1] = output_image[x][y][1];
          copy_image[x][y][2] = output_image[x][y][2];
          
        }
      }
  

}


void fillBlack(unsigned char black_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]) {

  for (int i = 0; i < BMP_WIDTH; i++) {
    for (int j = 0; j < BMP_HEIGTH; j++) {

      black_image[i][j][0] = 0;
      black_image[i][j][1] = 0;
      black_image[i][j][2] = 0;

    }
  }
}

/* void invert(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      for (int c = 0; c < BMP_CHANNELS; c++)
      {
      output_image[x][y][c] = 255 - input_image[x][y][c];
      }
    }
  }
} */



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
  toGreyScale(input_image, output_image);

  toBinary(output_image);

  fillBlack(black_image);

  fillCopy(output_image, copy_image);
  eroder(output_image, copy_image, 8);

  printf("Counter = " + counter);

  

  //Save image to file
  write_bitmap(black_image, argv[2]);

  printf("Done!\n");
  return 0;
}
