# include <stdio.h>
# include <stdlib.h>
# include <math.h>
#define EPSILON  0.001

/*Data structures*/
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
struct dict_node
{
    struct dict_node *next;
    struct cord_node *centroid;
    struct cord_node *sum;
    int avg_divisor;
};

/*Receives sum field (cord_node type) from dict_centroid, and some other cord_node type.
Adds them and saves the result in the input sum field. */
void vector_addition(struct cord_node* closest_cluster_sum_vec,struct cord_node* vector,int vector_len){
    double cord_value_vector;
    int j;
    for(j=0; j<vector_len; j++)
      {
        cord_value_vector = vector->value;
        closest_cluster_sum_vec->value = closest_cluster_sum_vec->value + cord_value_vector;
        vector = vector->next; 
        closest_cluster_sum_vec = closest_cluster_sum_vec->next; 
      }

}

/*Recives 2 vectors. Returns the euclidian distance between them*/
double euclidian_distance(struct cord_node* vec1, struct cord_node* vec2, int vector_len){
    double sum=0;
    int j;
    double cord_value_vec1;
    double cord_value_vec2;
    for(j=0; j<vector_len; j++)
      {
        cord_value_vec1 = vec1->value;
        cord_value_vec2 = vec2->value;
        vec1 = vec1->next; 
        vec2 = vec2->next; 
        sum += pow((cord_value_vec1-cord_value_vec2), 2);
      }
    sum = pow(sum,0.5);
    return sum;
}

/*Creates the deltas linked list, makes it as long as amount of clusters there are.*/
struct cord_node* init_deltas(int K){
      struct cord_node *head_deltas_node, *curr_deltas_node, *prev_deltas_node;
      int i;
      head_deltas_node = malloc(sizeof(struct cord_node));
      if(head_deltas_node==NULL){
        printf("An Error Has Occurred");
        exit(1);
      }

      curr_deltas_node = head_deltas_node;
      curr_deltas_node->next = NULL;
      for(i=0; i<K; i++)
      {
         curr_deltas_node->value = 1;
         curr_deltas_node->next = malloc(sizeof(struct cord_node));
         if((curr_deltas_node->next)==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
         prev_deltas_node = curr_deltas_node;
         curr_deltas_node = curr_deltas_node->next; 
      }
      free(curr_deltas_node);
      prev_deltas_node->next=NULL; 
      return head_deltas_node;
}

/*Initializes the vector for the sum field. Filled with zeros, vector is as long as the input vectors.*/
struct cord_node* ZERO_vector(int vector_len){
      struct cord_node *head_zcord_node, *curr_zcord_node, *prev_zcord_node;
      int l;

