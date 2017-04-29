#include<stdio.h>
#include <stdlib.h>     /* srand, rand */
#include<math.h>
#include <stdio.h>
#include <stdlib.h>


int n=10000;
int dim=8;
int cNum=128;	


int selectDim(double** data, int* flag, double* cutPoint){
	int i,j;
	
	double sum[dim];
	for(i=0;i<dim;i++)
		sum[i]=0;
	
	double avg[dim];
	for(i=0;i<dim;i++)
		avg[i]=0;
	
	int count=0;
	
	for(i=0;i<n;i++){
		if(flag[i]){
			count++;
			for(j=0;j<dim;j++){
				sum[j]=sum[j]+data[i][j];
				
			}
		}
	}
		
	for(i=0;i<dim;i++){
		avg[i]=sum[i]/count;
	}
	
	
	double SSE[8];
	for(i=0;i<dim;i++)
		SSE[i]=0;
	
	for(i=0;i<n;i++){
		if(flag[i]){
			for(j=0;j<dim;j++){
				SSE[j]=SSE[j]+pow((data[i][j]-avg[j]),2);
			}
		}
	}
	

	int max=0;
	for(i=1;i<dim;i++){
		if(SSE[i]>SSE[0]){
			max=i;
		}
	}
	
	cutPoint[0]=avg[max];
	return max;
}



void twoDTree(double** data,int*cluster_assign, int* flag,double* cutPoint, int* selected){
	
	int i;
	selected[0]=selectDim(data, flag,cutPoint);
	//printf("selected %d\n",selected[0]);
	for(i=0;i<n;i++){
		if(flag[i]){
			if(data[i][selected[0]]<cutPoint[0])
				cluster_assign[i]=0;
			else 
				cluster_assign[i]=1;
		}
	}
	
	
	
}

int maximumIndex(double** arr,int l, int d){
	int i, max=0;
	for(i=1;i<l;i++)
		if(arr[i][d]>arr[max][d])
			max=i;
	return max;
}

int minimumIndex(double** arr,int l, int d){
	int i, min=0;
	for(i=1;i<l;i++)
		if(arr[i][d]<arr[min][d])
			min=i;
	return min;
}



void kdTree(double** data,int*cluster_assign, int* cluster_size, int* flag, double** boundry,double*** boundry_all){
	
	int i,j,k;
	for(i=0;i<n;i++)
		flag[i]=1;
	
	

	

	for(j=0;j<dim;j++){
		boundry[j][0]=data[minimumIndex(data,n,j)][j];
		boundry[j][1]=data[maximumIndex(data,n,j)][j];		
	}
		
	
	double * cutPoint=(double*)malloc(sizeof(double));
	int * selected=(int*)malloc(sizeof(int));

	
	//int temp=selectDim(data,flag,cutPoint);
	//printf("%d",temp);
	
	twoDTree(data,cluster_assign,flag,cutPoint,selected);
	
	

	for(j=0;j<dim;j++){
		if(j!=selected[0]){
			boundry_all[0][j][0]=boundry[j][0];
			boundry_all[0][j][1]=boundry[j][1];
			boundry_all[1][j][0]=boundry[j][0];
			boundry_all[1][j][1]=boundry[j][1];
		}
		else{
			boundry_all[0][j][0]=boundry[j][0];
			boundry_all[0][j][1]=cutPoint[0];
			boundry_all[1][j][0]=cutPoint[0];
			boundry_all[1][j][1]=boundry[j][1];
		}
	}

	

	
	int size=2;
	
	
	
	while(size<cNum){
		
		for(k=0;k<size;k++){
			
			//update flag
			for(i=0;i<n;i++){
				if(cluster_assign[i]==k)
					flag[i]=1;
				else 
					flag[i]=0;
			}	
			
			twoDTree(data,cluster_assign,flag,cutPoint,selected);
			
			
			//update cluster_assign
			for(i=0;i<n;i++){
				if(flag[i]){
					if(cluster_assign[i]==0)
						cluster_assign[i]=k;
					
					else
						cluster_assign[i]=size+k;	
				}		
			}
				

			//update boundry_all
			for(j=0;j<dim;j++){
				if(j!=selected[0]){
					boundry_all[size+k][j][0]=boundry_all[k][j][0];
					boundry_all[size+k][j][1]=boundry_all[k][j][1];
				}
				else{
					boundry_all[size+k][j][1]=boundry_all[k][j][1];
					boundry_all[size+k][j][0]=cutPoint[0];
					boundry_all[k][j][1]=cutPoint[0];
				}
			}
					
			
		}
		
		size=2*size;
		
	}
	
	for(i=0;i<cNum;i++)
		cluster_size[i]=0;
	
	
	for(i=0;i<n;i++)
		cluster_size[cluster_assign[i]]++;

	
	
}

