#include<stdio.h>
#include<easyx.h>
#include<graphics.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>

#define ROWS 15//���̸��ӳߴ�
#define MAX_ROW 15
#define COLS 15
#define GRID_SIZE 65//���سߴ�
#define XOFFSET 40//ƫ����
#define YOFFSET 40


enum ChessType {//���̸��ӵ�״̬
	None = 0,
	Black = 1,
	White = -1
};
enum Player {//�������˻��ǵ���
	robot = -1,
	human = 1,

};





//��Ϸ�ṹ
struct Game {
	//0��ʾû�����ӣ�1��ʾ���壬-1��ʾ����
	int map[ROWS][COLS];
	//��Ϸ�Ƿ�������
	bool isRunning;
	//������Ϣ����
	ExMessage msg;//��Ϣ�ṹ�壬����������
	//��굱ǰ�����±�
	int row = 0;
	int col = 0;
	//��һ���µ����ӵ�λ������
	int currentchess_row = 0;
	int currentchess_col = 0;
	//��ǰ����
	ChessType currentChessType;
	Player player;
	//�������������
	int robotRow = 1;
	int robotCol = 1;
};
struct DirectionMap {
	int horizontal = {};
	int vertical = {};
	int leftOblique = {};
	int rightOblique = {};
};
typedef struct {
	int NumOf_2Dif[6], NumOf_1Dif[6], NumOf_0dif[6],NumOf_0jump[6],NumOf_1jump[6], NumOf_2jump[6],NumOf_warning;
}Count;

typedef struct State {
	int board[ROWS][COLS];//��һ��������
	int val;//��һ��������
	ChessType chesstype;//��һ������������
	int x, y;//���������
	State* prev;//��һ�����
} State;



//��Ϸ��ʼ��
void init(Game* pthis, int w, int h);
//��Ϸ����
void render(Game* pthis);
//��Ϸ����
void update(Game* pthis);
//������Ϸ����
void creatUI(Game* pthis, IMAGE* theimage);
//�˻��л�
void shiftplayer(Game* pthis);
//��������
void robotplay(Game* pthis);
//����������������
void figure_coord(Game* pthis);

//���������ӵ��ھӿ�λ��ֻ������Щ��λ
bool isNextchess(State* state, int x, int y, int n);//�ж�(x,y)�����Ƿ�Ϊ����������λ�á�
//nΪ1���ж�ֱ�����ڵĿ�λ��nΪ2���ж��������ڵ����п�λ

// �ֲ���������
int score(State* state, ChessType a);










//�ж�����Ϊ(x,y)��������Χ�����
//��һ10�����100������1000������10000������100000������10������100������1000��
//�����������û�б����������Ϊ����ֻ����һ�࣬��Ϊ�������������������û�з�����
Count detect_succession(int MAP[][COLS], ChessType a, DirectionMap directionMap[][COLS]);
//����������������
//��һ10�����100������1000������10000������100000������10������100������1000��
int countPoint(int MAP[][COLS], ChessType a, DirectionMap directionMap[][COLS]);
//������
//void generate_moves(State* state, int depth, bool maximizing_player, State* best_move);
//State alpha_beta_search(State* state, int depth);
//void generate_moves(State* state, int depth, bool maximizing_player, State* best_move);
//bool is_game_over(State* state);
//int search(int depth,State stepNode, int alpha, int beta, bool is_max);

