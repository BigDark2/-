#include<iostream>
#include<easyx.h>
#include<vector> //顺序表
#include<ctime>
using namespace std;

//定义精灵类
class Sprite {

public:
	Sprite() :Sprite(0,0) {};
	Sprite(int x,int y):m_x(x),m_y(y),m_color(RED) {};

	//绘制精灵
	virtual void draw() {
	
		//设置填充颜色
		setfillcolor(m_color);
		//绘制矩形
		fillrectangle(m_x, m_y, m_x+10, m_y+10);//绘制矩形
	}
	//移动
	void bodyBy(int dx, int dy) {
		m_x += dx;
		m_y += dy;
	}
	//碰撞检测
	bool collision(const Sprite& other) {
		return m_x == other.m_x && m_y == other.m_y;
	}
protected:
	int m_x;
	int m_y;
	COLORREF m_color; //颜色
};

//蛇类
class Snake : public Sprite {

public:
	Snake() :Snake(0, 0) {}
	Snake(int x, int y) :Sprite(x, y),dir(VK_RIGHT){
	//初始化三节蛇
		nodes.push_back(Sprite(20, 0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	}
	void draw() override {
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i].draw();
	}
	}
	//蛇的身体移动
	void bodyMove() {
		//身体跟着蛇头移动
		for (size_t i = nodes.size()-1; i >0; i--)
		{
			nodes[i] = nodes[i - 1];
		}
		//移动蛇头
		switch (dir)
		{
		case VK_UP:
			nodes[0].bodyBy(0, -10);
			break;
		case VK_DOWN:
			nodes[0].bodyBy(0, 10);
			break;
		case VK_LEFT:
			nodes[0].bodyBy(-10, 0);
			break;
		case VK_RIGHT:
			nodes[0].bodyBy(10, 0);
			break;
		}
	}
	bool collision(const Sprite& other) {
		return nodes[0].collision(other);
	};
	//蛇增加一节
	void incrment() {
		nodes.push_back(Sprite());
	}
private:
	//蛇的长度设置
	std::vector<Sprite> nodes;//蛇所有节点
public:
	int dir; //蛇的方向
};

//食物
class Food :public Sprite {
public:
	Food(): Sprite(0, 0) {
		changPos();
	}
	void draw()override {
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}
	//改变食物的坐标
	void changPos() {
		//随机生成坐标
		//如果m_x = rand()%640;则存在一个坑，例如，此函数结果可能为0 1 2 3 4 5 6 .... 639
		//但碰撞所需是0 10 20 30 40 50 ... 640 因为需要绝对数值相等，故存在坑
		//若	m_x = rand()%64*10; 则保证食物坐标是10的整数倍
		m_x = rand() % 64 * 10;
		m_y = rand() % 48 * 10;
	}
};
//游戏场景
class GameScene {
public:
	GameScene() {
	
	};
	void run() {
		BeginBatchDraw();//双缓冲绘图
		cleardevice();
		snake.draw();
		food.draw();
		EndBatchDraw();

		//蛇移动，改变坐标
		snake.bodyMove();

		snakeEatFood();
		//获取信息
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EX_KEY)) {
			onMsg(msg);
		}
	}
	//响应信息：鼠标信息 键盘信息
	void onMsg(const ExMessage& msg) {
	//如果有按键消息（有没有按键按下）
	//改变社的方向 获取键盘按键 _getch()
		if (msg.message == WM_KEYDOWN) {
		//判断具体的是按下的那个按键 virtual key coe 虚拟键码
			
			switch (msg.vkcode)
			{
			case VK_UP:
				if(snake.dir!=VK_DOWN)
				snake.dir = msg.vkcode;
				break;
			case VK_DOWN:
				if (snake.dir != VK_UP)
					snake.dir = msg.vkcode;
				break;
			case VK_LEFT:
				if (snake.dir != VK_RIGHT)
					snake.dir = msg.vkcode;
				break;
			case VK_RIGHT:
				if (snake.dir != VK_LEFT)
					snake.dir = msg.vkcode;
				break;
			}
		}
	
	}
	//判断蛇能否吃到食物
	void snakeEatFood() {
		if (snake.collision(food)) {//如果蛇和食物发生碰撞
		//蛇的节数增加
			snake.incrment();
		//食物重新产生新的位置
			food.changPos();
		}
	}
private:
	Snake snake;
	Food food;
};

int main() {

	initgraph(640,480,EX_SHOWCONSOLE);//依靠easyx弹出窗口
	//设置随机数种子
	srand(time(nullptr));
	
	GameScene scene;
	while (true) {
		scene.run();
		Sleep(100);
	}
	


	getchar();
	return 0;
}