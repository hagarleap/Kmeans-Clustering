# include <stdio.h>
# include <stdlib.h>
#define EPSILON  0.001;

struct cord
{
    double value;
    struct cord *next;
};
struct vector
{
    struct vector *next;
    struct cord *cords;
};

struct vector load_file(char* filename, int *N){
    FILE *fp;
    int count = 0;
    
    fp = fopen(filename, "r");

    struct vector *head_vec, *curr_vec, *next_vec, *prev_vec;
    struct cord *head_cord, *curr_cord, *next_cord;
    int i, j, rows = 0, cols;
    double n;
    char c;

    head_cord = malloc(sizeof(struct cord));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector));
    curr_vec = head_vec;
    curr_vec->next = NULL;


    while (fscanf(fp,"%lf%c", &n, &c) == 2)
    {
        count++;
        printf("%d", *N);
        printf("Entered n: %lf  c: %c \n", n, c);
        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
            prev_vec = curr_vec;
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;
            head_cord = malloc(sizeof(struct cord));
            curr_cord = head_cord;
            curr_cord->next = NULL;
            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
    }
    prev_vec->next = NULL;
    fclose(fp);
    *N = count;
    return *head_vec;
}



int main(int argc, char *argv[])
{
    int K;
    int iter;
    char *filename;
    char ch;
    struct vector *vectors;
    int *N;
    int i=0;
    N=&i;

    if ((argc > 4) || (argc <= 2)) {
        fprintf (stdout, "wrong number of arguments!\n");
        exit (1);
    }
        
    if (argc == 3) {
        K = atoi (argv[1]);
        iter = 200;
        filename = argv[2];
    }
    else{
        K = atoi (argv[1]);
        iter = atoi (argv[2]);
        filename = argv[3];
    }
    
    *vectors = load_file(filename, N);
    printf("%d", *N);

    return 1;

}