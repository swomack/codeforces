#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <array>
#include <vector>
#include <string.h>


using namespace std;


array<int, 1000009> elements;

array<long long, 1000009> greater_left;
array<long long, 1000009> lesser_right;

array<long long, 4000009> segment_tree;


void update(int node_id, int left, int right, int update_pos)
{
	if (left == right)
	{
		segment_tree[node_id] ++;
		return;
	}

	int mid = (left + right) / 2;

	if (update_pos <= mid)
		update(2 * node_id, left, mid, update_pos);
	else
		update(2 * node_id + 1, mid + 1, right, update_pos);

	segment_tree[node_id] = segment_tree[node_id * 2] + segment_tree[node_id * 2 + 1];
}

long long total_query(int node_id, int left, int right, int x, int y)
{
	if (segment_tree[node_id] == 0 || y < left || x > right || x > y)
		return 0;

	if (left == x && right == y)
		return segment_tree[node_id];

	int mid = (left + right) / 2;

	int result = 0;
	
	result += total_query(2 * node_id, left, mid, x, y > mid ? mid : y);
	result += total_query(2 * node_id + 1, mid + 1, right, x > (mid + 1) ? x : (mid + 1), y);

	return result;
}

int main()
{
	int N;
	cin >> N;

	vector<int> sorted_elements(N);

	unordered_map<int, int> actual_pos;
	actual_pos.clear();

	for (int i = 0; i < N; i++)
	{
		cin >> elements[i];
		sorted_elements[i] = elements[i];
		actual_pos[elements[i]] = i;
	}

	sort(begin(sorted_elements), end(sorted_elements));

	for (int i = 0; i < N; i++)
	{
		elements[actual_pos[sorted_elements[i]]] = (i + 1);
	}


	memset(&segment_tree[0], 0, sizeof(segment_tree));


	for (int i = 1; i < N; i++)
	{
		update(1, 1, 1000000, elements[i - 1]);
		greater_left[i] = total_query(1, 1, 1000000, elements[i] + 1, 1000000);
	}


	memset(&segment_tree[0], 0, sizeof(segment_tree));

	for (int i = N - 2; i >= 0; i--)
	{
		update(1, 1, 1000000, elements[i + 1]);
		lesser_right[i] = total_query(1, 1, 1000000, 1, elements[i] - 1);
	}

	long long result = 0;

	for (int i = 0; i < N; i++)
		result += (greater_left[i] * lesser_right[i]);

	cout << result << endl;

	return 0;
}

