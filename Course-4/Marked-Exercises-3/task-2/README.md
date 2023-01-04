# Marked Exercises 3

## Task 2

### Question 1 - The Log-Structured File System was created to address which of the following problems?

- Inefficient reading and writing
- File systems lacked RAID awareness
- Increasing disk sizes
- Frequent inode corruption

**Answer:**

- Inefficient reading and writing
- File systems lacked RAID awareness
- Increasing disk sizes

### Question 2 - Match the following LFS implementation data structures with their descriptions.

- **Segment Usage Table**: shows how much data is on each block.
- **Inode Map**: This shows where each inode on the disk is located. This is written directly into the segment.
- **Segment Summary**: This keeps track of the details for each block in the segment.
- **Inodes**: These carry physical block pointers to files.
