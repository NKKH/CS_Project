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
#include <math.h>
#include <time.h>

//TODO: Try different capture frames.
#define innerFrameSize 12
#define byteLength 8


// #define numByte (BMP_WIDTH * BMP_HEIGTH) / (byteLength) +(BMP_WIDTH * BMP_HEIGTH) % (byteLength)
#define numByte (BMP_WIDTH * BMP_HEIGTH) / (byteLength) + 1
#define cellDetectionThreshold 30

//TODO: Put these in a different file, and then include (PROTOTYPING)
void fillCopy(char erosion_image[numByte], char copy_image[numByte]);
void capture(char erosion_image[numByte]);
void checkInnerFrame(char erosion_image[numByte], int iInitial, int jInitial);
int checkOuterFrame(char erosion_image[numByte], int iInitial, int jInitial);
void markFinalImage(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y);
void printPicture(char erosion_image[numByte]);
void particleDivider(char erosion_image[numByte], int iStart, int jStart, int iEnd, int jEnd, int treshold, int version);
void eroderDiag(char erosion_image[numByte], char copy_image[numByte]);
//Declaring the array to store the image (unsigned char = unsigned 8 bit)
//TODO: Convert to bit representation for the erosion image.
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char test_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]; //Used only for testing..
char erosion_image[numByte] = {0};                             //used to erode image
char copy_image[numByte] = {0};                                //used as reference for erosion
char capturedCoord[numByte] = {0};
int counter = 0;

//For testing:
unsigned char pictureIncrementer = 0;

clock_t start, end;
double erosionTime = 0;
double toBinaryTime = 0;
double separationTime = 0;
double totalTime = 0;

//BIT MANIPULATION
void flipBit(char a[numByte], int i, int j)
{
  //On the first byte, we will ignore the first bit, since index = 8/8 will result in accessing the second byte. Henceforth, we utilize every bit in our byte array.
  int area = ((i + 1) * (j + 1) + (i) * (BMP_WIDTH - (j + 1)));

  int index = area >> 3;
  
  int numBit = area & (byteLength - 1); //Modular operation

  //Go to "index" byte, and flip the "numBit" bit, then conduct a XOR operations to flip.
  a[index] = a[index] ^ (1 << numBit);
}

int getBit(char a[numByte], int i, int j)
{
  int area = ((i + 1) * (j + 1) + (i) * (BMP_WIDTH - (j + 1)));

  int index = area >> 3;
  int numBit = area & (byteLength - 1);

  //comparison
  if (a[index] & (1 << numBit))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

//Greyscaling AND conversion to binary representation
void toBinary(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], char erosion_image[numByte])
{
  unsigned char color;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      color = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2]) / 3;

      //set bit to 1 if white.
      if (color > 90)
      {
        flipBit(erosion_image, x, y);
      }
    }
  }
}

