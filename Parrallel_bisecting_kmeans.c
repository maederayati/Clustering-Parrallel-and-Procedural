
#include <mpi.h>
#include<math.h>
#include<stdio.h>
#include <stdlib.h> 

	#define dim 8
	#define cNum 32
	#define n 120000


//to compute distance for two double pointer points 	
double distance(double** point1, double** point2){
	double sum=0;
	int i;
	for(i=0; i<dim;i++){
		sum=sum+pow((point1[i][0]-point2[i][0]), 2);
	}
	return sum;
}	

//to compute distance for two points one double pointer, one pointer
double distance2(double** point1, double* point2){
	double sum=0;
	int i;
	for(i=0; i<dim;i++){
		sum=sum+pow((point1[i][0]-point2[i]), 2);
	}
	return sum;
}	
	
	
double maximum(double* dist){	
	double max=dist[0];
	int i;
		for(i=1;i<2;i++){
			if(dist[i]>max){
				max=dist[i];
			}
		}	
		

	return max;
}


// to compute the index of second maximum in arry of integers
int secondMaximumIndex(int* dist, int t){
	
	int first = 0;
    int second = 1;
    int i;
		
    for(i=1;i<t;i++){
        if(dist[first] < dist[i]){
            second = first;
            first = i;
        }
        else if(dist[second] < dist[i] &&dist[i]!=dist[first]){
			second = i;
		}
        else if(dist[first]==dist[second]){
			second=i;
		}

    }
	return second;
	
}








//to compute the maximum index but integer array
int maximumIndex2(int* dist, int t){	
		int i;
		int max=0;
		for(i=1;i<t;i++){
			if(dist[i]>dist[max]){
				max=i;
			}
		}	
		return max;
}

//to compute the minimum index
int minimum(double* dist, int t){	
		int i;
		int min=0;
		for(i=1;i<t;i++){
			if(dist[i]<dist[min]){
				min=i;
			}
		}
		return min;

}


//to compute the maximum index
int maximum2(double* dist, int t){	
		int i;
		int max=0;
		for(i=1;i<t;i++){
			if(dist[i]>dist[max]){
				max=i;
			}
		}
		return max;
}	
	


