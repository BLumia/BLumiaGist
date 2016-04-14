#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

char buffer[1061], scaleBuffer[1061];
long long st, ed, num;
char numStr[1061];

int main()
{
    while(gets(buffer)) {
        gets(scaleBuffer);
        sscanf(buffer,"%lld",&num);
        sscanf(scaleBuffer,"%lld %lld",&st,&ed);
        sprintf(numStr,"%lld",num);
        if(strcmp(numStr, buffer) == 0) {
            if(st <= num && num <= ed) puts("YES");
            else puts("NO");
        } else {
            puts("NO");
        }
    }
}
