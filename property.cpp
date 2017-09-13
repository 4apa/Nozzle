#include "nozzle_lib.h"
//Binary search algorithm
size_t property::BinarySearch (double val, size_t DATA, const double *arr) const
{
    int l = 0;                  //left boundary
    int r = DATA - 1;           //right boundary
    double eps = ACCURACY_LEVEL;//accuracy

    while (l <= r)
    {
        int m = (r+l)/2;
        if (fabs(arr[m] - val) < eps) return m; //if lower then ACCURACY_LEVEL results are equal
        else if (arr[m] > val) r = m-1;
        else l = m+1;
    }
    return l; //if can't find, return the nearest neighbor from the right
}

double property::GetValue (double t, const double *a, size_t DATA, const double *arr) const
{
    size_t k = BinarySearch(t, DATA, arr);        //nearest neighbor from the right
    if (k == 0) return a[0];                      //value less than minimum
    if (k == DATA) return a[DATA - 1];            //value greater than the maximum
    return a[k-1] + (a[k] - a[k-1])*(t - arr[k-1])/
    (arr[k] - arr[k-1]);                          //linear aproximation of the value
}
