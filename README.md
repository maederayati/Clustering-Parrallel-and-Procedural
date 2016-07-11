#Parallel Implementation of the K-means Clustering Algorithm by Hybrid MPI and Open MP

## 1. Problem Description 

Clustering is the task of assigning a set of objects into groups (called clusters) so that the objects in the same cluster are more similar (in some sense or another) to each other than to those in other clusters. k-means clustering is a method of clustering which aims to partition n data points into k clusters (n >> k) in which each observation belongs to the cluster with the nearest mean.  The nearness is calculated by distance function which is Euclidian distance. One important assumption to be made is the data points are independent of each other. In other words there exists no dependency between any data points. 

##2. Basic  k-means algorithm (Serial)

The folowin is the pseudocode for the basic K-measn Algorithm:

**Input**: K, set of points $x<sub>1</sub>,...,x<sub>n</sub><br>
**Output**: A map from points to clusters 1,...,K<br>
Place centroid c<sub>1</sub>,…,c<sub>k</sub> at random places <br>
Repeat until convergence <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	For each point x<sub>i</sub><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;	Find nearest centroid c<sub>j</sub><br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;		Assign point x<sub>i</sub> to cluster j<br>
&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;For each cluster j= 1,…,k<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; New centroid c<sub>j</sub>=mean of all points x<sub>i</sub> assigned to cluster j in the previous step<br>

##3. Test Set<br>

### Test1: 

We initialized the (two-dimentional) data array inside our main function as follows:<br>
(0,0), (1,1),(2,2),(3,3)(4,4),(5,5),(6,6),(7,7),(8,8),(9,9),(10,10),(11,11)<br>

We also initialized the two centroid points as:<br>
(0,0), (5,5)<br><br>  



##Result<br>

###Test 1 Result:<br>

The following figure shows the process of updating the centroids.<br><br>

![Test 1 Result 1](https://github.com/maederayati/Parallel-Kmeans/blob/master/Test1_result1.jpg) <br><br>

Also the next figure shows that each data point belongs to which cluster at the end. <br><br>

![Test 1 Result 2](https://github.com/maederayati/Parallel-Kmeans/blob/master/Test1_result2.jpg) <br><br>
