# include <stdio.h>
int main() {
   printf("Hello, World!\n");
   return 0;

   int arr[2] = {0};
   int *p = 0;
   char *c = 'a';
   arr[0]= (int *)p;
   arr[1] = (int *)c;
   printf("%d%c", *arr[0], *arr[1])
}
