# include <stdio.h>
# include <stdlib.h>
#define EPSILON  0.001;

struct cord_node
{
    double value;
    struct cord_node *next;
};
struct vector_node
{
    struct vector_node *next;
    struct cord_node *cords;
};

struct dict_centroid
{
    struct dict_centroid *next;
    struct cord_node *centroid;
    struct cord_node *sum;
    int avg_divisor;

};



struct cord_node* ZERO_vector(int vector_len);

int main(int argc, char *argv[])
{
    FILE *fp;
    int K;
    int iter;
    char *filename;
    char ch;
    int N = 0;
    int vector_len=1;
    int flag=0;

    if ((argc > 4) || (argc <= 2)) {
        fprintf (stdout, "wrong number of arguments!\n");
        exit (1);
    }
        
    if (argc == 3) {
        K = atoi (argv[1]);
         if (K == 0) {
            fprintf (stdout, "Invalid number of clusters!\n");
            exit (1);
         }
        iter = 200;
        filename = argv[2];
    }
    
    else {
        K = atoi (argv[1]);
        if (K == 0) {
            fprintf (stdout, "Invalid number of clusters!\n");
            exit (1);
        }
        iter = atoi (argv[2]);
        if (iter==0) {
            fprintf (stdout, "Invalid number of iters!\n");
            exit (1);
         }
        filename = argv[3];
    }

    
    fp = fopen(filename, "r");

    int i=1;
    struct vector_node *head_vec, *curr_vec, *prev_vec;
    struct cord_node *head_cord, *curr_cord;
  
    struct cord_node *head_cord2, *curr_cord2;
    double n;
    char c;

    /*building the dict*/
    struct dict_centroid *head_dict_centroid, *curr_dict_centroid, *prev_dict_centroid;
    head_dict_centroid = malloc(sizeof(struct dict_centroid));
    curr_dict_centroid = head_dict_centroid;
    curr_dict_centroid->next = NULL;

    head_cord = malloc(sizeof(struct cord_node));
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector_node));
    curr_vec = head_vec;
    curr_vec->next = NULL;

    head_cord2 = malloc(sizeof(struct cord_node));
    curr_cord2 = head_cord2;
    curr_cord2->next = NULL;

    

    while (fscanf(fp,"%lf%c", &n, &c) == 2)
    {
        
        if (c == '\n')
        {
            N++;
           

            flag=1;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector_node));
            prev_vec = curr_vec;
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;

            head_cord = malloc(sizeof(struct cord_node));
            curr_cord = head_cord;
            curr_cord->next = NULL;

            if(i<=K){
                i++;
                curr_cord2->value = n;
                /*building the dict*/
                curr_dict_centroid->centroid =head_cord2;
                curr_dict_centroid->sum = ZERO_vector(vector_len); 
                curr_dict_centroid->avg_divisor =0;
                curr_dict_centroid->next = malloc(sizeof(struct dict_centroid));
                prev_dict_centroid = curr_dict_centroid;
                curr_dict_centroid = curr_dict_centroid->next;
      
                head_cord2 = malloc(sizeof(struct cord_node));
                curr_cord2 = head_cord2;
                curr_cord2->next = NULL;   
            }

            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord_node));
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;

        if(i<=K){
            curr_cord2->value = n;
            curr_cord2->next = malloc(sizeof(struct cord_node));
            curr_cord2 = curr_cord2->next;
            curr_cord2->next = NULL;
        }

        if (flag ==0){
            vector_len++;
        }

        
    }
    prev_vec->next = NULL;
    prev_dict_centroid->next = NULL;
    fclose(fp);
    
    printf("%d", N);
    
    if ((1>=K) || (K>=N)){
        fprintf (stdout, "Invalid number of clusters!\n");
        exit (1);
    }
    if ((1>=iter) || (K>=1000)){
        fprintf (stdout, "Invalid maximum iteration!\n");
        exit (1);
    }

     

    exit(0);

}

struct cord_node* ZERO_vector(int vector_len){

      struct cord_node *head_zcord_node, *curr_zcord_node, *prev_zcord_node;
      int l;

      head_zcord_node = malloc(sizeof(struct cord_node));
    //  assert(head_zcord_node!=NULL);
      curr_zcord_node = head_zcord_node;
      curr_zcord_node->next = NULL;


      for(l=0; l<vector_len; l++)
      {
         curr_zcord_node->value = 0;
         curr_zcord_node->next = malloc(sizeof(struct cord_node));
         prev_zcord_node = curr_zcord_node;
         curr_zcord_node = curr_zcord_node->next; 
     //  assert(curr_zcord_node!=NULL);
      }
      prev_zcord_node->next=NULL; 
      return head_zcord_node;
      
    }