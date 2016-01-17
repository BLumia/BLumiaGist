//G++
#include <bits/stdc++.h>
#define each(i,n) (int i=0;i<=(n);i++)
using namespace std;

int tree1[5000],tree2[5000];

void insert(int* tree, char data) {
	int cur = 1;
	data -= '0';
	while(tree[cur] != -1) {
		if(tree[cur]<data) {
			cur*=2;
		} else {
			cur=cur*2+1;
		}
	}
	tree[cur] = data;
}

int main() {
	int n,sLen;
	while(~scanf("%d",&n)) {
		if(n==0)break;
		memset(tree1,-1,sizeof(tree1));
		char buffer[61];
		scanf("%s",&buffer);
		sLen = strlen(buffer);	
		for each(i,sLen-1) {
			insert(tree1,buffer[i]);	
		}
	
		for each(i,n-1) {
			scanf("%s",&buffer);
			memset(tree2,-1,sizeof(tree2));
			for each(j,sLen-1) {
				insert(tree2,buffer[j]);
			}
			printf("%s\n",memcmp(tree1,tree2,sizeof(tree1))==0?"YES":"NO");

		}
	}	
}
