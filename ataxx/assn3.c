//개발환경:visual studio 2019
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<Windows.h>
#include<stdlib.h>
void get_data(int* row, int* col);
int start_game(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn);
void print_board(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn);
void init_board(int** board_cursor, int** board_horse, int row, int col);  //보드 만들어 진 후 초기 상태 초기화해주는 함수
int select_move(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn);
void what_is_score(int** board_horse, int row, int col, int* score1, int* score2);
void infect(int row_cursor, int col_cursor, int** board_horse, int turn);       //근처의 다른 말들을 감염시키는(자기 말들로 바꾸는 )함수
int change_turn(int** board_horse, int row, int col, int turn);
int is_game_end(int row, int col, int* score1, int* score2, int** board_horse);

int main()
{
	int i;
	int** board_cursor = NULL;        //보드판 커서의 위치를 나타내는 배열, 1이 있는 곳에 커서가 있다. 커서가 없는 곳은 0
	int** board_horse = NULL;          /*보드판 어디에 말이 있는지를 나타내는 배열,1이 들어있으면 player1의 말,
									 2가 들어있으면 선택된 player1의 말, 3이 들어있으면 player2의 말, 4가 들어있으면 선택된
									  player2의 말이 들어잇는것으로 가정*/

	int score1, score2;        //player1 과 player2의 점수를 나타내는 변수 
	int row, col;                   //입력받을 행과 열을 저장할 변수
	int turn = 1; //값이 1이면 player1의 순서, 2이면 player2의 순서
	int last_row_cursor1;
	int last_col_cursor1;
	int last_row_cursor2;
	int last_col_cursor2; //마지막으로 선택한 커서의 위치를 저장할 변수들
	int end_of_game = 1; //start_game 함수의 반환값을 저장할 변수, 1이면 계임을 계속 진행하는 것이고 0이면 게임 종료
	while (end_of_game == 1)              //start_game에서 0을 반환받기 이전까지는 계속 게임 실행
	{
		score1 = 0; score2 = 0; //게임 시작할 때는 player1의 점수와 player2의 점수를 일단 0으로 해놓는다.
		last_row_cursor1 = 0;
		last_col_cursor1 = 0;
		last_row_cursor2 = 0;
		last_col_cursor2 = 0; //마지막으로 선택한 커서의 위치를 저장할 변수들, select_move 함수의 조건문에서 이용하기 위해 초기에는 이 변수들의 값을 0으로 초기화해줌.
		get_data(&row, &col);  //게임판 크기 입력받음
		board_cursor = (int**)calloc(row, sizeof(int*)); // 메모리 할당
		for (i = 0; i < row; i++)
			*(board_cursor + i) = (int*)calloc(col, sizeof(int));
		board_horse = (int**)calloc(row, sizeof(int*));
		for (i = 0; i < row; i++)
			*(board_horse + i) = (int*)calloc(col, sizeof(int));      //메모리 할당
		init_board(board_cursor, board_horse, row, col);        //게임 시작했을 때 게임판을 초기조건에 따라 초기화
		end_of_game = start_game(board_cursor, board_horse, &score1, &score2, row, col, turn, &last_row_cursor1, &last_col_cursor1, &last_row_cursor2, &last_col_cursor2);  //게임 시작, 그리고 재시작 여부를 반환받아end_of_game에 저장
		for (i = 0; i < row; i++)
			free(*(board_cursor + i));
		for (i = 0; i < row; i++)
			free(*(board_horse + i));
		free(board_horse);
		free(board_cursor);//할당했던 메모리 해제
	}
	return 0;
}
void get_data(int* row, int* col)   //보드판의 행과 열의 개수를 입력받는 함수
{
	int i; //반복문에 사용할 변수
	*row = 0;
	*col = 0;
	printf(" [ATAXX]\n");
	while (*row < 4 || *row>20)         //입력받은 값이 4이상 20미만이여야 함
	{
		printf("row: ");
		scanf("%d", row);
	}
	while (*col < 4 || *col>20)        //입력받은 값이 4이상 20미만이여야 함
	{
		printf("col: ");
		scanf("%d", col);
	}
	*row += 4;
	*col += 4; //양옆과 위 아래에 2칸만큼의 여유공간을 만들어주기 위해(나머지 여유 공간은 막아두고, 움직일 수 없는 공간으로 가정할 것임) 실제로 입력받은것보다 4개씩 더 만들어 준다.


}

