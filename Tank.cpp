#include <graphics.h>
#include <Iostream>
#include <conio.h>        //鼠标键盘头文件
#include <Windows.h>
#pragma comment (lib,"Winmm.lib")   //导入声音库
using namespace std;

enum Direction { Up, Down, Left, Right, Fire };

//坦克定义
struct tank_s {
	int x, y, HP, voice;
	Direction direction;
};

//子弹的定义
struct Bullet {
	int bt_x, bt_y, bt_exist;
	Direction bt_birection;
};

//第一张地图
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

//开始菜单
void Begin(void) {
	IMAGE logo_img, illustrate_img;
	loadimage(&logo_img, _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/logo.bmp", 433, 147));            //logo图片
	loadimage(&illustrate_img, _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/2.png", 252, 135));    //说明图片
	putimage(110, 20, &logo_img);
	//导航按钮
	setlinecolor(WHITE);
	setfillcolor(BLACK);
	fillrectangle(230, 200, 310, 240);
	settextstyle(25, 0, _T("宋体"));
	outtextxy(240 + 5, 210, _T("说明"));
	fillrectangle(350, 200, 430, 240);
	settextstyle(25, 0, _T("宋体"));
	outtextxy(240 + 125, 210, _T("开始"));
	//鼠标位置捕捉
	MOUSEMSG mouse;
	while (true) {
		mouse = GetMouseMsg();
		switch (mouse.uMsg) {      //mouse.uMsg储存了鼠标当前的操作方式
		case WM_MOUSEMOVE:   //说明
			if (mouse.x > 230 && mouse.x < 310 && mouse.y>200 && mouse.y < 240) {
				putimage(150, 250, &illustrate_img);
			}
			else {
				solidrectangle(150, 250, 450, 550);         //绘制遮罩
			}
			break;
		case WM_LBUTTONDOWN:  //开始
			if (mouse.x > 350 && mouse.x < 430 && mouse.y>200 && mouse.y < 240) {
				cleardevice();
				return;
			}
		}
	}
}

//道具放置
void set_prop_map(int x, int y, int value) {
	map_1[y][x] = value;
	map_1[y][x + 1] = value;
	map_1[y + 1][x] = value;
	map_1[y + 1][x + 1] = value;
}

//场景
void Scene() {
	//场景元素
	IMAGE home, wall_1, wall_2;
	loadimage(&home, _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/home.png", 50, 50));
	loadimage(&wall_2, _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/wall1.png", 25, 25));
	loadimage(&wall_1, _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/wall2.png", 25, 25));

	int i, j;                      //i 是y ，j是 x
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

//坦克移动
void tank_walk(tank_s* tank, Direction d, IMAGE* tank_img, int step) {
	setfillcolor(BLACK);
	solidrectangle(tank->x * 25, tank->y * 25, tank->x * 25 + 50, tank->y * 25 + 50);
	if (step) {                     //判断是否要移动
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
			tank_walk(tank, Up, tank_img, 1);               //  1为前进一格
		}
		else if (tank->direction != Up) {
			tank->direction = Up;
			tank_walk(tank, Up, tank_img, 0);           //0为只转向不移动
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
	case Fire: {        //开火
		if (bullet->bt_exist == 0)
			PlaySound(_T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/paoji.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
			bullet->bt_exist = 1;            //已发射子弹，存在
		}
	}
			 break;
	}
}

//子弹飞行
void butllet_action(Bullet* bullet) {
	int x, y, x1, y1;              //子弹在二维数组的下标
	x = bullet->bt_x / 25;
	y = bullet->bt_y / 25;
	//擦除上一次绘制的子弹
	if (bullet->bt_exist == 1) {
		setfillcolor(BLACK);
		solidrectangle(bullet->bt_x, bullet->bt_y, bullet->bt_x + 3, bullet->bt_y + 3);
	}
	//根据方向计算子弹在“戏台”上的坐标
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
	//子弹飞出地图外了
	if (bullet->bt_x < 0 || bullet->bt_x > 650 || bullet->bt_y < 0 || bullet->bt_y  > 18 * 25) {
		bullet->bt_exist = 0;
		return;
	}
	//碰撞检测
	if (map_1[y][x] == 4 || map_1[y1][x1] == 4) {    //击中home
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
	//重新绘制子弹
	if (bullet->bt_exist == 1) {
		setfillcolor(WHITE);
		solidrectangle(bullet->bt_x, bullet->bt_y, bullet->bt_x + 3, bullet->bt_y + 3);
	}
}

//控制敌方向目标小范围随机前进
Direction enemy_direction(tank_s *tank,int x,int y) {
	int rand_direction;
	rand_direction = rand() % 100;
	if (tank->x > x) {
		if (tank->y < y) {   //目标在左下方
			if (rand_direction > 50) return Left;
			else return Down;
		}
		else {              //目标在左上方
			if (rand_direction > 50) return Left;
			else return Up;
		}
	}
	else {
		if (tank->y < y) {   //目标在右下方
			if (rand_direction > 50) return Right;
			else return Down;
		}
		else {              //目标在右上方
			if (rand_direction > 50) return Right;
			else return Up;
		}
	}
}

#define enemy_tank_num 3
//游玩方式
void Play() {
	tank_s mytank, enemy_tank[enemy_tank_num];            //坦克
	Bullet mybullet, enemy_tank_bullet[enemy_tank_num];           //炮弹
	mybullet.bt_exist = 0;
	for (int i = 0; i < enemy_tank_num; i++) {
		enemy_tank_bullet[i].bt_exist = 0;             //初始化炮弹
	}                     

	IMAGE mytank_img[5], enemy_tank_img[5];
	//加载我方坦克4个方向的贴图
	loadimage(&mytank_img[Up], _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/tankup.png"), 50, 50);
	loadimage(&mytank_img[Down], _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/tankdown.png"), 50, 50);
	loadimage(&mytank_img[Left], _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/tankleft.png"), 50, 50);
	loadimage(&mytank_img[Right], _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/tankright.png"), 50, 50);
	cout << &mytank_img[Left];
	//加载敌方坦克贴图
	loadimage(&enemy_tank_img[Up], _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/tankEnemyUp.png"), 50, 50);
	loadimage(&enemy_tank_img[Down], _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/tankEnemyDown.png"), 50, 50);
	loadimage(&enemy_tank_img[Left], _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/tankEnemyLeft.png"), 50, 50);
	loadimage(&enemy_tank_img[Right], _T("D:/ComputerPrograms/program/坦克大战/坦克大战/res/tankEnemyRight.png"), 50, 50);
	//我方初始化
	mytank.direction = Up; mytank.x = 9;  mytank.y = 16; mytank.HP = 1;
	set_prop_map(mytank.x, mytank.y, 200);
	putimage(mytank.x * 25, mytank.y * 25, &mytank_img[mytank.direction]);
	//前3个敌方坦克初始化
	for (int i = 0; i < enemy_tank_num; i++) {
		enemy_tank[i].direction = Down;
		enemy_tank[i].x = 12 * i;
		enemy_tank[i].y = 0;
		enemy_tank[i].HP = 1;
		set_prop_map(enemy_tank[i].x, enemy_tank[i].y, 100 + i);
		putimage(enemy_tank[i].x * 25, enemy_tank[i].y * 25, &enemy_tank_img[enemy_tank[i].direction]);
	}
	char key;
	int tmes = 0, enemy_total = 3;//目前存活的敌方坦克
	while (true) {
		if (!(tmes % 200)) {
			for (int i = 0; i < enemy_total; i++) {
				if (!(i % 2)) {        //攻击我方老巢
					enemy_tank[i].direction = enemy_direction(&enemy_tank[i], 13, 17);
				}
				else {                 //攻击我方
					enemy_tank[i].direction = enemy_direction(&enemy_tank[i], mytank.x, mytank.y);
				}
					tank_walk_mod(&enemy_tank[i], NULL, Fire, &enemy_tank_bullet[i]);  //敌方坦克开火
			}
		}
		else if (!(tmes % 50)) {
			for (int i = 0; i < enemy_total; i++) {    //移动存活的敌方坦克
				if (enemy_tank->HP) {
					tank_walk_mod(&enemy_tank[i], &enemy_tank_img[enemy_tank[i].direction], enemy_tank[i].direction, NULL);
				}
			}
		}

		if (_kbhit()) {     //检测键盘输入
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
			case 'j':        //开火
				tank_walk_mod(&mytank, NULL, Fire, &mybullet); break;
			case 'k':
				system("pause");
				break;
			}
		}
		
		if (mybullet.bt_exist) {    //刷新我方子弹
			butllet_action(&mybullet);
		}
		for (int i = 0; i < enemy_total; i++) {    //刷新敌方子弹
			if(enemy_tank_bullet[i].bt_exist)	butllet_action(&enemy_tank_bullet[i]);
		}
		Sleep(10);                      //暂停10毫秒
		tmes++;
	}
}

int main() {
	initgraph(650, 18 * 25);       //游戏窗口
	//Begin();
	Scene();
	Play();
	system("pause");
}