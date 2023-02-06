#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRINGSIZE 200  // for links, key words, query

///////////////////////// HASHTABLE FUNCTIONS STARTS /////////////////////////
struct item
{
    int attempt_count;
    char* key;
    char** values;
};
typedef struct item item;

struct hashTable
{
    item* data;
};
typedef struct hashTable hashTable;

void init_hashTable(int size, hashTable* h)
{
    int i;
    h->data = (item*) malloc(size*sizeof(item));

    for ( i = 0; i < size; i++)
    {
        h->data[i].key = NULL;
        h->data[i].values = (char**) malloc(sizeof(char*));
        h->data[i].values[0] = NULL;
    }
}

// for every key generate a corresponding index
int hashcode_horner(char* key_i, int n)
{
    int length = strlen(key_i), i, temp;
    unsigned long int hashKey;
    for ( i = 0; i < length; i++)
    {
        if (key_i[i] < 'Z')
            temp = key_i[i] - 'A' + 1;
        else
            temp = key_i[i] - 'a' + 1;
        hashKey = 31 * hashKey + temp;
    }
    return (int) (hashKey%n);
}

void insert(char* key_i, char* value_i, hashTable* h, int size)
{
// hornere gore index bul
    int i=0, index = hashcode_horner(key_i, size), count=1;
// indexten baslayarak ilk bos veya ayni keyli yeri bul
    while ((h->data[index].key != NULL) && (strcmp(h->data[index].key, key_i) != 0))
    {
        count ++;
        index = (index+1) % size;
    }
// ekleme
    h->data[index].key = (char*) malloc(strlen(key_i)+1);
    strcpy(h->data[index].key, key_i);
    h->data[index].attempt_count = count;

    while (h->data[index].values[i] != NULL)
        i ++;
    h->data[index].values = (char**) realloc(h->data[index].values, sizeof(char*)*(2*(i+1)));    // sadece i+1 yapinca belli bir ekleme sonrasi (realloc(): invalid next size) hatasi cikiyor ???
    h->data[index].values[i] = (char*) malloc(strlen(value_i)+1);
    strcpy(h->data[index].values[i++], value_i);
    h->data[index].values[i] = NULL; 
}

char** search(char* key_i, hashTable h, int size)
{
// hornere gore index bul
    int i=0, index = hashcode_horner(key_i, size);
// indexten baslayarak aramaya basla
    while ((h.data[index].key == NULL || strcmp(h.data[index].key, key_i) != 0) && i++ < size)
        index = (index+1) % size;

    if (h.data[index].key == NULL || strcmp(h.data[index].key, key_i) != 0)
        return NULL;
    i = 0;
    return h.data[index].values;
}

int isPrime(int n)
{
    // Corner case
    if (n <= 1)
        return 0;

    // Check from 2 to n-1
    for (int i = 2; i < n; i++)
        if (n % i == 0)
            return 0;

    return 1;
}

int find_size(int n, float lf)
{
    int r = n*(1.0/lf);
    while (!isPrime(r))
        r ++;
    return r;
}
//////////////////////// END ////////////////////////

//////////////////////// SIDE FUNCTIONS ////////////////////////
void Merge(char* arr[],int low,int mid,int high) //Merging the Array Function
{
    int nL= mid-low+1;
    int nR= high-mid;
    char** L=(char**) malloc(sizeof(char *)*nL);
    char** R=(char**) malloc(sizeof(char *)*nR);
    int i, j=0, k;
    for(i=0;i<nL;i++)
    {
        L[i]=(char*) malloc(sizeof(arr[low+i]));
        strcpy(L[i],arr[low+i]);
    }
    for(i=0;i<nR;i++)
    {
        R[i]=(char*) malloc(strlen(arr[mid+i+1]));
        strcpy(R[i],arr[mid+i+1]);
    }
    i=0;
    k=low;
    while(i<nL&&j<nR)
    {
        if(strcmp(L[i],R[j])<0)strcpy(arr[k++],L[i++]);
        else strcpy(arr[k++],R[j++]);
    }
    while(i<nL)strcpy(arr[k++],L[i++]);
    while(j<nR)strcpy(arr[k++],R[j++]);
}

void MergeSort(char* arr[],int low,int high) //Main MergeSort function
{
    if(low<high)
    {
        int mid=(low+high)/2;
        MergeSort(arr,low,mid);
        MergeSort(arr,mid+1,high);
        Merge(arr,low,mid,high);
    }
}

int find_intersection(char* array1[], char* array2[], char* intersection_array[], int size1, int size2)
{
    int i = 0, j = 0, k = 0;
    while ((i < size1) && (j < size2))
    {
        if (strcmp(array1[i], array2[j]) < 0)
            i++;
        else if (strcmp(array1[i], array2[j]) > 0)
            j++;
        else
        {
            strcpy(intersection_array[k], array1[i]);
            i++;
            j++;
            k++;
        }
    }
    return k;
}
 
