#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cmath>


using namespace std;

void update_seg_tree(vector<int>& seg_tree, int total_leaf)
{
	int level = log2(total_leaf);

	int range_end = seg_tree.size() - total_leaf;

	int process_node = total_leaf >> 1;

	int range_start = range_end - process_node + 1;

	for (int i = 0; i < level; i++)
	{
		if (i % 2) // xors
		{
			for (int j = range_start; j <= range_end; j++)
			{
				seg_tree[j - 1] = seg_tree[2 * j - 1] ^ seg_tree[2 * j];
			}
		}
		else // or
		{
			for (int j = range_start; j <= range_end; j++)
			{
				seg_tree[j - 1] = seg_tree[2 * j - 1] | seg_tree[2 * j];
			}
		}

		process_node >>= 1;

		range_end = range_start - 1;
		range_start = range_end - process_node + 1;
	}
}

void update_parents(vector<int>& seg_tree, int total_leaf, int pos)
{
	int level = log2(total_leaf);

	for (int i = 0; i < level; i++)
	{
		pos >>= 1;
		if (i % 2) // xors
		{
			seg_tree[pos - 1] = seg_tree[2 * pos - 1] ^ seg_tree[2 * pos];
		}
		else // or
		{
			seg_tree[pos - 1] = seg_tree[2 * pos - 1] | seg_tree[2 * pos];
		}
	}
}

int main()
{
	int M, N;

	scanf("%d %d", &M, &N);

	int t_l = 1;

	while (M--)
		t_l <<= 1;


	// total node in the segment tree
	int t_i_n = t_l - 1;
	int t_n = t_l + t_i_n;
	vector<int> s_tree(t_n);

	// build the segment tree
	int start = t_i_n;

	for (int i = 0; i < t_l; i++)
	{
		scanf("%d", &s_tree[start + i]);
	}


	// update the tree
	update_seg_tree(s_tree, t_l);


	// N query, N update
	for (int i = 0; i < N; i++)
	{
		int p, b;

		scanf("%d %d", &p, &b);

		int a_i = start + p;

		s_tree[a_i - 1] = b;

		// update the parents
		update_parents(s_tree, t_l, a_i);

		printf("%d\n", s_tree[0]);
		
	}



	return 0;
}