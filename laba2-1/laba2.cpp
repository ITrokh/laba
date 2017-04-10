#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;
struct Coordinates //структура координат(x-столбец, y-строка)
{
	int x;
	int y;
};


struct Tops
{
	int previous; //предедущее id
	bool closed; //проверка на закрытие струкктуры
	bool visited; //на открытие
	int G;//поточна€ стоимость
	int H;//эвристическа€
	int F;//обща€
	Coordinates coord;
};


struct Queue
{
	int data;
	int value;//значение, по которому выставл€етс€ приоритет
	Queue *next;
};

struct Labyrinth
{
	int v_n;//количество вершин

	int **matrix;// матрица лабиринта
	char **path; //матрица лабиринта с путем
	int max_x;//максимальна€ ширина 
	int max_y; //максимальна€ высота

	Coordinates in, out;//координаты начала и конца лабиринта

	Tops *v;//вершины графа
};

void a_star(Labyrinth *&gr); //gr-покажчик на граф
void GraphCreating(Labyrinth *&gr);//создаем массив структур и инициализируем 
void GraphClearing(Labyrinth *&gr);//удалени€е массивов
void ChangeTop(Labyrinth *&gr, int ver, Queue *&queueBegin);//смена вершин ,ver-количество вершин,queueBegin-указатель на начало очереди
int Heuristic(Coordinates out, Coordinates ver);
void LabyrinthFromFile(Labyrinth*& graph);
void ResultToFile(Labyrinth*& graph); //файл с результатом
void add_e(Queue *&queueBegin, int data, int value);//data-данные нового элемента, value-значение новго элемента


int GetQueueElement(Queue *&queueBegin);//берем элемент с приоритетной очереди
int main()
{
	Labyrinth *gr = NULL;
	LabyrinthFromFile(gr);
	gr->in.x = 1;
	gr->in.y = 6;
	gr->out.x = 5;
	gr->out.y = 1;



	// A* algorithm
	a_star(gr);

	ResultToFile(gr);
	GraphClearing(gr);
	system("pause");
}
template<typename T>
T** Create_Array(int n, int m)
{
	T **temp;

	temp = new T*[n];

	for (int i = 0; i < n; i++)
		temp[i] = new T[m];

	return temp;
}

template<typename T>
void Delete_Array(T **arr, int n)
{
	for (int i = 0; i < n; i++)
		delete arr[i];

	delete arr;
	arr = NULL;
}
void LabyrinthFromFile(Labyrinth*& graph)
{
	ifstream file("input.txt");
	int n, m;
	if (file.good())
	{
		
		char *str = new char[1024];
		n = 0;
		while (!file.eof())
		{
			file.getline(str, 1024);
			n++;
		}
		delete[] str;

		
		file.clear();
		file.seekg(0);//возвращаем указатель на начало файла

		char ch = '0';
		m = 0;
		while (ch != '\n')
		{
			file.get(ch);
			m++;
		}

		file.clear();
		file.seekg(0);

		m /= 2;

	

		graph = new Labyrinth;
		graph->matrix = Create_Array<int>(n, m);
		char** matr = Create_Array<char>(n, m);
		graph->max_x = n;
		graph->max_y = m;

		
		string line;
		int i = 0;
		while (getline(file, line))
		{
			for (int j = 0; j < line.length(); ++j)
				if (j % 2 == 0)
					matr[i][j / 2] = line[j];
			i++;
		}

		
		int number = 1;
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < m; ++j)
				if (matr[i][j] == 'X')
					graph->matrix[i][j] = 0;
				else
				{
					graph->matrix[i][j] = number;
					number++;
				}
		graph->v_n = number;

		// вывод лабиринта
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
				cout << setw(3) << graph->matrix[i][j];
			cout << endl;
		}

		Delete_Array<char>(matr, n);
	}
	else cout << "Error opening file." << endl;

	file.close();
}

void ResultToFile(Labyrinth*& graph)
{
	
	ofstream file("result.txt");
	if (file.good())
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
				file << " " << graph->path[i][j];
			file << "\n";
		}
		file.close();
		cout << "File successfully saved" << endl;
	}
	else cout << "Something went wrong " << endl;
}

void add_e(Queue *&queueBegin, int data, int value)
{

	if (queueBegin == NULL)
	{
		queueBegin = new Queue;
		queueBegin->data = data;
		queueBegin->value = value;
		queueBegin->next = NULL;
		return;
	}

	// добавл€ем элемент в очередь
	Queue *curr = queueBegin;
	while (curr->next != NULL)
	{
		if (curr->next->value > value)
		{
			Queue *temp = new Queue;
			temp->data = data;
			temp->value = value;
			temp->next = curr->next;
			curr->next = temp;
			return;
		}
		curr = curr->next;
	}

	// добавл€ем элемент в конец очереди
	Queue *temp = new Queue;
	temp->data = data;
	temp->value = value;
	if (value >= curr->value)
	{
		temp->next = NULL;
		curr->next = temp;
		return;
	}
	else
	{
		temp->next = curr;
		queueBegin = temp;
	}
}

