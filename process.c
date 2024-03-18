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
    // for(int i=0;i<img->width;i++){
    //     for(int j=0;j<img->height;j++){
    //         free(&img->pixel[i][j]);
    //         // free(*(img->pixel+i)+j);
    //     }
    // }
    // free(img);
    if (img == NULL) {
        return; // Nothing to free if the image is NULL
    }
    // Free the memory for pixel array
    if (img->pixel != NULL) {
        // Free each row of pixel array
        for (int i = 0; i < img->height; i++) {
            free(img->pixel[i]);
        }
        // Free the array of row pointers
        free(img->pixel);
    }
    // Free the struct Image itself
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
            if(fprintf(f,"%x %x %x ",img->pixel[i][j].red,img->pixel[i][j].green,img->pixel[i][j].blue)==EOF){
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
    // allocate struct Image
    struct Image *img = (struct Image*)malloc(sizeof(struct Image));
    img->width=source->width;
    img->height=source->height;
    img->pixel=(struct Pixel**)malloc(sizeof(struct Pixel*)*img->height);
    for (int i=0; i<img->height;i++) {
        img->pixel[i] = (struct Pixel*)malloc(sizeof(struct Pixel)*img->width);
    }

    // copy the image ? not sure about how to copy const struct
    for(int i=0;i<img->height;i++){
        for(int j=0;j<img->width;j++){
            img->pixel[i][j]=source->pixel[i][j];
        }
    }
    return img;
}

/* Perform your first task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns a new struct Image containing the result, or NULL on error. */
struct Image *apply_NOISE(const struct Image *source, const char *ns)
{
    /* TODO: Question 3 */
    // create a copy of original image
    struct Image *img = (struct Image*)malloc(sizeof(struct Image));
    img=copy_image(source);
    // generate the rand seed
    time_t t;
    srand((unsigned) time(&t));
    // convert strength to int
    int strength=atoi(ns);
    if(strength==0){
        return img;
    }
    // add the noise
    int temp;
    int divisor=strength*2+1;
    for(int i=0;i<img->height;i++){
        for(int j=0;j<img->width;j++){
            temp = (rand() % divisor)-strength;
            // avoid integer wraparound
            if(img->pixel[i][j].red+temp<=0xffff && img->pixel[i][j].red+temp>=0){
                img->pixel[i][j].red+=temp;
            }
            temp = (rand() % divisor)-strength;
            if(img->pixel[i][j].green+temp<=0xffff && img->pixel[i][j].green+temp>=0){
                img->pixel[i][j].green+=temp;
            }
            temp = (rand() % divisor)-strength;
            if(img->pixel[i][j].blue+temp<=0xffff && img->pixel[i][j].blue+temp>=0){
                img->pixel[i][j].blue+=temp;
            }
        }
    }
    return img;
}

/* Perform your second task.
 * (TODO: Write a better comment here, and rename the function.
 * You may need to add or change arguments depending on the task.)
 * Returns true on success, or false on error. */
bool apply_COMP(const struct Image *source, const struct Image *reference)
{
    /* TODO: Question 4 */
    if(source==NULL || reference==NULL){
        return false;
    }
    int identical=0,different=0;
    int coincident_width, coincident_height;
    // calculate the coincident part
    coincident_width= source->width > reference->width ? reference->width : source->width;
    coincident_height= source->height > reference->height ? reference->height : source->height;
    // count the same pixels
    for(int i=0;i<coincident_height;i++){
        for(int j=0;j<coincident_width;j++){
            if(source->pixel[i][j].red==reference->pixel[i][j].red && source->pixel[i][j].green==reference->pixel[i][j].green && source->pixel[i][j].blue==reference->pixel[i][j].blue){
                identical++;
            }
        }
    }
    // count the diffrent pixels
    int total=source->width*source->height + reference->width*reference->height;
    different=total-2*identical;

    printf("Identical pixels: %d\n",identical);
    printf("Different pixels: %d\n",different);
    return true;
}

int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    /* TODO: Remove this if you don't need to use rand() */
    srand(time(NULL));

    // read the number of input images
    int img_num=atoi(argv[1]);
    // create the arrays
    struct Image **image_list = (struct Image **)malloc(img_num * sizeof(struct Image *));
    struct Image **out_list = (struct Image **)malloc(img_num * sizeof(struct Image *));

    /* Check command-line arguments */
    // modify the argc number for noise task
    if (argc != 4+2*img_num) {
        fprintf(stderr, "Usage: process INPUTFILE OUTPUTFILE\n");
        return 1;
    }

    /* Load the input image */
    // struct Image *in_img = load_image(argv[2]);
    // if (in_img == NULL) {
    //     return 1;
    // }
    for(int i=0;i<img_num;i++){
        image_list[i] = load_image(argv[3+i]);
        if (image_list[i] == NULL) {
            return 1;
        }
    }

    /* Test of saving the input image */
    // if (!save_image(in_img, argv[2])) {
    //     fprintf(stderr, "Saving image to %s failed.\n", argv[2]);
    //     return 1;
    // }

    /* Test of copying the input image */
    // struct Image *copy_img = copy_image(in_img);
    // if (!save_image(copy_img, argv[2])) {
    //     fprintf(stderr, "Copying image to %s failed.\n", argv[2]);
    //     return 1;
    // }

    /* Apply the first process */
    // struct Image *out_img = apply_NOISE(in_img,argv[4]);
    // if (out_img == NULL) {
    //     fprintf(stderr, "First process failed.\n");
    //     free_image(in_img);
    //     return 1;
    // }
    for(int i=0;i<img_num;i++){
        out_list[i] = apply_NOISE(image_list[i],argv[3+2*img_num]);
        if (out_list[i] == NULL) {
            fprintf(stderr, "First process failed.\n");
            free_image(image_list[i]);
        return 1;
    }
    }

    /* Apply the second process */
    struct Image *reference_img = load_image(argv[2]);
    // if (!apply_COMP(in_img,reference_img)) {
    //     fprintf(stderr, "Second process failed.\n");
    //     free_image(in_img);
    //     free_image(out_img);
    //     return 1;
    // }
    for(int i=0;i<img_num;i++){
        if (!apply_COMP(image_list[i],reference_img)) {
            fprintf(stderr, "Second process failed.\n");
            free_image(image_list[i]);
            free_image(out_list[i]);
            return 1;
        }
    }
    free_image(reference_img);

    /* Save the output image */
    // if (!save_image(out_img, argv[3])) {
    //     fprintf(stderr, "Saving image to %s failed.\n", argv[3]);
    //     free_image(in_img);
    //     free_image(out_img);
    //     return 1;
    // }
    for(int i=0;i<img_num;i++){
        if(!save_image(out_list[i], argv[3+img_num+i])){
            fprintf(stderr, "Saving image to %s failed.\n", argv[3+img_num+i]);
            free_image(image_list[i]);
            free_image(out_list[i]);
            return 1;
        }
    }

    // free images
    // free_image(in_img);
    // free_image(out_img);
    for(int i=0;i<img_num;i++){
        free_image(image_list[i]);
        free_image(out_list[i]);
    }

    // free arrays
    free(image_list);
    free(out_list);

    return 0;
}
