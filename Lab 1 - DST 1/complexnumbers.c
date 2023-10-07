#include <stdio.h>
#include <math.h>


struct complex {double real; double imaginary;}; //declares the struct complex

struct complex z, z1, z2; //declares z,z1 and z2 as global structs to make it more convenient

struct complex conjugateZbar(struct complex z) {
    z.imaginary = -z.imaginary;

    return z; // makes a + bi into a - bi
}

struct complex complexAdd(struct complex z1,struct complex z2) { 
    // adds each part of z1 and z2 into z
    z.imaginary = z1.imaginary + z2.imaginary;
    z.real = z1.real + z2.real;

    return z;
}

struct complex complexMinus(struct complex z1,struct complex z2) { 
    // adds result of each z1 part minus each z2 part into z
    z.imaginary = z1.imaginary - z2.imaginary;
    z.real = z1.real - z2.real;

    return z;
}

struct complex complexMultiply(struct complex z1,struct complex z2) { 
    // multiplies z1 and z2 returns result as z
    z.imaginary = z1.imaginary * z2.real + z1.real * z2.imaginary; //all the xi numbers
    z.real = z1.real * z2.real - z1.imaginary * z2.imaginary; //xi^2 equals -x (real*real - i^2 parts * eachother)

    return z;
}

struct complex complexDivision(struct complex z1,struct complex z2) {
    // all the logic of division
    z.imaginary = (-1*z1.real*z2.imaginary+z1.imaginary*z2.real)/(pow(z2.real,2)+pow(z2.imaginary,2));
    z.real = (z1.real*z2.real+z1.imaginary*z2.imaginary)/(pow(z2.real,2)+pow(z2.imaginary,2));

    return z;
}

struct complex complexReciporal(struct complex z) { 
    float originalImaginary = z.imaginary; // declaring them as the structs values are set to different ones
    float originalReal = z.real;
    z.imaginary = -1*originalImaginary / (pow(originalReal,2)+pow(originalImaginary,2));
    z.real = originalReal / (pow(originalReal,2)+pow(originalImaginary,2));

    return z;
}

float complexMagnitute(struct complex z) {// calculates and returns magnitude
    float magnitude = sqrt((z.real*z.real) + (z.imaginary * z.imaginary));
    return magnitude;
}

int main(int argc, char const *argv[])
{
    
    printf("First real part please:\n");
    scanf("%f",&z1.real);
    printf("First imaginary part please:\n");
    scanf("%f",&z1.imaginary);

    printf("Second real part please:\n");
    scanf("%f",&z2.real);
    printf("Second imaginary part please:\n");
    scanf("%f",&z2.imaginary);
    

    /*
    z1.real = 2;
    z1.imaginary = 3;
    z2.real = 5;
    z2.imaginary = 7;
    */
    
    if (z2.imaginary == 0 && z2.real == 0)
    {
        printf("Please don't divide by zero \n");
    }
    else if(z1.imaginary == 0 && z1.real == 0)
    {
        printf("Please don't divide by zero \n");
    }
    
    

    z = complexAdd(z1,z2);
    printf("Complex add is : %f %fi \n", z.real,z.imaginary);
    
    z = complexMinus(z1,z2);
    printf("Complex minus is : %f %fi \n", z.real,z.imaginary);

    z = complexMultiply(z1,z2);
    printf("Complex multiply is : %f %fi \n", z.real,z.imaginary);

    z = complexDivision(z1,z2);
    printf("Complex division is : %f %fi \n", z.real,z.imaginary);

    z = conjugateZbar(z1);
    printf("Complex conjugate is : %f %fi \n", z.real,z.imaginary);

    z = complexReciporal(z1);
    printf("Complex reciporal is : %f %fi \n", z.real,z.imaginary);

    printf("Complex magnitude is : %f \n", complexMagnitute(z1));
    
    return 0;
}
