#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Node
{
    char name[200];
    int num;
    int indegrees;
    bool M_filtered;
    bool X_filtered;
    bool Y_filtered;
    int *connected;
    int num_connected;
} Node;

// stack utils
struct stack
{
    Node* arr;
    int size;
    int top;
};

struct stack* create_stack(int size)
{
    struct stack* s = (struct stack*) malloc(sizeof(struct stack));
    s->arr = (Node*) malloc(size*sizeof(Node));
    s->size = size;
    s->top = -1;
    return s;
}

Node peek(struct stack* s)
{
    return s->arr[s->top];
}

int is_full(struct stack* s)
{
    return s->size == s->top+1 ? 1 : 0;
}

int is_empty(struct stack* s)
{
    return -1 == s->top ? 1 : 0;
}

void pop(struct stack* s)
{
    if (is_empty(s))
        return;
        //printf("Stack is empty can't pop!\n");
    else
    {
        //printf("%d is popped\n", peek(s).num);
        s->top --;
    }
}

void push(struct stack* s, Node data)
{
    if (is_full(s))
        return;
        //printf("Stack is full can't push!\n");
    else
    {
        s->top ++;
        s->arr[s->top] = data;
        //printf("%d is pushed\n", peek(s).num);
    }
}

//general utils
void print_graph(char* info, Node* nodes, int node_count, bool print_indegrees, bool X_filter, bool Y_filter, int mode)
{
    if (!mode)
        return;
    int i, j;
    printf("\n%s\n", info);
    for ( i = 0; i < node_count; i++)
    {
        if (!nodes[i].M_filtered && !(X_filter && nodes[i].X_filtered) && !(Y_filter && nodes[i].Y_filtered))
        {
            if (print_indegrees)
                printf("Indegrees : %d\n", nodes[i].indegrees);
            printf("Node Num : %d\nName : %s\nNumber of connections : %d\nConnected to : ", 
                nodes[i].num, nodes[i].name, nodes[i].num_connected);
            for (j = 0; j < nodes[i].num_connected; j++)
                printf("%d ", nodes[i].connected[j]);
            printf("\n\n");
        }
    }
}

int remove_if_exits(Node node, int key)
{
    int i=0, index=-1;
    while (i < node.num_connected && index == -1)
    {
        if (node.connected[i++] == key)
            index = i-1;
    }

    if (index != -1) {
        if (index == node.num_connected-1)
            node.connected[index] = 0;
        else
        {
            for (i = index; i < node.num_connected - 1; i++)
                node.connected[i] = node.connected[i+1];
        }
        return 1;
    }
    return 0;
}

// solution utils
void set_indegrees(Node* nodes, int node_count)
{
    int i, j, k;
    for ( i = 0; i < node_count; i++)
        nodes[i].indegrees = 0;
    
    for ( i = 0; i < node_count; i++)
    {
        if (!nodes[i].M_filtered)
        {
            // Count the number of incoming connections for the current node
            for ( j = 0; j < node_count; j++)
                for ( k = 0; k < nodes[j].num_connected; k++)
                    if (nodes[j].connected[k] == nodes[i].num)
                        nodes[i].indegrees ++;
        }
    }
}

void M_function(Node *nodes, int node_count, int M)
{
// Function to filter nodes based on their in-degree
    // Flag to track if any nodes have been removed
    int removed = 1, i, j, k;

    // Loop until there are no more nodes with an in-degree less than M
    // If no nodes were removed, the loop can be stopped
    while (removed) {
        i=0;
        removed = 0;
        // Iterate through the array of nodes
        for ( i = 0; i < node_count; i++)
        {
            // Check if the in-degree of the current node is less than M
            if (nodes[i].indegrees < M && !nodes[i].M_filtered)
            {
                // increase indegrees from deleted node's connected array nodes
                for ( j = 0; j < nodes[i].num_connected; j++)
                    nodes[nodes[i].connected[j]-1].indegrees --;
                
                // remove node from connected arrays
                for ( k = 0; k < node_count; k++)
                    nodes[k].num_connected -= remove_if_exits(nodes[k], i+1);

                nodes[i].M_filtered = true;
                removed = 1;
            }
        }
    }
}

void X_function(Node *nodes, int node_count, int X)
{
    int i;
    for ( i = 0; i < node_count; i++)
        if (nodes[i].indegrees < X)
            nodes[i].X_filtered = true;
}