void firstSeparation(char erosion_image[numByte], int treshold)
{
  int coords[2]; // iStart, jStart
  int found = 0;

  //HORIZONTAL CHECK
  for (int i = 0; i < BMP_HEIGTH; i++)
  {
    for (int j = 0; j < BMP_WIDTH; j++)
    {

      if (getBit(erosion_image, i, j) == 1)
      {
        if (found == 0)
        {
          coords[0] = i;
          coords[1] = j;
          found = 1;
        }
      }
      else
      {
        if (found == 1 && getBit(erosion_image, i, j) == 0)
        {
          particleDivider(erosion_image, coords[0], coords[1], i, j - 1, treshold, 1);
          found = 0;
        }
      }
    }
  }

  found = 0;

  //VERTICAL CHECK
  for (int j = 0; j < BMP_WIDTH; j++)
  {
    for (int i = 0; i < BMP_HEIGTH; i++)
    {

      if (getBit(erosion_image, i, j) == 1)
      {
        if (found == 0)
        {
          coords[0] = i;
          coords[1] = j;
          found = 1;
        }
      }
      else
      {
        if (found == 1 && getBit(erosion_image, i, j) == 0)
        {
          particleDivider(erosion_image, coords[0], coords[1], i - 1, j, treshold, 2);
          found = 0;
        }
      }
    }
  }

  found = 0;

  //DIAGONAL NW-SE

  /*   for (int i = 0; i < BMP_HEIGTH; i++) {
    for (int j = 0; j < BMP_WIDTH - i; j++) {
      
      if (getBit(erosion_image,i+j,j) == 1) {
        
        if (found == 0) {
          coords[0] = i+j;
          coords[1] = j;
          found = 1;
        }
      } else {
        if (found == 1 && getBit(erosion_image,i+j,j) == 0) {
          particleDivider(erosion_image, coords[0], coords[1], (i+j)-1, j-1, treshold, 3);
          found = 0;
        }
      }
    }
  }

  found = 0;

    for (int j = 0; j < BMP_WIDTH; j++) {
    for (int i = 0; i < BMP_HEIGTH - j; i++) {
      
      if (getBit(erosion_image,i,j+i) == 1) {
        if (found == 0) {
          coords[0] = i;
          coords[1] = j+i;
          found = 1;
        }
      } else {
          if (found == 1 && getBit(erosion_image,i,j+i) == 0) {
            particleDivider(erosion_image, coords[0], coords[1], i-1, (j+i)-1, treshold, 3);
            found = 0;
          }
      }
    }
  }

  found = 0;
 */
  // DIAGONAL SW-NE

  /*   for (int i = BMP_HEIGTH - 1; i >= 0; i--){
    for (int j = 0; j <= i; j++) {
      if (getBit(erosion_image, i-j, j) == 1) {
        if (found == 0) {
          coords[0] = i-j;
          coords[1] = j;
          found = 1;

        }
      } else {
        if (found == 1 && getBit(erosion_image, i-j, j) == 0) {
          particleDivider(erosion_image, coords[0], coords[1], (i-j)+1, j-1, treshold, 4);
          found = 0;
        }
      }
    }
  }

  found = 0;

  for (int j = 0; j < BMP_WIDTH; j++) {
    
    for (int i = BMP_HEIGTH - 1; i - j >= 0; i--){
      if (getBit(erosion_image, i, j + (BMP_HEIGTH - i)) == 1) {
        
        if (found == 0) {
          coords[0] = i;
          coords[1] = j + (BMP_HEIGTH - i);
          found = 1;
        }
      } else {
          if (found == 1 && getBit(erosion_image, i, j + (BMP_HEIGTH - i)) == 0) {
            particleDivider(erosion_image, coords[0], coords[1], i+1, (j + (BMP_HEIGTH - i)) -1, treshold, 4);
            found = 0;
          }
      }
    }
  }

 */
}

void particleDivider(char erosion_image[numByte], int iStart, int jStart, int iEnd, int jEnd, int treshold, int version)
{

  int length;
  int nbSep;
  int itt;
  // HORIZONTAL
  if (version == 1)
  {

    length = (jEnd - jStart) + 1; // 65
    nbSep = length / treshold;    // 65/30 = 2
    itt = length / (nbSep + 1);   // 65/3 = 21

    for (int x = 1; x <= nbSep; x++)
    {
      flipBit(erosion_image, iStart, jStart + (x * itt));
    }

    // VERTICAL
  }
  else if (version == 2)
  {

    length = (iEnd - iStart) + 1;
    nbSep = length / treshold;
    itt = length / (nbSep + 1);

    for (int y = 1; y <= nbSep; y++)
    {
      flipBit(erosion_image, iStart + (y * itt), jStart);
    }

    // NW - SE
    /*   } else if (version == 3) {
    length = (int) sqrt( ((iEnd - iStart) + 1)*((iEnd - iStart) + 1) + ((jEnd - jStart) + 1)*((jEnd - jStart) + 1));
    //length = jEnd - jStart;
    nbSep = length / treshold;
    itt = length / (nbSep + 1);

    for (int xy = 1; xy <= nbSep; xy++) {
      flipBit(erosion_image, iStart + (xy*itt), jStart + (xy* itt));
    }
  */

    // SW - NE
    /*    } else if (version == 4) {
    length = (int) sqrt( ((iStart - iEnd) + 1)*((iStart - iEnd) + 1) + ((jEnd - jStart) + 1)*((jEnd - jStart) + 1));
    length = jEnd - jStart;
    nbSep = length / treshold;
    itt = length / (nbSep + 1);

    for (int xy = 1; xy <= nbSep; xy++) {
      flipBit(erosion_image, iStart - (xy*itt), jStart + (xy*itt));
    }*/
  }
}

