#include<stdio.h>
#include<stdlib.h>

#define SECTOR_NUM 10

int main(int argc, char *argv[])
{
    // YOUR CODE HERE
    int sum, head = 0;
    int sectors[SECTOR_NUM];
    printf("Enter disk locations to service: ");
    scanf("%d %d %d %d %d %d %d %d %d %d",
    &sectors[0], &sectors[1], &sectors[2], &sectors[3], &sectors[4],
    &sectors[5], &sectors[6], &sectors[7], &sectors[8], &sectors[9]);
    for (int i = 0; i < SECTOR_NUM; i++) {
        int cur_sector = sectors[i];
        printf("Sector %d\n", cur_sector);
        sum += abs(cur_sector - head);
        head = cur_sector;
    }
    printf("Total Disk Head Movement: %d\n", sum);
    return 0;
}