bool is_win(State* state) {
	int x = state->x;
	int y = state->y;
	int player = state->board[x][y];
	int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };

	for (int i = 0; i < 4; i++) {
		int count = 1;
		for (int j = 1; j < 5; j++) {
			int new_x = x + directions[i][0] * j;
			int new_y = y + directions[i][1] * j;
			if (new_x >= 0 && new_x < 15 && new_y >= 0 && new_y < 15 && state->board[new_x][new_y] == player) {
				count++;
			}
			else {
				break;
			}
		}
		for (int j = 1; j < 5; j++) {
			int new_x = x - directions[i][0] * j;
			int new_y = y - directions[i][1] * j;
			if (new_x >= 0 && new_x < 15 && new_y >= 0 && new_y < 15 && state->board[new_x][new_y] == player) {
				count++;
			}
			else {
				break;
			}
		}
		if (count >= 5) {
			return true;
		}
	}
	return false;
}
int search(State* StepNode, int depth, int alpha, int beta, bool is_max) {
	if (depth == 0 || is_win(StepNode)) {
		int s = score(StepNode, is_max ? Black : White);
		return s;
	}

	if (is_max) {
		int maxEval = -INFINITY; // ���踺����Ϊ��Сֵ
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				if (isNextchess) {
					State* NewstepNode = (State*)malloc(sizeof(State));
					memcpy(NewstepNode, StepNode, sizeof(State));
					NewstepNode->board[i][j] = Black; // ����
					NewstepNode->prev = StepNode;
					NewstepNode->x = i;
					NewstepNode->y = j;
					int s = search(NewstepNode, depth - 1, alpha, beta, is_max);
					NewstepNode->val = s;
					maxEval = max(maxEval, s);
					alpha = max(alpha, s); // ����alphaֵ
					if (beta <= alpha) { // alpha-beta��֦
						free(NewstepNode);
						return maxEval; // ���ص�ǰ���ֵ
					}
				}
			}
		}
		return maxEval; // �������ֵ
	}
	else { // is_min
		int minEval = INFINITY; // ����������Ϊ���ֵ
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				if (isNextchess) {
					State* NewstepNode = (State*)malloc(sizeof(State));
					memcpy(NewstepNode, StepNode, sizeof(State));
					NewstepNode->board[i][j] = White; // ����
					NewstepNode->prev = StepNode;
					NewstepNode->x = i;
					NewstepNode->y = j;
					int s = search(NewstepNode, depth - 1, alpha, beta, is_max);
					NewstepNode->val = s;
					minEval = min(minEval, s);
					beta = min(beta, s); // ����betaֵ
					if (beta <= alpha) { // alpha-beta��֦
						free(NewstepNode);
						return minEval; // ���ص�ǰ��Сֵ
					}
				}
			}
		}
		return minEval; // ������Сֵ
	}
}
typedef struct {
	int x;
	int y;
} Coordinate;

Coordinate get_best_move(State* state, int depth) {
	Coordinate best_move = { -1, -1 }; // ��ʼ���������Ϊ��Чֵ
	int best_score = -INFINITY; // ��ʼ����ѵ÷�Ϊ������

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (isNextchess(state, i, j,2)) { // �жϸ�λ���Ƿ��������
				State* new_state = (State*)malloc(sizeof(State));
				memcpy(new_state, state, sizeof(State));
				new_state->board[i][j] = Black; // ��������Ϊ��ɫ
				new_state->prev = state;
				new_state->x = i;
				new_state->y = j;

				int score = search(new_state, depth, -INFINITY, INFINITY, true); // ���������Ӻ�ľ���÷�
				if (score > best_score) { // ������ѵ÷ֺ��������
					best_score = score;
					best_move.x = i;
					best_move.y = j;
				}
				free(new_state); // �ͷ��ڴ�
			}
		}
	}
	return best_move; // ���������������
}





//�ж���������
bool horizontal(Game* pthis, int chess);
bool vertical(Game* pthis, int chess);
bool leftOblique(Game* pthis, int chess);
bool rightOblique(Game* pthis, int chess);
bool judge(Game* pthis);





