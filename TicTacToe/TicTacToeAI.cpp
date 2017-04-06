#include "StdAfx.h"
#include "TicTacToeAI.h"

/**
	함 수 : TicTacToeAI(GameBoard board)
	기 능 : 게임 보드판을 넘겨받는 생성자 함수
*/
TicTacToeAI::TicTacToeAI(GameBoard board)
{
	tttBoard = board;
	root = new Node;
	InitNode(root, 0);

	if(tttBoard.moveCnt % 2 == 0)
	{
		LAlevel = tttBoard.startLevel;
		criterion = tttBoard.starterCom;
	}
	else
	{
		LAlevel = tttBoard.oppLevel;
		criterion = tttBoard.oppnentCom;
	}
};

/**
	함 수 : InitNode(Node* node, int inDepth)
	기 능 : 출력할 노드를 깊이, eval값 정보로 초기화
*/
void TicTacToeAI::InitNode(Node* node, int inDepth)
{
	node->depth = inDepth;
	node->eval = 0;
	node->childCnt = 0;

	for(int i=0; i<16; i++)
		node->next[i] = NULL;
};

/**
	함 수 : GetRootNode()
	기 능 : 루트노드를 반환
*/
Node* TicTacToeAI::GetRootNode()
{
	return root;
};

/**
	함 수 : GetBestMove()
	기 능 : 최적의 좌표를 Minimax 알고리즘을 통해 구함
*/
void TicTacToeAI::GetBestMove()
{
	Position iList[16];				/* 현재 게임판에서 놓을 수 있는 좌표 개수, 위치 저장 */
	int	bestValue = -10000;			/* besteval 값 선언 */
	int	possible = 0;				/* 가능한 개수 저장 변수 */
		
	possible = GetPossibleMove(iList);	/*현재 가능한 수 좌표, 개수 저장 */

	for( int i = 0; i < possible; i++ )				/* 가능한 수 만큼 Minimax 알고리즘을 사용하여 수를 계산 */
	{
		root->next[i] = new Node;					/* 새로운 노드 생성 */
		InitNode(root->next[i], (root->depth)+1);	/* 노드 초기화 */
		tttBoard.DoMove(iList[i].x , iList[i].y);	/* 가능한 수를 둠 */
		int newValue = Minimize(root->next[i]);		/* Min함수호출로 값을 구해서 */
		root->next[i]->eval = newValue;				/* 노드에 저장 */
		tttBoard.UndoMove();						/* 두었던 수를 무름 */

		if( newValue > bestValue )		/* 구한 eval값이 best값보다 크다면 */
		{
			bestValue = newValue;		/* 값을 변경하고 해당 수정보를 저장 */
			bestX = iList[i].x;
			bestY = iList[i].y;
			root->eval = bestValue;		/* 노드에 값을 저장 */
		}
	}
	root->childCnt = possible;			/* 자식 노드 수를 저장 */
};

/**
	함 수 : Minimize(struct treeNode* root)
	기 능 : 현재 자식 노드들 eval값중에서 제일 작은 값으로 저장하는 함수
*/
int TicTacToeAI::Minimize(struct treeNode* root)
{
	Position iList[16];			/* 현재 게임판에서 놓을 수 있는 좌표 개수, 위치 저장 */
	int	possible = 0;			/* 둘수있는 개수 저장 변수 */
	int	bestValue = 10000;		/* besteval 값 선언 */

	possible = GetPossibleMove(iList); /*현재 가능한 수 좌표, 개수 저장 */
		
	if( possible == 0 || CheckEnd() || root->depth == LAlevel ) /* 가능한수 없거나, 게임종료면 eval 값을 계산하여 리턴 */
	{
		return EvaluateBoard(root);
	}		

	for( int i = 0; i < possible; i++ ) /* 가능한 수 만큼 Minimax 알고리즘을 사용하여 수를 계산 */
	{
		root->next[i] = new Node;					/* 노드 생성 후 초기화 */
		InitNode(root->next[i], root->depth+1);

		tttBoard.DoMove(iList[i].x , iList[i].y);	/* 가능한 수를 둠 */
		int newValue = Maximize(root->next[i]);		/* Max함수호출로 값을 구해서 */
		root->next[i]->eval = newValue;				/* 노드에 저장 */
		tttBoard.UndoMove();						/* 두었던 수를 무름 */

		if( newValue < bestValue )	/* 구한 eval값이 best값보다 작다면 */
		{				
			bestValue = newValue;	/* 값을 변경하고 해당값으로 노드값에 저장 */
			root->eval = bestValue;
		}
	}
	root->childCnt = possible;		/* 자식 노드개수 저장 */
	return bestValue;				/* bestValue 값 반환 */
};

