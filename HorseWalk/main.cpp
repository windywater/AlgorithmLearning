#include <iostream>
#include <stack>

using namespace std;

const int COLUMN = 5;
const int ROW = 5;
const int START_COLUMN = 0;
const int START_ROW = 0;

struct Pos
{
	int column;
	int row;
};

struct Step
{
	Pos pos;
	int tried;
};

Pos trySteps[8] = { { 1, -2 }, { 2, -1 }, { 2, 1 }, { 1, 2 }, { -1, 2 }, { -2, 1 }, { -2, -1 }, { -1, -2 } };

int main(int argc, char* argv[])
{
	char grid[COLUMN][ROW];
	for (int c = 0; c < COLUMN; c++)
	for (int r = 0; r < ROW; r++)
		grid[c][r] = 0;

	stack<Step> steps;

	Pos startPos = { START_COLUMN, START_ROW };
	Step start = { { startPos.column, startPos.row }, -1 };
	grid[startPos.column][startPos.row] = 1;
	steps.push(start);

	bool finished = false;
	while (!steps.empty())
	{
		// ע�ⷵ�����ã���ΪStep���tried��Ҫ��д��
		Step& lastStep = steps.top();

		while (true)
		{
			lastStep.tried++;
			if (lastStep.tried == 8)	// ���������з��򣬶����߹�
			{
				if (steps.size() == COLUMN*ROW)	// ���������и���
				{
					finished = true;
					break;
				}
				else
				{
					cout << "remove step: column: " << lastStep.pos.column << ", row: " << lastStep.pos.row << endl;
					// ��һ���ó�δ��״̬������ջ����ȥ
					grid[lastStep.pos.column][lastStep.pos.row] = 0;
					steps.pop();
					break;
				}
			}

			// ��һ��
			Pos nextOffset = trySteps[lastStep.tried];
			Step nextStep;
			nextStep.pos.column = lastStep.pos.column + nextOffset.column;
			nextStep.pos.row = lastStep.pos.row + nextOffset.row;

			// Խ�磬����һ������
			if (nextStep.pos.row < 0 || nextStep.pos.row >= ROW ||
				nextStep.pos.column < 0 || nextStep.pos.column >= COLUMN)
			{
				continue;
			}

			if (grid[nextStep.pos.column][nextStep.pos.row] == 0)	// ��û�߹�
			{
				nextStep.tried = -1;
				grid[nextStep.pos.column][nextStep.pos.row] = 1;
				steps.push(nextStep);

				cout << "new step: column: " << nextStep.pos.column << ", row: " << nextStep.pos.row << endl;

				break;
			}
			else	// ���߹���������
			{
				continue;
			}
		}

		if (finished)
		{
			stack<Step> assist;
			while (!steps.empty())
			{
				assist.push(steps.top());
				steps.pop();
			}

			int index = 1;
			while (!assist.empty())
			{
				Step& topStep = assist.top();
				cout << index++ << ": (" << topStep.pos.column << ", " << topStep.pos.row << ")" << endl;
				assist.pop();
			}

			break;
		}
	}

	return 0;
}
