#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

#define a 97      // Player1's Keyboard Input
#define A 65
#define d 100
#define D 68
#define w 119
#define W 87
#define s 115
#define S 83
#define f 102
#define F 70
#define LEFT 75      // Player2's Keyboard Input
#define RIGHT 77 
#define UP 72 
#define DOWN 80
#define m 109
#define M 77
#define p 112 // pause
#define P 80
#define ESC 27 // end the game

#define false 0
#define true 1

#define ACTIVE_BLOCK -2 // Block State
#define CEILLING -1     // 이동 가능한 공간 <= 0
#define EMPTY 0         // 이동 불가능한 공간 > 0
#define WALL 1
#define INACTIVE_BLOCK 2 

#define MAIN_X 11   // Game board Size
#define MAIN_Y 23 
#define MAIN_X_ADJ 3 // Game board 위치 조정
#define MAIN_Y_ADJ 1  

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 // Status info position
#define STATUS_Y_SCORE 14

int COLOR[3] = { 7, 14, 12 };

int blocks[7][4][4][4] = {
	//■■
	//■■
	{ { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },
	{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },
	//■■■■
	{ { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 } },
	//■■
	//  ■■
	{ { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },
	//  ■■
	//■■
	{ { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 } },
	//    ■
	//■■■
	{ { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0 },{ 0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0 } },
	//■
	//■■■
	{ { 0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0 } },
	//  ■
	//■■■
	{ { 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },
	{ 0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0 },{ 0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0 } }
}; // 테트리스 규칙에 맞게 기본 제공 코드 수정함

int b_type; // block type
int b_rotation; // block rotation
int b_type_next; //new block

int main_org[MAIN_Y][MAIN_X][2]; // 칸 상태랑 색깔 두 가지 다 저장하게 수정
int main_cpy[MAIN_Y][MAIN_X][2]; // org와 cpy 비교해서 달라진 부분만 cmd창에서 수정

int bx, by; // x,y position of moving block

int key; // keyboard input 

int speed = 50; // Game Speed 

int player; // 현재 조작 중인 player
int score[3]; // player1과 player2 점수 저장
int item[3]; // player가 가진 아이템의 개수 저장
int item_used[3]; 
int item_flag; // 0: no one used item 1/2: player1/2 used item
int turn; // 몇 턴 남았는지 저장

int new_block_on = 0;
int crush_on = 0;

void title(void);
void reset(void);
void reset_main(void); // reset main_org[][]
void reset_main_cpy(void); // reset main_cpy[][]
void draw_map(void);
void draw_main(void);
void new_block(void);
void check_key(void);
void play(void);

int check_crush(int bx, int by, int rotation);
void drop_block(void);
void move_block(int dir);
void check_line(void);

void check_game_over(void);
void pause(void);

void use_item(void);

void gotoxy(int x, int y) {
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

time_t  t;
struct  tm *ptm;
char  today[30];

int main() {
	srand((unsigned)time(NULL));

	CONSOLE_CURSOR_INFO CurInfo; // 커서 숨기기
	CurInfo.dwSize = 1;          // 원래 있던 함수는 삭제함
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo); // 원래 있던 enum 삭제함

	player = 1; //선플레이어

	title();
	reset();
	play();
}

void title(void) {
	int x = 8;
	int y = 3;

	gotoxy(x, y + 0); printf("┌─────────────┐");
	gotoxy(x, y + 1); printf("│                          │");
	gotoxy(x, y + 2); printf("│                          │");
	gotoxy(x, y + 3); printf("│                          │");
	gotoxy(x, y + 4); printf("└─────────────┘");

	gotoxy(x + 5, y + 2); printf("T E T R I S");
	gotoxy(x, y + 7); printf("Please Enter Any Key to Start..");
	
	x = 5; y = 6;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[1]);
	gotoxy(x + 5, y + 7);  printf("P1");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
	gotoxy(x, y + 9);  printf("   A   : Shift");
	gotoxy(x, y + 10); printf(" S   D : Left / Right");
	gotoxy(x, y + 11); printf("   W   : Drop");
	gotoxy(x, y + 12); printf("   F   : Use Item");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[2]);
	gotoxy(x + 17, y + 7);  printf("P2");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
	gotoxy(x + 12, y + 9);  printf("  △   : Shift");
	gotoxy(x + 12, y + 10); printf("◁  ▷ : Left / Right");
	gotoxy(x + 12, y + 11); printf("  ▽   : Drop");
	gotoxy(x + 12, y + 12); printf("   M   : Use Item");

	gotoxy(x + 6, y + 14); printf("   P   : Pause");
	gotoxy(x + 6, y + 15); printf("  ESC  : Quit");

	while (1) {
		if (kbhit()) break;
	}

	while (kbhit()) getch();
}

