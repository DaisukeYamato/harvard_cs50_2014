/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Risize a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int        n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];

    if (n < 1 || n > 100){
      printf(" n must be >= 1.\n");
      return 1;
    }
    
    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf, bf_new;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    bf_new = bf;
    

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi_new;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bi_new = bi;
    
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // resize bfSize
    int pad_old = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int pad_new = (4 - (n * bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    // total info (except for header + paddling)
    int content_size_bef = (bf.bfSize 
			    - sizeof(BITMAPFILEHEADER) 
			    - sizeof(BITMAPINFOHEADER))
      - pad_old * abs(bi.biHeight);
    int content_size_aft = sizeof(BITMAPFILEHEADER) 
      + sizeof(BITMAPINFOHEADER)
      + (content_size_bef * n * n)
      + pad_new * abs(bi.biHeight) * n;
    
    // new header
    bf_new.bfSize = content_size_aft;
    bi_new.biHeight = bi.biHeight * n;
    bi_new.biWidth = bi.biWidth * n;
    bi_new.biSizeImage = ((((bi_new.biWidth * bi_new.biBitCount) + 31) & ~31) / 8) * abs(bi_new.biHeight);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_new, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_new, sizeof(BITMAPINFOHEADER), 1, outptr);

     // Original RGB Array
    RGBTRIPLE org_rgb[abs(bi.biHeight)][bi.biWidth];
    // org_rgb[0][0].rgbtBlue = 0x00;
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
	  // temporary storage
	  //RGBTRIPLE triple;

	  // read RGB triple from infile
	  fread(&org_rgb[i][j], sizeof(RGBTRIPLE), 1, inptr);

	  // write RGB triple to outfile
	  //fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, pad_old, SEEK_CUR);
    }
    
    // write
    for (int i=0, abs_h = abs(bi_new.biHeight); i<abs_h; i++){
      for (int j=0, wid = bi_new.biWidth; j<wid; j++){
	//	printf("i: %d, j: %d, i/n: %d, j/n: %d", i, j, i/n, j/n);
	//	printf(" -- %x%x%x\n", org_rgb[i/n][j/n].rgbtBlue,
	//     org_rgb[i/n][j/n].rgbtGreen,
	//     org_rgb[i/n][j/n].rgbtRed);
	int row = floor(i/n);
	int col = floor(j/n);
	fwrite(&org_rgb[row][col], sizeof(RGBTRIPLE), 1, outptr);
	
      }
      for (int k=0; k<pad_new; k++){
	fputc(0x00, outptr);
      }
    }
    
    // check
    /*
    for (int i=0, abs_h = abs(bi.biHeight); i<abs_h; i++){
      for(int j=0, wid = bi.biWidth; j<wid; j++){
	printf("%x%x%x ", org_rgb[i][j].rgbtBlue,
	       org_rgb[i][j].rgbtGreen, org_rgb[i][j].rgbtRed); 
      }
      for (int k=0; k<pad_old; k++){
	printf("0x00 ");
      }
      printf("\n");
    }
    
    // new
    for (int i=0, abs_h = abs(bi_new.biHeight); i<abs_h; i++){
      for(int j=0, wid = bi_new.biWidth; j<wid; j++){
	int row = floor(i/n);
	int col = floor(j/n);
	printf("%x%x%x ", org_rgb[row][col].rgbtBlue,
	       org_rgb[row][col].rgbtGreen, org_rgb[row][col].rgbtRed); 
      }
      for (int k=0; k<pad_new; k++){
	printf("0x00 ");
      }
      printf("\n");
    }
    */
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
