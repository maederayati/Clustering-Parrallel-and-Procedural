/*

Author:   Maede Rayatidamavandi, Sara Sartoli
Date:    11/30/2015
Problem: Hybrid MPI/OpenMP Kmeans Implementation
Course:  Parallel computing

*/

#include <mpi.h>
#define NUM_THREADS 2
#define ARRAYSIZE   12
  

main(int argc, char **argv){
	

	
	
	MPI_Init(&argc, &argv);
	omp_set_num_threads(NUM_THREADS);	
	
	int myRank, mySize;
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Comm_size(MPI_COMM_WORLD, &mySize);
		
	double data[ARRAYSIZE][2];
	int portion=ARRAYSIZE/mySize;
	int cNum=2;
	double min;
	int cluster;
	int count[mySize][cNum][1];
	double sum[mySize][cNum][2][1];
	double c[cNum][2];
	double dist;
	int output[ARRAYSIZE][1];
	int i,j,k,l,m;
	double difsum, old,new;
	double dif[2][cNum];
	
	double sumtempx[cNum];
	double sumtempy[cNum];
	int counttemp[cNum];

	// Initialize a test Data
	for (i=myRank*portion;i<myRank*portion+portion;i++){
		data[i][0]=i;
		data[i][1]=i;
	}
	


	// Set Initial Centroid
	c[0][0]=0;
	c[0][1]=0;
	c[1][0]=5;
	c[1][1]=5;
	
	double difference[1];
	difference[0]=1;
	
	
	while (difference[0]>0){
	
		// Initial sum and count for taking average
		for(i=0;i<cNum;i++){
			count[myRank][i][0]=0;		
			sumtempx[i]=0;
			sumtempy[i]=0;
			counttemp[i]=0;
			for(j=0;j<mySize;j++){
				sum[j][i][0][0]=0;
				sum[j][i][1][0]=0;
			}
		}
		
		

		
		#pragma omp parallel
		{
			
		int i,j;
		int id=omp_get_thread_num();
		int dist, cluster, min=100000;
		int nthreads=omp_get_num_threads();	
		int portion=ARRAYSIZE/mySize;
		int threadPortion=portion/NUM_THREADS;



        // identify membership of data points to clusters
		for(i=myRank*portion+(id*threadPortion); i<myRank*portion+(id*threadPortion)+threadPortion;i++){
			for (j=0; j<cNum; j++){
				dist=sqrt(pow((data[i][0]-c[j][0]),2)+pow((data[i][1]-c[j][1]),2));
				if(min>dist){
					min=dist;
					cluster=j;
				}
			}
			output[i][0]=cluster;
		}


			
	    

		// each processor sum of points for each cluster within its portion
		for(i=0;i<cNum;i++){
			for (j=myRank*portion+(id*threadPortion);j<myRank*portion+(id*threadPortion)+threadPortion;j++){
				
				if(i==output[j][0]){
					sum[myRank][i][0][0]=sum[myRank][i][0][0]+data[j][0];
					sum[myRank][i][1][0]=sum[myRank][i][1][0]+data[j][1];
					count[myRank][i][0]=count[myRank][i][0]+1;
				}
				
			}
		}
		
     	}
     	
     	
     	// Pragma suit finishes here
			
        //each processor broadcast its sum for each cluster
		for(j=0;j<cNum;j++){
			for(i=0;i<mySize;i++){
				MPI_Bcast(sum[i][j][0], 1, MPI_DOUBLE,i,MPI_COMM_WORLD);
				MPI_Bcast(sum[i][j][1], 1, MPI_DOUBLE,i,MPI_COMM_WORLD);
				MPI_Bcast(count[i][j], 1, MPI_INTEGER,i,MPI_COMM_WORLD);		
			}
		}

		difsum=0;		
					
		// rank 0 add all the sums
		if(myRank==0){
			for(i=0;i<cNum;i++){
				for(j=0;j<mySize;j++){
					sumtempx[i]=sum[j][i][0][0]+sumtempx[i];
					sumtempy[i]=sum[j][i][1][0]+sumtempy[i];
					counttemp[i]=count[j][i][0]+counttemp[i];
				}
				
				old=c[i][0];
				c[i][0]=sumtempx[i]/counttemp[i];
				new=c[i][0];
				dif[0][i]=abs(old-new);
				// centroid before update
				old=c[i][1];
				c[i][1]=sumtempy[i]/counttemp[i];
				//centroid after update
				new=c[i][1];
				dif[1][i]=abs(old-new);
					
					
				for(k=0;k<2;k++){
					difsum=difsum+dif[k][i];
				}
					
			}
				
		}
			
		for (i=0;i<cNum;i++){
			MPI_Bcast(c[i], 2, MPI_DOUBLE,0,MPI_COMM_WORLD);
		}
			


		difference[0]=difsum;
		MPI_Bcast(difference, 1, MPI_DOUBLE,0,MPI_COMM_WORLD);



			
		if(myRank==0){
			

            
			for(i=0;i<cNum;i++){
				printf("centroid of cluster %d is (%3.2f,%3.2f) \n",i, c[i][0],c[i][1]);
			}
		
		
	   }
		
	
   }
   
	
		for(i=myRank*portion;i<myRank*portion+portion;i++)
		{
	       printf("data point number %d is mapped to cluster %d \n", i, output[i][0]);
		}

	MPI_Finalize();
	
}