void reset(void) {
	score[1] = 0;
	score[2] = 0;
	item[1] = 0;
	item[2] = 0;
	item_used[1] = 0;
	item_used[2] = 0;
	item_flag = 0;
	turn = 60;
	key = 0;
	crush_on = 0;

	system("cls");
	reset_main();
	reset_main_cpy();
	draw_map();
	draw_main();

	b_type_next = rand() % 7;
	new_block();
}

void reset_main(void) {
	int i, j;

	for (i = 0; i<MAIN_Y; i++)  // Game board initialize
		for (j = 0; j < MAIN_X; j++) {
			main_org[i][j][0] = 0;
			main_org[i][j][1] = COLOR[0];
		}

	for (j = 1; j<MAIN_X; j++) // 천장
		main_org[3][j][0] = CEILLING;

	for (i = 1; i<MAIN_Y - 1; i++) { // 좌우 벽
		main_org[i][0][0] = WALL;
		main_org[i][MAIN_X - 1][0] = WALL;
	}

	for (j = 0; j<MAIN_X; j++) // 바닥 벽
		main_org[MAIN_Y - 1][j][0] = WALL;

}

void reset_main_cpy(void) {
	int i, j;

	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			main_cpy[i][j][0] = 100;
			main_cpy[i][j][1] = COLOR[0];
		}
	}
}

void draw_map(void) { // Game Status 
	int y = 4;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[player]);
	gotoxy(STATUS_X_ADJ, y + 0); printf("Player%d's Turn!!", player);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	gotoxy(STATUS_X_ADJ, y + 2); printf("+-  N E X T  -+ ");
	gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 6); printf("|             |     %2d turns left", turn);
	gotoxy(STATUS_X_ADJ, y + 7); printf("+- - - - - - -+");

	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("P1's Score  :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 2); printf("     Item   :");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[1]);
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 1); printf("        %6d", score[1]);
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 3); printf("        %6d", item[1]);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
	gotoxy(STATUS_X_ADJ + 10, STATUS_Y_SCORE); printf("P2's Score  :");
	gotoxy(STATUS_X_ADJ + 10, STATUS_Y_SCORE + 2); printf("     Item   :");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[2]);
	gotoxy(STATUS_X_ADJ + 10, STATUS_Y_SCORE + 1); printf("        %6d", score[2]);
	gotoxy(STATUS_X_ADJ + 10, STATUS_Y_SCORE + 3); printf("        %6d", item[2]);

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 5); printf("  △      W   : Shift");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 6); printf("◁  ▷  A   D : Left / Right   P   : Pause");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 7); printf("  ▽      S   : Drop          ESC  : Quit");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 8); printf("   F      M   : Use Item");
}

void draw_main(void) {
	int i, j;

	for (j = 1; j<MAIN_X - 1; j++) // 블럭이 지나가면서 지워진 천장 다시 그리기
		if (main_org[3][j][0] == EMPTY) main_org[3][j][0] = CEILLING;

	for (i = 0; i<MAIN_Y; i++) {
		for (j = 0; j<MAIN_X; j++) {
			if (main_cpy[i][j][0] != main_org[i][j][0] || main_cpy[i][j][1] != main_org[i][j][1]) { // 비교해서 다른 부분만 수정
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (main_org[i][j][0]) {
				case EMPTY:
					printf("  ");
					break;
				case CEILLING:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
					printf(". ");
					break;
				case WALL:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
					printf("▩");
					break;
				case INACTIVE_BLOCK:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), main_org[i][j][1]);
					printf("□");
					break;
				case ACTIVE_BLOCK:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), main_org[i][j][1]);
					printf("■");
					break;
				}
			}
		}
	}
	for (i = 0; i<MAIN_Y; i++) { // main_cpy에 복사
		for (j = 0; j<MAIN_X; j++) {
			main_cpy[i][j][0] = main_org[i][j][0];
			main_cpy[i][j][1] = main_org[i][j][1];
		}
	}
}

