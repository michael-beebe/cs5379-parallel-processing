In my program, load balancing achieved by the following equation in order to distribute the rows equally between the processes:
Part 1: 
Row location = (current iteration * (#processes * 2)) + myRank 
Part 2: 
Row location = ((current iteration+1) * (#processes * 2) -1) - myRank 
Theses equations will distribute the rows as the following (if we assume P=4 and N=32), please note the data in the table is the row number:
 ![image](https://github.com/michael-beebe/cs5379-parallel-processing/assets/113784916/218e733d-2c36-4f31-8a28-39d96f9f0691)









Pesudocode
![image](https://github.com/michael-beebe/cs5379-parallel-processing/assets/113784916/10b666a1-c16e-4438-8e08-4d751cafe1b3)
 



The output 

bandar@LAPTOP-L4I1KEAJ:~/project5$ mpicc -o main main.c -lm
bandar@LAPTOP-L4I1KEAJ:~/project5$ mpirun -np 4 ./main
1.000000 	2.000000 	3.000000 	4.000000 	5.000000 	6.000000 	7.000000 	8.000000 	9.000000 	10.000000 	11.000000 	12.000000 	13.000000 	14.000000 	15.000000 	16.000000 	17.000000 	18.000000 	19.000000 	20.000000 	21.000000 	22.000000 	23.000000 	24.000000 	25.000000 	26.000000 	27.000000 	28.000000 	29.000000 	30.000000 	31.000000 	32.000000 	33.000000 	34.000000 	35.000000 	36.000000 	37.000000 	38.000000 	39.000000 	40.000000 	41.000000 	42.000000 	43.000000 	44.000000 	45.000000 	46.000000 	47.000000 	48.000000 	49.000000 	50.000000 	51.000000 	52.000000 	53.000000 	54.000000 	55.000000 	56.000000 	57.000000 	58.000000 	59.000000 	60.000000 	61.000000 	62.000000 	63.000000 	64.000000 	
Hello from rank 0
part 1 myrank=0  startPos=0 endPos=1
part 2 myrank=0  startPos=7 endPos=8
part 1 myrank=0  startPos=8 endPos=9
part 2 myrank=0  startPos=15 endPos=16
part 1 myrank=0  startPos=16 endPos=17
part 2 myrank=0  startPos=23 endPos=24
part 1 myrank=0  startPos=24 endPos=25
part 2 myrank=0  startPos=31 endPos=32
part
Hello from rank 1
part 1 myrank=1  startPos=1 endPos=2
part 2 myrank=1  startPos=6 endPos=7
part 1 myrank=1  startPos=9 endPos=10
part 2 myrank=1  startPos=14 endPos=15
part 1 myrank=1  startPos=17 endPos=18
part 2 myrank=1  startPos=22 endPos=23
part 1 myrank=1  startPos=25 endPos=26
part 2 myrank=1  startPos=30 endPos=31
part 1 myrank=1  startPos=33 endPos=34
part 2 myrank=1  startPos=38 endPos=39
part 1 myrank=1  startPos=41 endPos=42
part 2 myrank=1  startPos=46 endPos=47
part 1 myrank=1  startPos=49 endPos=50
part 2 myrank=1  startPos=54 endPos=55
part 1 myrank=1  startPos=57 endPos=58
part 2 myrank=1  startPos=62 endPos=63

Hello from rank 2
part 1 myrank=2  startPos=2 endPos=3
part 2 myrank=2  startPos=5 endPos=6
part 1 myrank=2  startPos=10 endPos=11
part 2 myrank=2  startPos=13 endPos=14
part 1 myrank=2  startPos=18 endPos=19
part 2 myrank=2  startPos=21 endPos=22
part 1 myrank=2  startPos=26 endPos=27
part 2 myrank=2  startPos=29 endPos=30
part 1 myrank=2  startPos=34 endPos=35
part 2 myrank=2  startPos=37 endPos=38
part 1 myrank=2  startPos=42 endPos=43
part 2 myrank=2  startPos=45 endPos=46
part 1 myrank=2  startPos=50 endPos=51
part 2 myrank=2  startPos=53 endPos=54
part 1 myrank=2  startPos=58 endPos=59
part 2 myrank=2  startPos=61 endPos=62
 1 myrank=0  startPos=32 endPos=33
part 2 myrank=0  startPos=39 endPos=40
part 1 myrank=0  startPos=40 endPos=41
part 2 myrank=0  startPos=47 endPos=48
part 1 myrank=0  startPos=48 endPos=49
part 2 myrank=0  startPos=55 endPos=56
part 1 myrank=0  startPos=56 endPos=57
part 2 myrank=0  startPos=63 endPos=64

Hello from rank 3
part 1 myrank=3  startPos=3 endPos=4
part 2 myrank=3  startPos=4 endPos=5
part 1 myrank=3  startPos=11 endPos=12
part 2 myrank=3  startPos=12 endPos=13
part 1 myrank=3  startPos=19 endPos=20
part 2 myrank=3  startPos=20 endPos=21
part 1 myrank=3  startPos=27 endPos=28
part 2 myrank=3  startPos=28 endPos=29
part 1 myrank=3  startPos=35 endPos=36
part 2 myrank=3  startPos=36 endPos=37
part 1 myrank=3  startPos=43 endPos=44
part 2 myrank=3  startPos=44 endPos=45
part 1 myrank=3  startPos=51 endPos=52
part 2 myrank=3  startPos=52 endPos=53
part 1 myrank=3  startPos=59 endPos=60
part 2 myrank=3  startPos=60 endPos=61
-155536.000000 	-157487.000000 	-159371.000000 	-161186.000000 	-162900.000000 	-164577.000000 	-166181.000000 	-167706.000000 	-168894.000000 	-170257.000000 	-171533.000000 	-172712.000000 	-172926.000000 	-173903.000000 	-174771.000000 	-175516.000000 	-174084.000000 	-174571.000000 	-174919.000000 	-175110.000000 	-171136.000000 	-170997.000000 	-170681.000000 	-170166.000000 	-162530.000000 	-161597.000000 	-160441.000000 	-159036.000000 	-146394.000000 	-144467.000000 	-142263.000000 	-139752.000000 	-120536.000000 	-117383.000000 	-113891.000000 	-110026.000000 	-82444.000000 	-77801.000000 	-72749.000000 	-67250.000000 	-29286.000000 	-22857.000000 	-15941.000000 	-8496.000000 	42090.000000 	50633.000000 	59749.000000 	69484.000000 135156.000000 	146173.000000 	157857.000000 	170258.000000 	253704.000000 	267587.000000 	282239.000000 	297714.000000 	401846.000000 	419019.000000437071.000000 	456060.000000 	584014.000000 	604933.000000 	626849.000000 	649824.000000 	Khalids-MacBook-Air:HW5 kmursi$ 
