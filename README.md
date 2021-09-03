# EDRR
Implementation of Efficient Dynamic Round Robin (EDRR) Algorithm for CPU scheduling

Implement in accordance to research paper by Muhammad Umar Farooq, Aamna Shakoor, Abu Bakar Siddiqu, University of Engineering and Technology, Lahore. Reasearhg paper avaiable here: https://umar17.github.io/papers/3.pdf

# How it works
This modified Round Robin algorithm uses dynamic time quantum. It applies a practical approach by
not sorting processes result in lower running time. As Round Robin algorithm produces higher
efficiency with time quantum of 80% of processes burst times. The proposed algorithm will
dynamically set the time quantum to be 80% of the highest burst time in the ready queue.

With that, processes with less than or equal to the dynamic time quantum (DTQ) will be assign to the
CPU. While processes with more than the DTQ will be requeue at the end of the ready queue. Once
all processes less than the DTQ have completed, processes with more than that will then be assign
to the CPU. 

# Proof of Code - Paper’s Test Cases 

Example 1:

![image](https://user-images.githubusercontent.com/74093833/131978546-1ac5c6fa-5e63-408b-9236-883987de3780.png)

Example 2:

![image](https://user-images.githubusercontent.com/74093833/131978809-c6598859-b38a-4af8-9cd1-af6653b8bff2.png)


# Proof of Code - Additional Test Cases

Additional 1: Incurring additional context switch

![image](https://user-images.githubusercontent.com/74093833/131979138-1cf8c9c2-a6df-4b9c-bf8a-24849b5009b1.png)

At time = 104, P1 is swap out, and P2 is swap in, incurring 1 additional context switch. Then P1 is
swap back in to complete the remaining burst time of 26, incurring another context switch. 


Additional 2: CPU idling (Very rare in real life)

![image](https://user-images.githubusercontent.com/74093833/131979565-c5a994c3-e94c-4d0a-bd22-71f46825fb52.png)

At time = 5, the next process has not arrived, therefore, there is an idling of 2. Since P2 is already
swapped out before time = 7, P3 does not require a context switch. 

