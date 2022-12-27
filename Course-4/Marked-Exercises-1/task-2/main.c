#include<stdio.h>
#include<stdlib.h>

#define SECTOR_NUM 10

int main(int argc, char *argv[])
{
    // YOUR CODE HERE
    int sum, head = 10;
    int sectors[SECTOR_NUM];
    printf("Enter disk locations to service: ");
    for (int i = 0; i < SECTOR_NUM; i++) scanf("%d", &sectors[i]);
    for (int i = 0; i < SECTOR_NUM; ++i)
    {
        for (int j = i + 1; j < SECTOR_NUM; ++j)
        {
            if (sectors[i] > sectors[j])
            {
                int tmp =  sectors[i];
                sectors[i] = sectors[j];
                sectors[j] = tmp;

            }
        }
    }
    for (int i = 0; i < SECTOR_NUM; i++) {
        int cur_sector = sectors[i];
        printf("Sector %d\n", cur_sector);
        sum += abs(cur_sector - head);
        head = cur_sector;
    }
    return 0;
}
