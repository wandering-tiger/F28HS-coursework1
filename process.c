/* This coursework specification, and the example code provided during the
 * course, is Copyright 2024 Heriot-Watt University.
 * Distributing this coursework specification or your solution to it outside
 * the university is academic misconduct and a violation of copyright law. */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel {
    // use unsigned int to read hex
    unsigned int red;
    unsigned int green;
    unsigned int blue;
};

/* An image loaded from a file. */
struct Image {
    /* TODO: Question 1 */
    int width;
    int height;
    struct Pixel **pixel;
};

/* Free a struct Image */
void free_image(struct Image *img)
{
    /* TODO: Question 2a */
    for(int i=0;i<img->width;i++){
        for(int j=0;j<img->height;j++){
            free(&img->pixel[i][j]);
            // free(*(img->pixel+i)+j);
        }
    }
    free(img);
}

/* Opens and reads an image file, returning a pointer to a new struct Image.
 * On error, prints an error message and returns NULL. */
struct Image *load_image(const char *filename)
{
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    /* Allocate the Image object, and read the image from the file */
    /* TODO: Question 2b */
    struct Image *img = (struct Image*)malloc(sizeof(struct Image));

    // read the header
    char* hshex=(char*)malloc(sizeof(char)*5);
    if(fscanf(f,"%s %d %d",hshex,&img->width,&img->height)==EOF){
        printf("wrong image\n");
        return img;
    }
    free(hshex);

    //print the size of image
    printf("image size is %d, %d\n",img->width,img->height);

    // read the image
    img->pixel=(struct Pixel**)malloc(sizeof(struct Pixel*)*img->height);
    for (int i=0; i<img->height;i++) {
        img->pixel[i] = (struct Pixel*)malloc(sizeof(struct Pixel)*img->width);
    }
    for(int i=0;i<img->height;i++){
        for(int j=0;j<img->width;j++){
            if(fscanf(f,"%x %x %x",&img->pixel[i][j].red,&img->pixel[i][j].green,&img->pixel[i][j].blue)==EOF){
                printf("wrong image\n");
                return img;
            }
        }
    }

    /* Close the file */
    fclose(f);

    if (img == NULL) {
        fprintf(stderr, "File %s could not be read.\n", filename);
        return NULL;
    }

    return img;
}

/* Write img to file filename. Return true on success, false on error. */
bool save_image(const struct Image *img, const char *filename)
{
    /* TODO: Question 2c */
    FILE *f = fopen(filename, "w");
    // write the header
    char *hshex= "HSHEX";
    if(fprintf(f,"%s %d %d ",hshex,img->width,img->height)==EOF){
        printf("write error\n");
        return false;
    }
    // write the image
    for(int i=0;i<img->height;i++){
        for(int j=0;j<img->width;j++){
            if(fprintf(f,"%x %x %x",img->pixel[i][j].red,img->pixel[i][j].green,img->pixel[i][j].blue)==EOF){
                printf("write error\n");
                return false;
            }
        }
    }
    fclose(f);
    return true;
}

/* Allocate a new struct Image and copy an existing struct Image's contents
 * into it. On error, returns NULL. */
struct Image *copy_image(const struct Image *source)
{
    /* TODO: Question 2d */
    
    return NULL;
}

/* Perform your first task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns a new struct Image containing the result, or NULL on error. */
struct Image *apply_BLUR(const struct Image *source)
{
    /* TODO: Question 3 */
    return NULL;
}

/* Perform your second task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns true on success, or false on error. */
bool apply_NORM(const struct Image *source)
{
    /* TODO: Question 4 */
    return false;
}

int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    /* TODO: Remove this if you don't need to use rand() */
    srand(time(NULL));

    /* Check command-line arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage: process INPUTFILE OUTPUTFILE\n");
        return 1;
    }

    /* Load the input image */
    struct Image *in_img = load_image(argv[1]);
    if (in_img == NULL) {
        return 1;
    }

    /* Test of saving the input image */
    if (!save_image(in_img, argv[2])) {
        fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
        return 1;
    }

    /* Apply the first process */
    struct Image *out_img = apply_BLUR(in_img);
    if (out_img == NULL) {
        fprintf(stderr, "First process failed.\n");
        free_image(in_img);
        return 1;
    }

    /* Apply the second process */
    if (!apply_NORM(out_img)) {
        fprintf(stderr, "Second process failed.\n");
        free_image(in_img);
        free_image(out_img);
        return 1;
    }

    /* Save the output image */
    if (!save_image(out_img, argv[2])) {
        fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
        free_image(in_img);
        free_image(out_img);
        return 1;
    }

    free_image(in_img);
    free_image(out_img);
    return 0;
}
