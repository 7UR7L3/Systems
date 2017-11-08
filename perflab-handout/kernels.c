/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following student struct 
 */
student_t student = {
    "Michael Rahn",     /* First member full name */
    "mira9150@colorado.edu"  /* First member email address */
    // also "Elijah Fisher", "elfi3966@colorado.edu"...
};

/*
scp -r ./perflab-handout mira9150@perf-01.cs.colorado.edu:~
ssh mira9150@perf-01.cs.colorado.edu
cd perflab-handout
make driver
./ driver
./ driver -g
    ^ grades i guess
*/

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/
 
/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 0; i < dim; i++){
        for (j = 0; j < dim; j++){
            dst[RIDX(dim-1-j, i, dim)].red   = src[RIDX(i, j, dim)].red;
            dst[RIDX(dim-1-j, i, dim)].green = src[RIDX(i, j, dim)].green;
            dst[RIDX(dim-1-j, i, dim)].blue  = src[RIDX(i, j, dim)].blue;
        }
    }
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */

/*
src     dest
{{1 2 3}  {{3 6 9}
 {4 5 6}   {2 5 8}
 {7 8 9}}  {1 4 7}}

  i
1 2 3 4 5 6 7 8 9
3 6 9 2 5 8 1 4 7

dim*(dim-i/dim)+dim-i%dim
x'=-y
y'=x

i'=dim-1-j
j'=i
n'=i'*dim+j' = dim(dim-1-j)+i = dim*dim-dim-(i/dim)*dim+(i%dim)


dim*dim-dim-((i+j)%dim)*dim+((i+j)/dim)

*/
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    // 0 1 2 3 4 5 6 7 8

    // 1 2 3 4 5 6 7 8 9
    // 3 6 9 2 5 8 1 4 7

    int i, j;
    pixel *sp,*dp;
    for( i = 0; i < dim; i += 32 )
        for( j = dim - 1; j >= 0; j-- ) // goes backwards in row of source
        {
            sp = src + i * dim + j;
            dp = dst + dim * ( dim-1-j ) + i;

            *(dp   ) = *(sp); sp += dim; // goes through corresponding row of destination
            *(dp+ 1) = *(sp); sp += dim;
            *(dp+ 2) = *(sp); sp += dim;
            *(dp+ 3) = *(sp); sp += dim;
            *(dp+ 4) = *(sp); sp += dim;
            *(dp+ 5) = *(sp); sp += dim;
            *(dp+ 6) = *(sp); sp += dim;
            *(dp+ 7) = *(sp); sp += dim;
            *(dp+ 8) = *(sp); sp += dim;
            *(dp+ 9) = *(sp); sp += dim;
            *(dp+10) = *(sp); sp += dim;
            *(dp+11) = *(sp); sp += dim;
            *(dp+12) = *(sp); sp += dim;
            *(dp+13) = *(sp); sp += dim;
            *(dp+14) = *(sp); sp += dim;
            *(dp+15) = *(sp); sp += dim;
            *(dp+16) = *(sp); sp += dim;
            *(dp+17) = *(sp); sp += dim;
            *(dp+18) = *(sp); sp += dim;
            *(dp+19) = *(sp); sp += dim;
            *(dp+20) = *(sp); sp += dim;
            *(dp+21) = *(sp); sp += dim;
            *(dp+22) = *(sp); sp += dim;
            *(dp+23) = *(sp); sp += dim;
            *(dp+24) = *(sp); sp += dim;
            *(dp+25) = *(sp); sp += dim;
            *(dp+26) = *(sp); sp += dim;
            *(dp+27) = *(sp); sp += dim;
            *(dp+28) = *(sp); sp += dim;
            *(dp+29) = *(sp); sp += dim;
            *(dp+30) = *(sp); sp += dim;
            *(dp+31) = *(sp);
        }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/
 
/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* Compute min of two integers */
static int min(int a, int b) {
    if(a<b){
        return a;
    }else{
        return b;
    }
}

/* Compute max of two integers */
static int max(int a, int b) {
    if(a>b){
        return a;
    }else{
        return b;
    }
}

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum){
    sum->red = 0;
    sum->green = 0;
    sum->blue = 0;
    sum->num = 0;
}

