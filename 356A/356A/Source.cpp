#include <iostream>
#include <set>

using namespace std;

int track_winner[300009];

int main()
{
	int K, F;

	set<int> alives;

	scanf("%d %d", &K, &F);

	for (int i = 1; i <= K; i++)
		alives.insert(i);

	while (F--)
	{
		int r_s, r_e, alive;

		scanf("%d %d %d", &r_s, &r_e, &alive);

		auto range_s = alives.equal_range(r_s).first;

		auto range_e = alives.equal_range(r_e).second;

		for (auto range = range_s; range != range_e; range++)
		{
			if (*range != alive)
				track_winner[*range] = alive;
		}

		alives.erase(range_s, range_e);	

		alives.insert(alive);
	}

	for (int i = 1; i <= K; i++)
	{
		cout << track_winner[i] << " ";
	}

	return 0;
}