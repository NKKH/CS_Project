//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out test1.bmp test1_grey.bmp

//cd C:\Users\Admin\Desktop\Computer systems\Project\CS_Project
//cd C:\Users\schle\penumbral_eclipse\CS_Project
//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99

//To run (win): main.exe example.bmp example_done.bmp
//To run (win): main.exe 1MEDIUM.bmp 1MEDIUM_done.bmp
//To run (win): main.exe 1HARD.bmp 1HARD_done.bmp

//main.exe samples\medium\1MEDIUM.bmp 1MEDIUM_done.bmp


#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

//TODO: Try different capture frames.
#define innerFrameSize 12
#define iterations 15
#define bytelength 8
#define numByte (BMP_WIDTH*BMP_HEIGTH)/(numByte)+(BMP_WIDTH*BMP_HEIGTH)%(numByte)

//TODO: Put these in a different file, and then include
void fillCopy(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH]);
void capture(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH]);
void checkInnerFrame(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial);
int checkOuterFrame(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial);
void finalImage(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y);
void printPicture(unsigned char test_image[BMP_WIDTH][BMP_HEIGTH], char *s);

//Declaring the array to store the image (unsigned char = unsigned 8 bit)
//TODO: Convert to bit representation for the erosion image.
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char test_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
char erosion_image[numByte]
unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH];
unsigned char capturedCoord[BMP_WIDTH][BMP_HEIGTH];



//TODO: Could experiment with larger intervals for capture.
/* int nbJumps = ((2 * ((BMP_WIDTH - 2)/innerFrameSize)) - 1);
int jumpSize = (innerFrameSize/2); 
int outerFrameSize = (innerFrameSize + 2); */

int counter = 0;

//BIT MANIPULATION
void set(char a[], int i, int j){

int area = ((i+1)*(j+1)+(i)*(BMP_WIDTH-(j+1)));

int index = area / numByte;
int numBit = area % numByte;

a[index] = a[index]^(1<<numBit);
}


int getBit(char a[], int i, int j){
    int area = ((i+1)*(j+1)+(i)*(BMP_WIDTH-(j+1)));

    int index = area / numByte;
    int numBit = area % numByte;

    if (a[index]&(1<<numBit)){
        return 1;
    }else{
        return 0;
    }
}


//TODO: One could do Greyscaling and binary-transformation at the same time
void toGreyScale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], char erosion_image[])
{
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {


     
    }
  }
}

void toBinary(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH])
{
  unsigned char color;
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

// REGULAR EROSION
//TODO: Could test different erosion patterns.
//TODO: Erode until no more white pixels.
void eroder(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH], int timer)
{
  //Test pictures for erosion
  char path[30];
  sprintf(path, "test_image\\image%d.bmp", timer);
  printPicture(erosion_image, path);

  unsigned char erode = 0;

  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (erosion_image[x][y] == 255)
      {

        erode = 0;
        // Checks edge cases
        /* if(x==0 || x == (BMP_WIDTH-1) || y==0 || y == (BMP_HEIGTH-1)
        || copy_image[x - 1][y] == 0||copy_image[x + 1][y] == 0||copy_image[x][y - 1] == 0||copy_image[x][y + 1] == 0)
        {
          erode=1;
        } */

        if (x == 0 || copy_image[x - 1][y] == 0)
        {

          erode = 1;
        }

        else if (x + 1 == BMP_WIDTH || copy_image[x + 1][y] == 0)
        {

          erode = 1;
        }

        else if (y == 0 || copy_image[x][y - 1] == 0)
        {

          erode = 1;
        }

        else if (y + 1 == BMP_HEIGTH || copy_image[x][y + 1] == 0)
        {

          erode = 1;
        }

        if (erode == 1)
        {
          erosion_image[x][y] = 0;
          erode = 0;
        }
      }
    }
  }

  capture(erosion_image);

  fillCopy(erosion_image, copy_image);
  timer--;

  if (timer > 0)
  {
    eroder(erosion_image, copy_image, timer);
  }
}

void capture(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH])
{

  for (int i = 0; i <= BMP_WIDTH - (innerFrameSize+2); i++)
  {
    for (int j = 0; j <= BMP_HEIGTH - (innerFrameSize+2); j++)
    {

      if (checkOuterFrame(erosion_image, i, j) == 1)
      {
        checkInnerFrame(erosion_image, i + 1, j + 1);
      }
    }
  }
}