int main() {
	Game game;
	init(&game, 1000, 1000);
	IMAGE img_bg;//����ͼƬ
	loadimage(&img_bg, L"D:\\��������Ϸ\\bk.jpg");//������Ҫ��ͼƬ
	putimage(0, 0, &img_bg);

	//����������

	//��Ϸ��ѭ��



	
	//Ĭ����������
	game.player = human;
	while (game.isRunning) {

		//��ȡ��Ϣ
		//��δ����
		if (judge(&game)) {
			MessageBox(GetHWnd(), L"������", L"XXX ", MB_OK);
			game.isRunning = false;
		}
		if (game.player == human) {
			if (peekmessage(&game.msg)) {
				update(&game);//����������ɰ�ɫ����
			}
			creatUI(&game, &img_bg);//���ɡ�������Ϸ����
		}
		else if (game.player == robot) {

			robotplay(&game);
			creatUI(&game, &img_bg);//���ɡ�������Ϸ����
			shiftplayer(&game);//�˻��л�
		}
	}
	return 0;
}
//��ʼ������
void init(Game* pthis, int w, int h) {
	srand(time(NULL));
	//����һ������
	initgraph(w, h, EX_SHOWCONSOLE);
	pthis->isRunning = true;
	pthis->row = -1;
	pthis->col = -1;
	//pthis->currentChessType=rand()%2?Black:White;
	pthis->currentChessType = White;
	//��������
	//0��ʾû�����ӣ�1��ʾ���壬-1��ʾ����
	int map[ROWS][COLS] = {  };
	memcpy(pthis->map, map, sizeof(int) * ROWS * COLS);
}
//��������
void render(Game* pthis) {
	//��������
	for (int i = 0; i < ROWS; i++) {
		for (int k = 0; k < COLS; k++) {


			if (pthis->map[i][k] != None) {
				//��ÿ���������Ͻǵ�����
				int x = k * GRID_SIZE + XOFFSET;
				int y = i * GRID_SIZE + YOFFSET;
				//��������
				if (pthis->map[i][k] == White) {
					setfillcolor(WHITE);
				}
				else if (pthis->map[i][k] == Black) {
					setfillcolor(BLACK);
				}

				//��������
				solidcircle(x, y, 20);
			}
		}
	}
	//���Ƶ�ǰ������ڵ���ʾ��
	if (pthis->row != -1 && pthis->col != -1) {
		setlinecolor(BLUE);//������ʾ��������ɫ
		int x = pthis->col * GRID_SIZE + XOFFSET;
		int y = pthis->row * GRID_SIZE + YOFFSET;
		circle(x, y, 25);
	}
}
void update(Game* pthis) {

	//����ƶ����ж����ָ��λ���ĸ����ӵ�����
	if (pthis->msg.message == WM_MOUSEMOVE) {
		for (int i = 0; i < ROWS; i++) {
			for (int k = 0; k < COLS; k++) {
				int cx = k * GRID_SIZE + XOFFSET;
				int cy = i * GRID_SIZE + YOFFSET;
				//�ж����ָ��λ�������ĸ����ӵ�����
				if (abs(pthis->msg.x - cx) < GRID_SIZE / 2 && abs(pthis->msg.y - cy) < GRID_SIZE / 2) {
					pthis->row = i;
					pthis->col = k;
					goto END_LOOP;
				}
			}
		}
	END_LOOP:;//��ת�ڴ�
	}

	//�³����ӣ�����pthis->map�ϼ�¼����λ�ú���ɫ
	else if (pthis->msg.message == WM_LBUTTONDOWN//���������
		&& pthis->row != -1 && pthis->col != -1 &&//����˺Ϸ���λ��
		pthis->map[pthis->row][pthis->col] == None)//��ǰλ��û������
	{
		pthis->map[pthis->row][pthis->col] = pthis->currentChessType;
		//��¼�����������ӵ�λ��
		pthis->currentchess_row = pthis->row;
		pthis->currentchess_col = pthis->col;
		//�л�����
		pthis->currentChessType = (ChessType)-pthis->currentChessType;
		shiftplayer(pthis);//�˻��л�

	}
}
//�ж�ʤ��
bool judge(Game* pthis) {
	int who = -pthis->currentChessType;
	if (horizontal(pthis, who))return true;
	if (vertical(pthis, who))return true;
	if (leftOblique(pthis, who))return true;
	if (rightOblique(pthis, who))return true;
	return false;
}
//�ж���������
//δ����
bool horizontal(Game* pthis, int chess) {
	int counter = 1; // ���赱ǰλ���Ѿ��Ǹ����ӵ���ɫ
	int start = pthis->currentchess_col - 1; // �ӵ�ǰ�е�ǰһ�п�ʼ���
	int end = pthis->currentchess_col + 1; // �ӵ�ǰ�еĺ�һ�п�ʼ���

	// ������
	for (int i = start; i >= 0 && pthis->map[pthis->currentchess_row][i] == chess; i--) {
		counter++;
	}
	// ���Ҽ��
	for (int i = end; i < 15 && pthis->map[pthis->currentchess_row][i] == chess; i++) {
		counter++;
	}

	return counter >= 5;
}

