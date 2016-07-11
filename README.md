## Problem Description 

Clustering is the task of assigning a set of objects into groups (called clusters) so that the objects in the same cluster are more similar (in some sense or another) to each other than to those in other clusters. k-means clustering is a method of clustering which aims to partition n data points into k clusters (n >> k) in which each observation belongs to the cluster with the nearest mean.  The nearness is calculated by distance function which is Euclidian distance. One important assumption to be made is the data points are independent of each other. In other words there exists no dependency between any data points. 

##Basic  k-means algorithm (Serial)

Input: K, set of points x1,…Xn
Place centroid c1,…ck at random places 
Repeat until convergence 
	For each point xi
		Find nearest centroid cj
		Assign point xi to cluster j
	For each cluster j= 1,…,k
		New centroid cj=mean of all points xi assigned to cluster j in the previous step

##Test Set 

/*we initialized the (two-dimentional) data array inside our main function as follows:
(0,0), (1,1),(2,2),(3,3)(4,4),(5,5),(6,6),(7,7),(8,8),(9,9),(10,10),(11,11)


##Result
The following figure shows the process of updating the centroids.

Also the next figure shows that each data point belongs to which cluster at the end. 

