#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <set>

#define each(i,n) (int i=1;i<=(n);i++)

using namespace std;

int main() {

    int dc;

    scanf("%d",&dc);
    while(dc--) {
        int m, n;
        scanf("%d%d",&n,&m);
        if(n%(m+1) != 0)
            puts("first");
        else
            puts("second");
    }

}
