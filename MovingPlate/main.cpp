#include <iostream>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <queue>
#include <string>

using namespace std;

const int PLATE_COUNT = 9;

struct Neighbour
{
	int up;
	int right;
	int down;
	int left;
};

// 邻接表，-1表示没有邻接
Neighbour adjacentTable[PLATE_COUNT] = {
	{ -1, 1, 3, -1 },	// 0
	{ -1, 2, 4, 0 },		// 1
	{ -1, -1, 5, 1 },	// 2
	{ 0, 4, 6, -1 },		// 3
	{ 1, 5, 7, 3 },		// 4
	{ 2, -1, 8, 4 },		// 5
	{ 3, 7, -1, -1 },	// 6
	{ 4, 8, -1, 6 },		// 7
	{ 5, -1, -1, 7 },	// 8
};

struct PlateState
{
	int plates[PLATE_COUNT];
	int blankIndex;	// 空白板块的序号
	vector<int> steps;	// 移动的步骤

	PlateState()
	{
	}

	PlateState(const PlateState& other)
	{
		memcpy(plates, other.plates, sizeof(int)*PLATE_COUNT);
		blankIndex = other.blankIndex;
		steps = other.steps;
	}

	inline bool operator ==(const PlateState& other) const
	{
		return memcmp(plates, other.plates, sizeof(int) * PLATE_COUNT) == 0;
	}

	inline size_t hashValue() const
	{
		string str;
		for (int i = 0; i < PLATE_COUNT; i++)
			str += (char)(plates[i] + '0');

		return std::hash<string>()(str);
	}
};

namespace std
{
	template<> struct hash<PlateState>
	{
		size_t operator()(const PlateState& x) const
		{
			return x.hashValue();
		}
	};
}

int main(int argc, char* argv[])
{
	PlateState finalState;
	int finalValues[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
	memcpy(finalState.plates, finalValues, sizeof(int)*PLATE_COUNT);
	finalState.blankIndex = 8;

	PlateState startState;
	//int startValues[9] = { 1, 2, 3, 4, 5, 6, 8, 7, 0 };	// no solution
	int startValues[9] = { 8, 1, 7, 5, 3, 6, 2, 4, 0 };
	memcpy(startState.plates, startValues, sizeof(int)*PLATE_COUNT);
	startState.blankIndex = 8;

	bool finished = false;
	int pushCount = 0;
	int popCount = 0;

	queue<PlateState> plateQueue;
	plateQueue.push(startState);

	unordered_set<PlateState> processedStates;
	PlateState frontState;

	while (!plateQueue.empty())
	{
		frontState = plateQueue.front();
		if (frontState == finalState)
		{
			finished = true;
			break;
		}

		plateQueue.pop();
		popCount++;

		// 记录出现过的状态，以免后续有重复
		if (processedStates.find(frontState) != processedStates.end())
			continue;
		else
			processedStates.insert(frontState);
			
		// 探测邻接板块，看下一步有哪些可走的状态
		Neighbour nbr = adjacentTable[frontState.blankIndex];

		// 和上、右、下、左相邻板块分别交换
		int nbrs[4] = { nbr.up, nbr.right, nbr.down, nbr.left };
		for (int i = 0; i < 4; i++)
		{
			int nbrIndex = nbrs[i];
			if (nbrIndex != -1)
			{
				PlateState moveState = frontState;
				moveState.steps.push_back(moveState.plates[nbrIndex]);
				swap(moveState.plates[moveState.blankIndex], moveState.plates[nbrIndex]);
				moveState.blankIndex = nbrIndex;
				plateQueue.push(moveState);
				pushCount++;
			}
		}
	}

	if (finished)
	{
		cout << "Push count: " << pushCount << endl;
		cout << "Pop count: " << popCount << endl;
		cout << "Total steps: " << frontState.steps.size() << endl;

		for (size_t i = 0; i < frontState.steps.size(); i++)
			cout << frontState.steps.at(i) << endl;
	}
	else
	{
		cout << "No solution." << endl;
	}

	system("pause");

	return 0;
}