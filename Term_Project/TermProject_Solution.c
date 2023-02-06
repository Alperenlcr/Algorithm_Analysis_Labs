#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prints int matrix or string matrix according to given parameter
void print_matrix(int **m, char **different_strings, int N, int i_s)
{
    int i, j;
    printf("\n");
    if (i_s == 1)
    {
        for ( i = 0; i < N; i++)
        {
            for ( j = 0; j < N; j++)
                printf("%3d ", m[i][j]);
            printf("\n");
        }
    }
    else
    {
        for ( i = 0; i < N; i++)
        {
            for ( j = 0; j < N; j++)
                printf("%20s ", different_strings[m[i][j]]);
            printf("\n");
        }
    }
    printf("\n");
}

// takes mode and N value from user and allocates int matrix
int** read_N_and_mode_allocate_matrix(int *N, int *mode)
{
    int **matrix;
    int i = 0;
    printf("Mode (Normal 1, Detailed 2): ");
    scanf("%d", mode);
    do
    {
        if (i != 0)
            printf("\nN must be greater than 2 and smaller than 9 !!\n");
        printf("N : ");
        scanf("%d", N);
        i ++;
    } while (*N<3 || *N>8);

    matrix = (int **) malloc((*N)*sizeof(int*));
    for ( i = 0; i < *N; i++)
        matrix[i] = (int *) malloc((*N)*sizeof(int));

    return matrix;
}

// takes string matrix words one by one from user and converts it to int matrix, also stores different words and key to reach them is their index
char** read_string_matrix_generate_int_matrix(int N, int **matrix)
{
    int i, j, k;
    char **different_strings, temp[100];

    different_strings = (char **) malloc(N*sizeof(char*));
    // first row input
    for ( i = 0; i < N; i++)
    {
        matrix[0][i] = i;
        scanf("%s", temp);
        different_strings[i] = (char *) malloc(strlen(temp));
        strcpy(different_strings[i], temp);
    }
    for ( i = 1; i < N; i++)
    {
        for ( j = 0; j < N; j++)
        {
            scanf("%s", temp);

            // search word in different_strings array
            k = 0;
            while (k != N)
            {
                if (strcmp(temp, different_strings[k]) == 0)
                {
                    matrix[i][j] = k;
                    k = N-1;
                }
                k ++;
            }
        }
    }

    return different_strings;
}

// shifts a row
void shift(int **m, int row_index, int N, int mode)
{
    int j, temp;
    temp = m[row_index][N-1];
    for ( j = N-1; j >= 0; j--)
        m[row_index][j+1] = m[row_index][j];
    m[row_index][0] = temp;
    char **tempp;
    if (mode != 1)
        print_matrix(m, tempp, N, 1);
}

// checks if the row values repeat themself at upper rows
int check_columns_upward(int **m, int row_index, int N)
{
    if (row_index == 0)
        return 1;

    int i, j=0, flag=1;
    while ( j < N && flag )
    {
        i = row_index;
        while ( i-- > 0 && flag )
            if (m[row_index][j] == m[i][j])
                flag = 0;
        j ++;
    }
    return flag;
}

int solution(int **matrix, int *shift_counts, int row_index, int N, int mode)
{
    int flag;
    if (row_index != N && row_index != -1)
    {
        if (check_columns_upward(matrix, row_index, N)) // if no same values at the same column
            row_index ++;
        else
        {
            if (shift_counts[row_index] < N-1)    // shift the row if not completed its period
            {
                shift(matrix, row_index, N, mode);
                shift_counts[row_index] ++;
            }
            else
            {
                shift_counts[row_index] = 0;
                row_index --;
                flag = 0;
                while (row_index >= 0 && !flag)
                {
                    if (shift_counts[row_index] < N-1)
                    {
                        shift(matrix, row_index, N, mode);
                        shift_counts[row_index] += 1;
                        flag = 1;
                    }
                    else
                    {
                        shift_counts[row_index] = 0;
                        row_index --;
                    }
                }
            }
        }
        solution(matrix, shift_counts, row_index, N, mode);
    }
    else if (row_index == N)
        return 1;
    else if (row_index == -1)
        return 0;
}

int main()
{
    int mode, N, **matrix = read_N_and_mode_allocate_matrix(&N, &mode);
    char **different_strings = read_string_matrix_generate_int_matrix(N, matrix);

    if (mode == 2)
    {
        // print initial matrixs
        print_matrix(matrix, different_strings, N, 1);
        print_matrix(matrix, different_strings, N, 2);
    }
    int *shift_counts = (int *) calloc(N, sizeof(int)), row_index = 1;
    if (solution(matrix, shift_counts, row_index, N, mode) == 1)
    {
        printf("\nMatrix is rearranged\n");
        if (mode == 2)
            print_matrix(matrix, different_strings, N, 1);
        print_matrix(matrix, different_strings, N, 2);
    }
    else
        printf("\nMatrix can not be rearranged\n");

    return 0;
}

/*

3
Kirmizi
Mavi
Yesil
Yesil
Mavi
Kirmizi
Kirmizi
Yesil
Mavi


3
Mor
Yesil
Siyah
Siyah
Mor
Yesil
Yesil
Siyah
Mor


5
1
2
3
4
5
1
2
3
4
5
1
2
3
4
5
1
2
3
4
5
1
2
3
4
5

3
a
b
c
b
a
c
c
a
b


5
5
1
2
3
4
5
1
2
3
4
4
5
1
2
3
3
4
5
1
2
2
3
4
5
1

4
s
y
k
m
y
m
k
s
s
m
k
y
y
s
k
m

*/


    /*
    First generate an array called shift_counts initial values are 0s
    Initialize row_index to 1
    call Funtion (exit conditions are ; row_index is equal to N or -1   (if recursion breaks because of row_index=-1, then no solution. if row_index=N, then matrix is reordered))
    In function
        check if N and above satisfies the rule
        satisfies:
            increment row_count
        not satisfies:
            check if the row is shiftable (shift_counts[row_index] < N-1)
            shiftable:
                shift and increase shift_counts[row_counts]
            not shiftable:
                move to upper row
                shift_counts[row_index] = 0
                row_index --;
                search for a shiftable row upwards, everytime going up reset shift count
                if not found
                    exit with no solution
                if found
                    shift that row
        Call Funtion

    PSEUDO CODE
    -----------
    shift_counts[N] = {0}
    row_index = 1
    solution(matrix, shift_counts, row_index, N, mode)
    solution:
        if (row_index != N && row_index != -1)
            if check_columns_upward(matrix, row_index, N):
                row_index += 1
            else:
                if shift_counts[row_index] < N-1:
                    shift(matrix, row_index, N)
                    shift_counts[row_index] += 1
                else:
                    shift_counts[row_index] = 0
                    row_index -= 1
                    flag = 0;
                    while row_index >= 0 and (not flag):
                        if shift_counts[row_index] < N-1:
                            shift(matrix, row_index, N)
                            shift_counts[row_index] += 1
                            flag = 1
                        else:
                            shift_counts[row_index] = 0
                            row_index -= 1
            solution(matrix, shift_counts, row_index, N, mode)
        else if row_index == N:
            solution found
        else if row_index == N:
            solution not found


    */