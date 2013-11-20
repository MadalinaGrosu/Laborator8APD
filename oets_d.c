 #include "mpi.h"
 #include <stdio.h>

   int main(argc,argv)
   int argc;
   char *argv[]; {
   int  numtasks, rank, len, rc, tag = 1; 
   char hostname[MPI_MAX_PROCESSOR_NAME];
   int x[9] = {8,1,3,5,9,4,7,2,6};
   int my_elem, elem;
   MPI_Status Stat;

   rc = MPI_Init(&argc,&argv);
   if (rc != MPI_SUCCESS) {
     printf ("Error starting MPI program. Terminating.\n");
     MPI_Abort(MPI_COMM_WORLD, rc);
     }

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(hostname, &len);
	
	MPI_Scatter(x,1,MPI_INT,&my_elem,1,MPI_INT,0,MPI_COMM_WORLD);
//	printf ("Number of tasks= %d My rank= %d My elem = %d\n", numtasks,rank,my_elem);
	int i;
	for (i = 0; i < 9; i++) 
	{
		if (i % 2 == rank % 2) {
			if (rank + 1 < numtasks)
			{
				MPI_Send(&my_elem, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
				MPI_Recv(&elem, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD, &Stat);
				if (my_elem > elem)
					my_elem = elem;
			}
		} else {
			if (rank > 0) {
				MPI_Recv(&elem, 1, MPI_INT, rank - 1, tag, MPI_COMM_WORLD, &Stat);
				MPI_Send(&my_elem, 1, MPI_INT, rank - 1, tag, MPI_COMM_WORLD);
				
				if (my_elem < elem)
					my_elem = elem;
			}
		}
	//	printf ("Number of tasks= %d My rank= %d My elem = %d\n", numtasks,rank,my_elem);
		MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Gather(&my_elem,1,MPI_INT,x,1,MPI_INT,0,MPI_COMM_WORLD);
	if (rank == 0)
		for (i = 0; i < 9; i++)
			printf("%d ", x[i]);

   MPI_Finalize();
 }
