//����ȯ��: visual studio 2019
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
void init_board(int board[][4]);
void print_board(int stateboard[][4], int board[][4]);
void print_iniboard(int board[][4]);
void reset(int board[][4], int stateboard[][4]);
void printline();
int check_input(int vector1_1, int vector1_2, int vector2_1, int vector2_2, int stateboard[][4]);
int input(int* score1, int* score2, int stateboard[][4], int turn, int board[][4]);
void PrintWinner(int score1, int score2);

int main()
{
	int board[4][4] = { 0 };        //���� �迭
	int stateboard[4][4] = { 0 };  //������ ���¸� ��Ÿ���� �迭, 0�̸� ���� ����, 1�̸� ���� ����
	int turn;                 //���� ������������ ��Ÿ���� ����, 1�̸� player 1   2�̸� player2
	int score1 = 0, score2 = 0;   //�÷��̾��� ������ ��Ÿ���� ����
	char YesOrNo='y';             //��������� ������ �Է¹޴� ����
	srand(time(NULL));
	while (YesOrNo == 'y')        //����� �������� y�� �Է¹޴� ���� ����ؼ� ������ ������
	{
		reset(board, stateboard); //���� ����۽� board�� stateboard�� 0���� �ʱ�ȭ�����ִ� �Լ� ȣ��
		init_board(board);
		turn = rand() % 2 + 1;    //���� ���� �������� ����
		print_iniboard(board); 
		Sleep(3000);
		system("cls");
		print_board(stateboard, board);
		while (score1 + score2 < 8)         //������ 8���� �� �� ���� �ݺ��ؼ� �Է¹���
		{
			turn = input(&score1, &score2, stateboard, turn, board);
		}                   //while�� ������ ������ 8�� �̶�� ���̹Ƿ� ������ ����� ����
		PrintWinner(score1, score2);  //������ ����Ǹ� ���� �������� ���
		printf("Continue? (y/n) ");        
		scanf(" %c", &YesOrNo);            //������ ����Ǹ� ������ ����� ������ �� ������ �Է¹���
		if (YesOrNo == 'n')
			break;                   //n�� �Է¹����� while���� ���������� ���α׷� ����
		score1 = 0;
		score2 = 0;        //������ ������ ������Ѵٸ� ������ �ʱ�ȭ������
		printf("\nNew Game!\n");
	}
	printf("Good bye");       //�� ���� while���� ������ ���� ����
	return 0;
	
}

