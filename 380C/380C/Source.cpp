#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)

class segment_tree
{
private:

	// each node of the segment tree will hold, result for this range, total left open in this range and total right open in this range
	struct node
	{
		int result;
		int left_unbalanced;
		int right_unbalanced;

		node() : result(0), left_unbalanced(0), right_unbalanced(0)
		{
		}
	};

	vector<node> seg_tree_nodes;
	int total_leafs;

public:
	segment_tree()
	{
		seg_tree_nodes.clear();
		total_leafs = 0;
	}


	void build_segment_tree(string&& s)
	{
		int level = ceil(log2(s.length()));

		total_leafs = pow(2, level);

		int total_nodes = 2 * total_leafs - 1;
		seg_tree_nodes.resize(total_nodes);

		construct_segment_tree(1, 1, total_leafs, s);

	}

	int query(int x, int y)
	{
		return get_query(1, 1, total_leafs, x, y).result;
	}

private:

	node get_query(int node_id, int left, int right, int x, int y)
	{
		if (left == x && right == y)
			return seg_tree_nodes[node_id - 1];

		int mid = (left + right) / 2;

		node result;

		if (x >= left && y <= mid)
			result = get_query(2 * node_id, left, mid, x, y);
		else if (x > mid && y <= right)
			result = get_query(2 * node_id + 1, mid + 1, right, x, y);
		else
		{
			node result1 = get_query(2 * node_id, left, mid, x, mid);
			node result2 = get_query(2 * node_id + 1, mid + 1, right, mid + 1, y);

			result.result = result1.result + result2.result;
			result.result += MIN(result1.left_unbalanced, result2.right_unbalanced);

			result.left_unbalanced = result2.left_unbalanced + MAX(0, result1.left_unbalanced - result2.right_unbalanced);
			result.right_unbalanced = result1.right_unbalanced + MAX(0, result2.right_unbalanced - result1.left_unbalanced);
		}

		return result;
	}

	void construct_segment_tree(int node_id, int left, int right, string& s)
	{
		if (left == right)
		{
			if (left <= s.length())
			{
				seg_tree_nodes[node_id - 1].result = 0;

				if (s[left - 1] == '(')
					seg_tree_nodes[node_id - 1].left_unbalanced = 1;
				else
					seg_tree_nodes[node_id - 1].right_unbalanced = 1;
			}

			return;
		}

		int mid = (left + right) / 2;

		construct_segment_tree(2 * node_id, left, mid, s);
		construct_segment_tree(2 * node_id + 1, mid + 1, right, s);

		// the result for this range will be, left_res + right_res + how many new balaced created from left open of left child and right open of right child
		seg_tree_nodes[node_id - 1].result = seg_tree_nodes[2 * node_id - 1].result +
			seg_tree_nodes[2 * node_id].result + MIN(seg_tree_nodes[2 * node_id - 1].left_unbalanced, seg_tree_nodes[2 * node_id].right_unbalanced);

		seg_tree_nodes[node_id - 1].left_unbalanced = seg_tree_nodes[2 * node_id].left_unbalanced + MAX(0, seg_tree_nodes[2 * node_id - 1].left_unbalanced - seg_tree_nodes[2 * node_id].right_unbalanced);
		seg_tree_nodes[node_id - 1].right_unbalanced = seg_tree_nodes[2 * node_id - 1].right_unbalanced + MAX(0,  seg_tree_nodes[2 * node_id].right_unbalanced - seg_tree_nodes[2 * node_id - 1].left_unbalanced);
	}
};


int main()
{
	string s;
	cin >> s;


	segment_tree seg_tree;
	seg_tree.build_segment_tree(move(s));


	int T;

	cin >> T;

	while (T--)
	{
		int x, y;

		cin >> x >> y;

		cout << seg_tree.query(x, y) * 2 << endl;
	}

	return 0;
}