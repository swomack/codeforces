#include <iostream>

using namespace std;

struct node
{
	int left;
	int value;
	int version;

	node()
	{
		left = value = version = -1;
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
		return;
	}

	int mid = (left + right) / 2;

	make_tree(2 * node, left, mid);
	make_tree(2 * node + 1, mid + 1, right);
}

void update(int node, int left, int right, int x, int y, int arra_pos, int version)
{
	if (y < left || x > right)
		return;

	if (left == x && right == y)
	{
		seg_tree[node].left = arra_pos;
		seg_tree[node].version = version;
		return;
	}

	int mid = (left + right) / 2;

	update(2 * node, left, mid, x, y < mid ? y : mid, arra_pos, version);
	update(2 * node + 1, mid + 1, right, x > mid + 1 ? x : mid + 1, y, arra_pos + (mid - x + 1), version);
}


int query_value(int node, int left, int right, int pos, int inherited_pos, int version)
{
	if (left == right)
	{
		if (inherited_pos == -1 && seg_tree[node].left == -1)
			return seg_tree[node].value;
		else if (seg_tree[node].version > version)
			return arrA[seg_tree[node].left];
		else
			return arrA[inherited_pos];
	}

	if (seg_tree[node].left != -1 && seg_tree[node].version > version)
	{
		inherited_pos = seg_tree[node].left + (pos - left);
		version = seg_tree[node].version;
	}

	int mid = (left + right) / 2;

	int result;

	if (pos <= mid)
		result = query_value(2 * node, left, mid, pos, inherited_pos, version);
	else
		result = query_value(2 * node + 1, mid + 1, right, pos, inherited_pos, version);

	return result;
}

int main()
{
	int N, M;
	cin >> N >> M;

	for (int i = 1; i <= N; i++)
		cin >> arrA[i];

	for (int i = 1; i <= N; i++)
		cin >> arrB[i];

	make_tree(1, 1, N);

	int version = 1;

	while (M--)
	{
		int q_type;
		cin >> q_type;

		switch (q_type)
		{
			case 1:
			{
				int s_a, s_b, len;
				cin >> s_a >> s_b >> len;
				update(1, 1, N, s_b, s_b + len - 1, s_a, version++);
				break;
			}

			case 2:
			{
				int pos;
				cin >> pos;
				cout << query_value(1, 1, N, pos, -1, -1) << endl;
				break;
			}
		}
	}

	return 0;
}