int start_game(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn, int* last_row_cursor1, int* last_col_cursor1, int* last_row_cursor2, int* last_col_cursor2)   //게임을 시작하는 함수, 게임이 종료된 후 게임을 계속 진행할 것이면 1을 반환 그렇지 않으면 0을 반환
{
	char yesorno = '0';         //게임을 계속할 것인지 입력받은 값을 저장할 변수, 반복문의 조건문에서 이용하기 위해 우선 값을 초기화시켜줌
	while (is_game_end(row, col, score1, score2, board_horse) == 0)            //게임이 끝난는지 확인하는 is_game_end함수를 호출하여 반환값이 0이면 게임이 끝나지 않은 것이므로 계속 진행
	{
		system("cls");                       //화면을 지움
		what_is_score(board_horse, row, col, score1, score2);             //바로 아래의 print_board에서 점수를 출력하기 때문에 우선 점수를 계산해준다.
		print_board(board_cursor, board_horse, score1, score2, row, col, turn);
		turn = select_move(board_cursor, board_horse, score1, score2, row, col, turn, last_row_cursor1, last_col_cursor1, last_row_cursor2, last_col_cursor2);    //말을 선택하고 이동시킴 그리고 플레이어의 순서를 반환
		system("cls");
		what_is_score(board_horse, row, col, score1, score2);            //while의 조건식인 is_game_end 함수에서 점수를 이용하기 때문에 select_move함수를 호출한 후 점수를 계산해 준다.
	}
	print_board(board_cursor, board_horse, score1, score2, row, col, turn);   //게임이 종료돼서 while이 끝난 후에도 게임판을 보여줘야 하므로 다시 게임판을 출력해준다.
	if (*score1 > *score2)
		printf("\nPlayer 1 wins\nContinue? (y/n)\n");             //게임이 종료되면 점수에 따라 결과를 출력하고 재시작할 것인지 말 것인지 입력받음
	else if (*score1 < *score2)
		printf("\nPlayer 2 wins\nContinue? (y/n)\n");
	else
		printf("\nDraw\nContinue? (y/n)\n");
	while (yesorno != 'y' && yesorno != 'n')          //y혹은 n을 입력받기 전 까지 계속 입력받음
	{
		scanf(" %c", &yesorno);
		if (yesorno == 'y')
		{
			system("cls");              //게임을 재시작할 것이라면 새 화면을 출력해주기 위해 이전까지의 화면을 전부 지워준다.   
			return 1;           //게임 계속 진행할 것이면 1 반환
		}
		else if (yesorno == 'n')
			return 0;     //게임 종료할 것이면 0 반환
	}
}
int is_game_end(int row, int col, int* score1, int* score2, int** board_horse)      //게임이 종료됐는지 확인하는 함수, 게임이 종료되면 0을 반환, 그렇지 않다면 1을 반환
{
	int i, j;   //반복문에서 이용할 변수
	int exist1 = 0, exist2 = 0;           //player1과 player2의 말이 존재하는지 확인할 변수, 존재하면 1, 존재하지 않으면 0으로 할 것이다.
	if (*score1 + *score2 == (row - 4) * (col - 4))           //총점이 사용가능한 보드판 개수와 같으면, 즉 보드판이 가득 차면 게임 종료이므로 1반환
		return 1;
	for (i = 0; i < row; i++)           //모든 보드판의 말에 대하여 검사한다.
		for (j = 0; j < col; j++)
		{
			if (*(*(board_horse + i) + j) == 1)                   //보드판 말의 상태가1이면 player1의 말이 있는 것이므로 exist1변수의 값을 1로 한다.
				exist1 = 1;
			else if (*(*(board_horse + i) + j) == 3)                   //보드판 말의 상태가3이면 player2의 말이 있는 것이므로 exist2변수의 값을 1로 해준다.
				exist2 = 2;
		}
	if (exist1 == 1 && exist2 == 2)          //player1의 말과 player2의 말이 모두 존재한다면 게임이 끝나지 않은 것이므로 0을 반환한다.
		return 0;
	else
		return 1;                             //그렇지 않은 경우 게임이 끝난 상황이므로 1을 반환한다.
}

