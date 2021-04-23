#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h> // For exit() function
#define k  100


 double distance (int* a , int* b)
{
     double myDistance=sqrt(pow(a[0]-b[0],2.0)+pow(a[1]-b[1],2.0)+pow(a[2]-b[2],2.0));
    return myDistance;
}

int main() {
    char c[1000];
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    clock_t begin, end , start , final;
    double CPU_time;
    FILE *fptr;
    double threshold = .1;
    double dis;
    int alpha ;
    double dmin;
    if ((fptr = fopen("TestCases/test1.txt", "r")) == NULL) {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    int objects[50000][3];
    int membership[50000];
    
    int clusters[k][3];
    int new_clusters[k][3];
    int new_clusters_size[k];
    int s[3];
    int i , j , N ,n;
    fscanf(fptr, "%d", &N);
    alpha = N;

    #pragma omp for
    for (i = 0; i < N; i++)
    {for (j = 0 ; j < 3 ; j++)
    {
        fscanf(fptr, "%d", &s[j]);
        if (i < k)
        {
           clusters[i][j] = s[j];
           new_clusters[i][j] = 0;
           new_clusters_size[i] = 0;
        }
        membership[i] =1 ;
      objects[i][j] = s[j];

    }
    }
   
    fclose(fptr);

begin = clock();
   start = omp_get_wtime();
while ((alpha/N) > threshold)
{
   // printf("hi");
    alpha = 0;
    #pragma omp for private (dmin , n  ,dis)
    for (int i = 0; i < N; i++)
    {dmin = INFINITY;
        for (int j = 0; j < k; j++)
    {
         dis = distance(objects[i] , clusters[j]);
        if (dis < dmin)
        {
            dmin = dis;
            n = j;
           
        }
      
    }
     
      if (membership[i] != n )
        {
            #pragma omp critical
            alpha +=1;
            membership[i] = n ;
        }
        #pragma omp critical
        new_clusters[n][0] = new_clusters[n][0] +  objects[i][0]  ;
        #pragma omp critical
        new_clusters[n][1] = new_clusters[n][1] +  objects[i][1]  ;
        #pragma omp critical
        new_clusters[n][2] = new_clusters[n][2] +  objects[i][2]  ;
        #pragma omp critical
        new_clusters_size[n] = new_clusters_size[n] + 1;
       // printf("%d\n" , new_clusters[n][0]);
        
    }
    #pragma omp for 
    for (int z = 0; z < k; z++)
    {
        for (int y = 0; y < 3; y++)
        {
          #pragma omp critical
          clusters[z][y] = new_clusters[z][y] / new_clusters_size[z];
          new_clusters[z][y] = 0;
          printf("%d\t" , clusters[z][y]);
        }
        printf("\n");
        new_clusters_size[z] = 0 ;
        
    }

    
}
end = clock();
final = omp_get_wtime();
CPU_time = (double)(end - begin) / CLOCKS_PER_SEC;// get processor time
printf("Vector summation took %f seconds to execute\n", CPU_time);


printf("Work took %f seconds\n", final - start);

    return 0;
}


// "~/Documents/parallel/Assignment2/TestCases/test3.txt"


