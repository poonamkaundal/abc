#include<mpi.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

void main(int argc,char *argv[])
{
	key_t key;
        int size,i;
        int shmid;
        int shmflg;
        char *s,*shm,*dest; 
	char c[4],a='a',g='g';
        key =5678;
       size=27;

        int numtasks,rank;
        MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	
	printf("process %d has started\n",rank);
           
       if(rank==0)
          {
            
             shmid = shmget(key,size,IPC_CREAT | 0666 );
              if(shmid<0)
                {
                   perror("shmget");
                   exit(1);
                 }
            
	      if((shm = shmat(shmid,NULL,0))==-1)
                {
                   perror("shmat");
                   exit(1);
                 }
        

          for(i=0;i<4;i++)
            {
                c[i]= a++;
            }
               
             c[i]=NULL;
             printf("data with rank 0 is %s\n",c);
             
             dest=shm;
             memcpy(dest,c,strlen(c)+1);
	     printf("shared memory value %s \n",shm);	   
		}
         
     
         MPI_Barrier(MPI_COMM_WORLD); 
         
        
        if(rank!=0)
           { 
                 
                 shmid=shmget(key,size,0666);
                 if(shmid<0)
                    {
                         perror("shmget");
                         exit(1);
                     }
                 shm=shmat(shmid,NULL,0);
               
                 if(shm==-1) 
                     {
                         perror("shmat");
                         exit(1);
                     }

           for(i=0;i<4;i++)
                  c[i]=g++;
                  
              
                  c[i]=NULL;
             
           printf("data with rank %d is %s \n",rank,c);
		
		dest=shm;           

		 memcpy(dest+rank*4,c,strlen(c)+1);  
                 printf("Shared memory value is %s \n",shm);   
           }    
       
  MPI_Finalize();

}