static void accumulate_sum(pixel_sum *sum, pixel p){
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
}
/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j, ii, jj;
    pixel_sum ps;
    
    for (j = 0; j < dim; j++){
        for (i = 0; i < dim; i++){
            initialize_pixel_sum(&ps);
            for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++){
                for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++){
                    accumulate_sum(&ps, src[RIDX(ii,jj,dim)]);
                }
            }
            dst[RIDX(i,j,dim)].red   = ps.red/ps.num;
            dst[RIDX(i,j,dim)].green = ps.green/ps.num;
            dst[RIDX(i,j,dim)].blue  = ps.blue/ps.num;
        }
    }
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
    dst[0].red = ( src[0].red + src[1].red + src[dim].red + src[dim+1].red ) / 4;
    dst[0].green = ( src[0].green + src[1].green + src[dim].green + src[dim+1].green ) / 4;
    dst[0].blue = ( src[0].blue + src[1].blue + src[dim].blue + src[dim+1].blue ) / 4;

    dst[dim-1].red = ( src[dim-1].red + src[dim-2].red + src[dim-1+dim].red + src[dim-2+dim].red ) / 4;
    dst[dim-1].green = ( src[dim-1].green + src[dim-2].green + src[dim-1+dim].green + src[dim-2+dim].green ) / 4;
    dst[dim-1].blue = ( src[dim-1].blue + src[dim-2].blue + src[dim-1+dim].blue + src[dim-2+dim].blue ) / 4;

    int i = dim*(dim-1);
    dst[i].red = ( src[i].red + src[i-dim].red + src[i+1].red + src[i-dim+1].red ) / 4;
    dst[i].green = ( src[i].green + src[i-dim].green + src[i+1].green + src[i-dim+1].green ) / 4;
    dst[i].blue = ( src[i].blue + src[i-dim].blue + src[i+1].blue + src[i-dim+1].blue ) / 4;

    i = dim*dim-1;
    dst[i].red = ( src[i].red + src[i-dim].red + src[i-1].red + src[i-dim-1].red ) / 4;
    dst[i].green = ( src[i].green + src[i-dim].green + src[i-1].green + src[i-dim-1].green ) / 4;
    dst[i].blue = ( src[i].blue + src[i-dim].blue + src[i-1].blue + src[i-dim-1].blue ) / 4;

    int k;
    for( k = dim; k < dim*(dim-1); k += dim )
    {
        dst[k].red = ( src[k].red + src[k-dim].red + src[k+dim].red + src[k+1].red + src[k-dim+1].red + src[k+dim+1].red ) / 6;
        dst[k].green = ( src[k].green + src[k-dim].green + src[k+dim].green + src[k+1].green + src[k-dim+1].green + src[k+dim+1].green ) / 6;
        dst[k].blue = ( src[k].blue + src[k-dim].blue + src[k+dim].blue + src[k+1].blue + src[k-dim+1].blue + src[k+dim+1].blue ) / 6;
    }

    for( k = dim-1+dim; k < dim*dim-1; k += dim )
    {
        dst[k].red = ( src[k].red + src[k-dim].red + src[k+dim].red + src[k-1].red + src[k-dim-1].red + src[k+dim-1].red ) / 6;
        dst[k].green = ( src[k].green + src[k-dim].green + src[k+dim].green + src[k-1].green + src[k-dim-1].green + src[k+dim-1].green ) / 6;
        dst[k].blue = ( src[k].blue + src[k-dim].blue + src[k+dim].blue + src[k-1].blue + src[k-dim-1].blue + src[k+dim-1].blue ) / 6;
    }

    for( k = 1; k < dim-1; k++ )
    {
        dst[k].red = ( src[k].red + src[k-1].red + src[k+1].red + src[k+dim].red + src[k+dim-1].red + src[k+dim+1].red ) / 6;
        dst[k].green = ( src[k].green + src[k-1].green + src[k+1].green + src[k+dim].green + src[k+dim-1].green + src[k+dim+1].green ) / 6;
        dst[k].blue = ( src[k].blue + src[k-1].blue + src[k+1].blue + src[k+dim].blue + src[k+dim-1].blue + src[k+dim+1].blue ) / 6;
    }

    for( k = dim*(dim-1)+1; k < dim*dim-1; k++ )
    {
        dst[k].red = ( src[k].red + src[k-1].red + src[k+1].red + src[k-dim].red + src[k-dim-1].red + src[k-dim+1].red ) / 6;
        dst[k].green = ( src[k].green + src[k-1].green + src[k+1].green + src[k-dim].green + src[k-dim-1].green + src[k-dim+1].green ) / 6;
        dst[k].blue = ( src[k].blue + src[k-1].blue + src[k+1].blue + src[k-dim].blue + src[k-dim-1].blue + src[k-dim+1].blue ) / 6;
    }

    int j;
    int a = dim+1;
    for( i = 1; i < dim-1; i++ )
    {
        for( j = 1; j < dim-1; j++ )
        {
            dst[a].red =
            (
                src[a-1-dim].red + src[a-dim].red + src[a+1-dim].red +
                src[a-1    ].red     + src[a].red + src[a+1    ].red +
                src[a-1+dim].red + src[a+dim].red + src[a+1+dim].red
            ) / 9;
            dst[a].green =
            (
                src[a-1-dim].green + src[a-dim].green + src[a+1-dim].green +
                src[a-1    ].green + src[a    ].green + src[a+1    ].green +
                src[a-1+dim].green + src[a+dim].green + src[a+1+dim].green
            ) / 9;
            dst[a].blue =
            (
                src[a-1-dim].blue + src[a-dim].blue + src[a+1-dim].blue +
                src[a-1    ].blue + src[a    ].blue + src[a+1    ].blue +
                src[a-1+dim].blue + src[a+dim].blue + src[a+1+dim].blue
            ) / 9;
            a++;
        }
        a += 2;
    }
    
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

