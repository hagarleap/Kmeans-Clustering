# include <stdio.h>
# include <stdlib.h>

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

int main(int argc, char *argv[])
{
    FILE *fp;
    int K;
    int iter;
    char *filename;
    char ch;

    if ((argc > 4) || (argc <= 1)) {
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

    
    fp = fopen(filename, "r");

    struct vector *head_vec, *curr_vec, *next_vec;
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
        printf("Entered n: %lf  c: %c \n", n, c);
        if (c == '\n')
        {
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector));
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

    fclose(fp);
    return 0;
}