#include <bits/stdc++.h>
#include <pthread.h>
using namespace std;

typedef struct Node Node;

struct Node {
  int val;
  Node *lt, *rt, *parent;
};
int n, a[100000];
vector<Node*> leaf;
queue<Node*> templeaf;

Node *insert(Node *root, int data){
  if(root == NULL) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->lt = temp->rt = temp->parent = NULL;
    temp->val = data;
    return temp;
  }
  else {
    if(root->val > data) {
      root->lt = insert(root->lt, data);
      root->lt->parent = root;
    }
    else {
      root->rt = insert(root->rt, data);
      root->rt->parent = root;
    }
    return root;
  }
}

void insertall() {
  int counter = 1;
  while(counter <= n) {
    Node *tmp = templeaf.front();
    templeaf.pop();
    tmp->lt = (Node*)malloc(sizeof(Node));
    tmp->lt->lt = tmp->lt->rt = NULL;
    tmp->lt->parent = tmp;
    tmp->lt->val = a[counter++];
    templeaf.push(tmp->lt);
    tmp->rt = (Node*)malloc(sizeof(Node));
    tmp->rt->lt = tmp->rt->rt = NULL;
    tmp->rt->parent = tmp;
    tmp->rt->val = a[counter++];
    templeaf.push(tmp->rt);
  }
}

void *execute(void *arg) {
  Node* root = (Node *)arg;
  while(root->parent != NULL){
    root->val = 0; 
    root = root->parent;
  }
  pthread_exit(NULL);
}

void postordertraversal(Node *root) {
  if(root != NULL && root->lt == NULL && root->rt == NULL)
    leaf.push_back(root);
  if(root != NULL) {
    postordertraversal(root->lt);
    //    printf("%d ", root->val);
    postordertraversal(root->rt);
  }
}

int main(){
  int counter = 0;
  pthread_t threads[10000];
  cin >> n;
  Node *root = NULL;
  for (int i=0; i<n; i++) {
    cin >> a[i];
  }
  root = insert(root, a[0]);
  templeaf.push(root);
  insertall();
  postordertraversal(root);
  Node *tmp = root;
  int rc;
  for (vector<Node*>::iterator iter = leaf.begin()
	 ; iter != leaf.end(); iter++) {
    rc = pthread_create(&threads[counter++], NULL, execute, (void *)(*iter));
  }
  while(counter--){
    pthread_join(threads[counter], NULL);
  }
  return 0;
}
