#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>

using namespace std;


vector<int> numbers(1000009);
vector<int> from_left_cumulative(1000009);
vector<int> from_right_cumulative(1000009);



// segment tree to hold how many times all the numbers occurred(cumulative) within it node's range 
class segment_tree
{
	struct node
	{
		long long total_count_in_range;
	};

	vector<node> seg_tree;
	long long total_leafs;

public:

	segment_tree(long long leafs)
	{
		long long level = ceil(log2(leafs));
		total_leafs = pow(2, level);

		seg_tree.resize(2 * total_leafs - 1);
	}

	void update_tree(long long update_position)
	{
		update(1, 1, total_leafs, update_position);
	}

	long long range_total_number_query(long long x, long long y)
	{
		return total_occurence_number_in_range(1, 1, total_leafs, x, y);
	}

private:
	// find the right position to update using left and right
	void update(long long node_id, long long left, long long right, long long update_position)
	{
		if (left < 1 || right > total_leafs || left > right)
			return;

		if (left == right && left == update_position)
		{
			seg_tree[node_id - 1].total_count_in_range++;
			return;
		} 

		// note this node, search left and right node to find 
		// the exact position of this node 
		long long mid = (left + right) / 2;

		if (update_position <= mid)
			update(2 * node_id, left, mid, update_position);
		else
			update(2 * node_id + 1, mid + 1, right, update_position);

		// update this node value
		seg_tree[node_id - 1].total_count_in_range = seg_tree[2 * node_id - 1].total_count_in_range + seg_tree[2 * node_id].total_count_in_range;
	}

	long long total_occurence_number_in_range(long long node_id, long long left, long long right, long long x, long long y)
	{
		if (x < 1 || y > total_leafs || x > y)
			return 0;

		if (left < 1 || right > total_leafs || left > right)
			return 0;

		if (x < left && y < left)
			return 0;

		if (x > right && y > right)
			return 0;

		if (seg_tree[node_id - 1].total_count_in_range == 0)
			return 0;

		if (left == x && right == y)
		{
			return seg_tree[node_id - 1].total_count_in_range;
		}

		long long mid = (left + right) / 2;

		long long result = total_occurence_number_in_range(2 * node_id, left, mid, x, mid > y ? y : mid);
		result += total_occurence_number_in_range(2 * node_id + 1, mid + 1, right, x > (mid + 1) ? x : (mid + 1), y);

		return result;
	}
};


int main()
{
	long long n;

	cin >> n;

	for (long long i = 0; i < n; i++)
		cin >> numbers[i];

	unordered_map<int, int> occurence_count;

	for (long long i = 0; i < n; i++)
	{
		occurence_count[numbers[i]] ++;
		from_left_cumulative[i] = occurence_count[numbers[i]];
	}

	occurence_count.clear();

	for (long long i = n - 1; i >= 0; i--)
	{
		occurence_count[numbers[i]] ++;
		from_right_cumulative[i] = occurence_count[numbers[i]];
	}


	segment_tree seg_tree(n + 10);	// n is the highest possible cumulative value (no number can occur more than n times)
								// we need to track how many times 1...n occurred in a range on right_cululative_count


	long long result = 0;

	for (long long i = n - 1; i >= 1; i--)
	{
		seg_tree.update_tree(from_right_cumulative[i]);

		// we just need to count how many count are there from 1 to less than left occurence count 
		result += seg_tree.range_total_number_query(1, from_left_cumulative[i - 1] - 1);
	}

	cout << result << endl;

	return 0;
}