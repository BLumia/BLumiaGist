#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <set>

#define each(i,n) (int i=1;i<=(n);i++)

using namespace std;

typedef struct seq{
    char dnaseq[61];
    int cnt;
} DNA;

DNA dna[161];

bool cmp(struct seq a, struct seq b) {
    return a.cnt<=b.cnt;
}

int main() {
    int dc;
    int n,m;
    char c;
    scanf("%d",&dc);

    while(dc--) {

    scanf("%d%d ",&n,&m);
    for(int i = 0; i < m; i++) {
        scanf("%s",&dna[i].dnaseq);
        dna[i].cnt = 0;
        for(int j = 0; j < n; j++) {
            for(int k = j+1; k < n; k++) {
                if(dna[i].dnaseq[j]>dna[i].dnaseq[k])
                dna[i].cnt++;
            }
        }
    }
    sort(dna,dna+m,cmp);
    for(int i = 0; i < m; i++) {
        puts(dna[i].dnaseq);
    }

    }
}
