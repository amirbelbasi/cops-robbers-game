#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#define MAX_RADIF 100
#define MAX_SOTUN 100
#define MAX_KALANTARI 100

int can_see(int i, int j, int nRadif, int nSotun, int mohit_bazi[MAX_RADIF][MAX_SOTUN]);
int thief_counter(int nRadif, int nSotun, int mohit_bazi[MAX_RADIF][MAX_SOTUN]);
int distance_func(int x, int y, int xs, int ys);
void rand_move(int nRadif, int nSotun, int i, int j, int* dozd_moves, int* polis_moves, int mohit_bazi[MAX_RADIF][MAX_SOTUN], int tmp_mohit_bazi[MAX_RADIF][MAX_SOTUN]);
void sys_move(int nRadif, int nSotun, int i, int j, int* dozd_moves, int* polis_moves, int mohit_bazi[MAX_RADIF][MAX_SOTUN], int tmp_mohit_bazi[MAX_RADIF][MAX_SOTUN]);
void display_arr(int nRadif, int nSotun, int nKalantari, int mohit_bazi[MAX_RADIF][MAX_SOTUN]);
void index_max(int distance[3][3], int* x, int* y);
void index_min(int distance[3][3], int* x, int* y);

int main()//dar mohit bazi: -1 <==> dozd | 0 <==> khane khali | (int n >= 1 & n = shomare kalantari) n <==> polis
{
	int nRadif, nSotun, nKalantari, nDozd, rand_x, rand_y, t, all = 0, dozd_moves = 0, polis_moves = 0, nPolis_kalantari[MAX_KALANTARI], mohit_bazi[MAX_RADIF][MAX_SOTUN] = { 0 };
	time_t s = time(NULL);
	srand(s);
	printf("tedad radif mohit bazi: ");
	scanf_s("%d", &nRadif);
	printf("tedad sotun mohit bazi: ");
	scanf_s("%d", &nSotun);
	printf("tedad dozd ha: ");
	scanf_s("%d", &nDozd);
	printf("tedad kalantari ha: ");
	scanf_s("%d", &nKalantari);
	for (int i = 1; i <= nKalantari; i++)//tedad polis haye har kalantari
	{
		int tmp;
		printf("tedad polis haye kalantari shomare #%d: ", i);
		scanf_s("%d", &tmp);
		nPolis_kalantari[i] = tmp;
	}
	for (int i = 1; i <= nKalantari; i++)
	{
		all += nPolis_kalantari[i];
	}
	all += nDozd;
	if (all > nRadif * nSotun)
	{
		printf("be tedad kafi baraye dozd ha va polis ha khane vojud nadarad :(");
		return -1;
	}
	for (int i = 1; i <= nKalantari; i++)//makan avalie polis ha
	{
		for (int j = 0; j < nPolis_kalantari[i]; j++)
		{
			do
			{
				rand_x = rand() % nRadif;
				rand_y = rand() % nSotun;
			} while (mohit_bazi[rand_x][rand_y]);
			mohit_bazi[rand_x][rand_y] = i;
		}
	}
	for (int i = 0; i < nDozd; i++)//makan avalie dozd ha
	{
		do
		{
			rand_x = rand() % nRadif;
			rand_y = rand() % nSotun;
		} while (mohit_bazi[rand_x][rand_y]);
		mohit_bazi[rand_x][rand_y] = -1;
	}
	system("cls");
	printf("t = 0s\n");
	display_arr(nRadif, nSotun, nKalantari, mohit_bazi);
	Sleep(1000);
	for (t = 1; thief_counter(nRadif, nSotun, mohit_bazi) > 0; t++)
	{
		Sleep(1000);
		system("cls");
		int tmp_mohit_bazi[MAX_RADIF][MAX_SOTUN] = { 0 };
		printf("t = %ds\n", t);
		for (int i = 0; i < nRadif; i++)//aval makan dozdha bayad malum shavad
		{
			for (int j = 0; j < nSotun; j++)
			{
				if (mohit_bazi[i][j] != -1) continue;
				if (can_see(i, j, nRadif, nSotun, mohit_bazi) == 0) rand_move(nRadif, nSotun, i, j, &dozd_moves, &polis_moves, mohit_bazi, tmp_mohit_bazi);
				else sys_move(nRadif, nSotun, i, j, &dozd_moves, &polis_moves, mohit_bazi, tmp_mohit_bazi);
			}
		}
		for (int i = 0; i < nRadif; i++)//polis haye ke mibinan dozdi
		{
			for (int j = 0; j < nSotun; j++)
			{
				if (mohit_bazi[i][j] <= 0 || can_see(i, j, nRadif, nSotun, mohit_bazi) == 0) continue;
				sys_move(nRadif, nSotun, i, j, &dozd_moves, &polis_moves, mohit_bazi, tmp_mohit_bazi);
			}
		}
		for (int kalantari_num = 1; kalantari_num <= nKalantari; kalantari_num++)//kalantari be kalantari
		{
			int x, y, a, b, min_distance, flag = 0;
			for (int i = 0; i < nRadif; i++)
			{
				for (int j = 0; j < nSotun; j++)
				{
					if (tmp_mohit_bazi[i][j] == kalantari_num)
					{
						if (!flag) x = i;
						if (!flag) y = j;
						flag++;
						continue;
					}
				}
			}
			for (int i = 0; i < nRadif; i++)//polis haye ke nmibinan dozdi
			{
				for (int j = 0; j < nSotun; j++)
				{
					a = i;
					b = j;
					if (can_see(i, j, nRadif, nSotun, mohit_bazi) == 1 || mohit_bazi[i][j] != kalantari_num) continue;
					if (flag == 0)
					{
						rand_move(nRadif, nSotun, i, j, &dozd_moves, &polis_moves, mohit_bazi, tmp_mohit_bazi);
						continue;
					}
					min_distance = fmax(nRadif, nSotun);
					for (int xs = fmax(i - 1, 0); xs <= fmin(i + 1, nRadif - 1); xs++)
					{
						for (int ys = fmax(j - 1, 0); ys <= fmin(j + 1, nRadif - 1); ys++)
						{
							if (mohit_bazi[xs][ys] > 0 || tmp_mohit_bazi[xs][ys] > 0) continue;
							if (distance_func(x, y, xs, ys) < min_distance)
							{
								min_distance = distance_func(x, y, xs, ys);
								a = xs;
								b = ys;
							}
						}
					}
					tmp_mohit_bazi[a][b] = kalantari_num;
					if (a != i || b != j)
					{
						tmp_mohit_bazi[i][j] = 0;
						polis_moves++;
					}
				}
			}
		}
		for (int i = 0; i < nRadif; i++)//tmp ==> mohit_bazi
		{
			for (int j = 0; j < nSotun; j++)
			{
				mohit_bazi[i][j] = tmp_mohit_bazi[i][j];
			}
		}
		display_arr(nRadif, nSotun, nKalantari, mohit_bazi);
	}
	Sleep(2000);
	system("cls");
	printf("tamam dozd ha gerefte shodan :)\n");
	printf("zaman kol: %ds\n", t - 1);
	printf("tedad kol harakat dozd ha: %d\n", dozd_moves);
	printf("tedad kol harakat polis ha: %d\n", polis_moves);
	return 0;
}

