#include <graphics.h>
#include <Iostream>
#include <conio.h>        //������ͷ�ļ�
#include <Windows.h>
#pragma comment (lib,"Winmm.lib")   //����������
using namespace std;

enum Direction { Up, Down, Left, Right, Fire };

//̹�˶���
struct tank_s {
	int x, y, HP, voice;
	Direction direction;
};

//�ӵ��Ķ���
struct Bullet {
	int bt_x, bt_y, bt_exist;
	Direction bt_birection;
};

//��һ�ŵ�ͼ
int map_1[18][26] = {
	{ 0,0,1,1,0,0,1,1,0,0,0,1,4,0,1,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,1,0,0,1,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1 },
	{ 2,2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2,2 },
	{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,0,0,0,0,0 }
};

void set_prop_map(int x, int y, int value);
void tank_walk(tank_s* tank, Direction d, IMAGE* tank_img, int step);
void butllet_action(Bullet* bullet);
void Play();
void tank_walk_mod(tank_s* tank, IMAGE* tank_img, Direction d, Bullet* bullet);
Direction enemy_direction(tank_s tank, int x, int y);

//��ʼ�˵�
void Begin(void) {
	IMAGE logo_img, illustrate_img;
	loadimage(&logo_img, _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/logo.bmp", 433, 147));            //logoͼƬ
	loadimage(&illustrate_img, _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/2.png", 252, 135));    //˵��ͼƬ
	putimage(110, 20, &logo_img);
	//������ť
	setlinecolor(WHITE);
	setfillcolor(BLACK);
	fillrectangle(230, 200, 310, 240);
	settextstyle(25, 0, _T("����"));
	outtextxy(240 + 5, 210, _T("˵��"));
	fillrectangle(350, 200, 430, 240);
	settextstyle(25, 0, _T("����"));
	outtextxy(240 + 125, 210, _T("��ʼ"));
	//���λ�ò�׽
	MOUSEMSG mouse;
	while (true) {
		mouse = GetMouseMsg();
		switch (mouse.uMsg) {      //mouse.uMsg��������굱ǰ�Ĳ�����ʽ
		case WM_MOUSEMOVE:   //˵��
			if (mouse.x > 230 && mouse.x < 310 && mouse.y>200 && mouse.y < 240) {
				putimage(150, 250, &illustrate_img);
			}
			else {
				solidrectangle(150, 250, 450, 550);         //��������
			}
			break;
		case WM_LBUTTONDOWN:  //��ʼ
			if (mouse.x > 350 && mouse.x < 430 && mouse.y>200 && mouse.y < 240) {
				cleardevice();
				return;
			}
		}
	}
}

//���߷���
void set_prop_map(int x, int y, int value) {
	map_1[y][x] = value;
	map_1[y][x + 1] = value;
	map_1[y + 1][x] = value;
	map_1[y + 1][x + 1] = value;
}

//����
void Scene() {
	//����Ԫ��
	IMAGE home, wall_1, wall_2;
	loadimage(&home, _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/home.png", 50, 50));
	loadimage(&wall_2, _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/wall1.png", 25, 25));
	loadimage(&wall_1, _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/wall2.png", 25, 25));

	int i, j;                      //i ��y ��j�� x
	for (i = 0; i != 18; i++) {
		for (j = 0; j != 26; j++) {
			switch (map_1[i][j]) {
			case 1:
				putimage(j * 25, i * 25, &wall_2); break;
			case 2:
				putimage(j * 25, i * 25, &wall_1); break;
			case 3:
				putimage(j * 25, i * 25, &home);
				set_prop_map(j, i, 4);
				break;
			}
		}
	}
}

//̹���ƶ�
void tank_walk(tank_s* tank, Direction d, IMAGE* tank_img, int step) {
	setfillcolor(BLACK);
	solidrectangle(tank->x * 25, tank->y * 25, tank->x * 25 + 50, tank->y * 25 + 50);
	if (step) {                     //�ж��Ƿ�Ҫ�ƶ�
		set_prop_map(tank->x, tank->y, 0);
		switch (d) {
		case Up:
			tank->y -= 1;
			break;
		case Down:
			tank->y += 1;
			break;
		case Left:
			tank->x -= 1;
			break;
		case Right:
			tank->x += 1;
			break;
		}
		set_prop_map(tank->x, tank->y, 200);
	}
	tank->direction = d;
	putimage(tank->x * 25, tank->y * 25, tank_img);
}

void tank_walk_mod(tank_s* tank, IMAGE* tank_img, Direction d, Bullet* bullet) {
	switch (d) {
	case Up:
		if (tank->direction == Up && map_1[tank->y - 1][tank->x + 1] == 0 && (tank->y - 1) >= 0 && map_1[tank->y - 1][tank->x] == 0) {
			tank_walk(tank, Up, tank_img, 1);               //  1Ϊǰ��һ��
		}
		else if (tank->direction != Up) {
			tank->direction = Up;
			tank_walk(tank, Up, tank_img, 0);           //0Ϊֻת���ƶ�
		}
		break;
	case Down:
		if (tank->direction == Down && map_1[tank->y + 2][tank->x + 1] == 0 && (tank->y + 2) <= 17 && map_1[tank->y + 2][tank->x] == 0) {
			tank_walk(tank, Down, tank_img, 1);
		}
		else if (tank->direction != Down) {
			tank->direction = Down;
			tank_walk(tank, Down, tank_img, 0);
		}
		break;
	case Left:
		if (tank->direction == Left && (tank->x - 1) >= 0 && map_1[tank->y][tank->x - 1] == 0 && map_1[tank->y + 1][tank->x - 1] >= 0 && map_1[tank->y + 1][tank->x - 1] == 0) {
			tank_walk(tank, Left, tank_img, 1);
		}
		else if (tank->direction != Left) {
			tank->direction = Left;
			tank_walk(tank, Left, tank_img, 0);
		}
		break;
	case Right:
		if (tank->direction == Right && (tank->x + 2) <= 25 && map_1[tank->y][tank->x + 2] == 0 && map_1[tank->y + 1][tank->x + 2] >= 0 && map_1[tank->y + 1][tank->x + 2] == 0) {
			tank_walk(tank, Right, tank_img, 1);
		}
		else if (tank->direction != Right) {
			tank->direction = Right;
			tank_walk(tank, Right, tank_img, 0);
		}
		break;
	case Fire: {        //����
		if (bullet->bt_exist == 0)
			PlaySound(_T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/paoji.wav"), NULL, SND_FILENAME | SND_ASYNC);
		if (bullet->bt_exist == 0) {
			switch (tank->direction) {
			case Up: {
				bullet->bt_x = tank->x * 25 + 24;
				bullet->bt_y = tank->y * 25 - 3;
				break;
			}
			case Down: {
				bullet->bt_x = tank->x * 25 + 24;
				bullet->bt_y = tank->y * 25 + 49;
				break;
			}
			case Left: {
				bullet->bt_x = tank->x * 25 + 3;
				bullet->bt_y = tank->y * 25 + 24;
				break;
			}
			case Right: {
				bullet->bt_x = tank->x * 25 + 49;
				bullet->bt_y = tank->y * 25 + 24;
				break;
			}
			}
			bullet->bt_birection = tank->direction;
			bullet->bt_exist = 1;            //�ѷ����ӵ�������
		}
	}
			 break;
	}
}

//�ӵ�����
void butllet_action(Bullet* bullet) {
	int x, y, x1, y1;              //�ӵ��ڶ�ά������±�
	x = bullet->bt_x / 25;
	y = bullet->bt_y / 25;
	//������һ�λ��Ƶ��ӵ�
	if (bullet->bt_exist == 1) {
		setfillcolor(BLACK);
		solidrectangle(bullet->bt_x, bullet->bt_y, bullet->bt_x + 3, bullet->bt_y + 3);
	}
	//���ݷ�������ӵ��ڡ�Ϸ̨���ϵ�����
	switch (bullet->bt_birection) {
	case Up:
		bullet->bt_y -= 2;
		x1 = x + 1;
		y1 = y;
		break;
	case Down:
		bullet->bt_y += 2;
		x1 = x + 1;
		y1 = y;
		break;
	case Left:
		bullet->bt_x -= 2;
		x1 = x;
		y1 = y + 1;
		break;
	case Right:
		bullet->bt_x += 2;
		x1 = x;
		y1 = y + 1;
		break;
	default:
		return;
	}
	//�ӵ��ɳ���ͼ����
	if (bullet->bt_x < 0 || bullet->bt_x > 650 || bullet->bt_y < 0 || bullet->bt_y  > 18 * 25) {
		bullet->bt_exist = 0;
		return;
	}
	//��ײ���
	if (map_1[y][x] == 4 || map_1[y1][x1] == 4) {    //����home
		bullet->bt_exist = 0;
		return;
	}
	if (map_1[y][x] == 1) {
		map_1[y][x] = 0;
		bullet->bt_exist = 0;
		setfillcolor(BLACK);
		solidrectangle(x * 25, y * 25, x * 25 + 25, y * 25 + 25);
	}
	if (map_1[y1][x1] == 1) {
		map_1[y1][x1] = 0;
		bullet->bt_exist = 0;
		setfillcolor(BLACK);
		solidrectangle(x1 * 25, y1 * 25, x1 * 25 + 25, y1 * 25 + 25);
	}
	if (map_1[y][x] == 2 && map_1[y1][x1] == 2) {
		bullet->bt_exist = 0;
	}
	//���»����ӵ�
	if (bullet->bt_exist == 1) {
		setfillcolor(WHITE);
		solidrectangle(bullet->bt_x, bullet->bt_y, bullet->bt_x + 3, bullet->bt_y + 3);
	}
}

//���Ƶз���Ŀ��С��Χ���ǰ��
Direction enemy_direction(tank_s *tank,int x,int y) {
	int rand_direction;
	rand_direction = rand() % 100;
	if (tank->x > x) {
		if (tank->y < y) {   //Ŀ�������·�
			if (rand_direction > 50) return Left;
			else return Down;
		}
		else {              //Ŀ�������Ϸ�
			if (rand_direction > 50) return Left;
			else return Up;
		}
	}
	else {
		if (tank->y < y) {   //Ŀ�������·�
			if (rand_direction > 50) return Right;
			else return Down;
		}
		else {              //Ŀ�������Ϸ�
			if (rand_direction > 50) return Right;
			else return Up;
		}
	}
}

#define enemy_tank_num 3
//���淽ʽ
void Play() {
	tank_s mytank, enemy_tank[enemy_tank_num];            //̹��
	Bullet mybullet, enemy_tank_bullet[enemy_tank_num];           //�ڵ�
	mybullet.bt_exist = 0;
	for (int i = 0; i < enemy_tank_num; i++) {
		enemy_tank_bullet[i].bt_exist = 0;             //��ʼ���ڵ�
	}                     

	IMAGE mytank_img[5], enemy_tank_img[5];
	//�����ҷ�̹��4���������ͼ
	loadimage(&mytank_img[Up], _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/tankup.png"), 50, 50);
	loadimage(&mytank_img[Down], _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/tankdown.png"), 50, 50);
	loadimage(&mytank_img[Left], _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/tankleft.png"), 50, 50);
	loadimage(&mytank_img[Right], _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/tankright.png"), 50, 50);
	cout << &mytank_img[Left];
	//���صз�̹����ͼ
	loadimage(&enemy_tank_img[Up], _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/tankEnemyUp.png"), 50, 50);
	loadimage(&enemy_tank_img[Down], _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/tankEnemyDown.png"), 50, 50);
	loadimage(&enemy_tank_img[Left], _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/tankEnemyLeft.png"), 50, 50);
	loadimage(&enemy_tank_img[Right], _T("D:/ComputerPrograms/program/̹�˴�ս/̹�˴�ս/res/tankEnemyRight.png"), 50, 50);
	//�ҷ���ʼ��
	mytank.direction = Up; mytank.x = 9;  mytank.y = 16; mytank.HP = 1;
	set_prop_map(mytank.x, mytank.y, 200);
	putimage(mytank.x * 25, mytank.y * 25, &mytank_img[mytank.direction]);
	//ǰ3���з�̹�˳�ʼ��
	for (int i = 0; i < enemy_tank_num; i++) {
		enemy_tank[i].direction = Down;
		enemy_tank[i].x = 12 * i;
		enemy_tank[i].y = 0;
		enemy_tank[i].HP = 1;
		set_prop_map(enemy_tank[i].x, enemy_tank[i].y, 100 + i);
		putimage(enemy_tank[i].x * 25, enemy_tank[i].y * 25, &enemy_tank_img[enemy_tank[i].direction]);
	}
	char key;
	int tmes = 0, enemy_total = 3;//Ŀǰ���ĵз�̹��
	while (true) {
		if (!(tmes % 200)) {
			for (int i = 0; i < enemy_total; i++) {
				if (!(i % 2)) {        //�����ҷ��ϳ�
					enemy_tank[i].direction = enemy_direction(&enemy_tank[i], 13, 17);
				}
				else {                 //�����ҷ�
					enemy_tank[i].direction = enemy_direction(&enemy_tank[i], mytank.x, mytank.y);
				}
					tank_walk_mod(&enemy_tank[i], NULL, Fire, &enemy_tank_bullet[i]);  //�з�̹�˿���
			}
		}
		else if (!(tmes % 50)) {
			for (int i = 0; i < enemy_total; i++) {    //�ƶ����ĵз�̹��
				if (enemy_tank->HP) {
					tank_walk_mod(&enemy_tank[i], &enemy_tank_img[enemy_tank[i].direction], enemy_tank[i].direction, NULL);
				}
			}
		}

		if (_kbhit()) {     //����������
			key = _getch();
			switch (key) {
			case 'w':
				tank_walk_mod(&mytank, &mytank_img[Up], Up, &mybullet); break;
			case 's':
				tank_walk_mod(&mytank, &mytank_img[Down], Down, &mybullet); break;
			case 'a':
				tank_walk_mod(&mytank, &mytank_img[Left], Left, &mybullet); break;
			case 'd':
				tank_walk_mod(&mytank, &mytank_img[Right], Right, &mybullet); break;
			case 'j':        //����
				tank_walk_mod(&mytank, NULL, Fire, &mybullet); break;
			case 'k':
				system("pause");
				break;
			}
		}
		
		if (mybullet.bt_exist) {    //ˢ���ҷ��ӵ�
			butllet_action(&mybullet);
		}
		for (int i = 0; i < enemy_total; i++) {    //ˢ�µз��ӵ�
			if(enemy_tank_bullet[i].bt_exist)	butllet_action(&enemy_tank_bullet[i]);
		}
		Sleep(10);                      //��ͣ10����
		tmes++;
	}
}

int main() {
	initgraph(650, 18 * 25);       //��Ϸ����
	//Begin();
	Scene();
	Play();
	system("pause");
}