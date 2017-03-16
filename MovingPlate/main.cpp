#include <iostream>
#include <memory>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <queue>

using namespace std;

const int PLATE_COUNT = 9;

struct Neighbour
{
	int up;
	int right;
	int down;
	int left;
};

Neighbour adjacentTable[PLATE_COUNT] = {
	{ -1, 1, 3 - 1 },	// 0
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
	int blankIndex;
	vector<int> steps;

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
		return plates[0]*1000 + plates[1]*100 + plates[2]*10+plates[3] + plates[6]*10000;
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
	int startValues[9] = { 1, 2, 3, 4, 5, 6, 8, 7, 0 };
	memcpy(startState.plates, startValues, sizeof(int)*PLATE_COUNT);
	startState.blankIndex = 8;

	bool finished = false;

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

		// 记录出现过的状态，以免后续有重复
		if (processedStates.find(frontState) != processedStates.end())
			continue;
		else
			processedStates.insert(frontState);
			
		Neighbour nbr = adjacentTable[frontState.blankIndex];

		// 空白和上邻板块交换
		if (nbr.up != -1)
		{
			PlateState upMoveState = frontState;
			upMoveState.steps.push_back(upMoveState.plates[nbr.up]);
			swap(upMoveState.plates[upMoveState.blankIndex], upMoveState.plates[nbr.up]);
			upMoveState.blankIndex = nbr.up;
			plateQueue.push(upMoveState);
		}

		// 空白和右邻板块交换
		if (nbr.right != -1)
		{
			PlateState rightMoveState = frontState;
			rightMoveState.steps.push_back(rightMoveState.plates[nbr.right]);
			swap(rightMoveState.plates[rightMoveState.blankIndex], rightMoveState.plates[nbr.right]);
			rightMoveState.blankIndex = nbr.right;
			plateQueue.push(rightMoveState);
		}

		// 空白和下邻板块交换
		if (nbr.down != -1)
		{
			PlateState downMoveState = frontState;
			downMoveState.steps.push_back(downMoveState.plates[nbr.down]);
			swap(downMoveState.plates[downMoveState.blankIndex], downMoveState.plates[nbr.down]);
			downMoveState.blankIndex = nbr.down;
			plateQueue.push(downMoveState);
		}

		// 空白和左邻板块交换
		if (nbr.left != -1)
		{
			PlateState leftMoveState = frontState;
			leftMoveState.steps.push_back(leftMoveState.plates[nbr.left]);
			swap(leftMoveState.plates[leftMoveState.blankIndex], leftMoveState.plates[nbr.left]);
			leftMoveState.blankIndex = nbr.left;
			plateQueue.push(leftMoveState);
		}
	}

	if (finished)
	{
		cout << "Solution: " << endl;
		for (size_t i = 0; i < frontState.steps.size(); i++)
		{
			cout << frontState.steps.at(i) << endl;
		}
	}
	else
	{
		cout << "No solution." << endl;
	}

	return 0;
}