// REGULAR EROSION
//TODO: Could test different erosion patterns.
//TODO: Erode until no more white pixels.
void eroder(char erosion_image[numByte], char copy_image[numByte])
{

  unsigned char erode = 0;
  unsigned char whiteFound = 0;
  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (getBit(erosion_image, x, y) == 1)
      {
        whiteFound = 1;

        if (x == 0 || getBit(copy_image, x - 1, y) == 0)
        {

          erode = 1;
        }

        else if (x + 1 == BMP_WIDTH || getBit(copy_image, x + 1, y) == 0)
        {

          erode = 1;
        }

        else if (y == 0 || getBit(copy_image, x, y - 1) == 0)
        {

          erode = 1;
        }

        else if (y + 1 == BMP_HEIGTH || getBit(copy_image, x, y + 1) == 0)
        {

          erode = 1;
        }

        if (erode == 1)
        {
          flipBit(erosion_image, x, y);
          erode = 0;
        }
      }
    }
  }

  capture(erosion_image);

  /*   //Test pictures for erosion
  printPicture(erosion_image);
 */
  fillCopy(erosion_image, copy_image);

  if (whiteFound)
  {
    eroder(erosion_image, copy_image);
  }
}

void capture(char erosion_image[numByte])
{

  for (int i = 0; i <= BMP_WIDTH - (innerFrameSize + 2); i++)
  {
    for (int j = 0; j <= BMP_HEIGTH - (innerFrameSize + 2); j++)
    {

      if (checkOuterFrame(erosion_image, i, j) == 1)
      {
        checkInnerFrame(erosion_image, i + 1, j + 1);
      }
    }
  }
}

void eroderDiag(char erosion_image[numByte], char copy_image[numByte])
{
  unsigned char erode = 0;
  unsigned char whiteFound = 0;

  for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      if (getBit(erosion_image, x, y) == 1)
      {
        whiteFound = 1;

        if (x == 0 || getBit(copy_image, x - 1, y) == 0)
        {
          erode = 1;
        }

        else if (x + 1 == BMP_WIDTH || getBit(copy_image, x + 1, y) == 0)
        {

          erode = 1;
        }

        else if (y == 0 || getBit(copy_image, x, y - 1) == 0)
        {

          erode = 1;
        }

        else if (y + 1 == BMP_HEIGTH || getBit(copy_image, x, y + 1) == 0)
        {

          erode = 1;
        }

        else if ((x + 1 == BMP_WIDTH || y + 1 == BMP_HEIGTH) || getBit(copy_image, x + 1, y + 1) == 0)
        {

          erode = 1;
        }

        else if ((x + 1 == BMP_WIDTH || y - 1 == 0) || getBit(copy_image, x + 1, y - 1) == 0)
        {

          erode = 1;
        }

        else if ((x - 1 == 0 || y + 1 == BMP_HEIGTH) || getBit(copy_image, x - 1, y + 1) == 0)
        {

          erode = 1;
        }

        else if ((x - 1 == 0 || y - 1 == 0) || getBit(copy_image, x - 1, y - 1) == 0)
        {

          erode = 1;
        }

        if (erode == 1)
        {
          flipBit(erosion_image, x, y);
          erode = 0;
        }
      }
    }
  }

  capture(erosion_image);

  fillCopy(erosion_image, copy_image);

  if (whiteFound)
  {
    eroderDiag(erosion_image, copy_image);
  }
}

