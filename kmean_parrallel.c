
#include <mpi.h>
#include<math.h>
#include<stdio.h>
#include <stdlib.h> 

	#define dim 8
	#define cNum 32
	#define n 120000

	
double distance(double point1[dim][1], double point2[dim][1]){
	double sum=0;
	int i;
	for(i=0; i<dim;i++){
		sum=sum+pow((point1[i][0]-point2[i][0]), 2);
	}
	return sum;
}	
	
	
double maximum(double dist[cNum]){	
	double max=dist[0];
	int i;
		for(i=1;i<cNum;i++){
			if(dist[i]>max){
				max=dist[i];
			}
		}	
		

	return max;
}	
	
	
main(int argc, char **argv){

	
	MPI_Init(&argc, &argv);	
	int rank, pNum;	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &pNum);
	
	double difference[1];	
	int shortest;
	int portion=n/pNum;	
	
	double data[pNum][portion][dim][1];
	int cluster_assign[pNum][portion];
	double cluster_center[cNum][dim][1];
	int cluster_size[pNum][cNum][1];
	double sum[pNum][cNum][dim][1];
	double tempSum[cNum][dim];
	int tempSize[cNum];
	double new_cluster_center[cNum][dim][1];
	double dist[cNum];
	
	int i,j,k;
	
	
	//initialize the datapoints to random number between 0-1
	for (i=0;i<portion;i++){
		for(j=0;j<dim;j++){
			data[rank][i][j][0]=(double)rand() / (double)RAND_MAX;
		}
	}

		
	
	//initialize the first centroids to be 1
	for(i=0;i<cNum;i++)
		for(j=0;j<dim;j++)
			cluster_center[i][j][0]=(double)rand() / (double)RAND_MAX;
			
		
	
	


		
	//assigning clusters to each datapoint.
	for(i=0;i<portion;i++){
		shortest=0;
		for (j=0;j<cNum;j++){			
			if(distance(data[rank][i],cluster_center[shortest])>
				distance(data[rank][i],cluster_center[j])){
				shortest=j;
			}
				
		}
		cluster_assign[rank][i]=shortest;
	}
	
	
		

	difference[0]=10;
	
	
	while (difference[0]>0.000001){
		
			

		for(i=0;i<cNum;i++)
			for (j=0;j<dim;j++)
				sum[rank][i][j][0]=0;
		
		
		for(i=0;i<dim;i++)
			for(j=0;j<portion;j++)
				sum[rank][cluster_assign[rank][j]][i][0]+=data[rank][j][i][0];
				


		for(i=0;i<cNum;i++)
			cluster_size[rank][i][0]=0;
		
		for(i=0;i<portion;i++)
			cluster_size[rank][cluster_assign[rank][i]][0]++;
			
	
		for(k=0;k<pNum;k++){
			for(i=0;i<cNum;i++){
				for(j=0;j<dim;j++){
					MPI_Bcast(sum[k][i][j], 1, MPI_DOUBLE,k,MPI_COMM_WORLD);
				}
				MPI_Bcast(cluster_size[k][i], 1, MPI_INTEGER,k,MPI_COMM_WORLD);
			}	
		}
		

		
		
		
		if(rank==0){
			
			

			
			for(i=0;i<cNum;i++)
				for(j=0;j<dim;j++)
					tempSum[i][j]=0;
				
			for(i=0;i<cNum;i++)
					tempSize[i]=0;
				
			for(i=0;i<dim;i++)
				for(j=0;j<cNum;j++)
					for(k=0;k<pNum;k++)
						tempSum[j][i]+=sum[k][j][i][0];
					
			for(i=0;i<cNum;i++)
				for(j=0;j<pNum;j++)
					tempSize[i]+=cluster_size[j][i][0];
			
			
			for(i=0;i<cNum;i++)
				for(j=0;j<dim;j++)
					new_cluster_center[i][j][0]=tempSum[i][j]/tempSize[i];
				
				
				
			for(i=0;i<cNum;i++)
				dist[i]=distance(new_cluster_center[i],cluster_center[i]);
								
				
			difference[0]=maximum(dist);

			for(i=0;i<cNum;i++)
				for(j=0;j<dim;j++)
					cluster_center[i][j][0]=new_cluster_center[i][j][0];
			
				
		}
		

		
			for(i=0;i<cNum;i++)
				for(j=0;j<dim;j++)	
					MPI_Bcast(new_cluster_center[i][j], 1, MPI_DOUBLE,0,MPI_COMM_WORLD);
				
			MPI_Bcast(difference, 1, MPI_DOUBLE,0,MPI_COMM_WORLD);	

			
	}
	
	
	
	double mySSE[pNum][1];
	mySSE[rank][0]=0;
	for(i=0;i<portion;i++)
		mySSE[rank][0]=mySSE[rank][0]+distance(data[rank][i],cluster_center[cluster_assign[rank][i]]);
	
	
	for(k=0;k<pNum;k++)
			MPI_Bcast(mySSE[k], 1, MPI_INTEGER,k,MPI_COMM_WORLD);
		
		
				
		
		
	
	

	
	if (rank==0){
		
		double SSE_sum=0;
		for(i=0;i<pNum;i++)
			SSE_sum+=mySSE[i][0];
		
		
		
		
		printf("For %d number of clusters and %d number of processors:\n",cNum, pNum);

		printf("\t SSE = %f\n",SSE_sum);
		printf("\t ----------------------\n");
		for(i=0;i<cNum;i++)
			for(j=0;j<dim;j++)
					printf("\t cluster center of cluster %d and dimention %d is %f\n",i,j,cluster_center[i][j][0]);
		
		printf("\t ----------------------\n");
		for(i=0;i<cNum;i++)
			printf("\t cluster size of cluster %d is %d\n",i,tempSize[i]);
		
		printf("\n****************************************************************************\n");
		
		
		
	}			
	
					
			
			
	

	MPI_Finalize();
}
