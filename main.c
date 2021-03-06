//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example_2.bmp example_grey.bmp

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"

//Function to invert pixels of an image (negative)

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
            
            if(output_image[x][y][0] <= 90){
              color = 0;
            } else{
              color = 255;
            }
            output_image[x][y][0] = color;
            output_image[x][y][1] = color;
            output_image[x][y][2] = color;
        }
    }
}

void erode (unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for (int x = 0; x < BMP_WIDTH; x++)
    {
        for (int y = 0; y < BMP_HEIGTH; y++)
        {
            
            if(output_image[x][y][0] <= 90){
              color = 0;
            } else{
              color = 255;
            }
            output_image[x][y][0] = color;
            output_image[x][y][1] = color;
            output_image[x][y][2] = color;
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

  //Declaring the array to store the image (unsigned char = unsigned 8 bit)
  unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
  unsigned char erode_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];

//Main function
int main(int argc, char** argv)
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
  toGreyScale(input_image,output_image);

  toBinary(output_image);

  for(i=0; i<BMP_WIDTH; i++){
    for (j=0 ; j<BMP_HEIGTH; j++){
      for (k=0 ; k<BMP_CHANNELS; k++){
        output_image[i][j][k]=erode_image[i][j][k]
      }
    }
  }

  erode(output_image,erode_image)

  //Save image to file
  write_bitmap(output_image, argv[2]);

  printf("Done!\n");
  return 0;
}