bool vertical(Game* pthis, int chess) {
	int counter = 1; // ���赱ǰλ���Ѿ��Ǹ����ӵ���ɫ
	int start = pthis->currentchess_row - 1; // �ӵ�ǰ�е�ǰһ�п�ʼ���
	int end = pthis->currentchess_row + 1; // �ӵ�ǰ�еĺ�һ�п�ʼ���

	// ���ϼ��
	for (int i = start; i >= 0 && pthis->map[i][pthis->currentchess_col] == chess; i--) {
		counter++;
	}
	// ���¼��
	for (int i = end; i < 15 && pthis->map[i][pthis->currentchess_col] == chess; i++) {
		counter++;
	}

	return counter >= 5;
}

bool leftOblique(Game* pthis, int chess) {
	int counter = 1; // ���赱ǰλ���Ѿ��Ǹ����ӵ���ɫ
	int startRow = pthis->currentchess_row - 1;
	int startCol = pthis->currentchess_col - 1;

	// �����ϼ��
	for (int i = startRow, j = startCol; i >= 0 && j >= 0 && pthis->map[i][j] == chess; i--, j--) {
		counter++;
	}
	// �����¼��
	for (int i = pthis->currentchess_row + 1, j = pthis->currentchess_col + 1; i < 15 && j < 15 && pthis->map[i][j] == chess; i++, j++) {
		counter++;
	}

	return counter >= 5;
}

bool rightOblique(Game* pthis, int chess) {
	int counter = 1; // ���赱ǰλ���Ѿ��Ǹ����ӵ���ɫ
	int startRow = pthis->currentchess_row - 1;
	int startCol = pthis->currentchess_col + 1;

	// �����ϼ��
	for (int i = startRow, j = startCol; i >= 0 && j < 15 && pthis->map[i][j] == chess; i--, j++) {
		counter++;
	}
	// �����¼��
	for (int i = pthis->currentchess_row + 1, j = pthis->currentchess_col - 1; i < 15 && j >= 0 && pthis->map[i][j] == chess; i++, j--) {
		counter++;
	}

	return counter >= 5;
}

void shiftplayer(Game* pthis) {
	if (pthis->player == human) {
		pthis->player = robot;
	}
	else if (pthis->player == robot) {
		pthis->player = human;
	}
}
void robotplay(Game* pthis) {
	figure_coord(pthis);
	printf("(%d,%d)\n", pthis->robotRow, pthis->robotCol);
	pthis->map[pthis->robotRow][pthis->robotCol] = pthis->currentChessType;
	pthis->currentChessType = (ChessType)-pthis->currentChessType;

}
//������������Ϸ����
void creatUI(Game* pthis, IMAGE* theimage) {
	BeginBatchDraw();//��ʼ������ͼ��˫���壩
	cleardevice();//����
	putimage(0, 0, theimage);//�ű���ͼ
	render(pthis);//�������Ӻ���ʾ��
	EndBatchDraw();//����˫�����ͼ
}
//�㷨���ġ�������������������
//δ����
void figure_coord(Game* pthis) {
	
	State* state =(State*)malloc(sizeof(State));
	memcpy(state->board, pthis->map, sizeof(int) * ROWS * COLS);
	Coordinate best_move= get_best_move(state, 6);
	pthis->robotRow = best_move.x;
	pthis->robotCol = best_move.y;
	pthis->currentchess_row = pthis->robotRow;
	pthis->currentchess_col = pthis->robotCol;

}

