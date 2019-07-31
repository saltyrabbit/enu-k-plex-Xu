#define _CRT_SECURE_NO_WARNINGS
#define range_of_V 1000000

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set> 
#include <map>
#include <iterator>
using namespace std;

map<int, int> G[range_of_V];
int degree[range_of_V];
int k;
int jmp2 = 0;
set<int> EMPTY1, EMPTY2, V;
int num_V, num_E;

void ini();
void FaPlexen_Rec(set<int>& P, set<int>& Cand, set<int>& Excl);
void update(set<int>& P, set<int>& Cand, set<int>& Excl, set<int>& newCand, set<int>& newExcl);
int check_k_plex(set<int>& S);
void emit(set<int>& P);
void level_down(set<int>& newP, set<int>& newCand, set<int>& newExcl);
int degree_in_subgraph(int v, set<int>& S);

int main()
{
	ini();
	FaPlexen_Rec(EMPTY1, V, EMPTY2);
}

void ini()
{
	printf("输入点数和边数（点序号从1开始）\n");
	scanf("%d%d", &num_V, &num_E);
	printf("输入k\n");
	scanf("%d", &k);
	for (int i = 1; i <= num_V; i++)
		V.insert(i);
	printf("输入边的两端顶点序号（序号从1开始）\n");
	for (int i = 1; i <= num_E; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		G[a][b] = G[b][a] = 1;
	}
}
void FaPlexen_Rec(set<int>& P, set<int>& Cand, set<int>& Excl)
{
	if (Cand.size() == 0 && Excl.size() == 0)
	{
		emit(P);
		return;
	}//line 4

	int up = 0;
	int updegree = range_of_V;
	set<int> S;
	set_union(P.begin(), P.end(), Cand.begin(), Cand.end(), inserter(S, S.begin()));
	set<int>::iterator iter = S.begin();
	while (iter != S.end())
	{
		int degree = degree_in_subgraph(*iter, S);
		if (degree < updegree)
		{
			up = *iter;
			updegree = degree;
		}
		iter++;
	}//line 5

	if (updegree >= S.size() - k)
	{
		int flag = 0;
		iter = Excl.begin();
		while (iter != Excl.end())
		{
			S.insert(*iter);
			if (check_k_plex(S))
			{
				flag = 1;
				break;
			}
			S.erase(*iter);
			iter++;
		}
		if (flag == 0)
		{
			emit(S);
		}
	}//line 8
	else if (P.find(up) != P.end())
	{
		set<int> Doing;
		iter = Cand.begin();
		while (iter != Cand.end())
		{
			if (G[up].find(*iter)==G[up].end())
				Doing.insert(*iter);
			iter++;
		}//line 10
		int p = k;
		iter = P.begin();
		while (iter != P.end())
		{
			if (G[up].find(*iter)==G[up].end())
				p--;
			iter++;
		}//line 11
		set<int> newP(P), newCand(Cand), newExcl(Excl);
		iter = Doing.begin();
		for (int i = 1; i <= p + 1; i++)
		{
			if (i == 1)
			{
				newExcl.insert(*iter);
				newCand.erase(*iter);
				level_down(newP, newCand, newExcl);
				newExcl.erase(*iter);
			}
			else if (i > 1 && i <= p)
			{
				newP.insert(*iter);
				iter++;
				newExcl.insert(*iter);
				newCand.erase(*iter);
				level_down(newP, newCand, newExcl);
				newExcl.erase(*iter);
			}
			else
			{
				newP.insert(*iter);
				iter++;
				while (iter != Doing.end())
				{
					newCand.erase(*iter);
					iter++;
				}
				level_down(newP, newCand, newExcl);
			}
		}//line 16
	}
	else
	{
		set<int> new2Cand, new2Excl;
		P.insert(up);
		Cand.erase(up);
		update(P, Cand, Excl, new2Cand, new2Excl);
		FaPlexen_Rec(P, new2Cand, new2Excl);
		P.erase(up);
		Excl.insert(up);
		FaPlexen_Rec(P, Cand, Excl);
		Cand.insert(up);
		Excl.erase(up);
	}

}
void level_down(set<int>& newP, set<int>& newCand, set<int>& newExcl)
{
	if (check_k_plex(newP))
	{
		jmp2 = 1;
		set<int> newnewCand, newnewExcl;
		update(newP, newCand, newExcl, newnewCand, newnewExcl);
		jmp2 = 0;
		FaPlexen_Rec(newP, newnewCand, newnewExcl);
	}
}
void update(set<int>& P, set<int>& Cand, set<int>& Excl, set<int>& newCand, set<int>& newExcl)
{
	set<int>::iterator iter1, iter2;
	if (jmp2 == 0)
	{
		iter1 = P.begin();
		while (iter1 != P.end())
		{
			degree[*iter1] = degree_in_subgraph(*iter1, P);
			iter1++;
		}
	}
	else
	{
		jmp2 = 0;
	}

	iter2 = Cand.begin();
	while (iter2 != Cand.end())
	{
		int flag = 1;
		int cnt = 0;
		iter1 = P.begin();
		while (iter1 != P.end())
		{
			if (G[*iter1].find(*iter2)!= G[*iter1].end())
			{
				cnt++;
			}
			else
			{
				if (P.size() - degree[*iter1] >= k)
				{
					flag = 0;
					break;
				}
			}
			iter1++;
		}
		if (P.size() + 1 - cnt > k)
			flag = 0;
		if (flag)
			newCand.insert(*iter2);
		iter2++;
	}

	iter2 = Excl.begin();
	while (iter2 != Excl.end())
	{
		int flag = 1;
		int cnt = 0;
		iter1 = P.begin();
		while (iter1 != P.end())
		{
			if (G[*iter1].find(*iter2) != G[*iter1].end())
			{
				cnt++;
			}
			else
			{
				if (P.size() - degree[*iter1] >= k)
				{
					flag = 0;
					break;
				}
			}
			iter1++;
		}
		if (P.size() + 1 - cnt > k)
			flag = 0;
		if (flag)
			newExcl.insert(*iter2);
		iter2++;
	}
}

int degree_in_subgraph(int v, set<int>& S)
{
	map<int, int>::iterator iter = G[v].begin();
	int cnt = 0;
	while (iter != G[v].end())
	{
		if (S.find(iter->first) != S.end())
			cnt++;
		iter++;
	}
	return cnt;

}
int check_k_plex(set<int>& S)
{
	int flag = 1;
	set<int>::iterator iter = S.begin();
	while (iter != S.end())
	{
		degree[*iter]=degree_in_subgraph(*iter, S);
		if (degree[*iter] < S.size() - k)
		{
			flag = 0;
			break;
		}
		iter++;
	}
	return flag;

}
void emit(set<int>& P)
{
	set<int>::iterator iter = P.begin();
	while (iter != P.end())
	{
		cout << *iter << ", ";
		iter++;
	}
	cout << endl;
}