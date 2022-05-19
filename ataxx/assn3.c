//����ȯ��:visual studio 2019
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<Windows.h>
#include<stdlib.h>
void get_data(int* row, int* col);
int start_game(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn);
void print_board(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn);
void init_board(int** board_cursor, int** board_horse, int row, int col);  //���� ����� �� �� �ʱ� ���� �ʱ�ȭ���ִ� �Լ�
int select_move(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn);
void what_is_score(int** board_horse, int row, int col, int* score1, int* score2);
void infect(int row_cursor, int col_cursor, int** board_horse, int turn);       //��ó�� �ٸ� ������ ������Ű��(�ڱ� ����� �ٲٴ� )�Լ�
int change_turn(int** board_horse, int row, int col, int turn);
int is_game_end(int row, int col, int* score1, int* score2, int** board_horse);

int main()
{
	int i;
	int** board_cursor = NULL;        //������ Ŀ���� ��ġ�� ��Ÿ���� �迭, 1�� �ִ� ���� Ŀ���� �ִ�. Ŀ���� ���� ���� 0
	int** board_horse = NULL;          /*������ ��� ���� �ִ����� ��Ÿ���� �迭,1�� ��������� player1�� ��,
									 2�� ��������� ���õ� player1�� ��, 3�� ��������� player2�� ��, 4�� ��������� ���õ�
									  player2�� ���� ����մ°����� ����*/

	int score1, score2;        //player1 �� player2�� ������ ��Ÿ���� ���� 
	int row, col;                   //�Է¹��� ��� ���� ������ ����
	int turn = 1; //���� 1�̸� player1�� ����, 2�̸� player2�� ����
	int last_row_cursor1;
	int last_col_cursor1;
	int last_row_cursor2;
	int last_col_cursor2; //���������� ������ Ŀ���� ��ġ�� ������ ������
	int end_of_game = 1; //start_game �Լ��� ��ȯ���� ������ ����, 1�̸� ������ ��� �����ϴ� ���̰� 0�̸� ���� ����
	while (end_of_game == 1)              //start_game���� 0�� ��ȯ�ޱ� ���������� ��� ���� ����
	{
		score1 = 0; score2 = 0; //���� ������ ���� player1�� ������ player2�� ������ �ϴ� 0���� �س��´�.
		last_row_cursor1 = 0;
		last_col_cursor1 = 0;
		last_row_cursor2 = 0;
		last_col_cursor2 = 0; //���������� ������ Ŀ���� ��ġ�� ������ ������, select_move �Լ��� ���ǹ����� �̿��ϱ� ���� �ʱ⿡�� �� �������� ���� 0���� �ʱ�ȭ����.
		get_data(&row, &col);  //������ ũ�� �Է¹���
		board_cursor = (int**)calloc(row, sizeof(int*)); // �޸� �Ҵ�
		for (i = 0; i < row; i++)
			*(board_cursor + i) = (int*)calloc(col, sizeof(int));
		board_horse = (int**)calloc(row, sizeof(int*));
		for (i = 0; i < row; i++)
			*(board_horse + i) = (int*)calloc(col, sizeof(int));      //�޸� �Ҵ�
		init_board(board_cursor, board_horse, row, col);        //���� �������� �� �������� �ʱ����ǿ� ���� �ʱ�ȭ
		end_of_game = start_game(board_cursor, board_horse, &score1, &score2, row, col, turn, &last_row_cursor1, &last_col_cursor1, &last_row_cursor2, &last_col_cursor2);  //���� ����, �׸��� ����� ���θ� ��ȯ�޾�end_of_game�� ����
		for (i = 0; i < row; i++)
			free(*(board_cursor + i));
		for (i = 0; i < row; i++)
			free(*(board_horse + i));
		free(board_horse);
		free(board_cursor);//�Ҵ��ߴ� �޸� ����
	}
	return 0;
}
void get_data(int* row, int* col)   //�������� ��� ���� ������ �Է¹޴� �Լ�
{
	int i; //�ݺ����� ����� ����
	*row = 0;
	*col = 0;
	printf(" [ATAXX]\n");
	while (*row < 4 || *row>20)         //�Է¹��� ���� 4�̻� 20�̸��̿��� ��
	{
		printf("row: ");
		scanf("%d", row);
	}
	while (*col < 4 || *col>20)        //�Է¹��� ���� 4�̻� 20�̸��̿��� ��
	{
		printf("col: ");
		scanf("%d", col);
	}
	*row += 4;
	*col += 4; //�翷�� �� �Ʒ��� 2ĭ��ŭ�� ���������� ������ֱ� ����(������ ���� ������ ���Ƶΰ�, ������ �� ���� �������� ������ ����) ������ �Է¹����ͺ��� 4���� �� ����� �ش�.


}

