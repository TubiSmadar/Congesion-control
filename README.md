# Congesion-control
> Made by Nerya Bigon and Eitan shenkolovski
* Assignment in networking course  


In this assignment we were requested to study the difference between two
Congestion Control algorithms - “cubic” and “reno”.    
In oreder to do so we've learned to build client and server using C, and decided each time what algorithm is used.  


The main difference between “reno” and “cubic” is the window increase function. While
“reno” uses the traditional linear increase (W=W+1), “cubic” implements a binary search
increase which can reach the available bandwidth much faster than “reno”.  

All of the tests were done on a file size of 1 mb because when we’ve tried larger files it didn’t
work as well.  

Here is a screenshot of wireshark in which we can see very clearly the fact that packets were lost
and retransmitted, the duplicated ACK etc..  

![Untitled picture](https://user-images.githubusercontent.com/66886354/146820871-50cdec9c-cd62-4050-af1b-bc24909822db.jpg)  


### A summary table of all the cases:
Percentage Lost | Cubic avg Time | Reno avg Time
-------------- | -------------- |--------------
0% | 0.033 | 0.031
10% | 0.32 | 0.39
15% | 3.84 | 1.15
20% | 5.55 | 2.95
25% | 6.38 | 43.99
30% | didn’t work | didn’t work  


### Graph to illustrate the differences:
We’ve noticed that if we run the program a couple time in a row we received different result,
and our assumption is that this is due to the variances in the tool been used to loose the
packets (tc).
But nevertheless we can see the differences between the algorithms:
While “reno” uses the traditional linear increase (W=W+1), “cubic” implements a binary
search increase which can reach the available bandwidth much faster than “reno”.
The spike we see in the “reno” is because of sudden major packets loss, and if we’ll take
way more samples then just an average of 5 the graph will be more refine.  

![Untitled picture](https://user-images.githubusercontent.com/66886354/146821456-3954a155-aebb-41ff-8491-e650eb4d12f8.jpg)