//���������ӵ��ھӿ�λ��ֻ������Щ��λ
//�ж�(x,y)�����Ƿ�Ϊ����������λ�á�
//nΪ1���ж�ֱ�����ڵĿ�λ��nΪ2���ж��������ڵ����п�λ
bool isNextchess(State* state, int x, int y, int n) {
	int MAP[ROWS][COLS];
	memcpy(MAP, state->board, sizeof(int) * ROWS * COLS);
	int i, j=0;
	//printf("b1");
	if (MAP[x][y] == 0)
	{

		if (n == 1)
		{
			for (i = x - 1; i < x + 2; i++) {
				for (j = y - 1; j < y + 2; j++) {
					if ((i > 0 && j > 0 && i < ROWS && j < COLS) && (MAP[i][j] == 1 || MAP[i][j] == -1))
						return true;
				}
			}
		}
		else
		{
			for (i = x - 2; i < x + 3; i += 2) {
				for (j = y - 2; j < y + 3; j += 2) {
					if ((i > 0 && j > 0 && i < ROWS && j < COLS) && (MAP[i][j] == 1 || MAP[i][j] == -1))
						return true;
				}
			}
		}
	}
	return false;
}
//��ַ�������
//��һ10�����100������1000������10000������100000������10������100������1000��
//�����������û�б����������Ϊ����ֻ����һ�࣬��Ϊ�������������������û�з�����
int score(State* state, ChessType a) {
	int MAP[ROWS][COLS];
	memcpy(MAP, state->board, sizeof(int) * ROWS * COLS);
	DirectionMap directionMap[ROWS][COLS] = {};
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			if (MAP[i][j] == 0) {
				directionMap[i][j].horizontal = 2;
				directionMap[i][j].vertical = 2;
				directionMap[i][j].leftOblique = 2;
				directionMap[i][j].rightOblique = 2;//��û�����ӵ�λ�ñ�ǣ�����ʱ��������Щ��
			}
		}
	}
	
		int Point = countPoint(MAP, Black, directionMap)-countPoint(MAP,White,directionMap);
	
	
	return Point;

}