void new_block(void) {
	int i, j;
	
	bx = (MAIN_X / 2) - 1; // 블럭 생성 위치 x좌표
	by = 0;               // 블럭 생성 위치 y좌표
	b_type = b_type_next; // 다음 블럭값 가져오기
	b_type_next = rand() % 7;
	b_rotation = 0;

	new_block_on = 0;

	for (i = 0; i<4; i++) { // 블럭 생성
		for (j = 0; j<4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1) {
				main_org[by + i][bx + j][0] = ACTIVE_BLOCK;
				main_org[by + i][bx + j][1] = COLOR[player];
			}
		}
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player == 1 ? COLOR[2] : COLOR[1]);
	for (i = 1; i<3; i++) { // NEXT 박스 안의 블럭
		for (j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 7);
				printf("■");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 7);
				printf("  ");
			}
		}
	}
}

void play(void)
{
	time(&t);
	ptm = localtime(&t);
	strftime(today, sizeof(today), "%Y-%m-%d %a %H:%M:%S", ptm);

	while (1) {
		for (int i = 0; i < 5; i++) {
			check_key();
			draw_main();

			Sleep(speed);
			if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
		}
		drop_block();

		check_game_over();
		if (new_block_on == 1) 
		{
			//(player == 1) ? player = 2 : player = 1;
			if (player == 1) player = 2;
			else player = 1;


			if (item_flag != 0) use_item();

			new_block();
			
			turn--;

			gotoxy(STATUS_X_ADJ, 2); printf("                        ");

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
			gotoxy(STATUS_X_ADJ + 10, STATUS_Y_SCORE - 4); printf("%2d turns left", turn);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[player]);
			gotoxy(STATUS_X_ADJ, 4); printf("Player%d's Turn!!", player);
		}
	}
}

void check_key(void) {
	key = 0;

	if (player == 1) {
		if (kbhit()) {
			key = getch();

			switch (key) {
			case a: case A:
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;
			case d: case D:
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;
			case s: case S:
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case w: case W:
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(0);
				// 바닥에 닿은 경우 위쪽으로 한 칸 띄워서 회전이 가능하면 그렇게 하기
				break;
			case f: case F:
				if (item_flag == 0 && item[1] > 0) {
					item_flag = 1;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
					gotoxy(STATUS_X_ADJ, 2); printf("<< Player1 used item >>");
				}
				break;
			case m: case M:
				if (item_flag == 0 && item[2] > 0) {
					item_flag = 2;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
					gotoxy(STATUS_X_ADJ, 2); printf("<< Player2 used item >>");
				}
				break;
			case p: case P:
				pause();
				break;
			case ESC:
				system("cls");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
				exit(0);
			}
		}
		while (kbhit()) getch();
	}
	else {
		if (kbhit()) {
			key = getch();
			if (key == 224) { // direction key
				do { key = getch(); } while (key == 224);
				switch (key) {
				case LEFT:
					if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
					break;
				case RIGHT:
					if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
					break;
				case DOWN:
					if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
					break;
				case UP:
					if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
					else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(0);
				}
			}
			else { // Not direction Key
				switch (key) {
				case f: case F:
					if (item_flag == 0 && item[1] > 0) {
						item_flag = 1;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
						gotoxy(STATUS_X_ADJ, 2); printf("<< Player1 used item >>");
					}
					break;
				case m: case M:
					if (item_flag == 0 && item[2] > 0) {
						item_flag = 2;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
						gotoxy(STATUS_X_ADJ, 1); printf("<< Player2 used item >>");
					}
					break;
				case P: case p:
					pause();
					break;
				case ESC:
					system("cls");
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
					exit(0);
				}
			}
			while (kbhit()) getch();
		}
	}
}

