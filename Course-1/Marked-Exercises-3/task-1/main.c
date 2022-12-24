#include <stdio.h>
#include <stdlib.h>

int main(void) {

  // YOUR CODE HERE!
  printf("Enter a value of N [1-10]: ");
  int N;
  scanf("%d", &N);

  if (N > 10 || N < 0) {
    printf("Error in input\n");
    exit(1);
  }

  int *arr = (int *)malloc(N * sizeof(N));
  if (arr == NULL) {
    printf("Memory not allocated\n");
    exit(1);
  }
  int sum = 0;
  for (int i = 0; i < N; i++) {
    printf("Enter #%d: ", (i + 1));
    scanf("%d", &arr[i]);
    sum += arr[i];
  }
  printf("Sum: %d\n", sum);
  free(arr);
  return 0;
}