void twoMeans(double**** data,double*** cluster_center,int** cluster_assign,
	int*** cluster_size,int** flag,int** tempSize,MPI_Comm comm){
	
	

	int rank, pNum;	
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &pNum);
	
	int portion=n/pNum;	
	int i,j,k;
	
	double sum[pNum][2][dim][1];
	double tempSum[2][dim];

	
	double *** new_cluster_center = (double ***)malloc(2*sizeof(double**));
	for (i = 0; i< 2; i++) {
		new_cluster_center[i] = (double **) malloc(dim*sizeof(double *));
		for (j = 0; j < dim; j++) {
			new_cluster_center[i][j] = (double *)malloc(1*sizeof(double));
		}
	}
	
	
	
	
	double* dist=(double*)malloc(2*sizeof (double));
	
	double difference[1];	
	int shortest;

		

	//check whether any processor has empty number of points
	int y=0;
	for(i=0;i<portion;i++){
		if(flag[rank][i]){
			y++;
		}
					
	}
				
		
	
		

	difference[0]=10;
	
	
	while (difference[0]>0.000001){
		
		
			//assigning clusters to each datapoint.
	for(i=0;i<portion;i++){
		
		if(y>0){
			if(flag[rank][i]){
				shortest=0;
				for (j=0;j<2;j++){			
					if(distance(data[rank][i],cluster_center[shortest])>
						distance(data[rank][i],cluster_center[j])){
						shortest=j;
					}
						
				}
				cluster_assign[rank][i]=shortest;
			}
		}
	}
		
			

		for(i=0;i<2;i++)
			for (j=0;j<dim;j++)
				sum[rank][i][j][0]=0;
		
		
		for(i=0;i<dim;i++){
			for(j=0;j<portion;j++){
				if(y>0){
					if(flag[rank][j]){
						sum[rank][cluster_assign[rank][j]][i][0]+=data[rank][j][i][0];
					}
				}
				
			}
				
		}		


		for(i=0;i<2;i++)
			cluster_size[rank][i][0]=0;
		
		for(i=0;i<portion;i++){
			if(y>0){
				if(flag[rank][i]){
					cluster_size[rank][cluster_assign[rank][i]][0]++;
				}
			}
		}	
	
		for(k=0;k<pNum;k++){
			for(i=0;i<2;i++){
				for(j=0;j<dim;j++){
					MPI_Bcast(sum[k][i][j], 1, MPI_DOUBLE,k,MPI_COMM_WORLD);
				}
				MPI_Bcast(cluster_size[k][i], 1, MPI_INTEGER,k,MPI_COMM_WORLD);
			}	
		}
		

		
		
		
		if(rank==0){
			
			

			
			for(i=0;i<2;i++)
				for(j=0;j<dim;j++)
					tempSum[i][j]=0;
				
			for(i=0;i<2;i++)
					tempSize[i][0]=0;
				
			for(i=0;i<dim;i++)
				for(j=0;j<2;j++)
					for(k=0;k<pNum;k++)
						tempSum[j][i]+=sum[k][j][i][0];
					
			for(i=0;i<2;i++)
				for(j=0;j<pNum;j++)
					tempSize[i][0]+=cluster_size[j][i][0];
			
			
			for(i=0;i<2;i++){
				for(j=0;j<dim;j++){
					if(tempSize[i][0]!=0){
						new_cluster_center[i][j][0]=tempSum[i][j]/tempSize[i][0];
					}
					else{
						new_cluster_center[i][j][0]=(double)rand() / (double)RAND_MAX;
					}
				}
			}
				
			for(i=0;i<2;i++)
				dist[i]=distance(new_cluster_center[i],cluster_center[i]);
								
				
			difference[0]=maximum(dist);

			for(i=0;i<2;i++)
				for(j=0;j<dim;j++)
					cluster_center[i][j][0]=new_cluster_center[i][j][0];
			
				
		}
		

		
			for(i=0;i<2;i++)
				for(j=0;j<dim;j++)	
					MPI_Bcast(new_cluster_center[i][j], 1, MPI_DOUBLE,0,MPI_COMM_WORLD);
				
			MPI_Bcast(difference, 1, MPI_DOUBLE,0,MPI_COMM_WORLD);	

			
	}
	
	for(i=0;i<2;i++)	
				MPI_Bcast(tempSize[i], 1, MPI_INTEGER,0,MPI_COMM_WORLD);
///////////////////////////////////////////////////////////////////	

}