int check_crush(int bx, int by, int b_rotation) {
	int i, j;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type][b_rotation][i][j] == 1 && main_org[by + i][bx + j][0]>0) return false;
		}
	}
	return true;
};

void drop_block(void) {
	int i, j;

	if (crush_on&&check_crush(bx, by + 1, b_rotation) == true) crush_on = 0;
	if (crush_on&&check_crush(bx, by + 1, b_rotation) == false) {
		for (i = 0; i<MAIN_Y; i++) {
			for (j = 0; j<MAIN_X; j++) {
				if (main_org[i][j][0] == ACTIVE_BLOCK) main_org[i][j][0] = INACTIVE_BLOCK;
			}
		}
		crush_on = 0;
		check_line();
		new_block_on = 1;
		return;
	}
	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++;
}

void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j][0] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					main_org[by + i][bx + j - 1][0] = ACTIVE_BLOCK;
					main_org[by + i][bx + j - 1][1] = COLOR[player];
				}
			}
		}
		bx--;
		break;

	case RIGHT:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j][0] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					main_org[by + i][bx + j + 1][0] = ACTIVE_BLOCK;
					main_org[by + i][bx + j + 1][1] = COLOR[player];
				}
			}
		}
		bx++;
		break;

	case DOWN:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j][0] = EMPTY;
			}
		}
		for (i = 0; i<4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					main_org[by + i + 1][bx + j][0] = ACTIVE_BLOCK;
					main_org[by + i + 1][bx + j][1] = COLOR[player];
				}
			}
		}
		by++;
		break;

	case UP:
		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j][0] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i<4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1)
				{
					main_org[by + i][bx + j][0] = ACTIVE_BLOCK;
					main_org[by + i][bx + j][1] = COLOR[player];
				}
			}
		}
		break;

	case 0:

		for (i = 0; i<4; i++) {
			for (j = 0; j<4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_org[by + i][bx + j][0] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i<4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					main_org[by + i - 1][bx + j][0] = ACTIVE_BLOCK;
					main_org[by + i - 1][bx + j][1] = COLOR[player];
				}
			}
		}
		by--;
		break;
	}
}

void check_line(void) {
	int i, block_amount, j, k;
	int cnt = 0; // 한 턴에 몇 줄을 지웠나

	for (i = MAIN_Y - 2; i > 3;) {
		for (block_amount = 1; block_amount < MAIN_X - 1; block_amount++)
			if (main_org[i][block_amount][0] <= 0) break;

		if (block_amount == MAIN_X - 1) { // 블록이 한 줄에 가득 찰 경우
			score[player]++;
			cnt++;
			for (j = i; j > 1; j--) { // Block Down
				for (k = 1; k < MAIN_X - 1; k++) {
					if (main_org[j - 1][k][0] != CEILLING) {
						main_org[j][k][0] = main_org[j - 1][k][0];
						main_org[j][k][1] = main_org[j - 1][k][1];
					}
					else main_org[j][k][0] = EMPTY;
				}
			}
		}
		else i--;
	}

	if (player == 1) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[1]);
		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 1); printf("        %6d", score[1]);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[2]);
		gotoxy(STATUS_X_ADJ + 10, STATUS_Y_SCORE + 1); printf("        %6d", score[2]);
	}

	if (cnt >= 2) {
		item[player]++;
		gotoxy(player == 1 ? STATUS_X_ADJ : STATUS_X_ADJ + 10, STATUS_Y_SCORE + 3); printf("        %6d", item[player]);
	}
}