int GetQueueElement(Queue *&queueBegin)
{
	int temp;

	if (queueBegin == NULL)
		return -1;

	if (queueBegin->next == NULL)
	{
		temp = queueBegin->data;

		delete queueBegin;
		queueBegin = NULL;
		return temp;
	}

	Queue *curr;
	curr = queueBegin;

	temp = curr->data;

	// удал€ем первый элемент
	queueBegin = queueBegin->next;
	delete curr;

	return temp;
}

void a_star(Labyrinth *&gr)
{
	// создаем приоритетную очередь
	Queue *PQ_head;
	PQ_head = NULL;

	GraphCreating(gr);

	// точка входа
	int in = gr->matrix[gr->in.x][gr->in.y] - 1;
	gr->v[in].coord = gr->in;
	gr->v[in].H = Heuristic(gr->out, gr->in);
	gr->v[in].G = 0;
	gr->v[in].F = gr->v[in].H;
	gr->v[in].visited = 1;
	int out = gr->matrix[gr->out.x][gr->out.y] - 1;

	int current = in;


	do
	{
		ChangeTop(gr, current, PQ_head);
		current = GetQueueElement(PQ_head);
	} while (current != -1 && current != out);

	// проверка, что путь найден
	if (current != out)
	{
		printf("You shall not pass this Labyrinth!!!");
		exit(1);
	}

	

	// «аполн€ем лабиринт "X" и " "
	for (int i = 0; i < gr->max_x; i++)
		for (int j = 0; j < gr->max_y; j++)
			if (gr->matrix[i][j] == 0)
				gr->path[i][j] = 'X';
			else
				gr->path[i][j] = ' ';

	// –исуем путь
	gr->path[gr->v[current].coord.x][gr->v[current].coord.y] = 1 + '0';
	int i = 1;
	while (current != in)
	{
		current = gr->v[current].previous;
		i++;
		if (i == 10)
			i = 49;
		if (i == 75)
			i = 17;
		if (i == 43)
		{
			printf("Ѕольше не сущещствует символов.");
			cin.get();
			exit(1);
		}
		gr->path[gr->v[current].coord.x][gr->v[current].coord.y] = i + '0';
	}

	// Output
	for (int i = 0; i < gr->max_x; i++)
	{
		for (int j = 0; j < gr->max_y; j++)
		{
			printf("%2c", gr->path[i][j]);
		}
		printf("\n");
	}
}

void GraphCreating(Labyrinth *&gr)
{
	gr->path = Create_Array<char>(gr->max_x, gr->max_y);
	gr->v = new Tops[gr->v_n];
	for (int i = 0; i < gr->v_n; i++)
	{
		gr->v[i].visited = 0;
		gr->v[i].closed = 0;
	}
}

void GraphClearing(Labyrinth *&gr)
{
	Delete_Array<int>(gr->matrix, gr->max_x);
	Delete_Array<char>(gr->path, gr->max_x);
	delete[] gr->v;
	gr->v = NULL;
}

void ChangeTop(Labyrinth *&gr, int ver, Queue *&queueBegin)
{
	// Coordinates of current top
	Coordinates adj;

	// id поточной вершины
	int id;

	// —мещение по x и y
	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, 1, -1 };


	for (int i = 0; i < 4; i++)
	{
		adj = gr->v[ver].coord;
		//  оординаты верхнего смещени€
		adj.x += dx[i];
		adj.y += dy[i];

		// ≈сли вершина из координат графа
		if (adj.x >= gr->max_x || adj.x < 0 || adj.y >= gr->max_y || adj.y < 0)
			continue;

		// Ѕерем id этой вершины
		id = gr->matrix[adj.x][adj.y] - 1;

		// ≈сли вершина есть частью стены
		if (id == -1)
			continue;

		// ≈сли вершина не была проверена 
		if (!gr->v[id].visited)
		{
			gr->v[id].H = Heuristic(gr->out, adj);
			gr->v[id].coord = adj;
			gr->v[id].closed = 0;
			gr->v[id].G = -1;
			gr->v[id].F = -1;

			gr->v[id].visited = 1;
		}

		// ≈сли вершина закрыта
		if (gr->v[id].closed)
			continue;

		if (gr->v[id].F == -1 || gr->v[id].G > gr->v[ver].G + 1)
		{
			gr->v[id].G = gr->v[ver].G + 1;
			gr->v[id].F = gr->v[id].G + gr->v[id].H;
			gr->v[id].previous = ver;
		}

		add_e(queueBegin, id, gr->v[id].F);
	}

	gr->v[ver].closed = 1;
}

int Heuristic(Coordinates out, Coordinates ver)
{
	return abs((out.x - ver.x) +(out.y - ver.y));
}