void SSE(double**** data,double*** cluster_center,int** cluster_assign,int** flag,MPI_Comm comm, double* SSE_sum ){


int rank, pNum;	
	MPI_Comm_rank(comm, &rank);
	MPI_Comm_size(comm, &pNum);
	
	int portion=n/pNum;	
	int i,j,k;


	double** mySSE=(double**)malloc(pNum* sizeof(double*));
	for(i=0;i<pNum;i++){
		mySSE[i]=(double*)malloc(sizeof(double));
	}
	mySSE[rank][0]=0;
	for(i=0;i<portion;i++){
		if(flag[rank][j]){
			mySSE[rank][0]=mySSE[rank][0]+distance(data[rank][i],cluster_center[cluster_assign[rank][i]]);
		}
	}
	
	for(k=0;k<pNum;k++)
			MPI_Bcast(mySSE[k], 1, MPI_DOUBLE,k,MPI_COMM_WORLD);



	if (rank==0){
		
		SSE_sum[0]=0;
		for(i=0;i<pNum;i++)
			SSE_sum[0]+=mySSE[i][0];
	}
	MPI_Bcast(SSE_sum, 1, MPI_DOUBLE,0,MPI_COMM_WORLD);

	
	
}


	
main(int argc, char **argv){

	
	MPI_Init(&argc, &argv);	
	int rank, pNum;	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &pNum);
	
	int portion=n/pNum;	
	int i,j,k;
	

	
	
	// Allocate memory for data --- double data[pNum][portion][dim][1];
	double **** data = (double ****)malloc(pNum*sizeof(double***));
	for (i = 0; i< pNum; i++) {
		data[i] = (double ***) malloc(portion*sizeof(double **));
		for (j = 0; j < portion; j++) {
			data[i][j] = (double **)malloc(dim*sizeof(double*));
			for(k=0;k<dim;k++){
				data[i][j][k]=(double *)malloc(1*sizeof(double));
			}
		}
	}


	// Allocate memory for flag --- int flag[pNum][portion];
	int ** flag = (int **)malloc(pNum*sizeof(int*));
	for (i = 0; i< pNum; i++) {
		flag[i] = (int *) malloc(portion*sizeof(int ));
	}
	
	// Allocate memory for cluster flag --- int cluster_flag[cNum] indicating whether a cluster is done;
	int** cluster_flag=(int**)malloc(pNum*sizeof(int*));
	for(i=0;i<pNum;i++) 
		cluster_flag[i]=(int*)malloc(cNum*sizeof(int*));
	
	for(i=0;i<pNum;i++) 
		cluster_flag[rank][i]=0;
	
	
	
	
	// Allocate memory for cluster_assign --- int cluster_assign[pNum][portion];
	int ** cluster_assign = (int **)malloc(pNum*sizeof(int*));
	for (i = 0; i< pNum; i++) {
		cluster_assign[i] = (int *) malloc(portion*sizeof(int));
	}
	
	
	
	
	
	// Allocate memory for cluster_center ---double cluster_center[cNum][dim][1];
	double *** cluster_center = (double ***)malloc(cNum*sizeof(double**));
	for (i = 0; i< cNum; i++) {
		cluster_center[i] = (double **) malloc(dim*sizeof(double *));
		for (j = 0; j < dim; j++) {
			cluster_center[i][j] = (double *)malloc(1*sizeof(double));
		}
	}
	
	
	//global cluster_center and cluster_size
	double **cluster_center_all=(double**)malloc(cNum*sizeof(double*));
    for (i=0; i<cNum; i++)
         cluster_center_all[i] = (double *)malloc(dim * sizeof(double));
	 
	int *cluster_size_all = (int *)malloc(cNum * sizeof(int));
	for(i=0;i<cNum;i++){
		cluster_size_all[i]=0;
	}
	
	
	
	
	
	
	
	// Allocate memory for cluster_size --- int cluster_size[pNum][cNum][1];
	int *** cluster_size = (int ***)malloc(pNum*sizeof(int**));
	for (i = 0; i< pNum; i++) {
		cluster_size[i] = (int **) malloc(cNum*sizeof(int *));
		for (j = 0; j < cNum; j++) {
			cluster_size[i][j] = (int *)malloc(1*sizeof(int));
		}
	}
	
	
	
	

	
	
	//initialize the datapoints to random number between 0-1
	
	srand(12);
	for(k=0;k<pNum;k++){
	for (i=0;i<portion;i++){
		for(j=0;j<dim;j++){
			data[k][i][j][0]=(double)rand() / (double)RAND_MAX;
		}
}}

		
	
	//initialize the first centroids to be random
	for(i=0;i<cNum;i++)
		for(j=0;j<dim;j++)
			cluster_center[i][j][0]=(double)rand() / (double)RAND_MAX;
			
			
	//initialize the first flag to be 1
	for(j=0;j<portion;j++)
		flag[rank][j]=1;
		
	//int* tempSize=(int*)malloc(2*sizeof(int));
	int** tempSize=(int**)malloc(3*sizeof(int*));
	for(i=0;i<3;i++) tempSize[i]=(int*)malloc(sizeof(int));
	
	twoMeans(data,cluster_center,cluster_assign,cluster_size,flag,tempSize,MPI_COMM_WORLD);

	cluster_flag[rank][0]=1;
	cluster_flag[rank][1]=1;
	
	
	for(j=0;j<dim;j++){
		cluster_center_all[0][j]=cluster_center[0][j][0];
		cluster_center_all[1][j]=cluster_center[1][j][0];
	}
	
	cluster_size_all[0]=tempSize[0][0];
	cluster_size_all[1]=tempSize[1][0];
	

	
	
	int size=2;
	
	double array1[5][2][8];
	double* array2=(double *)malloc(5 * sizeof(double));
	double* SSE_sum=(double *)malloc( sizeof(double));
	
	int selected=33;
	int selected2=33;
	int first,second;

	
	while(size<32){
		
		
		

		selected=maximumIndex2(cluster_size_all,size);
		selected2=secondMaximumIndex(cluster_size_all,size);
		

			
		if((cluster_size_all[selected]<(2*cluster_size_all[selected2]))||size==31 ){
		
			//set the flag
			for(i=0;i<portion;i++){
				if(cluster_assign[rank][i]!=selected){
					flag[rank][i]=0;
				}
				else{
					flag[rank][i]=1;
				}
			}
			
			
			
			
			//choose which clustering is the best
			for(i=0;i<5;i++){
				for(j=0;j<dim;j++){
					cluster_center[0][j][0]=(double)rand() / (double)RAND_MAX ;
					cluster_center[1][j][0]=(double)rand() / (double)RAND_MAX ;
				}
				twoMeans(data,cluster_center,cluster_assign,cluster_size,flag,tempSize,MPI_COMM_WORLD);
				for(j=0;j<dim;j++){
					array1[i][0][j]=cluster_center[0][j][0];
					array1[i][1][j]=cluster_center[1][j][0];
				}	
				//double** data,double** cluster_center,int* cluster_assign,int* flag,MPI_Comm comm 
				SSE(data,cluster_center,cluster_assign,flag, MPI_COMM_WORLD, SSE_sum);
				array2[i]=SSE_sum[0];
			
			}
			
			//find the cluster_center based on the above iterations
			for(i=0;i<2;i++)
				for(j=0;j<dim;j++)
					cluster_center[i][j][0]=array1[maximum2(array2,5)][i][j];
				

			twoMeans(data,cluster_center,cluster_assign,cluster_size,flag,tempSize,MPI_COMM_WORLD);
			
			
			
			
			
			//update cluster_assign
			for(i=0;i<portion;i++){
				if(flag[rank][i]){
					if(cluster_assign[rank][i]==0){
						cluster_assign[rank][i]=selected;
					}
					else{
						cluster_assign[rank][i]=size;
					}
					
				}
				
				
			}
			
			
			//update cluster_size
			cluster_size_all[selected]=tempSize[0][0];
			cluster_size_all[size]=tempSize[1][0];
			
			//update cluster_center_all
			for(j=0;j<dim;j++){
				cluster_center_all[selected][j]=cluster_center[0][j][0];
				cluster_center_all[size][j]=cluster_center[1][j][0];
			}
			size++;
			
		}	
		
		else {
			
			//first work on the maximum, i.e.selected
			for(i=0;i<portion;i++){
				if(cluster_assign[rank][i]!=selected){
					flag[rank][i]=0;
				}
				else{
					flag[rank][i]=1;
				}
			}
			
			
		
			for(i=0;i<5;i++){
				for(j=0;j<dim;j++){
					cluster_center[0][j][0]=(double)rand() / (double)RAND_MAX ;
					cluster_center[1][j][0]=(double)rand() / (double)RAND_MAX ;
				}
				twoMeans(data,cluster_center,cluster_assign,cluster_size,flag,tempSize,MPI_COMM_WORLD);
				for(j=0;j<dim;j++){
					array1[i][0][j]=cluster_center[0][j][0];
					array1[i][1][j]=cluster_center[1][j][0];
				}	
				//double** data,double** cluster_center,int* cluster_assign,int* flag,MPI_Comm comm 
				SSE(data,cluster_center,cluster_assign,flag, MPI_COMM_WORLD, SSE_sum);
				array2[i]=SSE_sum[0];
			
			}
			
			
			for(i=0;i<2;i++)
				for(j=0;j<dim;j++)
					cluster_center[i][j][0]=array1[maximum2(array2,5)][i][j];
				

			twoMeans(data,cluster_center,cluster_assign,cluster_size,flag,tempSize,MPI_COMM_WORLD);
			
			
			
			//update cluster_assign
			for(i=0;i<portion;i++){
				if(flag[rank][i]){
					if(cluster_assign[rank][i]==0){
						cluster_assign[rank][i]=selected;
					}
					else{
						cluster_assign[rank][i]=size;
					}
					
				}
				
				
			}
			
			
			//update cluster_size
			cluster_size_all[selected]=tempSize[0][0];
			cluster_size_all[size]=tempSize[1][0];
			
			//update cluster_center_all
			for(j=0;j<dim;j++){
				cluster_center_all[selected][j]=cluster_center[0][j][0];
				cluster_center_all[size][j]=cluster_center[1][j][0];
			}
			size++;
			
			
			
			//if you can still continue divide the other halh as well
			if(size<32){
				
				//set the flag
				for(i=0;i<portion;i++){
					if(cluster_assign[rank][i]!=selected2){
						flag[rank][i]=0;
					}
					else{
						flag[rank][i]=1;
					}
				}
				
				
			
				for(i=0;i<5;i++){
					for(j=0;j<dim;j++){
						cluster_center[0][j][0]=(double)rand() / (double)RAND_MAX ;
						cluster_center[1][j][0]=(double)rand() / (double)RAND_MAX ;
					}
					twoMeans(data,cluster_center,cluster_assign,cluster_size,flag,tempSize,MPI_COMM_WORLD);
					for(j=0;j<dim;j++){
						array1[i][0][j]=cluster_center[0][j][0];
						array1[i][1][j]=cluster_center[1][j][0];
					}	
				 
					SSE(data,cluster_center,cluster_assign,flag, MPI_COMM_WORLD, SSE_sum);
					array2[i]=SSE_sum[0];
				
				}
				
				
				for(i=0;i<2;i++)
					for(j=0;j<dim;j++)
						cluster_center[i][j][0]=array1[maximum2(array2,5)][i][j];
					

				twoMeans(data,cluster_center,cluster_assign,cluster_size,flag,tempSize,MPI_COMM_WORLD);
				
				
				
				
				
				//update cluster_assign
				for(i=0;i<portion;i++){
					if(flag[rank][i]){
						if(cluster_assign[rank][i]==0){
							cluster_assign[rank][i]=selected2;
						}
						else{
							cluster_assign[rank][i]=size;
						}
						
					}
					
					
				}
				
				
				//update cluster_size
				cluster_size_all[selected2]=tempSize[0][0];
				cluster_size_all[size]=tempSize[1][0];
				
				//update cluster_center_all
				for(j=0;j<dim;j++){
					cluster_center_all[selected2][j]=cluster_center[0][j][0];
					cluster_center_all[size][j]=cluster_center[1][j][0];
				}
				size++;
				
				
			}
			
			
			
		
			
		}
		

		
		
		
		
	}