void check_game_over(void) {
	int i;

	int x = 5;
	int y = 5;

	for (i = 1; i<MAIN_X - 2; i++) {
		if (main_org[3][i][0] > 0 || turn == 0) {  // end condition 
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
			gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
			gotoxy(x, y + 1); printf("▤                              ▤");
			gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
			gotoxy(x, y + 3);
			if (score[1] == score[2]) printf("▤  |     D   R   A   W     |   ▤");
			else printf("▤  |    Player%d's Win!!    |   ▤", score[1] > score[2] ? 1 : 2);
			gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
			gotoxy(x, y + 5); printf("▤                              ▤");
			gotoxy(x, y + 6); printf("▤   Player1's SCORE: %6d    ▤", score[1]);
			gotoxy(x, y + 7); printf("▤   Player2's SCORE: %6d    ▤", score[2]);
			gotoxy(x, y + 8); printf("▤                              ▤");
			gotoxy(x, y + 9); printf("▤  Press any key to restart..  ▤");
			gotoxy(x, y + 10); printf("▤                              ▤");
			gotoxy(x, y + 11); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

			FILE *fp = fopen("record.txt", "a");
			fprintf(fp, "%s\n\n", today);
			fprintf(fp, "Player1:\n");
			fprintf(fp, "   removed %d lines\n", score[1]);
			fprintf(fp, "   obtained %d items\n", item[1] + item_used[1]);
			fprintf(fp, "   used %d items\n", item_used[1]);
			fprintf(fp, "Player2:\n");
			fprintf(fp, "   removed %d lines\n", score[2]);
			fprintf(fp, "   obtained %d items\n", item[2] + item_used[2]);
			fprintf(fp, "   used %d items\n\n\n", item_used[2]);
			fclose(fp);

			Sleep(1000);
			while (kbhit()) getch();
			key = getch();
			reset();
		}
	}
}

void pause(void) {
	int i, j;

	int x = 5;
	int y = 5;

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
	gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	gotoxy(x, y + 1); printf("▤                              ▤");
	gotoxy(x, y + 2); printf("▤  +-----------------------+   ▤");
	gotoxy(x, y + 3); printf("▤  |       P A U S E       |   ▤");
	gotoxy(x, y + 4); printf("▤  +-----------------------+   ▤");
	gotoxy(x, y + 5); printf("▤  Press any key to resume..   ▤");
	gotoxy(x, y + 6); printf("▤                              ▤");
	gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	getch();

	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player == 1 ? COLOR[2] : COLOR[1]);
	for (i = 1; i<3; i++) {
		for (j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 7);
				printf("■");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 7);
				printf("  ");
			}
		}
	}
}

void use_item(void)
{
	char choice = '0';

	if (item_flag == 1) {
		item[1]--;
		item_used[1]++;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[1]);
		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE + 3); printf("        %6d", item[1]);
	}
	else {
		item[2]--;
		item_used[2]++;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[2]);
		gotoxy(STATUS_X_ADJ + 10, STATUS_Y_SCORE + 3); printf("        %6d", item[2]);
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), COLOR[0]);
	gotoxy(5, 5);  printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	gotoxy(5, 6);  printf("▤                                    ▤");
	gotoxy(5, 7);  printf("▤  1:  ■  2 :     3 :       4 :     ▤");
	gotoxy(5, 8);  printf("▤      ■    ■■      ■        ■  ▤");
	gotoxy(5, 9);  printf("▤      ■    ■■    ■■■    ■■  ▤");
	gotoxy(5, 10); printf("▤      ■                      ■    ▤");
	gotoxy(5, 11); printf("▤                                    ▤");
	gotoxy(5, 12); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	Sleep(5000);
	while (kbhit()) choice = getch();

	switch (choice)
	{
	case '1':
		b_type_next = 1;
		break;
	case '2':
		b_type_next = 0;
		break;
	case '3':
		b_type_next = 6;
		break;
	case '4':
		b_type_next = 2;
		break;
	default:
		b_type_next = rand() % 7;
	}

	system("cls");
	reset_main_cpy();
	draw_main();
	draw_map();

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), player == 1 ? COLOR[2] : COLOR[1]);
	for (int i = 1; i<3; i++) {
		for (int j = 0; j<4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 7);
				printf("■");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 7);
				printf("  ");
			}
		}
	}

	item_flag = 0;
}