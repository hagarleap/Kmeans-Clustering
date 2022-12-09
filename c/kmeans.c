# include <stdio.h>
# include <stdlib.h>
# include <math.h>
#define EPSILON  0.001

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


/*receives sum field (cord_node type) from dict_centroid, and some other cord_node type.
Adds each cord_node value to the sum field's cord node value iteratively.
in the end, dict_centroid.sum == dict_centroid.sum(old) + cord_node */

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


/*def euclidian_distance(vec1, vec2):
    sum = 0
    for i in range(len(vec1)):
        sum += (vec1[i]-vec2[i])**2
    return sum**(1/2) */
//recives 2 vector and return the euclidian distance between them
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

//making the deltas list as linked list on length
struct cord_node* init_deltas(int K){
      struct cord_node *head_deltas_node, *curr_deltas_node, *prev_deltas_node;
      int i;
      head_deltas_node = malloc(sizeof(struct cord_node));
      curr_deltas_node = head_deltas_node;
      curr_deltas_node->next = NULL;
      for(i=0; i<K; i++)
      {
         curr_deltas_node->value = 1;
         curr_deltas_node->next = malloc(sizeof(struct cord_node));
         prev_deltas_node = curr_deltas_node;
         curr_deltas_node = curr_deltas_node->next; 
      }
      prev_deltas_node->next=NULL; 
      return head_deltas_node;
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
/*
// def update_centroid(deltas, clusters, clusters_keys):
//     i=0
//     for old_centroid in clusters_keys:
//         new_centroid = []
//         avg_divisor = clusters[old_centroid][1]
//         for bit in clusters[old_centroid][0]:
//             new_centroid.append(bit/avg_divisor)

//         deltas[i] = euclidian_distance(old_centroid, new_centroid)
//         clusters.pop(old_centroid)
//         clusters[tuple(new_centroid)] = [[0 for i in range(len(new_centroid))],0]
*/

int update_centroid(struct dict_centroid *head_dict_centroid, struct cord_node *deltas,  int vector_len){
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
            //as long as ONE delta is bigger than epsilon, we want the while loop to keep going
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
   
    
    if ((1>=K) || (K>=N)){
        fprintf (stdout, "Invalid number of clusters!\n");
        exit (1);
    }
    if ((1>=iter) || (K>=1000)){
        fprintf (stdout, "Invalid maximum iteration!\n");
        exit (1);
    }

    //new head- for the lists of vector
    struct vector_node *vectors_list = head_vec;
    //new head- for the list of centroid dict
    struct dict_centroid *centroid_list_dict = head_dict_centroid;
    int iter_count = 0;
    struct cord_node* deltas = init_deltas(K);
    int max_delta_bigger_than_epsilon=1;
    double argmin;
    double dist;
    struct dict_centroid *closest_cluster;
/*while iter_count<iter and max(deltas)>epsilon:
        clusters_keys = list(clusters.keys())  - the list of centroids
        for x in vectors:  - moving on all the vectors list
            argmin = euclidian_distance(x, clusters_keys[0])
            closest_cluster = clusters_keys[0]
            for i in range(1,k):
                dist = euclidian_distance(x, clusters_keys[i])
                if dist<argmin:
                    argmin = dist
                    closest_cluster = clusters_keys[i]
            clusters[closest_cluster][1]+=1
            clusters[closest_cluster][0] = [clusters[closest_cluster][0][i]+x[i] for i in range(len(x))]
        
        update_centroid(deltas, clusters, clusters_keys)  
        iter_count+=1*/
     while ((iter_count<iter) && (max_delta_bigger_than_epsilon==1)){
        
        while(vectors_list != NULL){
            argmin = euclidian_distance(vectors_list->cords, centroid_list_dict->centroid, vector_len);
            closest_cluster = head_dict_centroid;
            centroid_list_dict= centroid_list_dict->next;
            while(centroid_list_dict != NULL){
                dist= euclidian_distance(vectors_list->cords, centroid_list_dict->centroid, vector_len);
                if (dist<argmin) {
                    argmin = dist;
                    //need to verify it does not move
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
        struct dict_centroid *centroid_head_for_UC =head_dict_centroid;
        struct cord_node *delta_head_for_UC = deltas;
        max_delta_bigger_than_epsilon = update_centroid(centroid_head_for_UC, delta_head_for_UC, vector_len);
        vectors_list= head_vec;
     } 


    exit(0);

}