// i and j initial are top left corner of the outer rectangle to check
int checkOuterFrame(char erosion_image[numByte], int iInitial, int jInitial)
{
  //Check outer columns
  for (int i = iInitial; i < iInitial + innerFrameSize + 2; i++)
  {
    //first column
    if (getBit(erosion_image, i, jInitial) == 1)
    {
      return 0;
    }
    //second column
    else if (getBit(erosion_image, i, jInitial + innerFrameSize + 1) == 1)
    {
      return 0;
    }
  }
  //Check outer rows
  for (int j = jInitial; j < jInitial + innerFrameSize + 2; j++)
  {
    if (getBit(erosion_image, iInitial, j) == 1)
    {
      return 0;
    }

    else if (getBit(erosion_image, iInitial + innerFrameSize + 1, j) == 1)
    {
      return 0;
    }
  }
  return 1; //everything is black
}

void checkInnerFrame(char erosion_image[numByte], int iInitial, int jInitial)
{
  unsigned char whiteExist = 0;
  for (int i = iInitial; i < iInitial + innerFrameSize; i++)
  {
    for (int j = jInitial; j < jInitial + innerFrameSize; j++)
    {
      if (getBit(erosion_image, i, j) == 1)
      {
        flipBit(erosion_image, i, j);
        whiteExist = 1;
      }
    }
  }

  if (whiteExist)
  {
    counter++;
    /* capturedCoord[iInitial+(innerFrameSize/2)][jInitial+(innerFrameSize/2)]=1; */ //I'd prefer the array, since you "lose" data by just painting the crosses..
    markFinalImage(input_image, iInitial + (innerFrameSize / 2), jInitial + (innerFrameSize / 2));
    whiteExist = 0;
  }
}

//TODO: Instead of copying the array, keep track of changes, and only modify those changes.
void fillCopy(char erosion_image[numByte], char copy_image[numByte])
{

  for (int i = 0; i < numByte; i++)
  {
    copy_image[i] = erosion_image[i];
  }
  /* for (int x = 0; x < BMP_WIDTH; x++)
  {
    for (int y = 0; y < BMP_HEIGTH; y++)
    {
      copy_image[x][y] = erosion_image[x][y];
    }
  } */
}

void markFinalImage(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y)
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

//Function for testing erosion. Can be deleted once project is finished.
void binaryToBMP(char erosion_image[numByte])
{
  for (int i = 0; i < BMP_WIDTH; i++)
  {
    for (int j = 0; j < BMP_HEIGTH; j++)
    {
      if (getBit(erosion_image, i, j) == 0)
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

void printPicture(char erosion_image[numByte])
{
  char path[30];
  sprintf(path, "test_image\\image%d.bmp", pictureIncrementer++);

  binaryToBMP(erosion_image);
  write_bitmap(test_image, path);
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
  int timeTests = 1;

  for (int i = 0; i < timeTests; i++)
  {
    read_bitmap(argv[1], input_image);

    start = clock();
    toBinary(input_image, erosion_image);
    end = clock();
    toBinaryTime += end - start;

    start = clock();
    firstSeparation(erosion_image, cellDetectionThreshold);
    end = clock();
    separationTime = end - start;

    fillCopy(erosion_image, copy_image);

    start = clock();
    eroderDiag(erosion_image, copy_image);
    end = clock();
    erosionTime += end - start;
  }

  toBinaryTime = (toBinaryTime * 1000.0 / CLOCKS_PER_SEC) / timeTests;
  erosionTime = (erosionTime * 1000.0 / CLOCKS_PER_SEC) / timeTests;
  separationTime = (separationTime * 1000.0 / CLOCKS_PER_SEC) / timeTests;
  totalTime = toBinaryTime + separationTime + erosionTime;

  printf("\n toBinaryTime: %f \n ErosionTime: %f \n SeparationTime: %f \n TotalTime: %f\n\n", toBinaryTime, erosionTime, separationTime, totalTime);

  char finalPath[30];
  sprintf(finalPath, "results\\%s", argv[2]);
  write_bitmap(input_image, finalPath);

  printf("Done! \n Count: %d\n", counter);
  return 0;
}