void what_is_score(int** board_horse, int row, int col, int* score1, int* score2)      //현재 각 플레이어의 점수 구하는 함수
{
	int i, j;     //반복문에 이용할 변수
	*score1 = 0, * score2 = 0;
	for (i = 0; i < row; i++)
		for (j = 0; j < col; j++)
		{
			if (*(*(board_horse + i) + j) == 1 || *(*(board_horse + i) + j) == 2)                   //보드판 말의 상태가1이거나 2이면 player1의 말이 있는 것이므로 score1변수의 값을 증가시킴
				(*score1)++;
			else if (*(*(board_horse + i) + j) == 3 || *(*(board_horse + i) + j) == 4)                   //보드판 말의 상태가3이거나 4이면 player2의 말이 있는 것이므로 score2변수의 값을 증가시킴
				(*score2)++;
		}
}

int select_move(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn, int* last_row_cursor1, int* last_col_cursor1, int* last_row_cursor2, int* last_col_cursor2)     //말을 선택하고 이동시킴 그리고 플레이어의 순서를 반환하는 함수
{
	int selected = 0; //선택했는지 안했는지 판단하는 변수, 0이면 선택안한상태, 1이면 선택한 상태
	char input = '0';    //키 입력받은것을 저장할 변수, 반복문에서 사용하기 위해 의미없는값으로 초기화시켜놓음
	int check;        //is_movable함수의 반환값을 저장하기 위한 변수
	int row_cursor = 0, col_cursor = 0;        //현재 커서가 있는 row와 col을 저장할 변수들
	int remember_row = 0, remember_col = 0;    //선택받은 말의 위치를 기억할 변수
	int re = 0;                        //말을 재선택했는지 안했는지 확인하는 변수, 재선택한 경우 1, 그렇지 않은 경우 0을 저장함.
	int i, j;                          //반복문에 사용할 변수
	int between_row, between_col;         //선택한 말과 말을 옮길 위치 사이의 거리를 나타낼 변수들

	if (turn == 1)       //player1의 순서일때에 대하여
	{
		if (*last_row_cursor1 == 0 && *last_col_cursor1 == 0)         //*last_row_cursor1 == 0 이고 *last_col_cursor == 0 인 경우는 맨 처음 뿐이다. 맨 처음에는 아래의 방식으로 커서의 위치를 정할 것이다. 다음부터는 이번 플레이어의 저번 턴에서의 마지막 커서 위치를 통해 커서위치를 계산할 것.
		{
			for (i = 0; i < row; i++)
			{
				for (j = 0; j < col; j++)
					if (*(*(board_cursor + i) + j) == 1)
					{
						row_cursor = i;
						col_cursor = j;
						*last_row_cursor2 = row - 3;             //player2의 첫 차례에서의 커서의 위치는(row-1,col-1)로 한다.
						*last_col_cursor2 = col - 3;

					}                            //현재 커서의 위치를 저장(board_cursor를 두번 역참조한 수 중 1을 가리키는 곳에 커서가 있다.)
			}
		}
		else  // 이번 플레이어의 저번 턴에서의 마지막 커서 위치를 통해 커서위치를 계산.
		{
			row_cursor = *last_row_cursor1;
			col_cursor = *last_col_cursor1;
			*(*(board_cursor + row_cursor) + col_cursor) = 1; //커서가 있는 곳에 1(커서가 있는 상태)을 대입해줌
		}

		while (input != ' ' || *(*(board_horse + row_cursor) + col_cursor) != 1)  //spacebar를 누르기 전까지 계속 키를 입력받음, spacebar를 누르더라도 그 자리에 자신의 말이 없으면 다시 입력받음
		{
			system("cls");
			print_board(board_cursor, board_horse, score1, score2, row, col, turn);

			input = _getch();
			if (input == 'i')    //위로 가려고 할 때
			{
				if (row_cursor != 2)   //위로 가려고 할 때 바로 위가 막혀있으면 커서의 위치를 옮기지 않는다. (row[2]즉 row[1]의 위쪽은 막혀있는 곳이다)
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //커서의 위치를 옮기기 위하여 원래 커서가 있던 위치는 비움.
					row_cursor--;                         //위로 가려고 할 때 커서의 위치를 위로 한 칸 올림
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'k')    //아래로 가려고 할 때
			{
				if (row_cursor != row - 3)   //아래로 가려고 할 때 바로 아래가 막혀있으면 커서의 위치를 옮기지 않는다. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //커서의 위치를 옮기기 위하여 원래 커서가 있던 위치는 비움.
					row_cursor++;                       //아래로 가려고 할 때 커서의 위치를 한 칸 내림
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'j')    //왼쪽으로 가려고 할 때
			{
				if (col_cursor != 2)   //왼쪽으로 가려고 할 때 바로 왼쪽이 막혀있으면 커서의 위치를 옮기지 않는다. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //커서의 위치를 옮기기 위하여 원래 커서가 있던 위치는 비움.
					col_cursor--;                       //왼쪽으로 가려고 할 때 커서의 위치를 왼쪽으로 한 칸 움직임
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'l')    //오른쪽으로 가려고 할 때
			{
				if (col_cursor != col - 3)   //오른쪽으로 가려고 할 때 바로 오른쪽이 막혀있으면 커서의 위치를 옮기지 않는다. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //커서의 위치를 옮기기 위하여 원래 커서가 있던 위치는 비움.
					col_cursor++;                       //오른쪽으로 가려고 할 때 커서의 위치를 오른쪽으로 한 칸 움직임
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			if (input == ' ')          //공백을 입력받았을 때 대하여 아래 문장들 수행
			{
				if (*(*(board_horse + row_cursor) + col_cursor) == 1 && (selected == 0 || re == 1))  //spacebar를 입력받았고 그 자리에 자신의 말이 있을 때, 그리고 아직 말을 선택하지 않았거나, 말을 재선택했을 때 아래 문장들을 수행하게 됨
				{
					selected = 1; //말이 선택된 것이므로 selected변수의 값을 1로 변경
					*(*(board_horse + row_cursor) + col_cursor) = 2;   // 선택된 자리의 말의 상태를 선택된 player1의 말로 변경해줌
					if (re == 1)              //재선택인 경우에 아래 문장 수행
					{
						*(*(board_horse + remember_row) + remember_col) = 1;//재선택인 경우에, 원래 선택했던 말의 상태는 선택받기 이전 상태로 돌려준다.
					}
					remember_row = row_cursor;
					remember_col = col_cursor;          //말을 선택받으면 선택받은 말의 위치를 저장

				}
				else if (*(*(board_horse + row_cursor) + col_cursor) == 1 && selected == 1) //이미 선택받은 상태에서 다른 말을 또 선택한다면, 이전에 선택했던 말은 취소하고 새 말을 선택한다.
				{

					*(*(board_horse + remember_row) + remember_col) = 1;//원래 선택했던 말의 상태는 선택받기 이전 상태로 돌려준다.
					*(*(board_horse + row_cursor) + col_cursor) = 2;   // 선택된 자리의 말의 상태를 선택된 player1의 말로 변경해줌
					re = 1;          //다음부터 선택은 말의 재선택이므로 re변수에 1 대입해준다.
					remember_row = row_cursor;
					remember_col = col_cursor;          //말을 선택하면 선택받은 말의 위치를 저장


				}
				else if (*(*(board_horse + row_cursor) + col_cursor) == 0 && selected == 1)   //말을 선택한 상태에서, 비어있는 곳을 커서로 가르킨 채 spacebar을 입력한다면 말을 이동하거나 복제한것으로 인지.
				{
					*last_row_cursor1 = row_cursor;
					*last_col_cursor1 = col_cursor;                               //마지막으로 선택한 커서의 위치를 저장
					between_row = remember_row - row_cursor;                     //선택한 말과 말을 옮길 위치 사이의 거리를 계산
					between_col = remember_col - col_cursor;
					if (between_row < 0)                     //거리가 음수가 나오면 양수로 바꿔줌
						between_row = -between_row;
					if (remember_col < 0)
						remember_col = -remember_col;
					if (between_row <= 1 && between_col <= 1)
					{
						*(*(board_horse + row_cursor) + col_cursor) = 1;   // 행과 열의 거리가 둘 다 1 이하인 경우, 1칸 움직인 것이므로 그 위치에 말을 복사해준다.
						*(*(board_horse + remember_row) + remember_col) = 1;
						infect(row_cursor, col_cursor, board_horse, turn);
						*(*(board_cursor + row_cursor) + col_cursor) = 0; //이전에 저장돼있던 커서의 위치만 저장하면 됨, 이전에 커서가 있던 자리에도 보드판에서 []가 출력되는것을 막기 위해 0으로 초기화시켜준다.
					}
					else if (between_row <= 2 && between_col <= 2)  // 행과 열의 거리중 하나라도 1 이상인 상태에서, 행과 열의 거리가 둘 다 2 이하인 경우, 2칸 움직인 것이므로 선택한 말을 선택한 위치로 옮겨준다.
					{
						*(*(board_horse + remember_row) + remember_col) = 0;
						*(*(board_horse + row_cursor) + col_cursor) = 1;
						infect(row_cursor, col_cursor, board_horse, turn);
						*(*(board_cursor + row_cursor) + col_cursor) = 0; //이전에 저장돼있던 커서의 위치만 저장하면 됨, 이전에 커서가 있던 자리에도 보드판에서 []가 출력되는것을 막기 위해 0으로 초기화시켜준다.
					}


				}

			}
		}
		return change_turn(board_horse, row, col, turn);           //change_turn함수를 호출하여 순서가 어떻게 되는지(순서를 바꿀 것인지 바꾸지 않을 것인지)결정 한 후 그 값을 반환한다.
	}
	if (turn == 2)       //player2의 순서일때에 대하여
	{
		row_cursor = *last_row_cursor2;                 //turn1 상황에서 미리 *last_row_cursor2 값과 *last_col_cursor2 값을 정해놨으므로 이렇게 한다.
		col_cursor = *last_col_cursor2;
		*(*(board_cursor + row_cursor) + col_cursor) = 1; //커서가 있는 곳에 1(커서가 있다는 뜻)을 대입해줌
		while (input != ' ' || *(*(board_horse + row_cursor) + col_cursor) != 3)  //spacebar를 누르기 전까지 계속 키를 입력받음, spacebar를 누르더라도 그 자리에 자신의 말이 없으면 다시 입력받음
		{
			system("cls");
			print_board(board_cursor, board_horse, score1, score2, row, col, turn);

			input = _getch();
			if (input == 'i')    //위로 가려고 할 때
			{
				if (row_cursor != 2)   //위로 가려고 할 때 바로 위가 막혀있으면 커서의 위치를 옮기지 않는다. (row[2]의 위는 막혀있는 곳이다)
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //커서의 위치를 옮기기 위하여 원래 커서가 있던 위치는 비움.
					row_cursor--;                         //위로 가려고 할 때 커서의 위치를 위로 한 칸 올림
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'k')    //아래로 가려고 할 때
			{
				if (row_cursor != row - 3)   //아래로 가려고 할 때 바로 아래가 막혀있으면 커서의 위치를 옮기지 않는다. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //커서의 위치를 옮기기 위하여 원래 커서가 있던 위치는 비움.
					row_cursor++;                       //아래로 가려고 할 때 커서의 위치를 한 칸 내림
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'j')    //왼쪽으로 가려고 할 때
			{
				if (col_cursor != 2)   //왼쪽으로 가려고 할 때 바로 왼쪽이 막혀있으면 커서의 위치를 옮기지 않는다. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //커서의 위치를 옮기기 위하여 원래 커서가 있던 위치는 비움.
					col_cursor--;                       //왼쪽으로 가려고 할 때 커서의 위치를 왼쪽으로 한 칸 움직임
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			else if (input == 'l')    //오른쪽으로 가려고 할 때
			{
				if (col_cursor != col - 3)   //오른쪽으로 가려고 할 때 바로 오른쪽이 막혀있으면 커서의 위치를 옮기지 않는다. 
				{
					*(*(board_cursor + row_cursor) + col_cursor) = 0;  //커서의 위치를 옮기기 위하여 원래 커서가 있던 위치는 비움.
					col_cursor++;                       //오른쪽으로 가려고 할 때 커서의 위치를 오른쪽으로 한 칸 움직임
					*(*(board_cursor + row_cursor) + col_cursor) = 1;
				}
			}
			if (input == ' ')          //공백을 입력받았을 때 대하여 아래 문장들 수행
			{
				if (*(*(board_horse + row_cursor) + col_cursor) == 3 && (selected == 0 || re == 1))  //spacebar를 입력받았고 그 자리에 자신의 말이 있을 때, 그리고 아직 말을 선택하지 않았거나, 말을 재선택했을 때 아래 문장들을 수행하게 됨
				{
					selected = 1; //말이 선택된 것이므로 selected변수의 값을 1로 변경
					*(*(board_horse + row_cursor) + col_cursor) = 4;   // 선택된 자리의 말의 상태를 선택된 player1의 말로 변경해줌
					if (re == 1)              //재선택인 경우에 아래 문장 수행
					{
						*(*(board_horse + remember_row) + remember_col) = 3;//재선택인 경우에, 원래 선택했던 말의 상태는 선택받기 이전 상태로 돌려준다.
					}
					remember_row = row_cursor;
					remember_col = col_cursor;          //말을 선택받으면 선택받은 말의 위치를 저장

				}
				else if (*(*(board_horse + row_cursor) + col_cursor) == 3 && selected == 1) //이미 선택받은 상태에서 다른 말을 또 선택한다면, 이전에 선택했던 말은 취소하고 새 말을 선택한다.
				{

					*(*(board_horse + remember_row) + remember_col) = 3;//원래 선택했던 말의 상태는 선택받기 이전 상태로 돌려준다.
					*(*(board_horse + row_cursor) + col_cursor) = 4;   // 선택된 자리의 말의 상태를 선택된 player1의 말로 변경해줌
					re = 1;          //다음부터 선택은 말의 재선택이므로 re변수에 1 대입해준다.
					remember_row = row_cursor;
					remember_col = col_cursor;          //말을 선택하면 선택받은 말의 위치를 저장

				}
				else if (*(*(board_horse + row_cursor) + col_cursor) == 0 && selected == 1)   //말을 선택한 상태에서, 비어있는 곳을 커서로 가르킨 채 spacebar을 입력한다면 말을 이동하거나 복제한것으로 인지.
				{
					*last_row_cursor2 = row_cursor;
					*last_col_cursor2 = col_cursor;                               //마지막으로 선택한 커서의 위치를 저장
					between_row = remember_row - row_cursor;                     //선택한 말과 말을 옮길 위치 사이의 거리를 계산
					between_col = remember_col - col_cursor;
					if (between_row < 0)                     //거리가 음수가 나오면 양수로 바꿔줌
						between_row = -between_row;
					if (remember_col < 0)
						remember_col = -remember_col;
					if (between_row <= 1 && between_col <= 1)
					{
						*(*(board_horse + row_cursor) + col_cursor) = 3;   // 행과 열의 거리가 둘 다 1 이하인 경우, 1칸 움직인 것이므로 그 위치에 말을 복사해준다.
						*(*(board_horse + remember_row) + remember_col) = 3;
						infect(row_cursor, col_cursor, board_horse, turn);
						*(*(board_cursor + row_cursor) + col_cursor) = 0; //이전에 저장돼있던 커서의 위치만 저장하면 됨, 이전에 커서가 있던 자리에도 보드판에서 []가 출력되는것을 막기 위해 0으로 초기화시켜준다.
					}
					else if (between_row <= 2 && between_col <= 2)  // 행과 열의 거리중 하나라도 1 이상인 상태에서, 행과 열의 거리가 둘 다 2 이하인 경우, 2칸 움직인 것이므로 선택한 말을 선택한 위치로 옮겨준다.
					{
						*(*(board_horse + remember_row) + remember_col) = 0;
						*(*(board_horse + row_cursor) + col_cursor) = 3;
						infect(row_cursor, col_cursor, board_horse, turn);
						*(*(board_cursor + row_cursor) + col_cursor) = 0; //이전에 저장돼있던 커서의 위치만 저장하면 됨, 이전에 커서가 있던 자리에도 보드판에서 []가 출력되는것을 막기 위해 0으로 초기화시켜준다.
					}

				}

			}
		}
		return change_turn(board_horse, row, col, turn);           //change_turn함수를 호출하여 순서가 어떻게 되는지(순서를 바꿀 것인지 바꾸지 않을 것인지)결정 한 후 그 값을 반환한다.
	}
}

int change_turn(int** board_horse, int row, int col, int turn)         //플레이어의 순서를 바꾸는지 확인하는 함수, 플레이어의 말 중 움직일 수 있는 말이 없다면 그 플레이어의 순서는 돌아오지 않고 같은 플레이어가 계속해서 게임을 진행한다. player1의 순서면 1을 반환, player2의 순서면 2를 반환한다.
{
	int i, j, k, l;           //반복문에서 사용할 변수
	if (turn == 1)          //현재 player1의 순서이면 아래 문장들 수행
	{
		for (i = 0; i < row; i++)
			for (j = 0; j < col; j++)
			{
				if (*(*(board_horse + i) + j) == 3)         //모든 player2의 말에 대하여, 그 말이 움직일 수 있는 말인지, 그렇지 않은 말인지 검사할 것이다.
					for (k = -2; k < 3; k++)
						for (l = -2; l < 3; l++)
						{
							if (*(*(board_horse + i + k) + j + l) != 0) //player2의 말로 부터 1칸,2칸 떨어진 공간 중 빈 공간이 없다면 의 말은 움직일 수 없는 말.그러므로 순서가 돌아가지 않고 그대로 player1이 진행한다.

								turn = 1;

							else
								return 2;              //player2의 말 중 한개라도 움직일 수 있는 말이 있으면 순서를 player2로 바꿔준다. 
						}
			}
	}
	if (turn == 2)          //현재 player2의 순서이면 아래 문장들 수행
	{
		for (i = 0; i < row; i++)
			for (j = 0; j < col; j++)
			{
				if (*(*(board_horse + i) + j) == 1)         //모든 player1의 말에 대하여, 그 말이 움직일 수 있는 말인지, 그렇지 않은 말인지 검사할 것이다.
					for (k = -2; k < 3; k++)
						for (l = -2; l < 3; l++)
						{
							if (*(*(board_horse + i + k) + j + l) != 0) //player1의 말로 부터 1칸,2칸 떨어진 공간 중 빈 공간이 없다면 의 말은 움직일 수 없는 말.그러므로 순서가 돌아가지 않고 그대로 player2이 진행한다.

								turn = 2;

							else
								return 1;              //player1의 말 중 한개라도 움직일 수 있는 말이 있으면 순서를 player1로 바꿔준다. 
						}
			}
	}
	return turn;
}
void infect(int row_cursor, int col_cursor, int** board_horse, int turn)          //이동시킨 말 근처의 다른 플레이어의 말들을 이동시킨 플레이어의 말로 바꾸는 함수
{
	if (turn == 1)         //player1의 순서면 아래 문장들 실행
	{
		if (*(*(board_horse + row_cursor + 1) + col_cursor + 1) == 3)         //커서로부터 1칸 떨어져 있는 player2의 말들을 전부 player1의 말로 바꿔준다.
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
	if (turn == 2)         //player2의 순서면 아래 문장들 실행
	{
		if (*(*(board_horse + row_cursor + 1) + col_cursor + 1) == 1)         //커서로부터 1 칸 떨어져 있는 player1의 말들을 전부 player2의 말로 바꿔준다.
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


void print_board(int** board_cursor, int** board_horse, int* score1, int* score2, int row, int col, int turn)    //점수상황, 순서, 게임판을 출력하는 함수
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
			if (*(*(board_horse + j + 2) + i + 2) == 1)   //player1의 말을 출력할 때.
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //커서가 있는 곳에 []를 출력해준다.
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
			else if (*(*(board_horse + j + 2) + i + 2) == 0)  //빈공간 출력할 떄.
			{
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //커서가 있는 곳에 []를 출력해준다.
					printf("[ ]");
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

					printf("   ");
				}
			}
			else if (*(*(board_horse + j + 2) + i + 2) == 2)  //선택된 player1의 말을 출력한다.
			{
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //커서가 있는 곳에 []를 출력해준다.
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
			else if (*(*(board_horse + j + 2) + i + 2) == 3) // player2의 말을 출력할 때
			{
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //커서가 있는 곳에 []를 출력해준다.
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
			else if (*(*(board_horse + j + 2) + i + 2) == 4) //선택된 player2의 말을 출력할 때
			{
				printf("|");
				if (*(*(board_cursor + j + 2) + i + 2) == 1)     //커서가 있는 곳에 []를 출력해준다.
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

void init_board(int** board_cursor, int** board_horse, int row, int col)   //맨 처음 게임판을 만든 후 초기 조건에 따라 게임판 안의 값들을 초기화 시켜주는 함수
{
	int i; //반복문에 사용할 변수
	for (i = 0; i < col; i++)
	{
		*(*board_horse + i) = 9;                //배열에 9가 들어있다면, 그쪽은 막힌 곳으로 가정할 것이다. 실제로 사용될 배열이 아닌 나머지 여유 공간은 9를 넣어서 막아두고 막힌 공간으로 인지하게 할 것이다.
		*(*(board_horse + 1) + i) = 9;          //실제로 사용될 배열이 아닌 나머지 여유 공간은 9를 넣어서 막아둔다.
		*(*(board_horse + row - 2) + i) = 9;
		*(*(board_horse + row - 1) + i) = 9;
	}
	for (i = 0; i < row; i++)
	{
		*(*(board_horse + i)) = 9;       //배열에 9가 들어있다면, 그쪽은 막힌 곳으로 가정할 것이다.
		*(*(board_horse + i) + 1) = 9;   //실제로 사용될 배열이 아닌 나머지 여유 공간은 9를 넣어서 막아둔다.
		*(*(board_horse + i) + col - 2) = 9;
		*(*(board_horse + i) + col - 1) = 9;
	}
	*(*(board_horse + 2) + 2) = 1;//실제로 사용할 배열의 (0,0)과 (0,col-1)지점에 player1의 말 놓는다.
	*(*(board_horse + 2) + col - 3) = 1; //실제로 사용할 배열의 (0,0)과 (0,col-1)지점에 player1의 말 놓는다.
	*(*(board_horse + row - 3) + 2) = 3;//실제로 사용할 배열의 (row-1,0)과 (row-1,col-1)지점에 player2의 말 놓는다.
	*(*(board_horse + row - 3) + col - 3) = 3; //실제로 사용할 배열의 (row-1,0)과 (row-1,col-1)지점에 player2의 말 놓는다.
	*(*(board_cursor + 2) + 2) = 1;          //맨 처음 커서의 위치 설정
}