/**
	함 수 : Maximize(struct treeNode* root)
	기 능 : 현재 자식 노드들 eval값중에서 제일 큰 값으로 저장하는 함수
*/
int TicTacToeAI::Maximize(struct treeNode* root)
{
	Position iList[16];			/* 현재 게임판에서 놓을 수 있는 좌표 개수, 위치 저장 */
	int	bestValue = -10000;		/* besteval 값 선언 */
	int	possible;				/* 둘수있는 개수 저장 변수 */
		
	possible = GetPossibleMove(iList);	/*현재 가능한 수 좌표, 개수 저장 */
		
	if( possible == 0 || CheckEnd() || root->depth == LAlevel )	/* 가능한수 없거나, 게임종료면 eval 값을 계산하여 리턴 */
	{
		return EvaluateBoard(root);
	}		

	for( int i = 0; i < possible; i++ )	/* 가능한 수 만큼 Minimax 알고리즘을 사용하여 수를 계산 */
	{
		root->next[i] = new Node;					/* 노드 생성 후 초기화 */
		InitNode(root->next[i], root->depth+1);

		tttBoard.DoMove(iList[i].x , iList[i].y);	/* 가능한 수를 둠 */		
		int newValue = Minimize(root->next[i]);		/* Min함수호출로 값을 구해서 */
		root->next[i]->eval = newValue;				/* 노드에 저장 */
		tttBoard.UndoMove();						/* 두었던 수를 무름 */

		if( newValue > bestValue )				/* 구한 eval값이 best값보다 크다면 */
		{				
			bestValue = newValue;				/* 값을 변경하고 해당값으로 노드값에 저장 */
			root->eval = bestValue;	
		}		
	}
	root->childCnt = possible;					/* 자식 노드개수 저장 */
	return bestValue;							/* bestValue 값 반환 */
};

/**
	함 수 :	CheckSymmetric(GameBoard preBoard, GameBoard newBoard)
	기 능 : 함수 대칭성을 검사하는 함수
*/
int TicTacToeAI::CheckSymmetric(GameBoard preBoard, GameBoard newBoard)
{
	int i, j, k, check = 0;

	char newboard[4][4];	/* 검사 대상 게임판을 저장할 변수 */
	char copy[4][4];		
	char temp[4][4];

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
		{
			copy[i][j] = preBoard.board[i][j];		/* 이전 게임판을 복사 */
			newboard[i][j] = newBoard.board[i][j];	/* 검사 대상 게임판을 복사 */
		}

	 /* copy 게임판을 왼쪽으로 회전, 총 3번 회전 */
	for(i=0; i<3; i++)
	{
		for(j=0; j<4; j++)            
			for(k=0; k<4; k++)
				temp[j][k]=copy[j][k];

		for(j=0; j<4; j++)       
			for(k=0; k<4; k++)
				copy[3-k][j]=temp[j][k];
			
		if(CheckSameBoard(copy, newboard))	/* 한번 회전할때마다 같은지 검사 */
			return 1;						/* 같으면 1 반환 */
	}

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			temp[i][j]=copy[i][j];
	
	 /* copy 게임판을 좌우로 대칭 */
	for(i=0;i<4;i++)
	{
		copy[i][0]=temp[i][3];
		copy[i][1]=temp[i][2];
		copy[i][2] = temp[i][1];
		copy[i][3] = temp[i][0];
	}

	if(CheckSameBoard(copy, newboard))	/* 대칭한 게임판 검사 */
		return 1;						/* 같으면 1 반환 */
	

	/* copy 게임판을 왼쪽으로 회전, 총 3번 회전 */
	for(i=0; i<3; i++)
	{
		for(j=0; j<4; j++)               
			for(k=0; k<4; k++)
				temp[j][k]=copy[j][k];

		for(j=0; j<4; j++)
			for(k=0; k<4; k++)
				copy[3-k][j]=temp[j][k];
			
		if(CheckSameBoard(copy, newboard))	/* 한번 회전할때마다 같은지 검사 */
			return 1;						/* 같으면 1 반환 */
	}
	return 0;
};

/**
	함 수 : CheckSameBoard(char (*board1)[3], char (*board2)[3])
	기 능 :	기존 게임판과 검사할 게임판을 받아와서 일치하는지 여부 검사
*/
int TicTacToeAI::CheckSameBoard(char (*board1)[4], char (*board2)[4])
{
	int i, j;
	int count = 0;

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			if(board1[i][j] == board2[i][j])
				count++;

	if(count == 16)	
		return 1;    /* 같으면 1 반환 */
	else			
		return 0;    /* 다르면 0 반환 */
};

/**
	함 수 : GetPossibleMove(Position* iList)
	기 능 : 현재 게임판에서 수를 둘때 둘수있는 곳의 좌표과, 총 개수를 구함
*/
int TicTacToeAI::GetPossibleMove(Position* iList)
{
	int	iNum = 0;	/* 가능한 수 개수 저장할 변수 */
	int	check = 0;	/* 대칭성 검사 변수 */
	int k=0;

	GameBoard tempBoard = tttBoard;	/* 게임판 복사 */
		
	for(int i=0; i<4; i++ )
	{
		for(int j=0; j<4; j++)
		{
			if( tttBoard.board[i][j] == ' ' )
			{
				check = 0;
				for(int k=0; k<iNum; k++)
				{
					tttBoard.DoMove(i, j);
					tempBoard.DoMove(iList[k].x, iList[k].y);
					check =	CheckSymmetric(tempBoard, tttBoard);	/* 대칭성 검사 */
					tempBoard.UndoMove();
					tttBoard.UndoMove();
					if(check == 1)		/* 대칭된다면 스킵 */
						break;
				}
				
				if(!check)				/* 아니라면, */
				{
					iList[iNum].x = i;		/* 현재 x좌표, y좌표 저장 */
					iList[iNum].y = j;
					iNum++;					/* 개수 증가 */				
				}
			}
		}
	}			
	return iNum;	/* 개수 반환 */
};

