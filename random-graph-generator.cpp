#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <ctime>
using namespace std;
int main()
{
	int n, m;
	cout<< "输入生成的点数、边数"<<endl;
	cin >> n >> m;
	ofstream file("graph.txt", ios::out);
	srand(time(NULL));
	file << n << " " << m << endl;
	while (m)
	{
		int a = rand()%(n-1)+1;
		int b = rand()%(n-1)+1;
		if (a != b)
		{
			file << a << " " << b << endl;
			m--;
		}
	}
}