int find_union(char* array1[], char* array2[], char* union_array[], int size1, int size2)
{
    int i = 0, j = 0, k = 0;
    while ((i < size1) && (j < size2))
    {
        if (strcmp(array1[i], array2[j]) < 0)
        {
            strcpy(union_array[k], array1[i]);
            i++;
            k++;
        }
        else if (strcmp(array1[i], array2[j]) > 0)
        {
            strcpy(union_array[k], array2[j]);
            j++;
            k++;
        }
        else
        {
            strcpy(union_array[k], array1[i]);
            i++;
            j++;
            k++;
        }
    }
    if (i == size1)
    {
        while (j < size2)
        {
            strcpy(union_array[k], array2[j]);
            j++;
            k++;
        }
    }
    else
    {
        while (i < size1)
        {
            strcpy(union_array[k], array1[i]);
            i++;
            k++;
        }
    }
    return k;
}
//////////////////////// END ////////////////////////

int find_different_keys_count(char** strings, int len)
{
// first we sort
    MergeSort(strings, 0, len-1);
// then find count of different elements
    int i, count = 1;
    for (i = 0; i < len-1; i++)
        if (strcmp(strings[i], strings[i+1]) != 0)
            count ++;
    return count;
}

int read_file_and_create_hashTable(float load_factor, hashTable* h)
{
    FILE *frp;
    int flag, j, l=0, k=0, i=0, different_keys_count;
    char c, link[STRINGSIZE];
    char** keys;

// Opening file
    if ((frp = fopen("Sample.txt", "r")) == NULL)
    {
        printf("\nError! opening file\n");
        exit(1);
    }

// Find number of keys
    // count spaces and enters -> answer is spaces + (enters+1)/2
    int space_count=0, enter_count=0, keys_size;
    while ((c=fgetc(frp)) != EOF) {
        if (c == ' ')
            space_count ++;
        else if (c == '\n')
            enter_count ++;
    }
    keys_size = space_count + (enter_count+1)/2;
    rewind(frp);

// dynamically allocate keys
    keys = (char**) malloc(keys_size*sizeof(char*));
    for ( j = 0; j < keys_size; j++)
        keys[j] = (char*) malloc(STRINGSIZE);

// Find how many different keys that we have
    flag = 0;
    while ((c=fgetc(frp)) != EOF) {
        if (flag == 0)
        {
            if (c == '\n')
                flag = 1;
        }
        else
        {
            keys[k][i++] = c;
            if (c == ' ')
            {
                keys[k][--i] = '\0';
                i = 0;
                k ++;
            }
            else if (c == '\n')
            {
                keys[k][--i] = '\0';
                i = 0;
                k ++;
                flag = 0;
            } 
        }
    }
    rewind(frp);
    different_keys_count = find_different_keys_count(keys, k);
    k=0;

// init hashTable
    int size = find_size(different_keys_count, load_factor);
    init_hashTable(size, h);

// insert to hashTable
    flag = 0;
    while ((c=fgetc(frp)) != EOF) {
        if (flag == 0)
        {
            link[l++] = c;
            if (c == '\n')
            {
                link[--l] = '\0';
                l = 0;
                flag = 1;
            }
        }
        else
        {
            keys[k][i++] = c;
            if (c == ' ')
            {
                keys[k][--i] = '\0';
                i = 0;
                k ++;
            }
            else if (c == '\n')
            {
                keys[k][--i] = '\0';
                for ( j = 0; j <= k; j++)
                    insert(keys[j], link, h, size);
                i = 0;
                k = 0;
                flag = 0;
            } 
        }
    }
    fclose(frp);
    free(keys);
    return size;
}

