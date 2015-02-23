#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NRA 4                
#define NCA 4                 
#define NCB 4                  
#define MASTER 0               


void main (int argc, char *argv[])
{
int	numtasks,              
	taskid,
	n=0,                
	numworkers,            
	source,               
	dest,                 
	rows,offset,                 i, j, k, rc;           
double	a[NRA][NCA],          
	b[NCA][NCB],
        d[NCA][NCB],
	e[NCA][NCB],          
	c[NRA][NCB];
          
MPI_Status status;
 // MPI RUN 
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
if (numtasks < 2 ) {
  printf("Need at least two MPI tasks. Quitting...\n");
  MPI_Abort(MPI_COMM_WORLD, rc);
  exit(1);
  }
numworkers = numtasks-1;

printf("task %d has started \n",taskid);

   if (taskid == MASTER)
   {
      printf("mpi_mm has started with %d tasks.\n",numtasks);
	

	printf("\t\nfor loop started ");
	
	offset=0;
	printf("Initializing arrays...\n");
      for (i=0; i<NRA; i++)
         {for (j=0; j<NCA; j++)
           {
              a[i][j]= i+j;
              printf("%2f ",a[i][j]);  
            }
          printf("\n");
         } 
      for (i=0; i<NCA; i++)
         {for (j=0; j<NCB; j++)
          {  b[i][j]= i+j;
            printf("%2f",b[i][j]);         
          }
          printf("\n");
          }
	
      for (dest=1; dest<=numworkers; dest++)
      {
	
	printf("\nsending array a %d times\n",n+1);
	
         MPI_Send(&a[offset][0],4,MPI_DOUBLE,dest,1,MPI_COMM_WORLD);
	
	printf("\nsending array b %d times\n",n+1);
         MPI_Send(&b,16,MPI_DOUBLE,dest,1,MPI_COMM_WORLD);
	
	n++;
         offset=offset+1;
      }
	
     
      
   }


else if(taskid>=MASTER)
     {
	
	printf("\t ....receiving data from array a....\n");
	MPI_Recv(&d,4,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
	for(i=0;i<4;i++)
	
	printf("\treceiveing data from array b ...\n");
       MPI_Recv(&e,16,MPI_DOUBLE,0,1,MPI_COMM_WORLD,&status);
	printf("\n\tDONE with master sending and receiving for task  %d\n",taskid);
	
	
       
       for (k=0; k<4; k++)
        { for (i=0; i<1; i++)
         {
            c[i][k] = 0.0;
            for (j=0; j<4; j++)
			{
			printf(" \td[%d][%d] is %f & e[%d][%d] is %f \n",i,j,d[i][j],j,k,e[j][k]);
	               c[i][k] = c[i][k] + d[i][j] * e[j][k];
			}
         }
	//printf("\nc[%d][%d] value is %f\n",i,k,c[i][k]);
         }
	
	
    }
	

MPI_Finalize();
printf("\n\tMULTIPLICATION DONE \n"); 

}