int start_game(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn, int* last_row_cursor1, int* last_col_cursor1, int* last_row_cursor2, int* last_col_cursor2)   //������ �����ϴ� �Լ�, ������ ����� �� ������ ��� ������ ���̸� 1�� ��ȯ �׷��� ������ 0�� ��ȯ
{
	char yesorno = '0';         //������ ����� ������ �Է¹��� ���� ������ ����, �ݺ����� ���ǹ����� �̿��ϱ� ���� �켱 ���� �ʱ�ȭ������
	while (is_game_end(row, col, score1, score2, board_horse) == 0)            //������ �������� Ȯ���ϴ� is_game_end�Լ��� ȣ���Ͽ� ��ȯ���� 0�̸� ������ ������ ���� ���̹Ƿ� ��� ����
	{
		system("cls");                       //ȭ���� ����
		what_is_score(board_horse, row, col, score1, score2);             //�ٷ� �Ʒ��� print_board���� ������ ����ϱ� ������ �켱 ������ ������ش�.
		print_board(board_cursor, board_horse, score1, score2, row, col, turn);
		turn = select_move(board_cursor, board_horse, score1, score2, row, col, turn, last_row_cursor1, last_col_cursor1, last_row_cursor2, last_col_cursor2);    //���� �����ϰ� �̵���Ŵ �׸��� �÷��̾��� ������ ��ȯ
		system("cls");
		what_is_score(board_horse, row, col, score1, score2);            //while�� ���ǽ��� is_game_end �Լ����� ������ �̿��ϱ� ������ select_move�Լ��� ȣ���� �� ������ ����� �ش�.
	}
	print_board(board_cursor, board_horse, score1, score2, row, col, turn);   //������ ����ż� while�� ���� �Ŀ��� �������� ������� �ϹǷ� �ٽ� �������� ������ش�.
	if (*score1 > *score2)
		printf("\nPlayer 1 wins\nContinue? (y/n)\n");             //������ ����Ǹ� ������ ���� ����� ����ϰ� ������� ������ �� ������ �Է¹���
	else if (*score1 < *score2)
		printf("\nPlayer 2 wins\nContinue? (y/n)\n");
	else
		printf("\nDraw\nContinue? (y/n)\n");
	while (yesorno != 'y' && yesorno != 'n')          //yȤ�� n�� �Է¹ޱ� �� ���� ��� �Է¹���
	{
		scanf(" %c", &yesorno);
		if (yesorno == 'y')
		{
			system("cls");              //������ ������� ���̶�� �� ȭ���� ������ֱ� ���� ���������� ȭ���� ���� �����ش�.   
			return 1;           //���� ��� ������ ���̸� 1 ��ȯ
		}
		else if (yesorno == 'n')
			return 0;     //���� ������ ���̸� 0 ��ȯ
	}
}
int is_game_end(int row, int col, int* score1, int* score2, int** board_horse)      //������ ����ƴ��� Ȯ���ϴ� �Լ�, ������ ����Ǹ� 0�� ��ȯ, �׷��� �ʴٸ� 1�� ��ȯ
{
	int i, j;   //�ݺ������� �̿��� ����
	int exist1 = 0, exist2 = 0;           //player1�� player2�� ���� �����ϴ��� Ȯ���� ����, �����ϸ� 1, �������� ������ 0���� �� ���̴�.
	if (*score1 + *score2 == (row - 4) * (col - 4))           //������ ��밡���� ������ ������ ������, �� �������� ���� ���� ���� �����̹Ƿ� 1��ȯ
		return 1;
	for (i = 0; i < row; i++)           //��� �������� ���� ���Ͽ� �˻��Ѵ�.
		for (j = 0; j < col; j++)
		{
			if (*(*(board_horse + i) + j) == 1)                   //������ ���� ���°�1�̸� player1�� ���� �ִ� ���̹Ƿ� exist1������ ���� 1�� �Ѵ�.
				exist1 = 1;
			else if (*(*(board_horse + i) + j) == 3)                   //������ ���� ���°�3�̸� player2�� ���� �ִ� ���̹Ƿ� exist2������ ���� 1�� ���ش�.
				exist2 = 2;
		}
	if (exist1 == 1 && exist2 == 2)          //player1�� ���� player2�� ���� ��� �����Ѵٸ� ������ ������ ���� ���̹Ƿ� 0�� ��ȯ�Ѵ�.
		return 0;
	else
		return 1;                             //�׷��� ���� ��� ������ ���� ��Ȳ�̹Ƿ� 1�� ��ȯ�Ѵ�.
}

