#include "GameBoard.h"

class TicTacToeAI
{
public:
	GameBoard tttBoard;		/* 최적의 수를 구할 보드 판 */

	int		LAlevel;		/* Look Ahead 레벨 */
	char	criterion;		/* 이 AI 구동하는 기준 컴퓨터 */
	int		bestX;			/* 최적의 X 좌표 */
	int		bestY;			/* 최적의 Y 좌표 */
	Node* root;				/* 출력할 노드 구조체 */

public :

	TicTacToeAI(GameBoard board);		
	void InitNode(Node* node, int inDepth);		/* AI 기본정보로 초기화 */
	Node* GetRootNode();						/* Tree 를 출력하기위한 Node 반환 */
	void GetBestMove();							/* 최적의 좌표를 구함 */
	int Minimize(struct treeNode* root);		/* Eval 값중 Min 값을 구함 */	
	int Maximize(struct treeNode* root);		/* Eval 값중 Max 값을 구함 */
	int CheckSymmetric(GameBoard preBoard, GameBoard newBoard);	/* 게임보드 대칭성 검사 */
	int CheckSameBoard(char (*board1)[4], char (*board2)[4]);	/* 대칭성 검사 중 보드가 일치하는지 검사 */
	int GetPossibleMove(Position* iList);						/* 현재 보드상태에서 갈수있는 좌표 개수 검사 */
	int CheckEnd();								/* 게임이 끝났는지 검사 */
	int EvaluateBoard(struct treeNode* root);						/* 현재 보드판의 Eval(평가값) 계산 */

};