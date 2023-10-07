#include <stdio.h>
#include <math.h>
#define SIZE 100

struct complex {
    float real; 
    float imaginary;
};

float realVector[] = {
    10.00000, 5.000000, 3.333333, 2.500000, 2.000000, 1.666667, 1.428571, 1.250000, 1.111111, 1.000000,
    0.909091, 0.833333, 0.769231, 0.714286, 0.666667, 0.625000, 0.588235, 0.555556, 0.526316, 0.500000,
    0.476190, 0.454545, 0.434783, 0.416667, 0.400000, 0.384615, 0.370370, 0.357143, 0.344828, 0.333333,
    0.322581, 0.312500, 0.303030, 0.294118, 0.285714, 0.277778, 0.270270, 0.263158, 0.256410, 0.250000,
    0.243902, 0.238095, 0.232558, 0.227273, 0.222222, 0.217391, 0.212766, 0.208333, 0.204082, 0.200000,
    0.196078, 0.192308, 0.188679, 0.185185, 0.181818, 0.178571, 0.175439, 0.172414, 0.169492, 0.166667,
    0.163934, 0.161290, 0.158730, 0.156250, 0.153846, 0.151515, 0.149254, 0.147059, 0.144928, 0.142857,
    0.140845, 0.138889, 0.136986, 0.135135, 0.133333, 0.131579, 0.129870, 0.128205, 0.126582, 0.125000,
    0.123457, 0.121951, 0.120482, 0.119048, 0.117647, 0.116279, 0.114943, 0.113636, 0.112360, 0.111111,
    0.109890, 0.108696, 0.107527, 0.106383, 0.105263, 0.104167, 0.103093, 0.102041, 0.101010, 0.100000
};

float imaginaryVector[] = {
    7.000000, 3.500000, 2.333333, 1.750000, 1.400000, 1.166667, 1.000000, 0.875000, 0.777778, 0.700000,
    0.636364, 0.583333, 0.538462, 0.500000, 0.466667, 0.437500, 0.411765, 0.388889, 0.368421, 0.350000,
    0.333333, 0.318182, 0.304348, 0.291667, 0.280000, 0.269231, 0.259259, 0.250000, 0.241379, 0.233333,
    0.225806, 0.218750, 0.212121, 0.205882, 0.200000, 0.194444, 0.189189, 0.184211, 0.179487, 0.175000,
    0.170732, 0.166667, 0.162791, 0.159091, 0.155556, 0.152174, 0.148936, 0.145833, 0.142857, 0.140000,
    0.137255, 0.134615, 0.132075, 0.129630, 0.127273, 0.125000, 0.122807, 0.120690, 0.118644, 0.116667,
    0.114754, 0.112903, 0.111111, 0.109375, 0.107692, 0.106061, 0.104478, 0.102941, 0.101449, 0.100000,
    0.098592, 0.097222, 0.095890, 0.094595, 0.093333, 0.092105, 0.090909, 0.089744, 0.088608, 0.087500,
    0.086420, 0.085366, 0.084337, 0.083333, 0.082353, 0.081395, 0.080460, 0.079545, 0.078652, 0.077778,
    0.076923, 0.076087, 0.075269, 0.074468, 0.073684, 0.072917, 0.072165, 0.071429, 0.070707, 0.070000
};

double averageMagnitute(struct complex *a, int size) { // Average Magnitude
    double combinedMagnitude = 0;
    double counter = 0;
    for (int i = 0; i < size; i++)
    {
        struct complex checkComplex = a[i]; // creates new struct out of information at pointer index each loop
        double magnitude = sqrt((checkComplex.real * checkComplex.real) + (checkComplex.imaginary * checkComplex.imaginary)); //calc
        combinedMagnitude += magnitude;
        counter++;
    }
    double averageMagnitude = combinedMagnitude/counter;
    return averageMagnitude;
}

double minumimMagnitute(struct complex *a, int size) { // Gets the minimum magnitude
    struct complex firstVal = a[0];
    double minMagnitude = sqrt((firstVal.real * firstVal.real) + (firstVal.imaginary * firstVal.imaginary)); //puts the first value to min to have a value.
    for (int i = 1; i < size; i++)// checks all but the first value as that is already in the declaration
    {
        struct complex checkComplex = a[i];
        double magnitude = sqrt((checkComplex.real * checkComplex.real) + (checkComplex.imaginary * checkComplex.imaginary));
        if (magnitude < minMagnitude)//checks if it is smaller
        {
            minMagnitude = magnitude;
        }
    }    
    return minMagnitude;
}

double maximumMagnitute(struct complex *a, int size) { // Gets the maximum magnitude
    double maxMagnitude = 0;// intitial can be 0 instead of a value
    for (int i = 0; i < size; i++)
    {
        struct complex checkComplex = a[i];
        double magnitude = sqrt((checkComplex.real * checkComplex.real) + (checkComplex.imaginary * checkComplex.imaginary));
        if (magnitude > maxMagnitude)//checks if it is bigger
        {
            maxMagnitude = magnitude;
        }
    }    
    return maxMagnitude;
}

double varianceMagnitute(struct complex *a, int size) { // calculates variance of the magnitude

    double avgMagnitute = averageMagnitute(a, size); // fetches average magnitude

    double counter = 0;
    double sumValuesMinusAverage = 0;
    for (int i = 0; i < size; i++)
    {
        struct complex checkComplex = a[i];
        double magnitude = sqrt((checkComplex.real * checkComplex.real) + (checkComplex.imaginary * checkComplex.imaginary));
        sumValuesMinusAverage += (pow((magnitude - avgMagnitute),2));
        counter++;
    }
    double varianceMagnitude = sumValuesMinusAverage/(counter);
    return varianceMagnitude;
}

int main(int argc, char const *argv[])
{
    struct complex theComplex[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        theComplex[i].real = realVector[i];
        theComplex[i].imaginary = imaginaryVector[i];
    }
    
    printf("Average Magnitude: %f \n",averageMagnitute(theComplex, SIZE));
    printf("Minimum Magnitude: %f \n",minumimMagnitute(theComplex, SIZE));
    printf("Maximum Magnitude: %f \n",maximumMagnitute(theComplex, SIZE));
    printf("Variance in Magnitudes: %f \n",varianceMagnitute(theComplex, SIZE));
    
    return 0;
}
