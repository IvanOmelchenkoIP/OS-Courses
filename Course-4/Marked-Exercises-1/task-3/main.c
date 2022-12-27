#include<stdio.h>
#include<stdlib.h>

#define SECTOR_NUM 10

int main(int argc, char *argv[])
{
    // YOUR CODE HERE
    int left_num, right_num;
    int left[SECTOR_NUM], right[SECTOR_NUM];
    int sum, head = 10;
    int sectors[SECTOR_NUM];
    printf("Enter disk locations to service: ");
    for (int i = 0; i < SECTOR_NUM; i++) scanf("%d", &sectors[i]);

    for (int i = 0; i < SECTOR_NUM; i++)
    {
      if (sectors[i] < head) left[left_num++] = sectors[i];
      else if (sectors[i] > head) right[right_num++] = sectors[i];
    }

    for (int i = 0; i < left_num; ++i)
    {
        for (int j = i + 1; j < left_num; ++j)
        {
            if (left[i] > left[j])
            {
                int tmp =  left[i];
                left[i] = left[j];
                left[j] = tmp;

            }
        }
      }

      for (int i = 0; i < right_num; ++i)
    {
        for (int j = i + 1; j < right_num; ++j)
        {
            if (right[i] > right[j])
            {
                int tmp =  right[i];
                right[i] = right[j];
                right[j] = tmp;

            }
        }
      }

    for (int i = 0; i < right_num; i++) {
        int cur_sector = right[i];
        printf("Sector %d\n", cur_sector);
        sum += abs(cur_sector - head);
        head = cur_sector;
    }
    for (int i = 0; i < left_num; i++) {
        int cur_sector = right[i];
        printf("Sector %d\n", cur_sector);
        sum += abs(cur_sector - head);
        head = cur_sector;
    }
    printf("Total Disk Head Movement: %d\n", sum);
    return 0;
}
