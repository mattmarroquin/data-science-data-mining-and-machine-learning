/*
 * Skeleton function for Parallel Programming, 
 * Assignment 3: K-Means Algorithm (OpenMP)
 *
 * To students: You should finish the implementation of k_means function
 * 
 * Author:
 *     Wei Wang <wei.wang@utsa.edu>
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <omp.h> /* OpenMP header */

#include "k_means.h"

//distance function prototype
double distance(struct point p, struct point u);
/*
 * k_means: k_means clustering algorithm implementation.
 *
 * Input parameters:
 *     struct point p[]: array of data points
 *     int m           : number of data points in p[]
 *     int k           : number of clusters to find
 *     int iters       : number of clustering iterations to run
 *
 * Output parameters:   
 *     struct point u[]: array of cluster centers
 *     int c[]         : cluster id for each data points
 */
void k_means(struct point p[MAX_POINTS], 
	    int m, 
	    int k,
	    int iters,
	    struct point u[MAX_CENTERS],
	    int c[MAX_POINTS])
{
	int j; /* To Students: add your local variables */
        int l = 0;
        int i = 0;
        double sumx, sumy;
        int cluster_size;
        double min_dist;
        double dist;

	/* randomly initialized the centers */
	/* Note: DO NOT CHANGE THIS RANDOM GENERATOR! */
	/* Note: DO NOT PARALLELIZE THIS LOOP */
	/* Note: THE INTERFACE TO random_center HAS CHANGED */
	for(j = 0; j < k; j++)
		u[j] = random_center(p);
	
	/* 
	 * To students: please implment K-Means algorithm with OpenMP here
	 * Your K-means implementation should do "iters" rounds of clustering. After 
	 * all iterations finish, array u[MAX_CENTERS] should have the coordinations 
	 * of your centers, and array c[MAX_POINTS] should have the cluster assignment
	 * for each point.
	 */


        //calculate means
            for(l=0; l<iters; l++){
                //find nearest center

        #pragma omp parallel shared(i)
        {
                    #pragma omp for private(j, min_dist, dist) nowait schedule(static)
                    for(i=0; i<m; i++){ 
                        min_dist = DBL_MAX;
                        for(j=0; j<k; j++){
                            dist = distance(p[i], u[j]);
                            if (dist < min_dist){
                                min_dist = dist;
                                c[i] = j;
                            }
                        }
                    }
        }//end parallel

        
            //update center for each cluster

        #pragma omp parallel shared(j)
        {
        #pragma omp for private(i, sumx, sumy, cluster_size) nowait schedule(static)
                    for(j=0; j<k; j++){
                        sumx=0; 
                        sumy=0; 
                        cluster_size=0;
                        for(i=0; i<m; i++){
                            if(c[i] == j){
                                sumx += p[i].x;
                                sumy += p[i].y;
                                cluster_size++;
                            }
                        }
                        if(cluster_size > 0){
                            u[j].x = sumx/cluster_size;
                            u[j].y = sumy/cluster_size;
                        }
                        else{
                            u[j] = random_center(p);
                        }
                    }
        }//end parllel
    }//end outer loop w/ l variable
	
	
	return;
}

//calculate euclidean distance
double distance(struct point p, struct point u){
    double px = p.x;
    double py = p.y;
    double ux = u.x;
    double uy = u.y;

    double dist = sqrt(pow(px - ux, 2) + pow(py - uy, 2));
    return dist;
}
