#include <graphics.h>
#include <conio.h>
#include <math.h>
#include <iostream>
#pragma comment(lib,"Winmm.lib")
using namespace std;

#define N 15
#define block_width 40
#define margin 30
#define board_width 660
#define board_height 660

int chessBox[N][N] = { 0 };
int aiScore[N][N] = { 0 }; // ����Ϊȫ�ֱ���
int humanScore[N][N] = { 0 }; // ����������ҷ���

double checkBound = block_width * 0.4; // Mouse check range.
int clickRow, clickCol; // Effective click location.
enum GameMode { NORMAL, HERO };//ѡ��ģʽ
enum ChessColor { NONE = 0, black = 1, white = 2 };
ChessColor playerColor = black; // Ĭ�����ִ����
ChessColor aiColor = white; // Ĭ��AIִ����
bool colorChosen = false; // ����Ƿ��Ѿ�ѡ������ɫ
bool modeChosen = false;//����Ƿ��Ѿ�ѡ����ģʽ
bool skill = false;//�Ƿ񷢶�����
bool skill22 = false;
GameMode gameMode;//ģʽ
IMAGE blackHero, whiteHero; // ����Ӣ�۶Ծ�ģʽ�µ�����ͼƬ
IMAGE skillButton;//���ܰ�ť

void init() {
    initgraph(board_width, board_height);
    setbkcolor(RGB(255, 205, 150));
    settextcolor(RGB(0,0,0));
    cleardevice();
    IMAGE picture;
    loadimage(&picture, "begin.jpg", 660, 660);
    putimage(0, 0, &picture);
    // �����δѡ��ģʽ����ʾģʽѡ�����
    if (!modeChosen) {
        ExMessage m;
        while (!modeChosen) {
            m = getmessage(EM_MOUSE);
            if (m.message == WM_LBUTTONDOWN) {
                if (m.x > 220 && m.x < 440 && m.y > 390 && m.y < 460) {
                    gameMode = NORMAL;
                    modeChosen = true;
                }
                else if (m.x > 220 && m.x < 440 && m.y > 510 && m.y < 580) {
                    gameMode = HERO;
                    modeChosen = true;
                    // ����Ӣ�۶Ծ�ģʽ�µ�����ͼƬ
                    loadimage(&blackHero, _T("blackHero.png"));
                    loadimage(&whiteHero, _T("whiteHero.png"));
                   
                    
                }
            }
        }
        cleardevice();
    }

    // �����δѡ����ɫ����ʾ��ɫѡ�����
    if (!colorChosen) {
        IMAGE picture;
        loadimage(&picture, "begin2.png", 660, 660);
        putimage(0, 0, &picture);
        if (gameMode == NORMAL) {
            settextstyle(40, 0, _T("΢���ź�"));
            outtextxy(50, 250, _T("�����ɫ��ѡ�����"));
            setfillcolor(BLACK);
            solidcircle(200, 400, block_width / 2);
            settextstyle(40, 0, _T("΢���ź�"));
            outtextxy(350, 250, _T("�����ɫ��ѡ�����"));
            setfillcolor(WHITE);
            solidcircle(450, 400, block_width / 2);
            // �ȴ����ѡ����ɫ
            ExMessage m;
            while (!colorChosen) {
                m = getmessage(EM_MOUSE);
                if (m.message == WM_LBUTTONDOWN) {
                    if (m.x > 180 && m.x < 220 && m.y > 380 && m.y < 420) {
                        playerColor = black;
                        aiColor = white;
                        colorChosen = true;
                    }
                    else if (m.x > 430 && m.x < 470 && m.y > 380 && m.y < 420) {
                        playerColor = white;
                        aiColor = black;
                        colorChosen = true;
                    }
                }
            }
        }
        else if (gameMode == HERO) {
            settextstyle(40, 0, _T("΢���ź�"));
            outtextxy(230, 50, _T("��ѡ�����Ӣ��"));
            settextstyle(30, 0, _T("΢���ź�"));
            outtextxy(50, 250, _T("�����ͼƬ��ѡ��ڴ�˧"));
            putimage(50, 300, &blackHero);

            settextstyle(30, 0, _T("΢���ź�"));
            outtextxy(350, 250, _T("�����ͼƬ��ѡ��������"));
            putimage(350, 300, &whiteHero);

            // �ȴ����ѡ��
            ExMessage m;
            while (!colorChosen) {
                m = getmessage(EM_MOUSE);
                if (m.message == WM_LBUTTONDOWN) {
                    if (m.x > 50 && m.x < 300 && m.y > 250 && m.y < 500) {
                        playerColor = black;
                        aiColor = white;
                        colorChosen = true;
                    }
                    else if (m.x > 350 && m.x < 600 && m.y > 300 && m.y < 550) {
                        playerColor = white;
                        aiColor = black;
                        colorChosen = true;
                    }
                }
            }
        }
        cleardevice();
    }

    // ��������
    setlinecolor(BLACK);
    for (int i = 0; i < N; i++) {
        line(margin, margin + i * block_width, margin + (N - 1) * block_width, margin + i * block_width);
        line(margin + i * block_width, margin, margin + i * block_width, margin + (N - 1) * block_width);
    }

    // ������Ԫ
    int centerX = margin + (N / 2) * block_width;
    int centerY = margin + (N / 2) * block_width;
    int radius = 5;
    setfillcolor(BLACK);
    solidcircle(centerX, centerY, radius);

    // ���� (����)
    settextstyle(25, 0, _T("΢���ź�"));
    setbkmode(TRANSPARENT);
    for (int i = 0; i < N; i++) {
        char num[3];
        sprintf_s(num, "%d", i + 1);
        outtextxy(margin / 2-10, margin + i * block_width - 10, num);
    }

    // ���� (��ĸ)
    char alphabet[2] = "A";
    for (int i = 0; i < N; i++) {
        outtextxy(margin + i * block_width - 5, margin / 2 - 10, alphabet);
        alphabet[0]++;
    }

    // ���ѡ����Ӣ��ģʽ�����Ƽ��ܰ�ť
    if (gameMode == HERO&& playerColor == black) {
        IMAGE picture;
        loadimage(&picture, "skillButton.jpg", 50, 50);
        putimage(board_width - 50, board_height - 50, &picture);
    }
    if (gameMode == HERO && playerColor == white) {
        IMAGE picture;
        loadimage(&picture, "skill2.jpg", 50, 50);
        putimage(board_width - 50, board_height - 50, &picture);
    }
}