void init_board(int board[][4])      //�������� �ʱ�ȭ�ϴ� �Լ�
{
	int raw,raw2;
	int col,col2;    //��� ���� ��Ÿ�� ���� ����
	int i;      //�ݺ����� �̿��� ����
	
		for (i=1;i<=8;)
		{
			raw = rand() % 4;                  //1�� ���� ��ǥ2���� �������� ����, 2�� ���� ��ǥ 2���� �������� ����...~~ 8�� ���� ��ǥ 2���� �������� �����Ѵ�
			col = rand() % 4;
			raw2 = rand() % 4;
			col2= rand() % 4;
			
			while (!(raw == raw2 && col == col2)&& board[raw][col] == 0&& board[raw2][col2] == 0)  //ù��° ��ǥ�� �ι� ° ��ǥ�� ���� ���ٸ� �ٽ� ��ǥ�� �����Ѵ�. ���� ���� �������� ��ǥ�� 0�� �ƴ� �ٸ� ���� ����ִٸ�, �̹� ä�� ��ǥ�̹Ƿ� �ٽ� ��ǥ�� �����Ѵ�. �׷��� ���� ��쿡�� ��ǥ�� ���� �������ش�.
			{
					board[raw][col] = i;
					board[raw2][col2] = i;
					i++;
			}
		}

		return;
}
void print_board(int stateboard[][4],int board[][4] )   //�������� ����ϴ� �Լ�
{
	int raw;
	int col;   //��� ���� ��Ÿ�� ����
	printf("\n    0   1   2   3\n");
	for (raw = 0; raw < 4; raw++)
	{
		printf("  +---+---+---+---+\n%d |", raw);
		for (col = 0; col < 4; col++)
		{
			if (stateboard[raw][col] == 1)
				printf("%2d |", board[raw][col]);
			else
				printf(" * |");
		}
		printf("\n");
	}
	printf("  +---+---+---+---+\n\n");
}
void print_iniboard(int board[][4])       //���� ���� �� ȭ���� ����ϴ� �Լ�
{
	int raw;
	int col;   //��� ���� ��Ÿ�� ����
	printf("Memorize the board number!!\n\n");
	printf("    0   1   2   3\n");
	for (raw = 0; raw < 4; raw++)
	{
		printf("  +---+---+---+---+\n%d |", raw);
		for (col = 0; col < 4; col++)
		{
				printf("%2d |", board[raw][col]);
		}
		printf("\n");
	}
	printf("  +---+---+---+---+\n");
}
int input(int* score1, int* score2, int stateboard[][4],int turn,int board[][4])   //��ǥ�� �Է¹ް�, ������ �˻��Ͽ� ������ �ø���. �׸��� �÷��̾��� ������ ��ȯ�Ѵ�.
{
	int vector1_1, vector1_2, vector2_1, vector2_2;   //�Է¹��� ��ǥ�� ������ ����
	if (turn == 1)
		printf("[player 1]: ");
	else
		printf("[player 2]: ");
	scanf("%d%d%d%d", &vector1_1, &vector1_2, &vector2_1, &vector2_2);
	while (check_input(vector1_1, vector1_2, vector2_1, vector2_2, stateboard) == 1)  //���������� �Է��� ���� ��� �ٽ� �Է¹���
	{
		if (turn == 1)
			printf("[player 1]: ");
		else
			printf("[player 2]: ");
		scanf("%d%d%d%d", &vector1_1, &vector1_2, &vector2_1, &vector2_2);
	}  
	stateboard[vector1_1][vector1_2] = 1;
	stateboard[vector2_1][vector2_2] = 1;               //�Ͻ������� ������ ���¸� �������� ����� �༭ �Է¹��� ���� � ���ڰ� ����־����� ������ش�. �Է¹��� ��ǥ�� ���� ���� ���� ���ٸ� �״�� ���»��·� ���ָ� �ǰ�, �ٸ� ��쿡�� �ٽ� ���»��¸� �������ƾ� �Ѵ�.
	print_board(stateboard,board);
	if (board[vector1_1][vector1_2] == board[vector2_1][vector2_2])         //�Է��� ��ǥ�� ���� ���� ������ ���� �ø���. 
	{
		if (turn == 1)
		{
			(*score1)++;
			printf("\nplayer 1 get score!\n");
			printline();
			printf("[player 1] %d : %d [player 2]\n", *score1, *score2);
			printline();
			if((*score1)+(*score2)<8)  //score1�� score2�� ���ؼ� 8�� �̻��� ��쿡�� ������ �������Ƿ� player get one more chance�� ����� �ʿ䰡 ����.
			printf("\nplayer 1 get one more chance!!\n");
		}
		else
		{
			(*score2)++;
			printf("\nplayer 2 get score!\n");
			printline();
			printf("[player 1] %d : %d [player 2]\n", *score1, *score2);
			printline();
			if ((*score1) + (*score2) < 8)  //score1�� score2�� ���ؼ� 8�� �̻��� ��쿡�� ������ �������Ƿ� player get one more chance�� ����� �ʿ䰡 ����.
			printf("\nplayer 2 get one more chance!!\n");
		}
	}
	else         //�Է��� ��ǥ�� �� ���� �ٸ��� ������ �ٲ��ش�.
	{
		stateboard[vector1_1][vector1_2] = 0;
		stateboard[vector2_1][vector2_2] = 0;        //���»��¸� ������� �������´�.
		printf("Mismatch!");
		Sleep(3000);
		system("cls");
		print_board(stateboard, board);
		if (turn == 1)
			turn = 2;
		else
			turn = 1;   //�Է��� ��ǥ�� �� ���� �ٸ��� ������ �ٲ��ش�.
	}
	return turn;                           
}
int check_input(int vector1_1, int vector1_2, int vector2_1, int vector2_2, int stateboard[][4])  //�Էµ� ���� �ùٸ� �Է����� Ȯ���ϴ� �Լ�
{
	if (vector1_1 == vector2_1 && vector1_2 == vector2_2)
	{
		printf("\nsame card... try again..\n");   //���� ī�� �������� ���
		return 1;   //���������� �Է��� ��� 1 ��ȯ
	}
	else if (stateboard[vector1_1][vector1_2] == 1 || stateboard[vector2_1][vector2_2] == 1)
	{
		printf("\nalready open... try again..\n");             //�ΰ��� ���� �� �ϳ��� �̹� �����ִ� ī���� ���
		return 1;   //���������� �Է��� 1�� ��ȯ
	}
	else
		return 0;         //�������� �Է��� 0�� ��ȯ

}
void printline()  //'-'�� ������ ������ŭ ����ϴ� �Լ�
{
	printf("--------------------------\n");
	return;
}

void reset(int board[][4], int stateboard[][4])       //������ �ٽ� ������ �� board�� stateboard�� �ٽ� 0���� �ʱ�ȭ�����ִ� �Լ�
{
	int raw;
	int col;
	for (raw = 0; raw < 4; raw++)
	{
		for (col = 0; col < 4; col++)
		{
			board[raw][col] = 0;
			stateboard[raw][col] = 0;
		}
		
	}
}
void PrintWinner(int score1, int score2)    //������ ����� ����ϴ� �Լ�
{
	if (score1 > score2)
	{
		printf("\nGame End! Player 1 wins!\n");
	}
	else if (score1 < score2)
	{
		printf("\nGame End! Player 2 wins!\n");
	}
	else
		printf("\nGame End! Draw!\n");                 //���� ���� �� �� �÷��̾��� ������ ���� ������ ȭ���� ����Ѵ�.
	return;

}