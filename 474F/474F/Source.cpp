#include <iostream>
#include <array>
#include <cmath>

using namespace std;



struct node
{
	int gcd, min, min_count;
};


array<node, 400009> segment_tree;

int gcd(int a, int b)
{
	if (a == 0)
		return b;

	return gcd(b % a, a);
}

void update(int node_id, int left, int right, int value, int update_pos)
{
	if (left == right && left == update_pos)
	{
		segment_tree[node_id].min = value;
		segment_tree[node_id].min_count = 1;
		segment_tree[node_id].gcd = value;
		return;
	}

	int mid = (left + right) / 2;

	if (update_pos <= mid)
		update(2 * node_id, left, mid, value, update_pos);
	else
		update(2 * node_id + 1, mid + 1, right, value, update_pos);


	if ((segment_tree[2 * node_id].min < segment_tree[2 * node_id + 1].min && segment_tree[2 * node_id].min > 0) || (segment_tree[2 * node_id + 1].min == 0))
	{
		segment_tree[node_id].min = segment_tree[2 * node_id].min;
		segment_tree[node_id].min_count = segment_tree[2 * node_id].min_count;
	}
	else if ((segment_tree[2 * node_id + 1].min < segment_tree[2 * node_id].min && segment_tree[2 * node_id + 1].min > 0) || (segment_tree[2 * node_id].min == 0))
	{
		segment_tree[node_id].min = segment_tree[2 * node_id + 1].min;
		segment_tree[node_id].min_count = segment_tree[2 * node_id + 1].min_count;
	}
	else
	{
		segment_tree[node_id].min = segment_tree[2 * node_id].min;
		segment_tree[node_id].min_count = segment_tree[2 * node_id].min_count + segment_tree[2 * node_id + 1].min_count;
	}

	segment_tree[node_id].gcd = gcd(segment_tree[2 * node_id].gcd, segment_tree[2 * node_id + 1].gcd);
}

pair<pair<int, int>, int> get_min_count_in_range(int node_id, int left, int right, int x, int y)
{
	if (segment_tree[node_id].min <= 0 || x > y || y < left || x > right)
		return make_pair(make_pair(0, 0), 0);


	if (left == x && right == y)
	{
		return make_pair(make_pair(segment_tree[node_id].min, segment_tree[node_id].min_count), segment_tree[node_id].gcd);
	}

	int mid = (left + right) / 2;

	auto res_left = get_min_count_in_range(2 * node_id, left, mid, x, y <= mid ? y : mid);
	auto res_right = get_min_count_in_range(2 * node_id + 1, mid + 1, right, x >= (mid + 1) ? x : (mid + 1), y);

	pair<pair<int, int>, int> res;
	
	if ((res_left.first.first < res_right.first.first && res_left.first.first > 0) || (res_right.first.first == 0))
	{
		res.first = make_pair(res_left.first.first, res_left.first.second);
	}
	else if ((res_right.first.first < res_left.first.first && res_right.first.first > 0) || (res_left.first.first == 0))
	{
		res.first = make_pair(res_right.first.first, res_right.first.second);
	}
	else
	{
		res.first = make_pair(res_left.first.first, res_right.first.second + res_left.first.second);
	}

	res.second = gcd(res_left.second, res_right.second);

	return res;
}


int main()
{
	int N;
	cin >> N;

	int total_leafs = pow(2, ceil(log2(N)));

	for (int i = 1; i <= N; i++)
	{
		int stn;

		cin >> stn;

		update(1, 1, total_leafs, stn, i);
	}

	int T;

	cin >> T;

	while (T--)
	{
		int r1, r2;

		cin >> r1 >> r2;

		auto winner = get_min_count_in_range(1, 1, total_leafs, r1, r2);

		if (winner.first.first > 0 && winner.first.first == winner.second)
		{
			cout << (r2 - r1 + 1) - winner.first.second << endl;
		}
		else
		{
			cout << (r2 - r1 + 1) << endl;
		}
	}


	return 0;
}