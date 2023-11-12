In my program, load balancing achieved by the following equation in order to distribute the rows equally between the processes:
Part 1: 
Row location = (current iteration * (#processes * 2)) + myRank 
Part 2: 
Row location = ((current iteration+1) * (#processes * 2) -1) - myRank 
Theses equations will distribute the rows as the following (if we assume P=4 and N=32), please note the data in the table is the row number:
 ![image](https://github.com/michael-beebe/cs5379-parallel-processing/assets/113784916/218e733d-2c36-4f31-8a28-39d96f9f0691)









Pesudocode
![image](https://github.com/michael-beebe/cs5379-parallel-processing/assets/113784916/10b666a1-c16e-4438-8e08-4d751cafe1b3)
 
