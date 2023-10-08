


CS5379 Assignment 3


The main function is responsible of doing the following (rank 0):
-	Initialize MPI.
-	Initialize the edge buffer.
-	Initialize the dist buffer.
Then, each processor will create their own buffer.
Finally all processors will call the f function.

f(SOURCE, n, **edge, *dist, rank,P)
	/* Rank 0 section */
If rank = 0
J = call choose function
		Loop from i=1 to P
			Send dist[i*(n/P) to n/P] to i
			Send found[i*(n/P) to n/P] to i
			Recv location from i
Find minimum dist with respect of the received buffer location.
	Loop from i=1 to P
			Send found[location to n/P] to i
			Send edge[j][ location to n/P] to i
			Send min value
			Recv dist from i
	Else
		Recv dist[0 to n/P] from rank 0
		Recv found[0 to n/P] from rank 0
J = call choose function
		Send location to rank 0
		Recv found[0 to n/P] from rank 0 
		Recv edge[j][0 to n/P] from rank 0
		Recv  min value from rank 0
		Find the min value
		Send dist to rank 0
	End If