void eroderDiag(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH], int timer)
{
  //Test pictures for erosion
  char path[30];
  sprintf(path, "test_image\\image%d.bmp", timer);
  printPicture(erosion_image, path);

  unsigned char erode = 0;

  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (erosion_image[x][y] == 255)
      {

        erode = 0;
        // Checks edge cases
        /* if(x==0 || x == (BMP_WIDTH-1) || y==0 || y == (BMP_HEIGTH-1)
        || copy_image[x - 1][y] == 0||copy_image[x + 1][y] == 0||copy_image[x][y - 1] == 0||copy_image[x][y + 1] == 0)
        {
          erode=1;
        } */

        if (x == 0 || copy_image[x - 1][y] == 0)
        {

          erode = 1;
        }

        else if (x + 1 == BMP_WIDTH || copy_image[x + 1][y] == 0)
        {

          erode = 1;
        }

        else if (y == 0 || copy_image[x][y - 1] == 0)
        {

          erode = 1;
        }

        else if (y + 1 == BMP_HEIGTH || copy_image[x][y + 1] == 0)
        {

          erode = 1;
        }

        else if ((x + 1 == BMP_WIDTH || y + 1 == BMP_HEIGTH) || copy_image[x+1][y+1] == 0)
        {

          erode = 1;
        }

        else if ((x + 1 == BMP_WIDTH || y - 1 == 0) || copy_image[x + 1][y - 1] == 0) {
          erode = 1;
        }

        else if ((x - 1 == 0 || y + 1 == BMP_HEIGTH) || copy_image[x-1][y+1] == 0) {
          erode = 1;
        }

        else if ((x - 1 == 0 || y - 1 == 0) || copy_image[x-1][y-1] == 0) {
          erode = 1;
        }

        if (erode == 1)
        {
          erosion_image[x][y] = 0;
          erode = 0;
        }
      }
    }
  }

  capture(erosion_image);

  fillCopy(erosion_image, copy_image);
  timer--;

  if (timer > 0)
  {
    eroderDiag(erosion_image, copy_image, timer);
  }
}




// i and j initial are top left corner of the outer rectangle to check
int checkOuterFrame(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial)
{
  //Check outer columns
  for (int i = iInitial; i < iInitial + innerFrameSize + 2; i++)
  {
    //first column
    if (erosion_image[i][jInitial] == 255)
    {
      return 0;
    }
    //second column
    else if (erosion_image[i][jInitial + innerFrameSize + 1] == 255)
    {
      return 0;
    }
  }
  //Check outer rows
  for (int j = jInitial; j < jInitial + innerFrameSize + 2; j++)
  {
    if (erosion_image[iInitial][j] == 255)
    {
      return 0;
    }

    else if (erosion_image[iInitial + innerFrameSize + 1][j] == 255)
    {
      return 0;
    }
  }
  return 1; //everything is black
}

void checkInnerFrame(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], int iInitial, int jInitial)
{
  unsigned char whiteExist = 0;
  for (int i = iInitial; i < iInitial + innerFrameSize; i++)
  {
    for (int j = jInitial; j < jInitial + innerFrameSize; j++)
    {
      if (erosion_image[i][j] == 255)
      {
        erosion_image[i][j] = 0;
        whiteExist = 1;
      }
    }
  }

  if (whiteExist)
  {
    counter++;
    /* capturedCoord[iInitial+(innerFrameSize/2)][jInitial+(innerFrameSize/2)]=1; */ //I'd prefer the array, since you "lose" data by just painting the crosses..
    finalImage(input_image, iInitial + (innerFrameSize / 2), jInitial + (innerFrameSize / 2));
    whiteExist=0;
  }
}


//TODO: Instead of copying the array, keep track of changes, and only modify those changes.
void fillCopy(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], unsigned char copy_image[BMP_WIDTH][BMP_HEIGTH])
{

  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      copy_image[x][y] = erosion_image[x][y];
    }
  }
}

void finalImage(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y)
{
  for (int k = 0; k < innerFrameSize; k++)
  {
    input_image[(x - (innerFrameSize / 2)) + k][y][0] = 255;
    input_image[(x - (innerFrameSize / 2)) + k][y][1] = 0;
    input_image[(x - (innerFrameSize / 2)) + k][y][2] = 0;

    input_image[x][(y - (innerFrameSize / 2)) + k][0] = 255;
    input_image[x][(y - (innerFrameSize / 2)) + k][1] = 0;
    input_image[x][(y - (innerFrameSize / 2)) + k][2] = 0;
  }
}

void binaryToBMP(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH])
{
  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      if (erosion_image[i][j] == 0)
      {
        test_image[i][j][0] = 0;
        test_image[i][j][1] = 0;
        test_image[i][j][2] = 0;
      }
      else
      {
        test_image[i][j][0] = 255;
        test_image[i][j][1] = 255;
        test_image[i][j][2] = 255;
      }
    }
  }
}

void printPicture(unsigned char erosion_image[BMP_WIDTH][BMP_HEIGTH], char *s)
{
  binaryToBMP(erosion_image);
  write_bitmap(test_image, s);
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
  eroder(erosion_image, copy_image, iterations);

  //binaryToBMP(erosion_image);
  //Save image to file
  write_bitmap(input_image, argv[2]);

  printf("Done! Count: %d\n", counter);
  return 0;
}