void what_is_score(int** board_horse, int row, int col, int* score1, int* score2)      //���� �� �÷��̾��� ���� ���ϴ� �Լ�
{
	int i, j;     //�ݺ����� �̿��� ����
	*score1 = 0, * score2 = 0;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
		{
			if (*(*(board_horse + i) + j) == 1 || *(*(board_horse + i) + j) == 2)                   //������ ���� ���°�1�̰ų� 2�̸� player1�� ���� �ִ� ���̹Ƿ� score1������ ���� ������Ŵ
				(*score1)++;
			else if (*(*(board_horse + i) + j) == 3 || *(*(board_horse + i) + j) == 4)                   //������ ���� ���°�3�̰ų� 4�̸� player2�� ���� �ִ� ���̹Ƿ� score2������ ���� ������Ŵ
				(*score2)++;
		}
}

int select_move(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn, int* last_row_cursor1, int* last_col_cursor1, int* last_row_cursor2, int* last_col_cursor2)     //���� �����ϰ� �̵���Ŵ �׸��� �÷��̾��� ������ ��ȯ�ϴ� �Լ�
{
	int selected = 0; //�����ߴ��� ���ߴ��� �Ǵ��ϴ� ����, 0�̸� ���þ��ѻ���, 1�̸� ������ ����
	char input = '0';    //Ű �Է¹������� ������ ����, �ݺ������� ����ϱ� ���� �ǹ̾��°����� �ʱ�ȭ���ѳ���
	int check;        //is_movable�Լ��� ��ȯ���� �����ϱ� ���� ����
	int row_cursor = 0, col_cursor = 0;        //���� Ŀ���� �ִ� row�� col�� ������ ������
	int remember_row = 0, remember_col = 0;    //���ù��� ���� ��ġ�� ����� ����
	int re = 0;                        //���� �缱���ߴ��� ���ߴ��� Ȯ���ϴ� ����, �缱���� ��� 1, �׷��� ���� ��� 0�� ������.
	int i, j;                          //�ݺ����� ����� ����
	int between_row, between_col;         //������ ���� ���� �ű� ��ġ ������ �Ÿ��� ��Ÿ�� ������

	if (turn == 1)       //player1�� �����϶��� ���Ͽ�
	{
		if (*last_row_cursor1 == 0 && *last_col_cursor1 == 0)         //*last_row_cursor1 == 0 �̰� *last_col_cursor == 0 �� ���� �� ó�� ���̴�. �� ó������ �Ʒ��� ������� Ŀ���� ��ġ�� ���� ���̴�. �������ʹ� �̹� �÷��̾��� ���� �Ͽ����� ������ Ŀ�� ��ġ�� ���� Ŀ����ġ�� ����� ��.
		{
			for (i = 0; i < row; i++)
			{
				for (j = 0; j < col; j++)
					if (*(*(board_cursor + i) + j) == 1)
					{
						row_cursor = i;
						col_cursor = j;
						*last_row_cursor2 = row - 3;             //player2�� ù ���ʿ����� Ŀ���� ��ġ��(row-1,col-1)�� �Ѵ�.
						*last_col_cursor2 = col - 3;

					}                            //���� Ŀ���� ��ġ�� ����(board_cursor�� �ι� �������� �� �� 1�� ����Ű�� ���� Ŀ���� �ִ�.)
			}
		}
		else  // �̹� �÷��̾��� ���� �Ͽ����� ������ Ŀ�� ��ġ�� ���� Ŀ����ġ�� ���.
		{
			row_cursor = *last_row_cursor1;
			col_cursor = *last_col_cursor1;
			*(*(board_cursor + row_cursor) + col_cursor) = 1; //Ŀ���� �ִ� ���� 1(Ŀ���� �ִ� ����)�� ��������
		}

		while (input != ' ' || *(*(board_horse + row_cursor) + col_cursor) != 1)  //spacebar�� ������ ������ ��� Ű�� �Է¹���, spacebar�� �������� �� �ڸ��� �ڽ��� ���� ������ �ٽ� �Է¹���
		{
			system("cls");
			print_board(board_cursor, board_horse, score1, score2, row, col, turn);

			input = _getch();
			if (input == 'i')    //���� ������ �� ��
			{
				if (row_cursor != 2)   //���� ������ �� �� �ٷ� ���� ���������� Ŀ���� ��ġ�� �ű��� �ʴ´�. (row[2]�� row[1]�� ������ �����ִ� ���̴�)
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //Ŀ���� ��ġ�� �ű�� ���Ͽ� ���� Ŀ���� �ִ� ��ġ�� ���.
					row_cursor--;                         //���� ������ �� �� Ŀ���� ��ġ�� ���� �� ĭ �ø�
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'k')    //�Ʒ��� ������ �� ��
			{
				if (row_cursor != row - 3)   //�Ʒ��� ������ �� �� �ٷ� �Ʒ��� ���������� Ŀ���� ��ġ�� �ű��� �ʴ´�. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //Ŀ���� ��ġ�� �ű�� ���Ͽ� ���� Ŀ���� �ִ� ��ġ�� ���.
					row_cursor++;                       //�Ʒ��� ������ �� �� Ŀ���� ��ġ�� �� ĭ ����
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'j')    //�������� ������ �� ��
			{
				if (col_cursor != 2)   //�������� ������ �� �� �ٷ� ������ ���������� Ŀ���� ��ġ�� �ű��� �ʴ´�. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //Ŀ���� ��ġ�� �ű�� ���Ͽ� ���� Ŀ���� �ִ� ��ġ�� ���.
					col_cursor--;                       //�������� ������ �� �� Ŀ���� ��ġ�� �������� �� ĭ ������
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'l')    //���������� ������ �� ��
			{
				if (col_cursor != col - 3)   //���������� ������ �� �� �ٷ� �������� ���������� Ŀ���� ��ġ�� �ű��� �ʴ´�. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //Ŀ���� ��ġ�� �ű�� ���Ͽ� ���� Ŀ���� �ִ� ��ġ�� ���.
					col_cursor++;                       //���������� ������ �� �� Ŀ���� ��ġ�� ���������� �� ĭ ������
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			if (input == ' ')          //������ �Է¹޾��� �� ���Ͽ� �Ʒ� ����� ����
			{
				if (*(*(board_horse + row_cursor) + col_cursor) == 1 && (selected == 0 || re == 1))  //spacebar�� �Է¹޾Ұ� �� �ڸ��� �ڽ��� ���� ���� ��, �׸��� ���� ���� �������� �ʾҰų�, ���� �缱������ �� �Ʒ� ������� �����ϰ� ��
				{
					selected = 1; //���� ���õ� ���̹Ƿ� selected������ ���� 1�� ����
					*(*(board_horse + row_cursor) + col_cursor) = 2;   // ���õ� �ڸ��� ���� ���¸� ���õ� player1�� ���� ��������
					if (re == 1)              //�缱���� ��쿡 �Ʒ� ���� ����
					{
						*(*(board_horse + remember_row) + remember_col) = 1;//�缱���� ��쿡, ���� �����ߴ� ���� ���´� ���ùޱ� ���� ���·� �����ش�.
					}
					remember_row = row_cursor;
					remember_col = col_cursor;          //���� ���ù����� ���ù��� ���� ��ġ�� ����

				}
				else if (*(*(board_horse + row_cursor) + col_cursor) == 1 && selected == 1) //�̹� ���ù��� ���¿��� �ٸ� ���� �� �����Ѵٸ�, ������ �����ߴ� ���� ����ϰ� �� ���� �����Ѵ�.
				{

					*(*(board_horse + remember_row) + remember_col) = 1;//���� �����ߴ� ���� ���´� ���ùޱ� ���� ���·� �����ش�.
					*(*(board_horse + row_cursor) + col_cursor) = 2;   // ���õ� �ڸ��� ���� ���¸� ���õ� player1�� ���� ��������
					re = 1;          //�������� ������ ���� �缱���̹Ƿ� re������ 1 �������ش�.
					remember_row = row_cursor;
					remember_col = col_cursor;          //���� �����ϸ� ���ù��� ���� ��ġ�� ����


				}
				else if (*(*(board_horse + row_cursor) + col_cursor) == 0 && selected == 1)   //���� ������ ���¿���, ����ִ� ���� Ŀ���� ����Ų ä spacebar�� �Է��Ѵٸ� ���� �̵��ϰų� �����Ѱ����� ����.
				{
					*last_row_cursor1 = row_cursor;
					*last_col_cursor1 = col_cursor;                               //���������� ������ Ŀ���� ��ġ�� ����
					between_row = remember_row - row_cursor;                     //������ ���� ���� �ű� ��ġ ������ �Ÿ��� ���
					between_col = remember_col - col_cursor;
					if (between_row < 0)                     //�Ÿ��� ������ ������ ����� �ٲ���
						between_row = -between_row;
					if (remember_col < 0)
						remember_col = -remember_col;
					if (between_row <= 1 && between_col <= 1)
					{
						*(*(board_horse + row_cursor) + col_cursor) = 1;   // ��� ���� �Ÿ��� �� �� 1 ������ ���, 1ĭ ������ ���̹Ƿ� �� ��ġ�� ���� �������ش�.
						*(*(board_horse + remember_row) + remember_col) = 1;
						infect(row_cursor, col_cursor, board_horse, turn);
						*(*(board_cursor + row_cursor) + col_cursor) = 0; //������ ������ִ� Ŀ���� ��ġ�� �����ϸ� ��, ������ Ŀ���� �ִ� �ڸ����� �����ǿ��� []�� ��µǴ°��� ���� ���� 0���� �ʱ�ȭ�����ش�.
					}
					else if (between_row <= 2 && between_col <= 2)  // ��� ���� �Ÿ��� �ϳ��� 1 �̻��� ���¿���, ��� ���� �Ÿ��� �� �� 2 ������ ���, 2ĭ ������ ���̹Ƿ� ������ ���� ������ ��ġ�� �Ű��ش�.
					{
						*(*(board_horse + remember_row) + remember_col) = 0;
						*(*(board_horse + row_cursor) + col_cursor) = 1;
						infect(row_cursor, col_cursor, board_horse, turn);
						*(*(board_cursor + row_cursor) + col_cursor) = 0; //������ ������ִ� Ŀ���� ��ġ�� �����ϸ� ��, ������ Ŀ���� �ִ� �ڸ����� �����ǿ��� []�� ��µǴ°��� ���� ���� 0���� �ʱ�ȭ�����ش�.
					}


				}

			}
		}
		return change_turn(board_horse, row, col, turn);           //change_turn�Լ��� ȣ���Ͽ� ������ ��� �Ǵ���(������ �ٲ� ������ �ٲ��� ���� ������)���� �� �� �� ���� ��ȯ�Ѵ�.
	}
	if (turn == 2)       //player2�� �����϶��� ���Ͽ�
	{
		row_cursor = *last_row_cursor2;                 //turn1 ��Ȳ���� �̸� *last_row_cursor2 ���� *last_col_cursor2 ���� ���س����Ƿ� �̷��� �Ѵ�.
		col_cursor = *last_col_cursor2;
		*(*(board_cursor + row_cursor) + col_cursor) = 1; //Ŀ���� �ִ� ���� 1(Ŀ���� �ִٴ� ��)�� ��������
		while (input != ' ' || *(*(board_horse + row_cursor) + col_cursor) != 3)  //spacebar�� ������ ������ ��� Ű�� �Է¹���, spacebar�� �������� �� �ڸ��� �ڽ��� ���� ������ �ٽ� �Է¹���
		{
			system("cls");
			print_board(board_cursor, board_horse, score1, score2, row, col, turn);

			input = _getch();
			if (input == 'i')    //���� ������ �� ��
			{
				if (row_cursor != 2)   //���� ������ �� �� �ٷ� ���� ���������� Ŀ���� ��ġ�� �ű��� �ʴ´�. (row[2]�� ���� �����ִ� ���̴�)
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //Ŀ���� ��ġ�� �ű�� ���Ͽ� ���� Ŀ���� �ִ� ��ġ�� ���.
					row_cursor--;                         //���� ������ �� �� Ŀ���� ��ġ�� ���� �� ĭ �ø�
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'k')    //�Ʒ��� ������ �� ��
			{
				if (row_cursor != row - 3)   //�Ʒ��� ������ �� �� �ٷ� �Ʒ��� ���������� Ŀ���� ��ġ�� �ű��� �ʴ´�. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //Ŀ���� ��ġ�� �ű�� ���Ͽ� ���� Ŀ���� �ִ� ��ġ�� ���.
					row_cursor++;                       //�Ʒ��� ������ �� �� Ŀ���� ��ġ�� �� ĭ ����
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'j')    //�������� ������ �� ��
			{
				if (col_cursor != 2)   //�������� ������ �� �� �ٷ� ������ ���������� Ŀ���� ��ġ�� �ű��� �ʴ´�. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //Ŀ���� ��ġ�� �ű�� ���Ͽ� ���� Ŀ���� �ִ� ��ġ�� ���.
					col_cursor--;                       //�������� ������ �� �� Ŀ���� ��ġ�� �������� �� ĭ ������
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'l')    //���������� ������ �� ��
			{
				if (col_cursor != col - 3)   //���������� ������ �� �� �ٷ� �������� ���������� Ŀ���� ��ġ�� �ű��� �ʴ´�. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //Ŀ���� ��ġ�� �ű�� ���Ͽ� ���� Ŀ���� �ִ� ��ġ�� ���.
					col_cursor++;                       //���������� ������ �� �� Ŀ���� ��ġ�� ���������� �� ĭ ������
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			if (input == ' ')          //������ �Է¹޾��� �� ���Ͽ� �Ʒ� ����� ����
			{
				if (*(*(board_horse + row_cursor) + col_cursor) == 3 && (selected == 0 || re == 1))  //spacebar�� �Է¹޾Ұ� �� �ڸ��� �ڽ��� ���� ���� ��, �׸��� ���� ���� �������� �ʾҰų�, ���� �缱������ �� �Ʒ� ������� �����ϰ� ��
				{
					selected = 1; //���� ���õ� ���̹Ƿ� selected������ ���� 1�� ����
					*(*(board_horse + row_cursor) + col_cursor) = 4;   // ���õ� �ڸ��� ���� ���¸� ���õ� player1�� ���� ��������
					if (re == 1)              //�缱���� ��쿡 �Ʒ� ���� ����
					{
						*(*(board_horse + remember_row) + remember_col) = 3;//�缱���� ��쿡, ���� �����ߴ� ���� ���´� ���ùޱ� ���� ���·� �����ش�.
					}
					remember_row = row_cursor;
					remember_col = col_cursor;          //���� ���ù����� ���ù��� ���� ��ġ�� ����

				}
				else if (*(*(board_horse + row_cursor) + col_cursor) == 3 && selected == 1) //�̹� ���ù��� ���¿��� �ٸ� ���� �� �����Ѵٸ�, ������ �����ߴ� ���� ����ϰ� �� ���� �����Ѵ�.
				{

					*(*(board_horse + remember_row) + remember_col) = 3;//���� �����ߴ� ���� ���´� ���ùޱ� ���� ���·� �����ش�.
					*(*(board_horse + row_cursor) + col_cursor) = 4;   // ���õ� �ڸ��� ���� ���¸� ���õ� player1�� ���� ��������
					re = 1;          //�������� ������ ���� �缱���̹Ƿ� re������ 1 �������ش�.
					remember_row = row_cursor;
					remember_col = col_cursor;          //���� �����ϸ� ���ù��� ���� ��ġ�� ����

				}
				else if (*(*(board_horse + row_cursor) + col_cursor) == 0 && selected == 1)   //���� ������ ���¿���, ����ִ� ���� Ŀ���� ����Ų ä spacebar�� �Է��Ѵٸ� ���� �̵��ϰų� �����Ѱ����� ����.
				{
					*last_row_cursor2 = row_cursor;
					*last_col_cursor2 = col_cursor;                               //���������� ������ Ŀ���� ��ġ�� ����
					between_row = remember_row - row_cursor;                     //������ ���� ���� �ű� ��ġ ������ �Ÿ��� ���
					between_col = remember_col - col_cursor;
					if (between_row < 0)                     //�Ÿ��� ������ ������ ����� �ٲ���
						between_row = -between_row;
					if (remember_col < 0)
						remember_col = -remember_col;
					if (between_row <= 1 && between_col <= 1)
					{
						*(*(board_horse + row_cursor) + col_cursor) = 3;   // ��� ���� �Ÿ��� �� �� 1 ������ ���, 1ĭ ������ ���̹Ƿ� �� ��ġ�� ���� �������ش�.
						*(*(board_horse + remember_row) + remember_col) = 3;
						infect(row_cursor, col_cursor, board_horse, turn);
						*(*(board_cursor + row_cursor) + col_cursor) = 0; //������ ������ִ� Ŀ���� ��ġ�� �����ϸ� ��, ������ Ŀ���� �ִ� �ڸ����� �����ǿ��� []�� ��µǴ°��� ���� ���� 0���� �ʱ�ȭ�����ش�.
					}
					else if (between_row <= 2 && between_col <= 2)  // ��� ���� �Ÿ��� �ϳ��� 1 �̻��� ���¿���, ��� ���� �Ÿ��� �� �� 2 ������ ���, 2ĭ ������ ���̹Ƿ� ������ ���� ������ ��ġ�� �Ű��ش�.
					{
						*(*(board_horse + remember_row) + remember_col) = 0;
						*(*(board_horse + row_cursor) + col_cursor) = 3;
						infect(row_cursor, col_cursor, board_horse, turn);
						*(*(board_cursor + row_cursor) + col_cursor) = 0; //������ ������ִ� Ŀ���� ��ġ�� �����ϸ� ��, ������ Ŀ���� �ִ� �ڸ����� �����ǿ��� []�� ��µǴ°��� ���� ���� 0���� �ʱ�ȭ�����ش�.
					}

				}

			}
		}
		return change_turn(board_horse, row, col, turn);           //change_turn�Լ��� ȣ���Ͽ� ������ ��� �Ǵ���(������ �ٲ� ������ �ٲ��� ���� ������)���� �� �� �� ���� ��ȯ�Ѵ�.
	}
}

int change_turn(int** board_horse, int row, int col, int turn)         //�÷��̾��� ������ �ٲٴ��� Ȯ���ϴ� �Լ�, �÷��̾��� �� �� ������ �� �ִ� ���� ���ٸ� �� �÷��̾��� ������ ���ƿ��� �ʰ� ���� �÷��̾ ����ؼ� ������ �����Ѵ�. player1�� ������ 1�� ��ȯ, player2�� ������ 2�� ��ȯ�Ѵ�.
{
	int i, j, k, l;           //�ݺ������� ����� ����
	if (turn == 1)          //���� player1�� �����̸� �Ʒ� ����� ����
	{
		for (i = 0; i < row; i++)
			for (j = 0; j < col; j++)
			{
				if (*(*(board_horse + i) + j) == 3)         //��� player2�� ���� ���Ͽ�, �� ���� ������ �� �ִ� ������, �׷��� ���� ������ �˻��� ���̴�.
					for (k = -2; k < 3; k++)
						for (l = -2; l < 3; l++)
						{
							if (*(*(board_horse + i + k) + j + l) != 0) //player2�� ���� ���� 1ĭ,2ĭ ������ ���� �� �� ������ ���ٸ� �� ���� ������ �� ���� ��.�׷��Ƿ� ������ ���ư��� �ʰ� �״�� player1�� �����Ѵ�.

								turn = 1;

							else
								return 2;              //player2�� �� �� �Ѱ��� ������ �� �ִ� ���� ������ ������ player2�� �ٲ��ش�. 
						}
			}
	}
	if (turn == 2)          //���� player2�� �����̸� �Ʒ� ����� ����
	{
		for (i = 0; i < row; i++)
			for (j = 0; j < col; j++)
			{
				if (*(*(board_horse + i) + j) == 1)         //��� player1�� ���� ���Ͽ�, �� ���� ������ �� �ִ� ������, �׷��� ���� ������ �˻��� ���̴�.
					for (k = -2; k < 3; k++)
						for (l = -2; l < 3; l++)
						{
							if (*(*(board_horse + i + k) + j + l) != 0) //player1�� ���� ���� 1ĭ,2ĭ ������ ���� �� �� ������ ���ٸ� �� ���� ������ �� ���� ��.�׷��Ƿ� ������ ���ư��� �ʰ� �״�� player2�� �����Ѵ�.

								turn = 2;

							else
								return 1;              //player1�� �� �� �Ѱ��� ������ �� �ִ� ���� ������ ������ player1�� �ٲ��ش�. 
						}
			}
	}
	return turn;
}
void infect(int row_cursor, int col_cursor, int** board_horse, int turn)          //�̵���Ų �� ��ó�� �ٸ� �÷��̾��� ������ �̵���Ų �÷��̾��� ���� �ٲٴ� �Լ�
{
	if (turn == 1)         //player1�� ������ �Ʒ� ����� ����
	{
		if (*(*(board_horse + row_cursor + 1) + col_cursor + 1) == 3)         //Ŀ���κ��� 1ĭ ������ �ִ� player2�� ������ ���� player1�� ���� �ٲ��ش�.
			*(*(board_horse + row_cursor + 1) + col_cursor + 1) = 1;
		if (*(*(board_horse + row_cursor + 1) + col_cursor) == 3)
			*(*(board_horse + row_cursor + 1) + col_cursor) = 1;
		if (*(*(board_horse + row_cursor + 1) + col_cursor - 1) == 3)
			*(*(board_horse + row_cursor + 1) + col_cursor - 1) = 1;
		if (*(*(board_horse + row_cursor) + col_cursor + 1) == 3)
			*(*(board_horse + row_cursor) + col_cursor + 1) = 1;
		if (*(*(board_horse + row_cursor) + col_cursor - 1) == 3)
			*(*(board_horse + row_cursor) + col_cursor - 1) = 1;
		if (*(*(board_horse + row_cursor - 1) + col_cursor + 1) == 3)
			*(*(board_horse + row_cursor - 1) + col_cursor + 1) = 1;
		if (*(*(board_horse + row_cursor - 1) + col_cursor) == 3)
			*(*(board_horse + row_cursor - 1) + col_cursor) = 1;
		if (*(*(board_horse + row_cursor - 1) + col_cursor - 1) == 3)
			*(*(board_horse + row_cursor - 1) + col_cursor - 1) = 1;
	}
	if (turn == 2)         //player2�� ������ �Ʒ� ����� ����
	{
		if (*(*(board_horse + row_cursor + 1) + col_cursor + 1) == 1)         //Ŀ���κ��� 1 ĭ ������ �ִ� player1�� ������ ���� player2�� ���� �ٲ��ش�.
			*(*(board_horse + row_cursor + 1) + col_cursor + 1) = 3;
		if (*(*(board_horse + row_cursor + 1) + col_cursor) == 1)
			*(*(board_horse + row_cursor + 1) + col_cursor) = 3;
		if (*(*(board_horse + row_cursor + 1) + col_cursor - 1) == 1)
			*(*(board_horse + row_cursor + 1) + col_cursor - 1) = 3;
		if (*(*(board_horse + row_cursor) + col_cursor + 1) == 1)
			*(*(board_horse + row_cursor) + col_cursor + 1) = 3;
		if (*(*(board_horse + row_cursor) + col_cursor - 1) == 1)
			*(*(board_horse + row_cursor) + col_cursor - 1) = 3;
		if (*(*(board_horse + row_cursor - 1) + col_cursor + 1) == 1)
			*(*(board_horse + row_cursor - 1) + col_cursor + 1) = 3;
		if (*(*(board_horse + row_cursor - 1) + col_cursor) == 1)
			*(*(board_horse + row_cursor - 1) + col_cursor) = 3;
		if (*(*(board_horse + row_cursor - 1) + col_cursor - 1) == 1)
			*(*(board_horse + row_cursor - 1) + col_cursor - 1) = 3;
	}

}


void print_board(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn)    //������Ȳ, ����, �������� ����ϴ� �Լ�
{
	int i, j;
	printf("[Player 1] %d : %d [Player2]", *score1, *score2);
	printf("\n\n[Player %d 's Turn]\n", turn);
	for (j = 0; j < row - 4; j++)
	{
		for (i = 0; i < col - 4; i++)
			printf("+---");
		printf("+\n");
		for (i = 0; i < col - 4; i++)
		{
			if (*(*(board_horse + j + 2) + i + 2) == 1)   //player1�� ���� ����� ��.
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //Ŀ���� �ִ� ���� []�� ������ش�.
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");;
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else if (*(*(board_horse + j + 2) + i + 2) == 0)  //����� ����� ��.
			{
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //Ŀ���� �ִ� ���� []�� ������ش�.
					printf("[ ]");
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

					printf("   ");
				}
			}
			else if (*(*(board_horse + j + 2) + i + 2) == 2)  //���õ� player1�� ���� ����Ѵ�.
			{
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //Ŀ���� �ִ� ���� []�� ������ش�.
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");;
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else if (*(*(board_horse + j + 2) + i + 2) == 3) // player2�� ���� ����� ��
			{
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //Ŀ���� �ִ� ���� []�� ������ش�.
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");;
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
			else if (*(*(board_horse + j + 2) + i + 2) == 4) //���õ� player2�� ���� ����� ��
			{
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //Ŀ���� �ִ� ���� []�� ������ش�.
				{
					printf("[");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf("@");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
					printf("]");;
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
					printf(" @ ");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
			}
		}
		printf("|\n");

	}
	for (i = 0; i < col - 4; i++)
		printf("+---");
	printf("+");
}

void init_board(int** board_cursor, int** board_horse, int row, int col)   //�� ó�� �������� ���� �� �ʱ� ���ǿ� ���� ������ ���� ������ �ʱ�ȭ �����ִ� �Լ�
{
	int i; //�ݺ����� ����� ����
	for (i = 0; i < col; i++)
	{
		*(*board_horse + i) = 9;                //�迭�� 9�� ����ִٸ�, ������ ���� ������ ������ ���̴�. ������ ���� �迭�� �ƴ� ������ ���� ������ 9�� �־ ���Ƶΰ� ���� �������� �����ϰ� �� ���̴�.
		*(*(board_horse + 1) + i) = 9;          //������ ���� �迭�� �ƴ� ������ ���� ������ 9�� �־ ���Ƶд�.
		*(*(board_horse + row - 2) + i) = 9;
		*(*(board_horse + row - 1) + i) = 9;
	}
	for (i = 0; i < row; i++)
	{
		*(*(board_horse + i)) = 9;       //�迭�� 9�� ����ִٸ�, ������ ���� ������ ������ ���̴�.
		*(*(board_horse + i) + 1) = 9;   //������ ���� �迭�� �ƴ� ������ ���� ������ 9�� �־ ���Ƶд�.
		*(*(board_horse + i) + col - 2) = 9;
		*(*(board_horse + i) + col - 1) = 9;
	}
	*(*(board_horse + 2) + 2) = 1;//������ ����� �迭�� (0,0)�� (0,col-1)������ player1�� �� ���´�.
	*(*(board_horse + 2) + col - 3) = 1; //������ ����� �迭�� (0,0)�� (0,col-1)������ player1�� �� ���´�.
	*(*(board_horse + row - 3) + 2) = 3;//������ ����� �迭�� (row-1,0)�� (row-1,col-1)������ player2�� �� ���´�.
	*(*(board_horse + row - 3) + col - 3) = 3; //������ ����� �迭�� (row-1,0)�� (row-1,col-1)������ player2�� �� ���´�.
	*(*(board_cursor + 2) + 2) = 1;          //�� ó�� Ŀ���� ��ġ ����
}
