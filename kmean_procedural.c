#include<stdio.h>
#include <stdlib.h>     /* srand, rand */
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
/*	

	SOME NOTATION:
	- 	cluster_assign[] is an array of size n where cluster_assign[i] 
		indicates which of the k clusters the ith data is assigned to.	
	-	cluster_center[] is an array of size dim*k where 
		cluster_center[0]-cluster_center[dim-1] indicates 
		the location of the center of the 0th cluster.	
	
	-	cluster_size[] is an array of size k, where cluster_size[i]
		indicates how many data items the ith cluster has.
		
	-	data[] is an array of size dim* n, where data[0]-data[dim-1]
		stores the 0th data item. 

	-	A metric for evaluating kmeans:
			\sigma_{j=1}^k  \sigma_{x_i \in cluster j}^k	(X_i -\bar(X))^2
			
			this is the sum of square of standard diviations of each cluster.
		
		Bellow we have:
		1. data was initialized in the main function. Data is 
			multidimentinal and has random values between 0 and 1 
		2. in the main() "kmeans" function is called 
		3. For first cluster center points we use the k first ponts in data.
			Print the result to a file.
		
	
		
*/

	int dim=2;
	int k=5;
	int n=10000;

float maximum(float dist[k]){	
		float max=dist[0];
		for(int i=1;i<k;i++)
			if(dist[i]>max)
				max=dist[i];
			
		
	//printf("max %f", max);
		return max;
}	


//this function computes the square of distance between two data points
float distance(float point1[dim], float point2[dim]){
	float sum=0;
	for(int i=0; i<dim;i++)
		sum=sum+pow((point1[i]-point2[i]), 2);
	
	return sum;
}

void kmeans(int dim, int n, float data[n][dim], int k, int cluster_assign[n], 
	float cluster_center[k][dim], int cluster_size[k]){
		
	//dist[i] is the distance between ith new and old centroid 	
	float dist[k];
	
	int shortest;
	
	//for storing the updated centroid
	float new_cluster_center[k][dim];
	
	float max=1;
	
	//maximum of difference between old and new centroids. 
	while(max>0.0000001){
		
		//assigning clusters to each datapoint.
		for(int i=0 ;i<n;i++){
			shortest=0;
			for (int j=0;j<k;j++){
				if (distance(data[i],cluster_center[shortest])>distance(data[i],cluster_center[j]))
					shortest=j;
				cluster_assign[i]=shortest;
			}
		}
		

	
		//calculating he number of datapoints in each cluster.
		//cluster_size[i] is the number of datapoints in ith cluster.
		for(int i=0;i<k;i++) cluster_size[i]=0;	
		for(int i=0;i<n;i++)
			cluster_size[cluster_assign[i]]++;

		//defined for summation of datapoints in each cluster
		float sum[k][dim];	
	
		//set sum to zero at the begining
		for (int i=0;i<k;i++)
			for (int j=0;j<dim;j++)
					sum[i][j]=0;
	
		//calculate the summation of datapoints in each cluster
		for(int j=0;j<dim;j++)
			for(int i=0;i<n;i++)
				sum[cluster_assign[i]][j]=sum[cluster_assign[i]][j]+data[i][j];
		
		//compute new cluster centers
		for (int i=0;i<k;i++)
			for(int j=0;j<dim;j++)
				new_cluster_center[i][j]=sum[i][j]/cluster_size[cluster_assign[i]];
			
		//compute the distance between old and new centroids
		//dist[i] is the distance between ith new and old centroid 
		for(int i=0;i<k;i++)
			dist[i]=distance(new_cluster_center[i],cluster_center[i]);
	
		//update the maximum of distance between old and new centroids
		max=maximum(dist);
	
		
		//update new clusters
		for (int i=0;i<k;i++)
			for(int j=0;j<dim;j++)
				cluster_center[i][j]=new_cluster_center[i][j];
			
	}
	
}
	
void main(int argc, char *argv[]){
	

	float data[n][dim];
	int cluster_assign[n];
	float cluster_center[k][dim];
	int cluster_size[k];
	
	float latitude;
	float longitude;
	//initialize the ddatapoints to random number between 0-1

	
	/*for (int i=0; i<n;i++){
		scanf("%f,%f\n", &latitude, &longitude);
		data[i][0]=latitude;
		data[i][1]=longitude;
		i++;
		
	}*/
	for(int i=0;i<n;i++)
		for(int j=0;j<dim;j++)
			data[i][j]=(float)rand() / (float)RAND_MAX ;
		


	for(int i=0;i<k;i++)
		for(int j=0;j<dim;j++)
			cluster_center[i][j]=data[i][j];
		
		
	kmeans(dim, n, data, k, cluster_assign, 
	cluster_center, cluster_size);
	
	
	

		printf("For %d number of clusters:\n",k);
		for(int i=0;i<k;i++) 
			for(int j=0;j<dim;j++)
				printf("\t cluster center of cluster %d and dimention %d is %f\n",i,j,cluster_center[i][j]);
	
		printf("\t ----------------------\n");
		for(int i=0;i<k;i++) 
			printf("\t cluster size of cluster %d is %d\n",i,cluster_size[i]);
		
		printf("\t ----------------------\n");
		float SSE=0;
		
		for(int i=0;i<n;i++)
				SSE+=distance(data[i], cluster_center[cluster_assign[i]]);
		
		printf("\t SSE = %f\n\n",SSE);
		printf("****************************************************************************\n");

		
}