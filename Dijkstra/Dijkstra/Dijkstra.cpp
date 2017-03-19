#include <stdio.h>

#define INT_MAX		2147483647	// �ִ� ����
#define TRUE	1
#define FALSE	0
#define MAX_VERTICES	7		// ������ ��
#define INF 1000				// ���Ѵ�(������ ���� ���)

int weight[MAX_VERTICES][MAX_VERTICES] =	// ��Ʈ��ũ�� ���� ���
{
	{   0,   7, INF, INF,   3,  10, INF },
	{   7,   0,   4,  10,   2,   6, INF },
	{ INF,   4,   0,   2, INF, INF, INF },
	{ INF,  10,   2,   0,  11,   9,   4 },
	{   3,   2, INF,  11,   0, INF,   5 },
	{  10,   6, INF,   9, INF,   0, INF },
	{ INF, INF, INF,   4,   5, INF,   0 }
};

int distance[MAX_VERTICES];		// ���� �������κ����� �ִ� ��� �Ÿ�
int found[MAX_VERTICES];		// �湮�� ���� ǥ��
int node[MAX_VERTICES];			// �ִ� ��� �湮 ���
int pathTemp[MAX_VERTICES][MAX_VERTICES];	// �ִ� ��� �ӽ�����
int path[MAX_VERTICES][MAX_VERTICES];		// �ִ� ���

// �ּ� �Ÿ� ���� �Լ�
int choose(int distance[], int n, int found[])
{
	int i, min, minpos;
	min = INT_MAX;
	minpos = -1;
	for(i = 0; i < n; i++){
		if(distance[i] < min && !found[i]){
			min = distance[i];
			minpos = i;
		}
	}
	return minpos;
}

// �ִ� ��� ���� �Լ�
void shortest_pathTemp(int start, int n)
{
	int i, j, u, w, m;
	for(i = 0; i < n; i++){		// �ʱ�ȭ
		distance[i] = weight[start][i];
		found[i] = FALSE;
	}
	found[start] = TRUE;		// ���� ���� �湮 ǥ��
	distance[start] = 0;

	for(i = 0; i < n-2; i++){
		u = choose(distance, n ,found);
		found[u] = TRUE;

		for(w = 0; w < n; w++){
			if(!found[w]){
				if(distance[u] + weight[u][w] < distance[w]){
					distance[w] = distance[u] + weight[u][w];
					node[w] = u;			// �ִ� ��� ��� ����
				}
			}
		}
	}

	// �ӽ� ���� ��带 -1�� �ʱ�ȭ
	for(i = 0; i < n; i++){
		for(j = 0; j < n ;j++){
			pathTemp[i][j] = -1;
			path[i][j] = -1;
		}
	}

	// �ִ� ��� ��� ã�ư���
	for(i = 0; i < n; i++){
		m = i;
		for(j = 0; j < n; j++){
			while(1){
				pathTemp[i][j++] = m;
				if(node[m] == 0)
					break;
				m = node[m];
			}
			if(node[m] == 0)
				break;
		}
	}

	// ���� �� ����
	int k;
	for(i = 0; i < n; i++){
		k = 0;
		for(j = 0; j < n; j++){
			if(pathTemp[i][n-j-1] != -1){
				path[i][k] = pathTemp[i][n-j-1];
				k++;
			}
		}
	}
}

void print_distance(int start)
{
	int i;
	for(i = 0; i < MAX_VERTICES; i++){
		printf("%d���� %d������ �ִ� ��� �Ÿ� : %d\n", start, i, distance[i]);
	}
}

void print_node(int start)
{
	int i, j;
	for(i = 0; i < MAX_VERTICES; i++){
		printf("%d���� %d������ �ִ� ��� : %d", start, i, start);
		for(j = 0; j < MAX_VERTICES; j++){
			if(path[i][j] != -1)
				printf(" -> %d", path[i][j]);
		}
		printf("\n");
	}
}

void main()
{
	shortest_pathTemp(0, MAX_VERTICES);

	print_distance(0);
	print_node(0);
}