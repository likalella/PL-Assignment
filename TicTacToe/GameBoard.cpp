#include "StdAfx.h"
#include "GameBoard.h"

/**
함 수 : GameBoard()
기 능 : GameBoard 클래스의 생성자, 게임판 멤버변수,
이전 수 저장 배열을 동적으로 메모리 할당해줌
*/
// lika 9->16, 3 ->4
GameBoard::GameBoard()
{
	preMoves = new Position[16];
	board = new char*[4];

	for (int i = 0; i<4; i++)
	{
		board[i] = new char[4];
	}
};

/**
함 수 : GameBoard(const GameBoard& copy)
기 능 : 복사 생성자로써, 대칭성 검사에서 게임판을 복사할때,
동적 할당을 고려한 깊은 복사를 할 수있게 해줌
*/
//lika 9->16, 3->4
GameBoard::GameBoard(const GameBoard& copy)
{
	preMoves = new Position[16];
	board = new char*[4];

	for (int i = 0; i<4; i++)
	{
		board[i] = new char[4];
	}

	for (int i = 0; i<4; i++)
		for (int j = 0; j<4; j++)
			board[i][j] = copy.board[i][j];

	for (int i = 0; i<16; i++)
	{
		preMoves[i].x = copy.preMoves[i].x;
		preMoves[i].y = copy.preMoves[i].y;
	}

	moveCnt = copy.moveCnt;
	state = copy.state;
	starterCom = copy.starterCom;
	oppnentCom = copy.oppnentCom;
	//startLevel = copy.startLevel;
	oppLevel = copy.oppLevel;
};

/**
함 수 : GetBoardStat()
기 능 : 게임판 상태 반환
*/
int GameBoard::GetBoardStat()
{
	return state;
};

/**
함 수 : InitBoard(int startCom, int movedCnt, int nlevelA, int nlevelB)
기 능 : 초기화에 필요한 정보를 인자를 중심으로 게임판을 초기화해주는 함수
*/
// lika 3->4
void GameBoard::InitBoard(int startCom, int movedCnt, int nlevelA)
{
	moveCnt = movedCnt;		/* 몇번 째 수를 저장 */

	starterCom = 'X';		oppnentCom = 'O';
	oppLevel = nlevelA;

	if (movedCnt == 0)		/* 불러온 게임인지 여부를 검사해서 보드판 초기화 */
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				board[i][j] = ' ';
			}
		}
	}
};

/**
함 수 : RandomMove(int preX, int preY)
기 능 : 무르기를 시전했을때 이전에 놓았던 수와 다른곳에 놓도록 랜덤하게 좌표생성
*/
// lika 3->4
void GameBoard::RandomMove()
{
	int newX, newY;
	int preX, preY;
	bool overlap = true;
	srand(GetTickCount());

	UndoMove();
	preX = preMoves[moveCnt].x;
	preY = preMoves[moveCnt].y;

	while (overlap)
	{
		newX = rand() % 4;
		newY = rand() % 4;

		if (moveCnt <= 14)	/* 빈 좌표가 2개 이상일때 */ // lika : 
		{
			if ((newX != preX) && (newY != preY))	/* 랜덤으로생성한 좌표와 이전좌표를 비교하고 */
				if (board[newX][newY] == ' ')		/* 해당 좌표가 비어있는 공간인지 검사 */
					overlap = false;
		}
		else
		{
			if (board[newX][newY] == ' ')
				overlap = false;
		}
	}
	DoMove(newX, newY);
};

/**
함 수 : DoMove(int x, int y)
기 능 :	입력받은 좌표값으로 게임판에 수를 둠
*/
void GameBoard::DoMove(int x, int y)
{
	if (moveCnt % 2 == 0)			/* 누구 순서인지 검사 */
		board[x][y] = starterCom;
	else
		board[x][y] = oppnentCom;

	preMoves[moveCnt].x = x;
	preMoves[moveCnt].y = y;

	moveCnt++;						/* 현재 수를 증가 시킴 */
};

/**
함 수 : UndoMove()
기 능 : 놓았던 수를 한단계 이전상태로 돌려놓는 함수
*/
// lika : undo 2개겠지
void GameBoard::UndoMove()
{
	moveCnt--;
	board[preMoves[moveCnt].x][preMoves[moveCnt].y] = ' ';
}

/**
함 수 : CheckState()
기 능 : 현재 보드판의 상태, 누가이겼는지, 비겼는지를 검사해서 상태값을 변경해주는 함수
*/
//lika : 3->4, 4X4에 맞게 체크하는 조건 추가.
void GameBoard::CheckState()
{
	char temp[4][4];
	int i;

	for (i = 0; i<4; i++)				/* 가로 방향을 검사 */
	{
		if (board[i][0] != ' ')
		{
			if ((board[i][0] == 'X') && (board[i][1] == 'X') && (board[i][2] == 'X') && (board[i][3] == 'X'))
			{
				state = STATE_WINB;
				return;
			}
			if ((board[i][0] == 'O') && (board[i][1] == 'O') && (board[i][2] == 'O') && (board[i][3] == 'O'))
			{
				state = STATE_WINA;
				return;
			}
		}
		if (board[0][i] != ' ')		/* 세로 방향을 검사 */
		{
			if ((board[0][i] == 'X') && (board[1][i] == 'X') && (board[2][i] == 'X') && (board[3][i] == 'X'))
			{
				state = STATE_WINB;
				return;
			}
			if ((board[0][i] == 'O') && (board[1][i] == 'O') && (board[2][i] == 'O') && (board[3][i] == 'O'))
			{
				state = STATE_WINA;
				return;
			}
		}
	}

	if ((board[0][0] != ' ') && (board[0][0] == board[1][1]) && (board[0][0] == board[2][2]) && (board[0][0] == board[3][3]))	/* 첫번째 대각선 검사 */
	{
		if (board[0][0] == 'X')
		{
			state = STATE_WINB;
			return;
		}
		else
		{
			state = STATE_WINA;
			return;
		}
	}

	if ((board[0][3] != ' ') && (board[0][3] == board[1][2]) && (board[0][3] == board[2][1]) && (board[0][3] == board[3][0])) /* 두번째 대각선 검사 */
	{
		if (board[0][3] == 'X')
		{
			state = STATE_WINB;
			return;
		}
		else
		{
			state = STATE_WINA;
			return;
		}
	}

	if (moveCnt > 15)
	{
		state = STATE_DRAW;		/* 비겼을 때 */
	}
};