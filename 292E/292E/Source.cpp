#include <iostream>

using namespace std;

struct node
{
	int left, right;
	int value;

	node()
	{
		left = right = value = -1;
	}
};


int arrB[100009];
int arrA[100009];
node seg_tree[400009];

void make_tree(int node, int left, int right)
{
	if (left == right)
	{
		seg_tree[node].value = arrB[left];
	}

	int mid = (left + right) / 2;

	make_tree(2 * node, left, mid);
	make_tree(2 * node + 1, mid + 1, right);
}

void update(int node, int left, int right, int x, int y, int arra_pos)
{
	if (y < left || x > right)
		return;

	if (left == x && right == y)
	{
		seg_tree[node].left = arra_pos;
		seg_tree[node].right = arra_pos + (y - x);
		return;
	}

	int mid = (left + right) / 2;

	update(2 * node, left, mid, x, y < mid ? y : mid, arra_pos);
	update(2 * node + 1, mid + 1, right, x > mid + 1 ? x : mid + 1, y, arra_pos);
}


int query_value(int node, int left, int right, int pos, int inherited_pos)
{
	if (left == right)
	{
		if (inherited_pos == -1)
			return seg_tree[node].value;


	}
}

int main()
{
	int N, M;
	cin >> N >> M;

	for (int i = 1; i <= N; i++)
		cin >> arrA[i];

	for (int i = 1; i <= N; i++)
		cin >> arrB[i];

	return 0;
}