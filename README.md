# Congesion-control
> Made by Nerya Bigon and Eitan shenkolovski
* Assignment in networking course  


In this assignment we were requested to study the difference between two
Congestion Control algorithms - “cubic” and “reno”.  

The main difference between “reno” and “cubic” is the window increase function. While
“reno” uses the traditional linear increase (W=W+1), “cubic” implements a binary search
increase which can reach the available bandwidth much faster than “reno”.  

All of the tests were done on a file size of 1 mb because when we’ve tried larger files it didn’t
work as well.  

Here is a screenshot of wireshark that we can see very clearly the fact that packets were lost
and retransmitted, the duplicated ACK etc..  