//��һ10�����100������1000������10000������100000������10������100������1000��
//�����������û�б����������Ϊ����ֻ����һ�࣬��Ϊ�������������������û�з�����
Count detect_succession(int MAP[][COLS], ChessType a, DirectionMap directionMap[][COLS]) 
{   
	int point = 0;//��¼�÷�
	int x, y;
	int NumOfSame, NumOfDifference,NumOfjump = 0;
	Count count = { 0 };
	

	//1.�������
	for (int j = 0; j < COLS; j++) 
	{
		for (int i = 0; i < ROWS; i++)
		{
			x = i, y = j;
			NumOfSame, NumOfDifference = 0;
			//�����Ӧ�㲻����������a����ֱ������
			if (MAP[x][y] != a || directionMap[x][y].horizontal != 0) {
				continue;
			}

			while (MAP[x][y] == a && directionMap[x][y].horizontal == 0 && x < ROWS) {
				NumOfSame = 1;
				NumOfDifference = 0;

				directionMap[x][y].horizontal = 1;//��ǣ��Ѿ�������
				NumOfSame++;//��������������+1
				x++;//����������һ������
			}
			if (x<ROWS&&MAP[x][y] == -a) {
				NumOfDifference++;//��¼���ҷ������ʱײ�ϵĵ�һ����ɫ����
				
			}
			else {
				x++;
				if (x < ROWS && MAP[x][y] == a) {
					NumOfjump++;//��¼�����ĵ�һ���м����һ�ӵ����
				}
				x--;
			}
			x -= NumOfSame;//�ص�������
			//��ʼ���������
			while (MAP[x][y] == a && directionMap[x][y].horizontal == 0 && x >= 0) {
				directionMap[x][y].horizontal = 1;//��ǣ��Ѿ�������
				NumOfSame++;//��������������+1
				x--;//����������һ������

			}
			if (x>0&&MAP[x][y] == -a) {
				NumOfDifference++;//��¼ײ�ϵĵڶ�����ɫ����
				
			}
			else {
				x--;
				if (x >= 0 && MAP[x][y] == a) {
					NumOfjump++;
				}
				x++;
			}
			if (NumOfDifference == 0)count.NumOf_0dif[NumOfSame-1]++;
			if (NumOfDifference == 1)count.NumOf_1Dif[NumOfSame-1]++;
			if (NumOfDifference == 2) count.NumOf_2Dif[NumOfSame - 1]++;
			if (NumOfjump == 0)count.NumOf_0jump[NumOfSame - 1]++;
			if (NumOfjump == 1)count.NumOf_1jump[NumOfSame - 1]++;
			if (NumOfjump == 2)count.NumOf_2jump[NumOfSame - 1]++;
			if (a == White && NumOfSame >= 3)count.NumOf_warning+=NumOfSame;
			NumOfSame = 0;
		}
	}


	//2.�������
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			x = i, y = j;
			//�����Ӧ�㲻����������a����ֱ������
			if (MAP[x][y] != a || directionMap[x][y].vertical != 0)
			{
				continue;
			}

			while (MAP[x][y] == a && directionMap[x][y].vertical == 0 && y < ROWS)
			{
				NumOfSame = 1;
				NumOfDifference = 0;

				directionMap[x][y].horizontal = 1;//��ǣ��Ѿ�������
				NumOfSame++;//��������������+1
				y++;//����������һ������
			}
			if (y<COLS && MAP[x][y] == -a)
			{
				NumOfDifference++;//��¼���·������ʱײ�ϵĵ�һ����ɫ����
				
			}
			else {
				y++;
				if (y < COLS && MAP[x][y] == a) {
					NumOfjump++;
				}
				y--;
			}
			y -= NumOfSame;//�ص�������
			//��ʼ���Ϸ������
			while (MAP[x][y] == a && directionMap[x][y].vertical == 0 && x >= 0)
			{
				directionMap[x][y].vertical = 1;//��ǣ��Ѿ�������
				NumOfSame++;//��������������+1
				y--;//����������һ������

			}
			if (MAP[x][y] == -a)
			{
				NumOfDifference++;//��¼ײ�ϵĵڶ�����ɫ����
				
			}
			else {
				y--;
				if (y >= 0 && MAP[x][y] == a) {
					NumOfjump++;
				}
				y++;
			}
			if (NumOfDifference == 0)count.NumOf_0dif[NumOfSame - 1]++;
			if (NumOfDifference == 1)count.NumOf_1Dif[NumOfSame - 1]++;
			if (NumOfDifference == 2)count.NumOf_2Dif[NumOfSame - 1]++;
			if (NumOfjump == 0)count.NumOf_0jump[NumOfSame - 1]++;
			if (NumOfjump == 1)count.NumOf_1jump[NumOfSame - 1]++;
			if (NumOfjump == 2)count.NumOf_2jump[NumOfSame - 1]++;
			if (a == White && NumOfSame >= 3)count.NumOf_warning += NumOfSame;
			NumOfSame = 0;
		}
	}


	//3.б������&&����
	for (int j = 0; j < COLS; j++)
	{
		for (int i = 0; i < ROWS; i++)
		{
			x = i, y = j;
			//�����Ӧ�㲻����������a����ֱ������
			if (MAP[x][y] != a || directionMap[x][y].leftOblique != 0)
			{
				continue;
			}

			while (MAP[x][y] == a && directionMap[x][y].leftOblique == 0 && y < ROWS && x < ROWS)
			{
				NumOfSame = 1;
				NumOfDifference = 0;

				directionMap[x][y].leftOblique = 1;//��ǣ��Ѿ�������
				NumOfSame++;//��������������+1
				x++;
				y++;//����������һ������
			}
			if (MAP[x][y] == -a)
			{
				NumOfDifference++;//��¼�����·������ʱײ�ϵĵ�һ����ɫ����
				
			}else {
				x++;
				y++;
				if (x < ROWS && y < COLS && MAP[x][y] == a) {
					NumOfjump++;
				}
				x--;
				y--;
			}
				x -= NumOfSame;
				y -= NumOfSame;//�ص�������
			
			//��ʼ�����Ϸ������
			while (MAP[x][y] == a && directionMap[x][y].leftOblique == 0 && x >= 0 && y >= 0)
			{
				directionMap[x][y].leftOblique = 1;//��ǣ��Ѿ�������
				NumOfSame++;//��������������+1
				x--;
				y--;//����������һ������

			}
			if (MAP[x][y] == -a)
			{
				NumOfDifference++;//��¼ײ�ϵĵڶ�����ɫ����
				
			}
			else {
				x--;
				y--;
				if (x >= 0 && y >= 0 && MAP[x][y] == a) {
					NumOfjump++;
				}
				x++;
				y++;
			}
			if (NumOfDifference == 0)count.NumOf_0dif[NumOfSame - 1]++;
			if (NumOfDifference == 1)count.NumOf_1Dif[NumOfSame - 1]++;
			if (NumOfDifference == 2) count.NumOf_2Dif[NumOfSame - 1]++;
			if (NumOfjump == 0)count.NumOf_0jump[NumOfSame - 1]++;
			if (NumOfjump == 1)count.NumOf_1jump[NumOfSame - 1]++;
			if (NumOfjump == 2)count.NumOf_2jump[NumOfSame - 1]++;
			if (a == White && NumOfSame >= 3)count.NumOf_warning += NumOfSame;
			NumOfSame = 0;
		}
	}

	//4.б������&����
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = COLS - 1; j >= 0; j--)
		{
			x = i, y = j;
			//�����Ӧ�㲻����������a����ֱ������
			if (MAP[x][y] != a || directionMap[x][y].rightOblique != 0)
			{
				continue;
			}

			while (MAP[x][y] == a && directionMap[x][y].rightOblique == 0 && y < COLS && x >= 0)
			{
				NumOfSame = 1;
				NumOfDifference = 0;

				directionMap[x][y].rightOblique = 1;//��ǣ��Ѿ�������
				NumOfSame++;//��������������+1
				x--;
				y++;//����������һ������
			}
			if (MAP[x][y] == -a)
			{
				NumOfDifference++;//��¼�����·������ʱײ�ϵĵ�һ����ɫ����

			}
			else {
				x--;
				y++;
				if (x >= 0 && y < COLS && MAP[x][y] == a) {
					NumOfjump++;
				}
				x++;
				y--;
			}
			x += NumOfSame;
			y -= NumOfSame;//�ص�������
			//��ʼ�����Ϸ������
			while (MAP[x][y] == a && directionMap[x][y].rightOblique == 0 && y >= 0 && x < ROWS)
			{
				directionMap[x][y].rightOblique = 1;//��ǣ��Ѿ�������
				NumOfSame++;//��������������+1
				x++;
				y--;//����������һ������

			}
			if (MAP[x][y] == -a)
			{
				NumOfDifference++;//��¼ײ�ϵĵڶ�����ɫ����
				
			}
			else {
				x++;
				y--;
				if (x < ROWS && y >= 0 && MAP[x][y] == a) {
					NumOfjump++;
				}
				x--;
				y++;
			}
			if (NumOfDifference == 0)count.NumOf_0dif[NumOfSame - 1]++;
			if (NumOfDifference == 1)count.NumOf_1Dif[NumOfSame - 1]++;
			if (NumOfDifference == 2) count.NumOf_2Dif[NumOfSame - 1]++;
			if (NumOfjump == 0)count.NumOf_0jump[NumOfSame - 1]++;
			if (NumOfjump == 1)count.NumOf_1jump[NumOfSame - 1]++;
			if (NumOfjump == 2)count.NumOf_2jump[NumOfSame - 1]++;
			if (a == White && NumOfSame >= 3)count.NumOf_warning += NumOfSame;
			NumOfSame = 0;
		}
	}
	return count;
}
//����������������
//��һ10�����100������1000������10000������100000������10������100������1000��
int countPoint(int MAP[][COLS], ChessType a, DirectionMap directionMap[][COLS]) {
	int point = 0;
	Count count=detect_succession(MAP, a, directionMap);
	for (int i = 0; i < 5; i++) {
		point += count.NumOf_0dif[i] * pow(10, i) + count.NumOf_1Dif[i] * pow(9, i) + count.NumOf_2Dif[i] * 0 +
			count.NumOf_0jump[i] * pow(9, i) + count.NumOf_1jump[i] * pow(10, i)+count.NumOf_2jump[i]*(pow(10,i)+pow(8,i))+count.NumOf_warning*100;
	}
	return point;

}
