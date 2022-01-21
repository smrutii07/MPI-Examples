#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>

void print_array(int *row,int num_elem){
    int i;
    for(i = 0;i<num_elem;i++){
        printf("%d ",row[i]);
    }
    printf("\n");
}

int main(int argc,char *argv[]){

    MPI_Init(&argc,&argv);
    int rank,comm_size;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_size);

    int *buffer, *my_buffer;
    int i,j;
    int index;
    
    int bheight,bwidth,nsteps,block_height;


    if (argc != 3) {
        fprintf(stderr, "Usage: %s rows cols\n", argv[0]);
        exit(1);
    }

    bheight = atoi(argv[1]);
    bwidth = atoi(argv[2]);
    nsteps = atoi(argv[3]);

    block_height = bheight/comm_size;

    int matrix[bheight][bwidth];
    int num_elem = block_height*bwidth;
    memset( matrix, 0, bheight*bwidth*sizeof(int) );

    buffer = (int *)malloc(sizeof(int)*bheight*bwidth);
    my_buffer = (int*)malloc(sizeof(int)*block_height*bwidth);

    srand(1);

    int k = 10;
    for (i = 1; i <= bheight; i++) {
        for (j = 1; j <= bwidth; j++) {
            matrix[i][j] = k;
            k++;
        }
    }

    for(i = 1;i<=bheight;i++){
        for(j=1;j<=bwidth;j++){
            buffer[index] = matrix[i][j];
            index++;
        }
    }

    MPI_Scatter( buffer, num_elem, MPI_INT, my_buffer, 
    num_elem, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received elements: ", rank);
    print_array(my_buffer,num_elem);

    MPI_Finalize();

return 0;
}
   