void Y_function(Node* nodes, int node_count, int Y)
{
    int i, j, *histogram = (int*) calloc(node_count,sizeof(int));
    for ( j = 0; j < node_count; j++)
    {
        if (!nodes[j].M_filtered)
        {
            Node node=nodes[j];
            bool* visited = (bool*) malloc(node_count*sizeof(bool));
            for ( i = 0; i < node_count; i++)  visited[i]=false;
            struct stack* s = create_stack(node_count);
            // Push the current source node
            push(s, node);
            while (!is_empty(s))
            {
                // Pop a vertex from stack and print it
                node = peek(s);
                pop(s);
                // Stack may contain same vertex twice. So
                // we need to print the popped item only
                // if it is not visited.
                if(visited[node.num-1] == false)
                {
                    // printf("%d -> ", node.num);
                    histogram[node.num-1] ++;
                    visited[node.num-1] = true;
                }
                // Get all adjacent vertices of the popped vertex s
                // If a adjacent has not been visited, then push it
                // to the stack.
                for ( i = 0; i < node.num_connected; i++)
                    if (!visited[node.connected[i]-1])
                        push(s, nodes[node.connected[i]-1]);
            }
        }
    }

    for ( i = 0; i < node_count; i++)
        if (histogram[i]-1 < Y && !nodes[i].M_filtered)
            nodes[i].Y_filtered = true;
}

// file utils
int find_node_number(char* file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Initialize the line counter
    int count = 0;

    // Read the lines of the file one by one
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Increment the line counter
        count++;
    }

    // Close the file
    fclose(fp);

    return count/2;
}

int compare(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

int* convertion(char* str)
{
//convert_a_string_numbers_seperated_with_commas_to_a_sorted_int_array
    // Get the length of the string
    int len = strlen(str);

    // Create an array to store the integers
    int* nums = (int*) malloc(((len+1)/2)*sizeof(int));

    // Initialize a counter for the array
    int count = 1;
    nums[0]=-999;

    // Split the string into tokens using the comma as the delimiter
    char *token = strtok(str, ",");
    while (token != NULL)
    {
        // Convert the token to an integer and store it in the array
        nums[count] = atoi(token);
        count++;

        // Get the next token
        token = strtok(NULL, ",");
    }
    // Sort the array using the qsort function
    qsort(nums, count, sizeof(int), compare);
    nums[0]=count-1;

    return nums;
}

void replace_comma_to_space(char* str)
{
    // Get the length of the array
    int len = strlen(str), i;

    // Iterate through the array
    for (i = 0; i < len; i++) {
        // Check if the current character is a comma
        if (str[i] == ',') {
            // Replace the comma with a space
            str[i] = ' ';
        }
    }
}

Node* read_file(char* file_name, int mode, int* node_count)
{
    int node_num, c=0, i;
    *node_count = find_node_number(file_name);
    char name[200];
    char line[200];

    // Create an array to store all of the nodes
    Node* nodes;
    nodes = (Node*) malloc((*node_count)*sizeof(Node));

    FILE* fp;
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        printf("\nError! opening file\n");
        exit(1);
    }

    while (EOF != fscanf(fp, "%d,%s\n", &node_num, name))
    {
        replace_comma_to_space(name);
        fscanf(fp, "%s\n", line);
        int* nums = convertion(line);
        
        // Create the nodes
        nodes[c].M_filtered = false;
        nodes[c].M_filtered = false;
        nodes[c].X_filtered = false;
        nodes[c].Y_filtered = false;
        nodes[c].num = node_num;
        nodes[c].indegrees = -1;
        strcpy(nodes[c].name, name);
        nodes[c].connected = (int*) malloc(nums[0] * sizeof(int));
        for (i = 1; i < nums[0]+1; i++)
            nodes[c].connected[i-1] = nums[i];
        nodes[c].num_connected = nums[0];

        c ++;
    }

    fclose(fp);
    set_indegrees(nodes, *node_count);
    return nodes;
}

// driver code
int main()
{
// defining variables, taking inputs
    int mode, M, X, Y, node_count;
    char file_name[30] = "socialNET.txt", info[100];
    printf("Enter mode(0->normal / 1->detailed), M, X, Y space seperated (1 2 2 2) : ");
    scanf("%d %d %d %d", &mode, &M, &X, &Y);
    Node* graph = read_file(file_name, mode, &node_count);    // creates graph from txt file

// printing initial graph
    sprintf(info, "\n\n*********************\n\nINITIAL GRAPH\n\n");
    print_graph(info, graph, node_count, mode, false, false, mode);

// applying M filter and printing graph
    M_function(graph, node_count, M);
    sprintf(info, "\n\n*********************\n\nGRAPH AFTER M:%d FILTER\n\n", M);
    print_graph(info, graph, node_count, mode, false, false, mode);

// applying X filter and printing nodes
    X_function(graph, node_count, X);
    sprintf(info, "\n\n*********************\n\nNODES AFTER X:%d FILTER ON M:%d FILTERED GRAPH\n\n", X, M);
    print_graph(info, graph, node_count, mode, true, false, mode);

// applying Y filter and printing nodes
    Y_function(graph, node_count, Y);
    sprintf(info, "\n\n*********************\n\nNODES AFTER Y:%d FILTER ON M:%d FILTERED GRAPH\n\n", Y, M);
    print_graph(info, graph, node_count, mode, false, true, mode);

// printing influcer nodes
    sprintf(info, "\n\n*********************\n\nINFLUCERS ACCORDING TO M:%d  X:%d  Y:%d\n\n", M, X, Y);
    print_graph(info, graph, node_count, mode, true, true, 1);

    return 0;
}