void display_arr(int nRadif, int nSotun, int nKalantari, int mohit_bazi[MAX_RADIF][MAX_SOTUN])//namayesh mohit
{
	int nSpace = log10(nKalantari);
	for (int i = 0; i < nRadif; i++)
	{
		for (int j = 0; j < nSotun; j++)
		{
			printf("[");
			for (int k = 0; k < nSpace; k++) printf(" ");
			if (mohit_bazi[i][j] == -1) printf(" T ");
			else if (mohit_bazi[i][j] == 0) printf("   ");
			else
			{
				printf("D %d", mohit_bazi[i][j]);
				for (int k = log10(mohit_bazi[i][j]); k < nSpace; k++) printf(" ");
				printf("]");
				continue;
			}
			for (int k = 0; k < nSpace; k++) printf(" ");
			printf("]");
		}
		printf("\n");
	}
}


int can_see(int i, int j, int nRadif, int nSotun, int mohit_bazi[MAX_RADIF][MAX_SOTUN])//dozd mitavand polis ra bbinad ya baraks
{
	if (mohit_bazi[i][j] == -1)
	{
		for (int x = fmax(i - 2, 0); x <= fmin(i + 2, nRadif - 1); x++)
		{
			for (int y = fmax(j - 2, 0); y <= fmin(j + 2, nSotun - 1); y++)
			{
				if (mohit_bazi[x][y] > 0) return 1;
			}
		}
		return 0;
	}
	else
	{
		for (int x = fmax(i - 2, 0); x <= fmin(i + 2, nRadif - 1); x++)
		{
			for (int y = fmax(j - 2, 0); y <= fmin(j + 2, nSotun - 1); y++)
			{
				if (mohit_bazi[x][y] < 0) return 1;
			}
		}
		return 0;
	}
}

void rand_move(int nRadif, int nSotun, int i, int j, int* dozd_moves, int* polis_moves, int mohit_bazi[MAX_RADIF][MAX_SOTUN], int tmp_mohit_bazi[MAX_RADIF][MAX_SOTUN])//harkat random
{
	while (true)
	{
		int x_move = (rand() % 3) - 1, y_move = (rand() % 3) - 1;
		if (i + x_move < 0 || i + x_move >= nRadif || j + y_move < 0 || j + y_move >= nSotun) continue;
		if (x_move == 0 && y_move == 0)
		{
			tmp_mohit_bazi[i][j] = mohit_bazi[i][j];
			break;
		}
		if (mohit_bazi[i + x_move][j + y_move] != 0 || tmp_mohit_bazi[i + x_move][j + y_move] != 0) continue;
		tmp_mohit_bazi[i + x_move][j + y_move] = mohit_bazi[i][j];
		tmp_mohit_bazi[i][j] = 0;
		if (mohit_bazi[i][j] == -1) (*dozd_moves)++;
		else (*polis_moves)++;
		break;
	}
}

