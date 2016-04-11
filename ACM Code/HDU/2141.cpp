#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <set>

#define each(i,n) (int i=1;i<=(n);i++)

using namespace std;

int a[502], b[502], c[502], aa, bb, cc;
long long axb[250061];
bool succ;

void bsearch(int x, int arrSize) {
    int left, right, mid;
    left = 0;
    right = arrSize-1;
    while(left<=right) {
        mid=(left+right)>>1;
        if(axb[mid]>x)
            right=mid-1;
        else if(axb[mid]<x)
            left=mid+1;
        else {
            succ = true;
            return;
        }
    }
    return;
}

int main() {

    int caseNo = 1, cnt, sum;

    while(~scanf("%d%d%d",&aa, &bb, &cc)) {

        printf("Case %d:\n", caseNo);
        caseNo++;

        for each(i,aa) {
            scanf("%d",&a[i-1]);
        }
        for each(i,bb) {
            scanf("%d",&b[i-1]);
        }
        for each(i,cc) {
            scanf("%d",&c[i-1]);
        }

        //speed up
        int ooxx = 0;
        for each(i,aa) {
            for each(j,bb) {
                axb[ooxx] = a[i-1] + b[j-1];
                ooxx++;
            }
        }
        sort(axb,axb+ooxx);
        sort(c,c+cc);

        scanf("%d", &cnt);
        while(cnt--) {
            scanf("%d", &sum);
            succ = false;
            for(int i = 0;i<cc;i++) {
                //if(c[i] > sum) break;
                bsearch(sum - c[i], ooxx); //may change succ val;
                if(succ) {
                    break;
                }
            }
            printf("%s\n", succ?"YES":"NO");
        }

    }

}
