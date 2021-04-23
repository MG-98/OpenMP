#include <stdio.h>
#include <stdlib.h>
 

void merging(int arr[], int left_i, int mid, int right_i)
{
    int n1 = mid - left_i + 1;
    int n2 = right_i - mid;
 
  
    int L[n1], R[n2];
 
  #pragma omp for 
    for (int i = 0; i < n1; i++)
        L[i] = arr[left_i + i];
    #pragma omp for 
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
 
   
    int  x = 0; 
    int y = 0; 
    int k = left_i;
    #pragma omp task
    {
     while (x < n1 && y < n2) {
        if (L[x] <= R[y]) {
            #pragma omp taskwait
            arr[k] = L[x];
            x++;
        }
        else {
            #pragma omp taskwait
            arr[k] = R[y];
            y++;
        }
        k++;
    }
    
  
    while (x < n1) {
        #pragma omp taskwait
        arr[k] = L[x];
        x++;
        k++;
    }
    
 
    while (y < n2) {
        #pragma omp taskwait
        arr[k] = R[y];
        y++;
        k++;
    }
    }
}
 

void MergeSort(int arr[], int left_i, int right_i)
{
    if (left_i < right_i) {
       
        int mid = left_i + (right_i - left_i) / 2;

        //printf("%d\t%d\t%d\n" , left_i , mid , right_i);
         
        MergeSort(arr, left_i , mid);
        MergeSort(arr, mid+1 , right_i);

        merging(arr, left_i , mid , right_i);
       }
}
 


int main()
{
    int sz = 10000;
    int arr[sz];
    for(int i=0;i<sz;i++)
      arr[i]=rand()%100000;
     //int arr[] = { 12, 11, 13, 5, 6, 7 };
    int arr_size = sizeof(arr) / sizeof(arr[0]);
 
    printf("Given array is \n");
    
clock_t start = omp_get_wtime();
    MergeSort(arr, 0, arr_size - 1);
clock_t end = omp_get_wtime(); 


    printf("\nSorted array is \n");
    
    for (int i = 0; i < arr_size; i++)
        printf("%d ", arr[i]);
    
    printf("Work took %f seconds\n", end - start);

    return 0;
}