void normal_mode(hashTable h, int size)
{
    int i, j, k;
    char** r, **r1, **r2, **UNIONarray, **INTERSECTIONarray;
    char query[STRINGSIZE], query_ori[STRINGSIZE];
// get query
    printf("Enter Query use (ve, veya) to separate : ");
    scanf("%[^\n]%*c", query);
    strcpy(query_ori, query);

// generate words
    char** words;   // to store keys in query
    char* token;
    int count = 0;
    i = 0;
    while (query[i++] != '\0')
        if (query[i] == ' ')
            count ++;
    count = (count/2)+1;

    words = (char**) malloc(count*sizeof(char*));
    for (int i = 0; i < count; i++)
        words[i] = (char*) malloc(STRINGSIZE);

    token = strtok(query, " ");
    i = 0; j = 0;
    while (token)
    {
        i ++;
        if (i % 2 == 1)
            strcpy(words[j++], token);
        token = strtok(NULL, " ");
    }

// intersection for first two key search result value arrays
    if (strstr(query_ori, " ve ") != NULL)
    {
// arr1
        r = search(words[0], h, size);
        int size1=0;j=0;
        if (r == NULL)
        {
            printf("\n%s is not found in hashtable\n", words[0]);
            size1 = 0;
        }
        else
            while (r[j++] != NULL)
                size1 ++;
        r1 = (char**) malloc(size1*sizeof(char*));
        printf("\nKey : %s\nValues : ", words[0]);
        for ( i = 0; i < size1; i++)
        {
            r1[i] = (char*) malloc(STRINGSIZE);
            strcpy(r1[i], r[i]+8);          // https:// i atmam gerekiyor cunku merge sort calismiyor niyese
            printf("https://%s ", r1[i]);
        }
        printf("\n");
        MergeSort(r1, 0, size1-1);
// arr2
        r = search(words[1], h, size);
        int size2=0;j=0;
        if (r == NULL)
        {
            printf("\n%s is not found in hashtable\n", words[1]);
            size2 = 0;
        }
        else
            while (r[j++] != NULL)
                size2 ++;
        r2 = (char**) malloc(size2*sizeof(char*));
        printf("\nKey : %s\nValues : ", words[1]);
        for ( i = 0; i < size2; i++)
        {
            r2[i] = (char*) malloc(STRINGSIZE);
            strcpy(r2[i], r[i]+8);
            printf("https://%s ", r2[i]);
        }
        printf("\n");
        MergeSort(r2, 0, size2-1);
// intersection
        int size_intersection;
        if (size1 < size2)
            size_intersection = size1;
        else
            size_intersection = size2;
        INTERSECTIONarray = (char**) malloc(size_intersection*sizeof(char*));
        for (i = 0; i < size_intersection; i++)
            INTERSECTIONarray[i] = (char*) malloc(STRINGSIZE);
        size_intersection = find_intersection(r1, r2, INTERSECTIONarray, size1, size2);
        printf("\nINTERSECTION array size : %d  Array : ", size_intersection);
        for (i = 0; i < size_intersection; i++)
            printf("https://%s ", INTERSECTIONarray[i]);
    }

// union for first two key search result value arrays
    else if (strstr(query_ori, " veya ") != NULL)
    {
// arr1
        r = search(words[0], h, size);
        int size1=0;j=0;
        if (r == NULL)
        {
            printf("\n%s is not found in hashtable\n", words[0]);
            size1 = 0;
        }
        else
            while (r[j++] != NULL)
                size1 ++;
        r1 = (char**) malloc(size1*sizeof(char*));
        printf("\nKey : %s\nValues : ", words[0]);
        for ( i = 0; i < size1; i++)
        {
            r1[i] = (char*) malloc(STRINGSIZE);
            strcpy(r1[i], r[i]+8);          // https:// i atmam gerekiyor cunku merge sort calismiyor niyese
            printf("https://%s ", r1[i]);
        }
        printf("\n");
        MergeSort(r1, 0, size1-1);
// arr2
        r = search(words[1], h, size);
        int size2=0;j=0;
        if (r == NULL)
        {
            printf("\n%s is not found in hashtable\n", words[1]);
            size2 = 0;
        }
        else
            while (r[j++] != NULL)
                size2 ++;
        r2 = (char**) malloc(size2*sizeof(char*));
        printf("\nKey : %s\nValues : ", words[1]);
        for ( i = 0; i < size2; i++)
        {
            r2[i] = (char*) malloc(STRINGSIZE);
            strcpy(r2[i], r[i]+8);
            printf("https://%s ", r2[i]);
        }
        printf("\n");
        MergeSort(r2, 0, size2-1);
// union
        int size_union = size1+size2;
        UNIONarray = (char**) malloc(size_union*sizeof(char*));
        for (i = 0; i < size_union; i++)
            UNIONarray[i] = (char*) malloc(STRINGSIZE);
        size_union = find_union(r1, r2, UNIONarray, size1, size2);
        printf("\nUNION array size : %d  Array : ", size_union);
        for (i = 0; i < size_union; i++)
            printf("https://%s ", UNIONarray[i]);
    }
    else
    {
        r = search(query, h, size);
        if (r == NULL)
            printf("\n%s is not found in hashtable\n", query);
        else
        {
            j = 0;
            printf("\nKey : %s\nValues : ", query);
            while (r[j] != NULL)
                printf("%s ", r[j++]);
            printf("\n");
        }
    }
}

int main()
{
    hashTable h;
    float load_factor;
    printf("\nLoad Factor : ");
    scanf("%f", &load_factor);
    int i, j, k, choice, size;
    size = read_file_and_create_hashTable(load_factor, &h);
    char** r;
    do
    {
        printf("\nEnter 1 for normal mode\n");
        printf("Enter 2 for detailed mode\n");
        printf("Enter 0 to exit\n");
        printf("Choice : ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            normal_mode(h, size);
            break;
        case 2:
            printf("\nDETAILED MODE\n------------\n");
            printf("\nHash Table length : %d\n", size);
            printf("\nHash Table\n------------");
            for (i = 0; i < size; i++)
            {
                printf("\n\nIndex :%3d     Attempt Count :%3d     Key :    ", i, h.data[i].attempt_count);
                if (h.data[i].key != NULL)
                    printf("%s", h.data[i].key);
                else
                {
                    printf("%s\nValues : NULL", "NULL");
                    continue;
                }
                j = 0;
                printf("\nValues : ");
                while (h.data[i].values[j] != NULL)
                    printf("%s ", h.data[i].values[j++]);
            }
            break;
        case 0:
            printf("\nQUITING\n");
            break;
        default:
            printf("\nWrong choice!\n");
            break;
        }
    } while (choice != 0);

    free(h.data);
    return 0;
}
