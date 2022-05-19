//개발환경: visual studio 2019
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
	int board[4][4] = { 0 };        //보드 배열
	int stateboard[4][4] = { 0 };  //보드의 상태를 나타내는 배열, 0이면 닫힌 상태, 1이면 오픈 상태
	int turn;                 //현재 누구차례인지 나타내는 변수, 1이면 player 1   2이면 player2
	int score1 = 0, score2 = 0;   //플레이어의 점수를 나타내는 변수
	char YesOrNo='y';             //재시작할지 안할지 입력받는 변수
	srand(time(NULL));
	while (YesOrNo == 'y')        //재시작 문구에서 y를 입력받는 동안 계속해서 게임을 시작함
	{
		reset(board, stateboard); //게임 재시작시 board와 stateboard를 0으로 초기화시켜주는 함수 호출
		init_board(board);
		turn = rand() % 2 + 1;    //누가 먼저 시작할지 결정
		print_iniboard(board); 
		Sleep(3000);
		system("cls");
		print_board(stateboard, board);
		while (score1 + score2 < 8)         //총점이 8점이 될 때 까지 반복해서 입력받음
		{
			turn = input(&score1, &score2, stateboard, turn, board);
		}                   //while이 끝나면 총점이 8점 이라는 것이므로 게임이 종료된 상태
		PrintWinner(score1, score2);  //게임이 종료되면 누가 승자인지 출력
		printf("Continue? (y/n) ");        
		scanf(" %c", &YesOrNo);            //게임이 종료되면 게임을 계속할 것인지 말 것인지 입력받음
		if (YesOrNo == 'n')
			break;                   //n을 입력받으면 while문을 빠져나오고 프로그램 종료
		score1 = 0;
		score2 = 0;        //게임이 끝나고 재시작한다면 점수를 초기화시켜줌
		printf("\nNew Game!\n");
	}
	printf("Good bye");       //맨 위의 while문이 끝나면 게임 종료
	return 0;
	
}

