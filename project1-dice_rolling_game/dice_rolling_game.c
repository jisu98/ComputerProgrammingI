#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

int wins = 0, loses = 0, cash = 100000, betting;
int com[3], user[3];

void gotoxy(int x, int y)
{
	COORD pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void mainMenu();
void myState();
void drawDice(int _i, int _number);
void _drawDice(int n, int x, int y);
void diceRollingGame();
void bet(int sum);
void roll(int sum);
void startGame();
void end();
void retry();

int main(void)
{
	mainMenu();
	printf("\n");
	return 0;
}

void mainMenu()
{
	system("cls");

	printf("1. My State\n");
	printf("2. Dice Rolling Game\n");
	printf("3. End\n\n");
	printf("Enter your choice : ");

	char choice = getch();

	if (choice == '1') myState();
	else if (choice == '2') diceRollingGame();
	else if (choice != '3') mainMenu();
}

void myState()
{
	system("cls");

	printf("Current cash : %d\n", cash);
	printf("Wins : %d\n", wins);
	printf("Loses : %d\n\n", loses);
	printf("Type any key to go main menu...\n");
	
	getch();
	mainMenu();
}

void drawDice(int _i, int _number)
{
	switch (_i)
	{
	case 1:
		printf("¦£¦¡¦¡¦¡¦¤");
		break;
	case 2:
		switch (_number)
		{
		case 0:
			printf("¦¢      ¦¢");
			break;
		case 1:
		case 2:
			printf("¦¢      ¦¢");
			break;
		case 3:
			printf("¦¢    ¡Ü¦¢");
			break;
		case 4:
		case 5:
		case 6:
			printf("¦¢¡Ü  ¡Ü¦¢");
			break;
		}
		break;
	case 3:
		switch (_number)
		{
		case 0:
			printf("¦¢      ¦¢");
			break;
		case 1:
		case 3:
		case 5:
			printf("¦¢  ¡Ü  ¦¢");
			break;
		case 2:
		case 6:
			printf("¦¢¡Ü  ¡Ü¦¢");
			break;
		case 4:
			printf("¦¢      ¦¢");
			break;
		}
		break;
	case 4:
		switch (_number)
		{
		case 0:
			printf("¦¢      ¦¢");
			break;
		case 1:
		case 2:
			printf("¦¢      ¦¢");
			break;
		case 3:
			printf("¦¢¡Ü    ¦¢");
			break;
		case 4:
		case 5:
		case 6:
			printf("¦¢¡Ü  ¡Ü¦¢");
			break;
		}
		break;
	case 5:
		printf("¦¦¦¡¦¡¦¡¦¥");
		break;
	}
}

void _drawDice(int n, int x, int y)
{
	for (int i = 1; i < 6; i++)
	{
		gotoxy(x, y + i - 1);
		drawDice(i, n);
	}
	printf("\n");
}

void diceRollingGame()
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < 3; i++)
		com[i] = rand() % 6 + 1;

	int sum = com[0] + com[1] + com[2];

	bet(sum);
	roll(sum);
}

void bet(int sum)
{
	int flag;
	char _betting[100];

	do {
		system("cls");
		printf("Current cash : %d\n\n", cash);
		printf("Sum of computer's dices : %d\n\n", sum);
		printf("Input your betting : ");

		flag = 1;

		fflush(stdin);
		gets(_betting);

		for (int i = 0; i < strlen(_betting); i++)
			if (_betting[i]<'0' || _betting[i]>'9')
			{
				flag = 0;
				break;
			}
	} while (flag == 0);

	betting = atoi(_betting);
	if (betting < 1 || betting > cash) bet(sum);
}

void roll(int sum)
{
	int option, i;

	system("cls");

	printf("Computer's dice total is %d\n", sum);

	for (i = 0; i < 3; i++)
		_drawDice(0, 1 + 10 * i, 2);

	do {
		user[0] = rand() % 6 + 1;
		user[1] = rand() % 6 + 1;
		user[2] = sum - user[0] - user[1];
	} while (user[2] < 1 || user[2] > 6);

	for (i = 0; i < 3; i++)
		_drawDice(user[i], 1 + 10 * i, 7);

	printf("Type ESC to Start Game\n");
	printf("Type s to Surrender\n");
	printf("Any key. Rolling dice again\n");

	option = getch();

	if (option == 0x1b) startGame();
	else if (option == 's')
	{
		cash -= 5000;
		loses++;
		retry();
	}
	else roll(sum);
}

void startGame()
{
	int i, cnt = 0;
	char choice;

	system("cls");

	for (i = 0; i < 3; i++)
	{
		gotoxy(1, 1);
		printf("Stage %d", i + 1);

		_drawDice(com[i], 1 + i * 10, 2);
		_drawDice(user[i], 1 + i * 10, 7);

		if (user[i] > com[i])
		{
			cnt++;
			printf("User Win    \n");
		}
		else printf("Computer Win\n");

		Sleep(2000);
	}

	if (cnt > 1)
	{
		printf("Final winner : User!!\n");
		wins++;
		cash += betting * 2;
	}
	else
	{
		printf("Final winner : Computer!!\n");
		loses++;
		cash -= betting;
	}

	if (cash < 5000) end();
	else retry();
}

void end()
{
	Sleep(2000);
	system("cls");
	printf("YOU LOSE\n\n");
	printf("User's cash is less than 5000, so you cannot bet any more.\n\n");
	printf("Current cash : %d\n", cash);
	printf("Wins : %d\n", wins);
	printf("Loses : %d\n\n", loses);
	
	exit(0);
}

void retry()
{
	char choice;

	printf("\n");
	printf("1. Retry\n");
	printf("2. Back to main menu\n");
	printf("Enter your choice : ");

	do {
		choice = getch();
		if (choice == '1') diceRollingGame();
		else if (choice == '2') mainMenu();
	} while (choice != '1'&&choice != '2');
}