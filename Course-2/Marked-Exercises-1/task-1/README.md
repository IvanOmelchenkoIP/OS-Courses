# Marked Exercises 1

## Task 1 - define the functions of MyMalloc() and MyFree()

### Your malloc() function should:
 
- Accept an argument size_t called noOfBytes indicating the number of bytes requested
- Initialize memory if it hasn’t already been initialized
- Traverse through metadate blocks looking for one to fit the size request

*If a block exactly fits the request…*

- When it is found, indicate the block is no longer free
- Return the starting address of the allocated block

*If a block is too large…*

- Split the block and allocate only what is required

*Otherwise…*

- Print "Sorry. No sufficient memory to allocate.\n"

### Your free() function should:

- Take the pointer to a block of previously allocated memory
- Verify the address passed in is within the address range of our memory array

*If it is…*

- Indicate the block is free
- Merge any free blocks within the memory space

*If not…*

- Print "Please provide a valid allocated pointer \n"