      head_zcord_node = malloc(sizeof(struct cord_node));
      if(head_zcord_node==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
      curr_zcord_node = head_zcord_node;
      curr_zcord_node->next = NULL;


      for(l=0; l<vector_len; l++)
      {
         curr_zcord_node->value = 0;
         curr_zcord_node->next = malloc(sizeof(struct cord_node));
         if((curr_zcord_node->next)==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
         prev_zcord_node = curr_zcord_node;
         curr_zcord_node = curr_zcord_node->next; 
      }
      free(curr_zcord_node);
      prev_zcord_node->next=NULL; 
      return head_zcord_node;
      
}

/*Calculates new centroid using the avg_divisor and sum vector, saves it in the sum vector. Then calculates the 
delta difference between the new and old centroid. Finally it replaces the centroid field with the sum field,
and replaces the sum field with zeros. It also resets the avg_divisor field, and returns an integer 1 or 0.
0 means that all of the values in delta are lesser than epsilon. Otherwise, at least one is larger.*/
int update_centroid(struct dict_node *head_dict_centroid, struct cord_node *deltas,  int vector_len){
    int i =0;
    int max_delta_bigger_than_epsilon=0;
    struct cord_node *curr_sum_node;
    struct cord_node *curr_centroid_node;
    
    while(head_dict_centroid!=NULL){

        curr_sum_node = head_dict_centroid->sum;

        for(i=0;i<vector_len;i++){
            curr_sum_node->value = curr_sum_node->value /head_dict_centroid->avg_divisor;
            curr_sum_node = curr_sum_node->next;
        }
        head_dict_centroid->avg_divisor=0;
        deltas->value = euclidian_distance(head_dict_centroid->centroid, head_dict_centroid->sum ,vector_len);
        if((deltas->value) > EPSILON){
            /*as long as ONE delta is bigger than epsilon, we want the while loop to keep going */
            max_delta_bigger_than_epsilon=1;
        }

        curr_centroid_node = head_dict_centroid->centroid;
        curr_sum_node = head_dict_centroid->sum;
        for (i=0; i<vector_len; i++){
            curr_centroid_node->value=curr_sum_node->value;
            curr_sum_node->value=0;
            curr_centroid_node= curr_centroid_node->next;
            curr_sum_node= curr_sum_node->next;
        }
        head_dict_centroid = head_dict_centroid->next;
    }
    return max_delta_bigger_than_epsilon;
}

/*Frees memory for an input cord node*/
void delete_cord_node(struct cord_node* cord_node){
    if (cord_node!=NULL){
        struct cord_node* next_cord;
        next_cord = NULL;
        if (cord_node != NULL)
        {
            next_cord= cord_node->next;
        }
        while(next_cord != NULL) {
            free(cord_node);
            cord_node = next_cord;
            next_cord = cord_node->next;
        }
        if (cord_node != NULL) { 
            free(cord_node);
        }  
    } 
}



/*Frees memory for an input vector node*/
void delete_vector_list( struct vector_node *vectors_list)
{
    if(vectors_list!=NULL){
        struct vector_node *curr_vector, *next_vector;
        curr_vector = vectors_list;
        next_vector = curr_vector->next;

        while (next_vector != NULL )
        {
            if (curr_vector->cords!=NULL){
                delete_cord_node(curr_vector->cords);
            }
            free(curr_vector);
            curr_vector = next_vector;
            next_vector = curr_vector->next;
        
        }
        if (curr_vector->cords!=NULL){
        delete_cord_node(curr_vector->cords);
        }
        free(curr_vector);
       }
}

/*Frees memory for an input dict node node*/
void delete_dict_list(struct dict_node *head_dict_centroid){
    if(head_dict_centroid!=NULL){
        struct dict_node *curr_dict, *next_dict;
        curr_dict = head_dict_centroid;
        next_dict = curr_dict->next;

        while (next_dict != NULL )
        {   
            if (curr_dict->centroid!=NULL){
            delete_cord_node(curr_dict->centroid);
            }
            if (curr_dict->sum!=NULL){
            delete_cord_node(curr_dict->sum);
            }
            free(curr_dict);
            curr_dict = next_dict;
            next_dict = curr_dict->next;
        
        }
        if (curr_dict->centroid!=NULL){
        delete_cord_node(curr_dict->centroid);
        }
        if (curr_dict->sum!=NULL){
        delete_cord_node(curr_dict->sum);
        }
        free(curr_dict);
    }
}

int main(int argc, char *argv[])
{
    int K, iter, b, N = 0, vector_len=1, flag=0, i=1, max_delta_bigger_than_epsilon=1, iter_count = 0;
    double n, argmin, dist;
    char c;

    struct vector_node *vectors_list, *head_vec, *curr_vec, *prev_vec;
    struct cord_node *result_cord, *head_cord, *curr_cord, *head_cord2, *curr_cord2, *deltas, *delta_head_for_UC;
    struct dict_node *centroid_list_dict, *centroid_head_for_UC, *result, *head_dict_centroid;
    struct dict_node *curr_dict_centroid, *prev_dict_centroid, *closest_cluster;

    /*Initial checks that inputs are valid, and converting inputs into correct data types*/
    if ((argc > 3) || (argc < 2)) {
        printf ("wrong number of arguments!");
        exit (1);
    }
        
    if (argc == 2) {
        K = atoi (argv[1]);
         if (K<=1) {
            printf ("Invalid number of clusters!");
            exit (1);
         }
        iter = 200;
    }    
    else {
        K = atoi (argv[1]);
        if (K<=1) {
            printf("Invalid number of clusters!");
            exit (1);
        }
        iter = atoi (argv[2]);
        if ((iter==0) ||(iter>1000)) {
            printf ("Invalid number of iters!");
            exit (1);
        }
    }

    /*Simultaneously building from the text file the list of input vectors and the Dictionary
     that holds the centroids */
    head_dict_centroid = malloc(sizeof(struct dict_node));
    if(head_dict_centroid==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
    curr_dict_centroid = head_dict_centroid;
    curr_dict_centroid->next = NULL;

    head_cord = malloc(sizeof(struct cord_node));
    if(head_cord==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
    curr_cord = head_cord;
    curr_cord->next = NULL;

    head_vec = malloc(sizeof(struct vector_node));
    if(head_vec==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
    curr_vec = head_vec;
    curr_vec->next = NULL;

    head_cord2 = malloc(sizeof(struct cord_node));
    if(head_cord2==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
    curr_cord2 = head_cord2;
    curr_cord2->next = NULL;

    while (scanf("%lf%c", &n, &c) == 2)
    {        
        if (c == '\n')
        {
            N++;
            flag=1;
            curr_cord->value = n;
            curr_vec->cords = head_cord;
            curr_vec->next = malloc(sizeof(struct vector_node));
            if((curr_vec->next)==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
            prev_vec = curr_vec;
            curr_vec = curr_vec->next;
            curr_vec->next = NULL;

            head_cord = malloc(sizeof(struct cord_node));
            if(head_cord==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
            curr_cord = head_cord;
            curr_cord->next = NULL;

            /*building the dict*/
            if(i<=K){
                i++;
                curr_cord2->value = n;
                curr_dict_centroid->centroid =head_cord2;
                curr_dict_centroid->sum = ZERO_vector(vector_len); 
                curr_dict_centroid->avg_divisor =0;
                curr_dict_centroid->next = malloc(sizeof(struct dict_node));
                if((curr_dict_centroid->next)==NULL){
                     printf("An Error Has Occurred");
                     exit(1);
                }
                prev_dict_centroid = curr_dict_centroid;
                curr_dict_centroid = curr_dict_centroid->next;
      
                head_cord2 = malloc(sizeof(struct cord_node));
                if(head_cord2==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
                curr_cord2 = head_cord2;
                curr_cord2->next = NULL;   
            }

            continue;
        }

        curr_cord->value = n;
        curr_cord->next = malloc(sizeof(struct cord_node));
        if((curr_cord->next)==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
        curr_cord = curr_cord->next;
        curr_cord->next = NULL;
        curr_cord->value = 0.0;

        if(i<=K){
            curr_cord2->value = n;
            curr_cord2->next = malloc(sizeof(struct cord_node));
            if((curr_cord2->next)==NULL){
            printf("An Error Has Occurred");
            exit(1);
         }
            curr_cord2 = curr_cord2->next;
            curr_cord2->next = NULL;
            curr_cord2->value = 0.0;
        }

        if (flag ==0){
            vector_len++;
        }

    }
    free(curr_cord);
    free(curr_cord2);
    delete_vector_list(curr_vec);
    delete_dict_list(curr_dict_centroid);  

    
    prev_vec->next = NULL;
    prev_dict_centroid->next = NULL;   
    
    if (K>=N){
        printf ("Invalid number of clusters!");
        if(head_vec!=NULL){
            delete_vector_list(head_vec);
        }
        if(head_dict_centroid!=NULL){
            delete_dict_list(head_dict_centroid);
        }
        exit (1);
    }

    /*new head- for the lists of vector */
    vectors_list = head_vec;
    /*new head- for the list of centroid dict */
    centroid_list_dict = head_dict_centroid;
    deltas = init_deltas(K);

    /*Comparing and updating the centroids dictionary until we reach the maximum amount of iterations, or
    all deltas are less than epsilon.*/
     while ((iter_count<iter) && (max_delta_bigger_than_epsilon==1)){
        
        while(vectors_list != NULL){
            argmin = euclidian_distance(vectors_list->cords, centroid_list_dict->centroid, vector_len);
            closest_cluster = head_dict_centroid;
            centroid_list_dict= centroid_list_dict->next;
            while(centroid_list_dict != NULL){
                dist= euclidian_distance(vectors_list->cords, centroid_list_dict->centroid, vector_len);
                if (dist<argmin) {
                    argmin = dist;
                    /*need to verify it does not move */
                    closest_cluster = centroid_list_dict;
                }
                centroid_list_dict= centroid_list_dict->next;
            }
            closest_cluster->avg_divisor=  closest_cluster->avg_divisor + 1;
            vector_addition(closest_cluster->sum,vectors_list->cords, vector_len);
            vectors_list= vectors_list->next;
            centroid_list_dict = head_dict_centroid;
        }
        iter_count+=1;
        centroid_head_for_UC =head_dict_centroid;
        delta_head_for_UC = deltas;
        max_delta_bigger_than_epsilon = update_centroid(centroid_head_for_UC, delta_head_for_UC, vector_len);
        vectors_list= head_vec;
     } 

    /*Get the copy of the head of the dict for final print */
    result = head_dict_centroid; 
    while(result!=NULL){

        result_cord = result->centroid;

        for(b=0; b<vector_len-1; b++){
            /*Print all but last vector cord with comma at end */
            printf("%.4f,", result_cord->value); 
            result_cord = result_cord->next;
        }

        /*Last vector cord printed without comma, with "enter" */
        printf("%.4f\n", result_cord->value); 
        result = result->next;  
    }

    /*Free the following: head_vec, deltas, head_dict_centroid*/
    if(head_vec!=NULL){
        delete_vector_list(head_vec);
    }
    if(head_dict_centroid!=NULL){
        delete_dict_list(head_dict_centroid);
    }
    delete_cord_node(deltas);

    exit(0);
}
