#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <sstream>
# include <stdio.h>
# include <stdlib.h>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int lowbit(int x)
{
	
	return x  & (-x);
}
int main(int argc, char** argv) {
int n = 2048;
cout<<lowbit(n)<<endl;
}

