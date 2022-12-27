#include<stdio.h>
#include<stdlib.h>

#define SECTOR_NUM 10

int main(int argc, char *argv[])
{
  // YOUR CODE HERE
  int sum = 0;
  int head = 10;
  int sectors[SECTOR_NUM];
  printf("Enter disk locations to service: ");
  for (int i = 0; i < SECTOR_NUM; i++) scanf("%d", &sectors[i]);

  for (int i = 0; i < SECTOR_NUM; i++) {
    for (int j = 0; j < SECTOR_NUM - i - 1; j++) {
      if (sectors[j] > sectors[j + 1]) {
        int temp = sectors[j];
        sectors[j] = sectors[j + 1];
        sectors[j + 1] = temp;
      }
    }
  }

  int head_index;
  for (int i = 0; i < SECTOR_NUM; i++) {
    if (head < sectors[i]) {
      head_index = i;
      break;
    }
  }
  for (int i = head_index; i < SECTOR_NUM; i++) {
    int cur_sector = sectors[i];
    printf("Sector %d\n", cur_sector);
    sum += abs(cur_sector - head);
    head = cur_sector;
  }
  for (int i = 0; i < head_index; i++) {
    int cur_sector = sectors[i];
    printf("Sector %d\n", cur_sector);
    sum += abs(cur_sector - head);
    head = cur_sector;
  }
  printf("Total Disk Head Movement: %d\n", sum);
  return 0;
}
