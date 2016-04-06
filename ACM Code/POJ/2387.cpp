/*
 * 最短路水题
 * 好久不做了都没手感了...写完RE，闹了半天把T和N搞混了..
 * */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cstring>

using namespace std;

int graph[1061][1061];
int dist[1061];
int used[1061];

int main() {

	int T, N;
	int s, e, d;
	while(~scanf("%d%d",&T,&N)) {

		for(int i = 1;i<=N;i++) {
			for(int j = 1;j<=N;j++) {
				graph[i][j] = 0x3f3f3f3f;
			}
		}

		for(int i=0;i<T;i++) {
			scanf("%d%d%d",&s,&e,&d);
			graph[s][e] = graph[e][s] = graph[e][s] > d ?
										d :
										graph[e][s];
		}

		memset(dist,0x3f3f3f3f,sizeof(dist));
		memset(used,0,sizeof(used));
		int curPos = 1, nextPos;
		int curMin;
		dist[curPos] = 0;
		for(int no=1;no<=N;no++) {
			curMin = 0x3f3f3f3f;
			for(int i=1;i<=N;i++) {
				if (curPos==i || used[i]==1) continue;
				dist[i] = dist[curPos] + graph[i][curPos] < dist[i] ?
						  dist[curPos] + graph[i][curPos] :
						  dist[i];
				if (curMin > dist[i]) {
					curMin = dist[i];
					nextPos = i;
				}
			}
			used[curPos] = 1;
			curPos = nextPos;
		}
		printf("%d\n",dist[N]);
	}

	return 0;

}
