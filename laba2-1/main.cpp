
#include "iostream"
using namespace std;

/*struct GreenList {
	int priority;
	int xPosition;
	int yPosition;
	int wayFromcurr;
	GreenList *next;
};

struct RedList {
	int priority;
	int xPosition;
	int yPosition;
	int wayFrom1;
	RedList *next;
};

char Matrix[8][8] = {
	{ 'X','X','X','X','X','X','X','X' },
	{ 'X','X','X','X','X','X','.','X' },
	{ 'X','X','.','.','.','.','.','X' },
	{ 'X','X','.','X','.','X','.','X' },
	{ 'X','X','.','X','.','X','.','X' },
	{ 'X','X','.','X','.','.','.','X' },
	{ 'X','X','X','X','X','X','X','X' },
	{ 'X','X','X','X','X','X','X','X' }, };
int xFPos, yFPos, xLPos, yLPos;

void checkFirstLastPosition();
void findNeighbours(GreenList **start, int Fx, int Fy, int Lx, int Ly);
//void AddToGreenList(GreenList **begin, int xPosition, int yPosition);
void AddToGreenList(GreenList **begin, int xPosition, int yPosition, int bestNeighbout);
//int findBestNeighbour(int currPosx, int currPosy, int finalPossx, int finalPossy, int firstPossx, int firstPossy);
void Print(GreenList *begin);

int main()
{

	checkFirstLastPosition();

	GreenList *begin = NULL;
	begin = new GreenList;
	begin->xPosition = 0;
	begin->yPosition = 0;
	begin->next = NULL;
	findNeighbours(&begin, xFPos, yFPos, xLPos, yLPos);
	Print(begin);
	system("pause");
}

void checkFirstLastPosition()
{
	do
	{
		cout << "Enter FirstPosiont Y and X : ";
		cin >> xFPos >> yFPos;
	} while ((xFPos < 1 || xFPos>7) || (yFPos < 1 || yFPos>7) || Matrix[xFPos][yFPos] == 'X');

	do
	{
		cout << "Enter LastPosiont Y and X : ";
		cin >> xLPos >> yLPos;
	} while ((xLPos < 1 || xLPos>7) || (yLPos < 1 || yLPos>7) || Matrix[xLPos][yLPos] == 'X');

}

void findNeighbours(GreenList **start, int Fx, int Fy, int Lx, int Ly)
{
	char neighbours[4];
	int mass[5][2] = {};
	cout << "Your Start is on " << Matrix[Fx][Fy];
	mass[0][1] = Fx; mass[0][2] = Fy;
	if (Matrix[Fx + 1][Fy] != 'X') { mass[1][0] = Fx + 1; mass[1][1] = Fy; };
	if (Matrix[Fx][Fy + 1] != 'X') { mass[2][0] = Fx; mass[2][1] = Fy + 1; };
	if (Matrix[Fx - 1][Fy] != 'X') { mass[3][0] = Fx - 1; mass[3][1] = Fy; };
	if (Matrix[Fx][Fy - 1] != 'X') { mass[4][0] = Fx; mass[4][1] = Fy - 1; };
	cout << "Your neighbour is ";
	//GreenList *g = new GreenList;
	//GreenList *g = NULL;
	for (int i = 1; i < 5; i++)
	{
		if (mass[i][0] != NULL && mass[i][1] != NULL)
		{
			cout << "\n";
			cout << mass[i][0] << " and " << mass[i][1] << endl;

			//int bestNeifhbour = findBestNeighbour(mass[i][0], mass[i][1],Lx,Ly,0,0);
			AddToGreenList(start, mass[i][0], mass[i][1], 0);// 0- ïîêà  íå èñïîëüçóåòñÿ
															 //AddToGreenList(&g, 200, 200);
		}

	}
	//Print(start);

}

void addToRedList(RedList **begin, int xPosition, int yPosition)
{
	RedList *r = new RedList;
	r = *begin;
	while (r->next != NULL)
	{
		r = r->next;
	}
	r->next = new RedList;
	r = r->next;
	r->xPosition = xPosition;
	r->yPosition = yPosition;
	r->next = NULL;

}

void AddToGreenList(GreenList **begin, int xPosition, int yPosition, int bestNeighbout)
{
	GreenList*b = new GreenList;
	b = *begin;
	while (b->next != NULL)
	{
		b = b->next;
	}
	b->next = new GreenList;
	b = b->next;
	b->xPosition = xPosition;
	b->yPosition = yPosition;
	b->wayFromcurr = bestNeighbout;
	b->next = NULL;

}

void dellFromGreenList(GreenList **begin, int xPosition, int yPosition)
{

}

void Print(GreenList *begin)
{
	GreenList *print = begin;
	while (print)
	{
		cout << print->xPosition << " and " << print->yPosition << endl;
		print = print->next;
	}
}

/*int findBestNeighbour(int currPosx, int currPosy, int finalPossx, int finalPossy,int firstPossx,int firstPossy)
{
int xschet; int yschet;
if (currPosx < finalPossx) { xschet = finalPossx - currPosx; }
if(currPosx > finalPossx) { xschet = currPosx-finalPossx ; }

if (currPosy < finalPossy) { xschet = finalPossy - currPosy; }
if (currPosy > finalPossy) { xschet = currPosy - finalPossy; }
int result = xschet + yschet;
return result;
}*/
