# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>


int main(int argc, char *argv[])
{
    int    k, iter;

    if ((argc > 3) || (argc <= 1)) {
        fprintf (stdout, "wrong number of arguments!\n");
        exit (1);
    }
    
    if (argc =2) {
        k = atoi (argv[0]);
        iter = 200;
    }
    else{
        k = atoi (argv[0]);
        iter = atoi (argv[1]);
    }
    

   
}


  
    // f = open(input_data, 'r')
    // vectors = []
    // vector = f.readline().strip().split(",")
    // while vector[0] != "":
    //     vectors.append([float(x) for x in vector])
    //     vector = f.readline().strip().split(",")
    // f.close()
//loading the txt to an array of doubles

int find_array_dimensions(char *filename){
    int MAX_LINE_LENGTH=100000;
    int count_line=0;
    int count_N=0;
    int res[2];
    // Open file in read-only mode
    textfile = fopen(filename, "r");
    if(textfile == NULL){
        return 1;
    }

    //figuring N (len of the vector) 
    fgets(line, MAX_LINE_LENGTH, textfile);
    char *token;
    /* get the first token- first number */
    token = strtok(line, delimiter);
    /* walk through other tokens */
    while( token != NULL ) {
        count_N++;
    }

    count_line=1;
    //figuring and number of lines
    while(fgets(line, MAX_LINE_LENGTH, textfile)){
        if (strlen(line)>0){
            count_line++;
        }      
    }
    fclose(textfile);
    res[0]=count_line;
    res[1]=count_N;

    return res;
}
    


double load_text(char *filename,count_line,count_N){
    int MAX_LINE_LENGTH=100000;
    char delimiter= ",";
    double num; 
    int i=0, j=0; 
    // Open file in read-only mode
    textfile = fopen(filename, "r");
    double vectors[count_line][count_N];
    while(fgets(line, MAX_LINE_LENGTH, textfile)){
        char *token;
        /* get the first token- first number */
        token = strtok(line, delimiter);
        /* walk through other tokens */
        while( token != NULL ) {
            if (j<count_N-1){
                num = atof(token);
            }
            else{
                num_without/n = strtok(token, "\n");
                num = atof(num_without/n);
            }
            vectors[i][j]=num;
            token = strtok(NULL, delimiter);
            j++;
        }
        i++;
    }
    fclose(textfile);
    
    return vectors;
}
    