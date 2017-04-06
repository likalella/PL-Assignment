#include "StdAfx.h"
#include "GameBoard.h"

/**
	함 수 : GameBoard()
	기 능 : GameBoard 클래스의 생성자, 게임판 멤버변수, 
			이전 수 저장 배열을 동적으로 메모리 할당해줌
*/
GameBoard::GameBoard()
{
	preMoves = new Position[9];
	board = new char*[3];
	
	for(int i=0; i<3; i++)
	{
		board[i] = new char[3];
	}
};

/**
	함 수 : GameBoard(const GameBoard& copy)
	기 능 : 복사 생성자로써, 대칭성 검사에서 게임판을 복사할때,
			동적 할당을 고려한 깊은 복사를 할 수있게 해줌
*/
GameBoard::GameBoard(const GameBoard& copy)
{
	preMoves = new Position[9];
	board = new char*[3];
	
	for(int i=0; i<3; i++)
	{
		board[i] = new char[3];
	}

	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			board[i][j] = copy.board[i][j];

	for(int i=0; i<9; i++)
	{
		preMoves[i].x = copy.preMoves[i].x;
		preMoves[i].y = copy.preMoves[i].y;
	}

	moveCnt = copy.moveCnt;
	state = copy.state;
	starterCom = copy.starterCom;
	oppnentCom = copy.oppnentCom;
	startLevel = copy.startLevel;
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
void GameBoard::InitBoard(int startCom, int movedCnt, int nlevelA, int nlevelB)
{
	moveCnt = movedCnt;		/* 몇번 째 수를 저장 */

	if(startCom == 0)		/* 시작 컴퓨터와 컴퓨터 레벨을 설정 */
	{	
		starterCom = 'X';		oppnentCom = 'O';
		startLevel = nlevelA;	oppLevel = nlevelB;			
	}
	else				
	{
		starterCom = 'O';		oppnentCom = 'X';
		startLevel = nlevelB;	oppLevel = nlevelA;
	}

	if(movedCnt == 0)		/* 불러온 게임인지 여부를 검사해서 보드판 초기화 */
	{
		for(int i=0; i<3; i++)
			for(int j=0; j<3; j++)
				board[i][j] = ' ';
	}
};

/**
	함 수 : RandomMove(int preX, int preY)
	기 능 : 무르기를 시전했을때 이전에 놓았던 수와 다른곳에 놓도록 랜덤하게 좌표생성
*/
void GameBoard::RandomMove()
{
	int newX, newY;
	int preX, preY;
	bool overlap = true;
	srand(GetTickCount());
	
	UndoMove();
	preX = preMoves[moveCnt].x;
	preY = preMoves[moveCnt].y;

	while(overlap)
	{
		newX = rand()%3;
		newY = rand()%3;

		if(moveCnt <= 7)	/* 빈 좌표가 2개 이상일때 */
		{
			if((newX != preX) && (newY != preY))	/* 랜덤으로생성한 좌표와 이전좌표를 비교하고 */
				if(board[newX][newY] == ' ')		/* 해당 좌표가 비어있는 공간인지 검사 */
					overlap = false;
		}
		else
		{
			if(board[newX][newY] == ' ')
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
	if( moveCnt%2 == 0 )			/* 누구 순서인지 검사 */
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
void GameBoard::UndoMove()
{
	moveCnt--;
	board[preMoves[moveCnt].x][preMoves[moveCnt].y] = ' ';
}

/**
	함 수 : CheckState()
	기 능 : 현재 보드판의 상태, 누가이겼는지, 비겼는지를 검사해서 상태값을 변경해주는 함수
*/
void GameBoard::CheckState()
{
	char temp[3][3];
	int i;
	
	for(i=0; i<3; i++)				/* 가로 방향을 검사 */
	{	
		if(board[i][0]!=' ')
		{
			if((board[i][0]=='X') && (board[i][1]=='X') && (board[i][2]=='X'))
			{
				state = STATE_WINA;
				return;
			}
			if((board[i][0]=='O') && (board[i][1]=='O') && (board[i][2]=='O'))
			{
				state = STATE_WINB;
				return;
			}
		}
		if(board[0][i]!=' ')		/* 새로 방향을 검사 */
		{
			if((board[0][i]=='X') && (board[1][i]=='X') && (board[2][i]=='X'))
			{
				state = STATE_WINA;
				return;
			}
			if((board[0][i]=='O') && (board[1][i]=='O') && (board[2][i]=='O'))
			{
				state = STATE_WINB;
				return;
			}
		}
	}
		
	if((board[0][0]!=' ')&&(board[0][0]==board[1][1])&&(board[0][0]==board[2][2]))	/* 첫번째 대각선 검사 */
	{
		if(board[0][0]=='X')
		{
			state = STATE_WINA;
			return;
		}
		else
		{
			state = STATE_WINB;
			return;
		}
	}
	
	if((board[0][2]!=' ')&&(board[0][2]==board[1][1])&&(board[0][2]==board[2][0])) /* 두번째 대각선 검사 */
	{
		if(board[0][2]=='X')
		{
			state = STATE_WINA;
			return;
		}
		else
		{
			state = STATE_WINB;
			return;
		}
	}

	if( moveCnt > 8 )
	{
		state = STATE_DRAW;		/* 비겼을 때 */
	}
};