double distance(double* arr1,double* arr2){
	int i;
	double dist=0;
	for(i=0;i<dim;i++)
		dist=dist+pow((arr1[i]-arr2[i]),2);
	return dist;
	
}


void serach(double** data,int * cluster_assign,int * cluster_size,double ***boundry_all,
	double* query,double* closest_pt, int* num_of_pts_checked){
	
	
	int i,j,f,s;
	for(i=0;i<cNum;i++){
		f=0;
		for(j=0;j<dim;j++){
			if (query[j]>=boundry_all[i][j][0] && query[j]<=boundry_all[i][j][1]){
				f++;
			}
		}
		if (f==8){
			s=i;
		}
		
	}

	i=0;
	while(cluster_assign[i]!=s){
		i++;
	}
	double dist=distance(query, data[i]);
	int nn=i;
	num_of_pts_checked[0]=0;
	double tempDist=distance(query, data[i]);
	while(i<n){
		if (cluster_assign[i]==s){
			num_of_pts_checked[0]++;
			if(distance(query, data[i])<dist){
				nn=i;
				dist=distance(query, data[i]);
			}
		}
		i++;
	}
	for(i=0;i<dim;i++)
		closest_pt[i]=data[nn][i];
	
};




int main(){
	
	int i,j,k;

	
	double** data=(double**)malloc(n* sizeof(double*));
	for (i=0; i<n; i++) 
	  data[i] = (double*)malloc(dim * sizeof(double)); 
  

	for(i=0;i<n;i++)
		for(j=0;j<dim;j++)
			data[i][j]= (double)rand() / (double)RAND_MAX ;
		
	int *cluster_size = (int *)malloc(cNum * sizeof(int));
	int *cluster_assign = (int *)malloc(n * sizeof(int));
	int *flag = (int *)malloc(n * sizeof(int));

	
	for(i=0;i<2;i++)
		cluster_size[i]=0;
	
		//boundry[dim][2];
	double** boundry=(double**)malloc(dim* sizeof(double*));
	for (i=0; i<dim; i++) 
		boundry[i] = (double*)malloc(2 * sizeof(double)); 
  
  
  
	//boundry_all[128][dim][2]
	double*** boundry_all=(double***)malloc(cNum* sizeof(double*));
	for (i=0; i<cNum; i++){ 
		boundry_all[i] = (double**)malloc(dim * sizeof(double*));
		for(j=0;j<dim;j++)
			boundry_all[i][j] = (double*)malloc(2 * sizeof(double));
	}
  
		
	kdTree(data,cluster_assign,cluster_size,flag, boundry,boundry_all);
	
	
	double* query=(double*)malloc(dim* sizeof(double));
	
	
	double* closest_pt=(double*)malloc(dim* sizeof(double));
	int* num_of_pts_checked= (int*)malloc(sizeof(int));
	
	for(k=0;k<5;k++){
		for(i=0;i<dim;i++)
			query[i]=(double)rand() / (double)RAND_MAX ;
		serach(data,cluster_assign,cluster_size,boundry_all,query,closest_pt, num_of_pts_checked);
		printf("For query %d number of checked points are %d\n", k,num_of_pts_checked[0]);
	}
	
	
	return 0;
		
}