# Marked Exercises 1

## Task 2

**Write a program that implements the SCAN disk scheduling algorithm. This program should:**

- Prompt the user to enter   10  disk locations to service   (0−199) 
- The disk head should start on sector   10  and move toward sector   0 
- Sort the requests from smallest to largest
- Find the current location of the disk head and move toward sector   0 , servicing requests along the way
- Switch direction at   0  and move toward higher requests, servicing until all are finished
- Calculate the total movement of the disk head
- Print the requests in the order they’re serviced
- Print the total movement of the disk head