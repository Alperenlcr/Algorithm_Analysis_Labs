#include <stdio.h>
#include <time.h>       // for clock_t, clock(), CLOCKS_PER_SEC
#include <stdlib.h>
#define MIN -100000

struct result
{
    int left;
    int right;
    int max;
};


int power(int base, int a)
{
    if (a != 0)
        return (base * power(base, a - 1));
    else
        return 1;
}


void create_random_array(int min_number, int max_number, int length, int** arr)
{
    int range = max_number - min_number, i;
    *arr = (int*) malloc(length*sizeof(int));

    if (*arr == NULL)
        return;

    for(i = 0; i < length; i++)
        (*arr)[i] = (rand()%range)+min_number;
}


struct result brute_force(int* arr, int left, int right)
{
    struct result r;
    int i, j;
    long int sum;
    r.max = MIN;
    for ( i = left; i <= right; i++)
    {
        sum = 0;
        for ( j = i; j <= right; j++)
        {
            sum += arr[j];
            if (r.max < sum)
            {
                r.max = sum;
                r.left = i;
                r.right = j;
            }
        }
    }
    return r;
}


struct result find_dmax(int* arr, int left, int right)
{
    struct result r;
    // choosing mid point
    int N = right - left + 1,
            mid = N%2==1 ? left+(N+1)/2 : arr[(N/2)-1] > arr[(N/2)] ? (N/2)-1+left : (N/2)+left,
                i;
    long int sum;

    // searching right side
    r.max = arr[mid];
    sum = r.max;
    r.left = mid;
    r.right = mid;

    for ( i = mid+1; i <= right; i++)
    {
        sum += arr[i];
        if (r.max < sum)
        {
            r.max = sum;
            r.right = i;
        }
    }

    sum = r.max;

    for ( i = mid-1; i >= left; i--)
    {
        sum += arr[i];
        if (r.max < sum)
        {
            r.max = sum;
            r.left = i;
        }
    }

    return r;
}


struct result solution(int* arr, int l, int r)
{
// stop condition 1 length
    if (r == l)
        return (struct result) {.left = l, .right = r, .max = arr[l]};


// solution left
    struct result b_max = solution(arr, l, (r+l)/2);

// solution right
    struct result c_max = solution(arr, ((r+l)/2)+1, r);

    struct result d_max = find_dmax(arr, b_max.left, c_max.right);

    if (b_max.max >= c_max.max && b_max.max >= d_max.max)
        return b_max;
    else if (c_max.max >= b_max.max && c_max.max >= d_max.max)
        return c_max;
    else
        return d_max;
}


void check_correctness(int size, int min, int max)
{
    int i;
    int* arr;
    printf("\nChecking correctness\n-------------------------\n");  //https://ostermiller.org/calc/sum.html
    create_random_array(min, max, size, &arr);
 //   int arr[19] = {1, -2, 4, 6, -5, 10, -5, -1, 9, 3, -2, 2, 0, 1, -6, 5, -1, -4, 2};
  //  size = 19;
    struct result recursive_sol = solution(arr, 0, size-1);
    printf("ARRAY : ");
    for ( i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
    printf("Result: %7d, %7d, %7d\n", recursive_sol.left, recursive_sol.right, recursive_sol.max);
    free(arr);
}


int main()
{
    srand(time(NULL));
    int* arr, i, size;
    clock_t end, begin;
    double time_spent;

// 1000 elements in range [-100, 100]
    check_correctness(1000, -100, 100);
// 15 elements in range [-100, -10]
    check_correctness(15, -100, -10);
// 15 elements in range [50, 60]
    check_correctness(15, 50, 60);
// 10 elements in range [-10, 10]
    check_correctness(10, -10, 10);

    printf("\n\nTest cases for time complexity\n-----------------------------\n");
    for ( i = 1; i < 6; i++)
    {
        size = power(10, i);
        create_random_array(-1000, 100, size, &arr);
    // to store the execution time of code
        time_spent = 0.0;
        begin = clock();

        struct result recursive_sol = solution(arr, 0, size-1);

        end = clock();
        // calculate elapsed time by finding difference (end - begin) and
        // dividing the difference by CLOCKS_PER_SEC to convert to seconds
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Size: 10^%d     Result: %7d, %7d, %7d     Time Recursive: %f\n", i, recursive_sol.left, recursive_sol.right, recursive_sol.max, time_spent);

    // to store the execution time of code
        time_spent = 0.0;
        begin = clock();

        struct result brute_sol = brute_force(arr, 0, size-1);

        end = clock();
        // calculate elapsed time by finding difference (end - begin) and
        // dividing the difference by CLOCKS_PER_SEC to convert to seconds
        time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Size: 10^%d     Result: %7d, %7d, %7d     Time Brute Force: %f\n\n", i, brute_sol.left, brute_sol.right, brute_sol.max, time_spent);

        free(arr);
    }

    return 0;
}