/**
	함 수 : CheckEnd()
	기 능 : 현재 게임판 상태를 점검 하는 함수 
*/
int TicTacToeAI::CheckEnd()
{
	tttBoard.CheckState();		
	if( tttBoard.state != GameBoard::STATE_PLAY )
	{
		tttBoard.state = GameBoard::STATE_PLAY;
		return 1;
	}
	else
		return 0;
}

/**
	함 수 : EvaluateBoard()
	기 능 : 해당 게임판에서 이길수있는 수들을 검사해서 평가값을 반환해주는 함수 
*/
int TicTacToeAI::EvaluateBoard(struct treeNode* root)
{
	char calBoard[4][4];
	int eval = 0, k = 0;		
	int comA = 0, comB = 0;

	tttBoard.CheckState();
	if( tttBoard.state != GameBoard::STATE_PLAY )
	{
		if(tttBoard.state == GameBoard::STATE_WINA)
		{
			if(this->criterion == 'X')
				eval = 100 - root->depth;
			else
				eval = -100 + root->depth;
			tttBoard.state = GameBoard::STATE_PLAY;

			return eval;
		}
		else if(tttBoard.state == GameBoard::STATE_WINB)
		{
			if(this->criterion == 'X')
				eval = -100 + root->depth;
			else
				eval = 100 - root->depth;
			tttBoard.state = GameBoard::STATE_PLAY;

			return eval;
		}		
	}
	
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			calBoard[i][j] = tttBoard.board[i][j];

	/* 가로 3줄, 세로 3줄을 각각 X, O로 이길수있는 수를 계산 */
	for(int i=0;i<4;i++)
	{
		if( (calBoard[i][0]=='X' || calBoard[i][0]==' ') && 
			(calBoard[i][1]=='X' || calBoard[i][1]==' ') && 
			(calBoard[i][2]=='X' || calBoard[i][2]==' ') &&
			(calBoard[i][3] == 'X' || calBoard[i][3] == ' '))
			comA++;
		if( (calBoard[i][0]=='O' || calBoard[i][0]==' ') && 
			(calBoard[i][1]=='O' || calBoard[i][1]==' ') && 
			(calBoard[i][2]=='O' || calBoard[i][2]==' ') &&
			(calBoard[i][3] == 'O' || calBoard[i][3] == ' '))
			comB++;
		if( (calBoard[0][i]=='X' || calBoard[0][i]==' ') && 
			(calBoard[1][i]=='X' || calBoard[1][i]==' ') && 
			(calBoard[2][i]=='X' || calBoard[2][i]==' ') &&
			(calBoard[3][i] == 'X' || calBoard[3][i] == ' '))
			comA++;
		if( (calBoard[0][i]=='O' || calBoard[0][i]==' ') && 
			(calBoard[1][i]=='O' || calBoard[1][i]==' ') && 
			(calBoard[2][i]=='O' || calBoard[2][i]==' ') &&
			(calBoard[3][i] == 'O' || calBoard[3][i] == ' '))
			comB++;
	}
	
	/* 왼쪽, 오른쪽 대각선에서 이길수있는 수를 계산 */
	if( (calBoard[0][0]=='X' || calBoard[0][0]==' ') && 
		(calBoard[1][1]=='X' || calBoard[1][1]==' ') && 
		(calBoard[2][2]=='X' || calBoard[2][2]==' ') &&
		(calBoard[3][3] == 'X' || calBoard[3][3] == ' '))
		comA++;
	if( (calBoard[0][0]=='O' || calBoard[0][0]==' ') && 
		(calBoard[1][1]=='O' || calBoard[1][1]==' ') && 
		(calBoard[2][2]=='O' || calBoard[2][2]==' ') &&
		(calBoard[3][3] == 'O' || calBoard[3][3] == ' '))
		comB++;

	if( (calBoard[0][3]=='X' || calBoard[0][3]==' ') && 
		(calBoard[1][2]=='X' || calBoard[1][2]==' ') && 
		(calBoard[2][1]=='X' || calBoard[2][1]==' ') &&
		(calBoard[3][0] == 'X' || calBoard[3][0] == ' '))
		comA++;
	if( (calBoard[0][3]=='O' || calBoard[0][3]==' ') && 
		(calBoard[1][2]=='O' || calBoard[1][2]==' ') && 
		(calBoard[2][1]=='O' || calBoard[2][1]==' ') &&
		(calBoard[3][0] == 'O' || calBoard[3][0] == ' '))
		comB++;

	/* 현재 AI기준이 컴퓨터 A인지 B인지에 따라 이길수있는 수를 계산해서 반환 */
	if(criterion == 'X')
		eval = comA-comB;
	else
		eval = comB-comA; 
	return eval;
};