// Draw Chess
void drawChess(int row, int col, ChessColor color) {
    int x = margin + col * block_width;
    int y = margin + row * block_width;

    if (gameMode == NORMAL) {
        COLORREF chessColor = (color == black) ? RGB(0, 0, 0) : RGB(255, 255, 255);
        setfillcolor(chessColor);
        solidcircle(x, y, block_width / 2 - 5);
    }
    else if (gameMode == HERO) {
        IMAGE picture;
        if (color == black) {
            loadimage(&picture, "blackHero.png", block_width - 10, block_width - 10);
        }
        else if (color == white) {
            loadimage(&picture, "whiteHero.png", block_width - 10, block_width - 10);
        }
        putimage(x - (block_width / 2 - 5), y - (block_width / 2 - 5), &picture);
    }

    PlaySound(TEXT("sound_put.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

// Check click point
bool checkClick(ExMessage msg) {
    int x = msg.x;
    int y = msg.y;
    int col = (x - margin) / block_width;
    int row = (y - margin) / block_width;

    // Check if click is within the board boundaries
    if (row < 0 || row >= N-1 || col < 0 || col >= N-1) {
        return false;
    }

    int leftTopX = margin + col * block_width;
    int leftTopY = margin + row * block_width;
    double disLT, disLB, disRT, disRB;
    // Calculate 4 distances
    disLT = sqrt((x - leftTopX) * (x - leftTopX) + (y - leftTopY) * (y - leftTopY));
    disLB = sqrt((x - leftTopX) * (x - leftTopX) + (y - (leftTopY + block_width)) * (y - (leftTopY + block_width)));
    disRT = sqrt((x - (leftTopX + block_width)) * (x - (leftTopX + block_width)) + (y - leftTopY) * (y - leftTopY));
    disRB = sqrt((x - (leftTopX + block_width)) * (x - (leftTopX + block_width)) + (y - (leftTopY + block_width)) * (y - (leftTopY + block_width)));
    //Check if the distance is smaller than the boundary
    if (disLT < checkBound) {
        clickCol = col;
        clickRow = row;
    }
    else if (disLB < checkBound) {
        clickCol = col;
        clickRow = row + 1;
    }
    else if (disRT < checkBound) {
        clickCol = col + 1;
        clickRow = row;
    }
    else if (disRB < checkBound) {
        clickCol = col + 1;
        clickRow = row + 1;
    }
    else {
        return false;
    }

    return true;
}


// Check if there are five chess pieces in a line
bool checkWin(int row, int col, ChessColor color) {
    int count = 1; // Count for the current chess piece
    int i, j;

    // Check horizontally
    for (i = col + 1; i < N && chessBox[row][i] == color; i++, count++);
    for (i = col - 1; i >= 0 && chessBox[row][i] == color; i--, count++);
    if (count >= 5) return true;

    // Check vertically
    count = 1;
    for (j = row + 1; j < N && chessBox[j][col] == color; j++, count++);
    for (j = row - 1; j >= 0 && chessBox[j][col] == color; j--, count++);
    if (count >= 5) return true;

    // Check diagonally (top-left to bottom-right)
    count = 1;
    for (i = col + 1, j = row + 1; i < N && j < N && chessBox[j][i] == color; i++, j++, count++);
    for (i = col - 1, j = row - 1; i >= 0 && j >= 0 && chessBox[j][i] == color; i--, j--, count++);
    if (count >= 5) return true;

    // Check diagonally (top-right to bottom-left)
    count = 1;
    for (i = col + 1, j = row - 1; i < N && j >= 0 && chessBox[j][i] == color; i++, j--, count++);
    for (i = col - 1, j = row + 1; i >= 0 && j < N && chessBox[j][i] == color; i--, j++, count++);
    if (count >= 5) return true;

    return false;
}
bool isOutOfBounds(int row, int col) {
    return row < 0 || col < 0 || row >= N || col >= N;
}

bool isForbidden(int color, int row, int col) {
    int longChainCount = 0;
    int fourFourCount = 0;
    int threeThreeCount = 0;
    int directs[4][2] = { {1, 0}, {1, 1}, {0, 1}, {-1, 1} };

    for (int k = 0; k < 4; ++k) {
        int count = 1; // current point included
        int x = directs[k][0];
        int y = directs[k][1];

        for (int i = 1; i <= 4; ++i) {
            if (isOutOfBounds(row + i * y, col + i * x)) break;
            if (chessBox[row + i * y][col + i * x] == color) count++;
            else break;
        }
        for (int i = 1; i <= 4; ++i) {
            if (isOutOfBounds(row - i * y, col - i * x)) break;
            if (chessBox[row - i * y][col - i * x] == color) count++;
            else break;
        }

        // Check long chain (overlines)��������
        if (count > 5) ++longChainCount;

        // Check 4-4����
        if (count == 4) ++fourFourCount;

        // Check 3-3����
        if (count == 3) {
            int openEnds = 0;
            if (!isOutOfBounds(row + 4 * y, col + 4 * x) && chessBox[row + 4 * y][col + 4 * x] == 0) openEnds++;
            if (!isOutOfBounds(row - 4 * y, col - 4 * x) && chessBox[row - 4 * y][col - 4 * x] == 0) openEnds++;
            if (openEnds == 2) ++threeThreeCount;
        }
    }

    // Forbidden if there is long chain, or 4-4, or 3-3
    return longChainCount > 0 || fourFourCount >= 2 || threeThreeCount >= 2;
}

/*5.Calculate the score of current game*/
void calculateScore() {
    int humanNum = 0;
    int AINum = 0;
    int emptyNum = 0;
    int directs[4][2] = { {1,0}, {1,1}, {0,1}, {-1,1} };
    memset(humanScore, 0, sizeof(humanScore));
    memset(aiScore, 0, sizeof(aiScore));
    int scoreMap[15][15] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
        {0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
        {0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
        {0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
        {0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
        {0,1,2,3,4,5,6,7,6,5,4,3,2,1,0},
        {0,1,2,3,4,5,6,6,6,5,4,3,2,1,0},
        {0,1,2,3,4,5,5,5,5,5,4,3,2,1,0},
        {0,1,2,3,4,4,4,4,4,4,4,3,2,1,0},
        {0,1,2,3,3,3,3,3,3,3,3,3,2,1,0},
        {0,1,2,2,2,2,2,2,2,2,2,2,2,1,0},
        {0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };
    //Scan the board, calculate the score.
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            humanScore[row][col] = scoreMap[row][col];
            aiScore[row][col] = scoreMap[row][col];
            if (chessBox[row][col] == 0) {
                for (int k = 0; k < 4; k++) {
                    int x = directs[k][0];
                    int y = directs[k][1];
                    //reset
                    humanNum = 0;
                    AINum = 0;
                    emptyNum = 0;

                    //Calculate human score. (Forward)
                    for (int i = 1; i <= 4; i++) {
                        if (chessBox[row + i * y][col + i * x] == playerColor) {
                            humanNum++;
                        }
                        else if (chessBox[row + i * y][col + i * x] == 0) { // Empty points.
                            emptyNum++;
                            break;
                        }
                        else  // AI chess point.
                            break;
                    }
                    //Calculate human score. (Backward)
                    for (int i = 1; i <= 4; i++) {
                        if (chessBox[row - i * y][col - i * x] == playerColor) {
                            humanNum++;
                        }
                        else if (chessBox[row - i * y][col - i * x] == 0) { // Empty points.
                            emptyNum++;
                            break;
                        }
                        else  // AI chess point.
                            break;
                    }

                    //Check which type of chess is it.
                    if (humanNum == 1) { // ��������                
                        if (emptyNum == 1) { // �߶�
                            humanScore[row][col] += 10;
                        }
                        else if (emptyNum == 2) { // ���
                            humanScore[row][col] += 20;
                        }
                    }
                    else if (humanNum == 2) {
                        if (emptyNum == 1) // ����
                            humanScore[row][col] += 50;
                        else if (emptyNum == 2) // ����
                            humanScore[row][col] += 100;
                    }
                    else if (humanNum == 3) {
                        if (emptyNum == 1) // ����
                            humanScore[row][col] += 200;
                        else if (emptyNum == 2) // ����
                            humanScore[row][col] += 500;
                    }
                    else if (humanNum == 4) { // ����
                        humanScore[row][col] += 1000;
                    }
                    else {
                        humanScore[row][col] += 1;
                    }

                    emptyNum = 0;
                    //Calculate AI score. (Forward)
                    for (int i = 1; i <= 4; i++) {
                        if (chessBox[row + i * y][col + i * x] == aiColor) { // AI chess point.
                            AINum++;
                        }
                        else if (chessBox[row + i * y][col + i * x] == 0) { // Empty points.
                            emptyNum++;
                            break;
                        }
                        else { // Human chess point.
                            break;
                        }
                    }

                    //Calculate AI score. (Backward)
                    for (int i = 1; i <= 4; i++) {
                        if (chessBox[row - i * y][col - i * x] == aiColor) { // AI chess point.
                            AINum++;
                        }
                        else if (chessBox[row - i * y][col - i * x] == 0) { // Empty points.
                            emptyNum++;
                            break;
                        }
                        else { // Human chess point.
                            break;
                        }
                    }

                    //Check which type of chess is it.
                    if (AINum == 1) { // Two pieces connected.
                        if (emptyNum == 1) { // Sleep two.
                            aiScore[row][col] += 15;
                        }
                        else if (emptyNum == 2) { // Live two.
                            aiScore[row][col] += 25;
                        }
                    }
                    else if (AINum == 2) {
                        if (emptyNum == 1) // Sleep three
                            aiScore[row][col] += 60;
                        else if (emptyNum == 2) // Live three
                            aiScore[row][col] += 110;
                    }
                    else if (AINum == 3) {
                        if (emptyNum == 1) // Four
                            aiScore[row][col] += 210;
                        else if (emptyNum == 2) // Live four
                            aiScore[row][col] += 510;
                    }
                    else if (AINum == 4) { // Five in a row
                        aiScore[row][col] += 1010;
                    }
                    else {
                        aiScore[row][col] += 1; // Default score
                    }



                }
            }
        }
    }

    // Find the maximum score for human and AI.
    int humanMaxScore = 0;
    int aiMaxScore = 0;
    bool humanForbidden = false;
    bool aiForbidden = false;

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (humanScore[row][col] > humanMaxScore) {
                humanMaxScore = humanScore[row][col];
                if (isForbidden(playerColor, row, col)) {
                    humanForbidden = true;
                }
            }
            if (aiScore[row][col] > aiMaxScore) {
                aiMaxScore = aiScore[row][col];
                if (isForbidden(aiColor, row, col)) {
                    aiForbidden = true;
                }
            }
        }
    }
    if (aiForbidden) {
        aiMaxScore = -10000; // һ���㹻С��ֵ��ȷ�����ᱻѡΪ����ƶ�
    }
}

int main() {
    init();
    ExMessage m;
    ChessColor currentPlayer = playerColor; // ��ʼ�����ѡ�����ɫ
    bool gameOver = false;
    if (gameMode == NORMAL) {
        mciSendString("open bgm.mp3", 0, 0, 0); // ������
        mciSendString("play bgm.mp3 repeat", 0, 0, 0); // �������֣�repeat �����ظ���������
    }
    if (gameMode == HERO) {
        mciSendString("open victory.mp3", 0, 0, 0); // ������
        mciSendString("play victory.mp3 repeat", 0, 0, 0); // �������֣�repeat �����ظ���������
    }
    while (!gameOver) {
        m = getmessage(EM_MOUSE);
        if (m.message == WM_LBUTTONDOWN) {
            // ����Ƿ����˼��ܰ�ť
            if (gameMode == HERO &&playerColor==black&&
                m.x > board_width - 80 && m.x < board_width &&
                m.y > board_height - 80 && m.y < board_height) {
                skill = true;
                outtextxy(200, board_height - 50, _T("����������������!!"));
                memset(aiScore, 0, sizeof(aiScore));
                mciSendString("open yuyin.mp3", 0, 0, 0); // ������
                mciSendString("play yuyin.mp3", 0, 0, 0); // ��������
                
            }
            if (gameMode == HERO && playerColor == white &&
                m.x > board_width - 80 && m.x < board_width &&
                m.y > board_height - 80 && m.y < board_height) {
                skill22 = true;
                outtextxy(200, board_height - 50, _T("�������ܲ���֮��!!"));
                
                mciSendString("open skillWhite.mp3", 0, 0, 0); // ������
                mciSendString("play skillWhite.mp3", 0, 0, 0); // ��������

            }
            
            if (checkClick(m)) {
                if (chessBox[clickRow][clickCol] == NONE) {
                    chessBox[clickRow][clickCol] = currentPlayer;
                    drawChess(clickRow, clickCol, currentPlayer);

                    // ��鵱ǰ����Ƿ��ʤ
                    if (checkWin(clickRow, clickCol, currentPlayer)) {
                        
                            if (currentPlayer == playerColor) {
                                IMAGE picture;
                                loadimage(&picture, "PlayerWin.jpg", 660, 660);
                                putimage(0, 0, &picture);
                                _getch();
                                gameOver = true;
                            }
                            else if(currentPlayer == playerColor && !skill22){
                                IMAGE picture;
                                loadimage(&picture, "AiWin.jpg", 660, 660);
                                putimage(0, 0, &picture);
                                _getch();
                                gameOver = true;
                            }
                            
                        
                        
                    }

                    // �������̵�����
                    if (!skill) {
                        calculateScore();
                    }
                    

                    // �������
                    currentPlayer = (currentPlayer == playerColor) ? aiColor : playerColor;

                    // AI ����
                    if (currentPlayer == aiColor && !gameOver) {
                        int maxScore = 0;
                        int maxRow = 0, maxCol = 0;
                        for (int row = 0; row < N; row++) {
                            for (int col = 0; col < N; col++) {
                                // �޸��߼���AI ����
                                if (humanScore[row][col] > maxScore && humanScore[row][col] > aiScore[row][col] && chessBox[row][col] == NONE) {
                                    maxScore = humanScore[row][col];
                                    maxRow = row;
                                    maxCol = col;
                                }
                                if (aiScore[row][col] > maxScore && chessBox[row][col] == NONE) {
                                    maxScore = aiScore[row][col];
                                    maxRow = row;
                                    maxCol = col;
                                }
                            }
                        }

                        if (maxScore > 0) {
                            chessBox[maxRow][maxCol] = currentPlayer;
                            drawChess(maxRow, maxCol, currentPlayer);
                            // ��� AI �Ƿ��ʤ
                            if (checkWin(maxRow, maxCol, currentPlayer)) {
                                if (currentPlayer == white && !skill22) {
                                    IMAGE picture;
                                    loadimage(&picture, "AiWin.jpg", 660, 660);
                                    putimage(0, 0, &picture);
                                    _getch();

                                    gameOver = true;
                                }
                                
                            }
                        }
                        // �������
                        currentPlayer = (currentPlayer == playerColor) ? aiColor : playerColor;
                    }
                }
            }
            
        }
    }

    closegraph();
    return 0;
}