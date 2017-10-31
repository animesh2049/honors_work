#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;
int n, a[100000];
void *execute(void *arg) {
  int myindex = *(int *)arg;
  int tmp;
  while(myindex != 0) {
    tmp = a[myindex];
    myindex /= 2;
  }
}

int main(){
  pthread_t threads[10000];
  cin >> n;
  for (int i=1; i<=n; i++) {
    scanf("%d", &a[i]);
  }
  for (int i=n/2+1; i<=n; i++) {
    pthread_create(&threads[i], NULL, execute, (void *)&i);
  }
  for (int i=n/2+1; i<=n; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
