#include<stdio.h>
#include<stdlib.h>

#define SECTOR_NUM 10

int main(int argc, char *argv[])
{
  // YOUR CODE HERE
  int sum;
  int head = 10;
  int sectors[SECTOR_NUM];
  printf("Enter disk locations to service: ");
  for (int i = 0; i < SECTOR_NUM; i++) scanf("%d", &sectors[i]);
  for (int i = 0; i < SECTOR_NUM; ++i) {
    for (int j = i + 1; j < SECTOR_NUM; ++j) {
      if (sectors[i] > sectors[j]) {
        int tmp =  sectors[i];
        sectors[i] = sectors[j];
        sectors[j] = tmp;
      }
    }
  }

  int head_index;
  for(int i = 0; i < SECTOR_NUM; i++) {
    if(head < sectors[i]) {
      head_index = i;
      break;
    }
  }

  int i;
  for(i = head_index - 1; i >= 0; i--) {
    int cur_sector = sectors[i];
    printf("Sector %d\n", cur_sector);
    sum += abs(cur_sector - head);
    head = cur_sector;
  }
  sum += abs(sectors[i + 1] - 0);
  head = 0;
  for(i = head_index; i < SECTOR_NUM; i++) {
    int cur_sector = sectors[i];
    printf("Sector %d\n", cur_sector);
    sum += abs(cur_sector - head);
    head = cur_sector;
  }
  printf("Total Disk Head Movement: %d\n", sum);
  return 0;
}