void init_board(int board[][4])      //보드판을 초기화하는 함수
{
	int raw,raw2;
	int col,col2;    //행과 열을 나타낼 변수 선언
	int i;      //반복문에 이용할 변수
	
		for (i=1;i<=8;)
		{
			raw = rand() % 4;                  //1을 넣을 좌표2개를 랜덤으로 생성, 2를 넣을 좌표 2개를 랜덤으로 생성...~~ 8을 넣을 좌표 2개를 랜덤으로 생성한다
			col = rand() % 4;
			raw2 = rand() % 4;
			col2= rand() % 4;
			
			while (!(raw == raw2 && col == col2)&& board[raw][col] == 0&& board[raw2][col2] == 0)  //첫번째 좌표와 두번 째 좌표의 값이 같다면 다시 좌표를 생성한다. 또한 수를 넣으려는 좌표에 0이 아닌 다른 수가 들어있다면, 이미 채운 좌표이므로 다시 좌표를 생성한다. 그렇지 않은 경우에는 좌표에 값을 대입해준다.
			{
					board[raw][col] = i;
					board[raw2][col2] = i;
					i++;
			}
		}

		return;
}
void print_board(int stateboard[][4],int board[][4] )   //보드판을 출력하는 함수
{
	int raw;
	int col;   //행과 열을 나타낼 변수
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
void print_iniboard(int board[][4])       //게임 시작 시 화면을 출력하는 함수
{
	int raw;
	int col;   //행과 열을 나타낼 변수
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
int input(int* score1, int* score2, int stateboard[][4],int turn,int board[][4])   //좌표를 입력받고, 조건을 검사하여 점수를 올린다. 그리고 플레이어의 순서를 반환한다.
{
	int vector1_1, vector1_2, vector2_1, vector2_2;   //입력받은 좌표를 저장할 변수
	if (turn == 1)
		printf("[player 1]: ");
	else
		printf("[player 2]: ");
	scanf("%d%d%d%d", &vector1_1, &vector1_2, &vector2_1, &vector2_2);
	while (check_input(vector1_1, vector1_2, vector2_1, vector2_2, stateboard) == 1)  //비정상적인 입력을 받은 경우 다시 입력받음
	{
		if (turn == 1)
			printf("[player 1]: ");
		else
			printf("[player 2]: ");
		scanf("%d%d%d%d", &vector1_1, &vector1_2, &vector2_1, &vector2_2);
	}  
	stateboard[vector1_1][vector1_2] = 1;
	stateboard[vector2_1][vector2_2] = 1;               //일시적으로 보드의 상태를 오픈으로 만들어 줘서 입력받은 값에 어떤 숫자가 들어있었는지 출력해준다. 입력받은 좌표에 들은 값이 서로 같다면 그대로 오픈상태로 해주면 되고, 다른 경우에는 다시 오픈상태를 돌려놓아야 한다.
	print_board(stateboard,board);
	if (board[vector1_1][vector1_2] == board[vector2_1][vector2_2])         //입력한 좌표에 들은 값이 같으면 점수 올린다. 
	{
		if (turn == 1)
		{
			(*score1)++;
			printf("\nplayer 1 get score!\n");
			printline();
			printf("[player 1] %d : %d [player 2]\n", *score1, *score2);
			printline();
			if((*score1)+(*score2)<8)  //score1과 score2를 더해서 8점 이상인 경우에는 게임이 끝났으므로 player get one more chance를 출력할 필요가 없다.
			printf("\nplayer 1 get one more chance!!\n");
		}
		else
		{
			(*score2)++;
			printf("\nplayer 2 get score!\n");
			printline();
			printf("[player 1] %d : %d [player 2]\n", *score1, *score2);
			printline();
			if ((*score1) + (*score2) < 8)  //score1과 score2를 더해서 8점 이상인 경우에는 게임이 끝났으므로 player get one more chance를 출력할 필요가 없다.
			printf("\nplayer 2 get one more chance!!\n");
		}
	}
	else         //입력한 좌표에 든 값이 다르면 순서를 바꿔준다.
	{
		stateboard[vector1_1][vector1_2] = 0;
		stateboard[vector2_1][vector2_2] = 0;        //오픈상태를 원래대로 돌려놓는다.
		printf("Mismatch!");
		Sleep(3000);
		system("cls");
		print_board(stateboard, board);
		if (turn == 1)
			turn = 2;
		else
			turn = 1;   //입력한 좌표에 든 값이 다르면 순서를 바꿔준다.
	}
	return turn;                           
}
int check_input(int vector1_1, int vector1_2, int vector2_1, int vector2_2, int stateboard[][4])  //입력된 값이 올바른 입력인지 확인하는 함수
{
	if (vector1_1 == vector2_1 && vector1_2 == vector2_2)
	{
		printf("\nsame card... try again..\n");   //같은 카드 선택했을 경우
		return 1;   //비정상적인 입력의 경우 1 반환
	}
	else if (stateboard[vector1_1][vector1_2] == 1 || stateboard[vector2_1][vector2_2] == 1)
	{
		printf("\nalready open... try again..\n");             //두개의 선택 중 하나라도 이미 열려있는 카드인 경우
		return 1;   //비정상적인 입력은 1을 반환
	}
	else
		return 0;         //정상적인 입력은 0을 반환

}
void printline()  //'-'을 적절한 개수만큼 출력하는 함수
{
	printf("--------------------------\n");
	return;
}

void reset(int board[][4], int stateboard[][4])       //게임을 다시 시작할 때 board와 stateboard를 다시 0으로 초기화시켜주는 함수
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
void PrintWinner(int score1, int score2)    //게임의 결과를 출력하는 함수
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
		printf("\nGame End! Draw!\n");                 //게임 종료 후 각 플레이어의 점수에 따라 적절한 화면을 출력한다.
	return;

}