/*	
	
*/	
	
	
	
	
	
	
	
	
	
	
	
	

	//compute SSE
	double mySSE[pNum][1];
	for(i=0;i<pNum;i++)
		mySSE[pNum][0]=0;
	
	for(i=0;i<portion;i++){
		mySSE[rank][0]=mySSE[rank][0]+distance2(data[rank][i],cluster_center_all[cluster_assign[rank][i]]);

	}

	for(k=0;k<pNum;k++){
		
			MPI_Bcast(mySSE[k], 1, MPI_INTEGER,k,MPI_COMM_WORLD);
			
	}
	
	
	
	if (rank==0){
		

		double SSE_sum2=0;
		for(i=0;i<pNum;i++){
			SSE_sum2+=mySSE[i][0];

		}
		
		printf("For %d number of clusters and %d number of processors:\n",cNum, pNum);
		
		printf("\t SSE = %f\n",SSE_sum2);
		
		
		
		printf("\t ----------------------\n");
		for(i=0;i<cNum;i++)
			for(j=0;j<dim;j++)
					printf("\t cluster center of cluster %d and dimention %d is %f\n",i,j,cluster_center_all[i][j]);
		
	
		printf("\t ----------------------\n");
		
		for(i=0;i<cNum;i++)
			printf("\t cluster size of cluster %d is %d\n",i,cluster_size_all[i]);
		
		printf("\n****************************************************************************\n");
		
		
	
		
	}			
	
				
		
		
	

	MPI_Finalize();
}