void sys_move(int nRadif, int nSotun, int i, int j, int* dozd_moves, int* polis_moves, int mohit_bazi[MAX_RADIF][MAX_SOTUN], int tmp_mohit_bazi[MAX_RADIF][MAX_SOTUN])//dozd az polis farar mikone & polis be donbal dozd mire
{
	int min_distance, a, b, distance[3][3];
	for (int z1 = 0; z1 < 3; z1++)
	{
		for (int z2 = 0; z2 < 3; z2++)
		{
			distance[z1][z2] = -5;
		}
	}
	if (mohit_bazi[i][j] == -1)
	{
		for (int xs = fmax(i - 1, 0); xs <= fmin(i + 1, nRadif - 1); xs++)
		{
			for (int ys = fmax(j - 1, 0); ys <= fmin(j + 1, nRadif - 1); ys++)
			{
				if (mohit_bazi[xs][ys] != 0 || tmp_mohit_bazi[xs][ys] == -1) continue;
				min_distance = 5;
				for (int x = fmax(i - 2, 0); x <= fmin(i + 2, nRadif - 1); x++)
				{
					for (int y = fmax(j - 2, 0); y <= fmin(j + 2, nSotun - 1); y++)
					{
						if (mohit_bazi[x][y] > 0)
						{
							if (distance_func(x, y, xs, ys) < min_distance) min_distance = distance_func(x, y, xs, ys);
						}
					}
				}
				distance[xs - i + 1][ys - j + 1] = min_distance;
			}
		}
		index_max(distance, &a, &b);
		tmp_mohit_bazi[a + i - 1][b + j - 1] = -1;
		if (a != 1 || b != 1)
		{
			tmp_mohit_bazi[i][j] = 0;
			(*dozd_moves)++;
		}
	}
	else
	{
		for (int xs = fmax(i - 1, 0); xs <= fmin(i + 1, nRadif - 1); xs++)
		{
			for (int ys = fmax(j - 1, 0); ys <= fmin(j + 1, nRadif - 1); ys++)
			{
				if (mohit_bazi[xs][ys] > 0 || tmp_mohit_bazi[xs][ys] > 0) continue;
				min_distance = 5;
				for (int x = fmax(i - 2, 0); x <= fmin(i + 2, nRadif - 1); x++)
				{
					for (int y = fmax(j - 2, 0); y <= fmin(j + 2, nSotun - 1); y++)
					{
						if (mohit_bazi[x][y] == -1)
						{
							if (distance_func(x, y, xs, ys) < min_distance) min_distance = distance_func(x, y, xs, ys);
						}
					}
				}
				distance[xs - i + 1][ys - j + 1] = min_distance;
			}
		}
		index_min(distance, &a, &b);
		tmp_mohit_bazi[a + i - 1][b + j - 1] = mohit_bazi[i][j];
		if (a != 1 || b != 1)
		{
			tmp_mohit_bazi[i][j] = 0;
			(*polis_moves)++;
		}
	}
}

int distance_func(int x, int y, int xs, int ys)//fasele oqlidosi 2 noqte ra dar mohit midahad
{
	return fmin(fabs(x - xs), fabs(y - ys)) + fabs(fabs(x - xs) - fabs(y - ys));
}

void index_min(int distance[3][3], int* x, int* y)//index kuchaktarin ozv ra midahad
{
	int min_element, flagg = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (distance[i][j] != -5)
			{
				if (!flagg) min_element = distance[i][j];
				if (distance[i][j] < min_element) min_element = distance[i][j];
				flagg++;
			}
		}
	}
	while (true)
	{
		if (flagg == 0)
		{
			*x = 1;
			*y = 1;
			break;
		}
		int x_move = rand() % 3, y_move = rand() % 3;
		if (distance[x_move][y_move] == min_element)
		{
			*x = x_move;
			*y = y_move;
			break;
		}
	}
}

void index_max(int distance[3][3], int* x, int* y)//index bozorgtarin ozv ra midahad
{
	int max_element, flagg = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (distance[i][j] != -5)
			{
				if (!flagg) max_element = distance[i][j];
				if (distance[i][j] > max_element) max_element = distance[i][j];
				flagg++;
			}
		}
	}
	while (true)
	{
		if (flagg == 0)
		{
			*x = 1;
			*y = 1;
			break;
		}
		int x_move = rand() % 3, y_move = rand() % 3;
		if (distance[x_move][y_move] == max_element)
		{
			*x = x_move;
			*y = y_move;
			break;
		}
	}
}

int thief_counter(int nRadif, int nSotun, int mohit_bazi[MAX_RADIF][MAX_SOTUN])//tedad dozd hara mishomarad
{
	int cnt = 0;
	for (int i = 0; i < nRadif; i++)
	{
		for (int j = 0; j < nSotun; j++)
		{
			if (mohit_bazi[i][j] == -1) cnt++;
		}
	}
	return cnt;
}