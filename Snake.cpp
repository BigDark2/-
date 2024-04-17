#include<iostream>
#include<easyx.h>
#include<vector> //˳���
#include<ctime>
using namespace std;

//���徫����
class Sprite {

public:
	Sprite() :Sprite(0,0) {};
	Sprite(int x,int y):m_x(x),m_y(y),m_color(RED) {};

	//���ƾ���
	virtual void draw() {
	
		//���������ɫ
		setfillcolor(m_color);
		//���ƾ���
		fillrectangle(m_x, m_y, m_x+10, m_y+10);//���ƾ���
	}
	//�ƶ�
	void bodyBy(int dx, int dy) {
		m_x += dx;
		m_y += dy;
	}
	//��ײ���
	bool collision(const Sprite& other) {
		return m_x == other.m_x && m_y == other.m_y;
	}
protected:
	int m_x;
	int m_y;
	COLORREF m_color; //��ɫ
};

//����
class Snake : public Sprite {

public:
	Snake() :Snake(0, 0) {}
	Snake(int x, int y) :Sprite(x, y),dir(VK_RIGHT){
	//��ʼ��������
		nodes.push_back(Sprite(20, 0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	}
	void draw() override {
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i].draw();
	}
	}
	//�ߵ������ƶ�
	void bodyMove() {
		//���������ͷ�ƶ�
		for (size_t i = nodes.size()-1; i >0; i--)
		{
			nodes[i] = nodes[i - 1];
		}
		//�ƶ���ͷ
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
	//������һ��
	void incrment() {
		nodes.push_back(Sprite());
	}
private:
	//�ߵĳ�������
	std::vector<Sprite> nodes;//�����нڵ�
public:
	int dir; //�ߵķ���
};

//ʳ��
class Food :public Sprite {
public:
	Food(): Sprite(0, 0) {
		changPos();
	}
	void draw()override {
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}
	//�ı�ʳ�������
	void changPos() {
		//�����������
		//���m_x = rand()%640;�����һ���ӣ����磬�˺����������Ϊ0 1 2 3 4 5 6 .... 639
		//����ײ������0 10 20 30 40 50 ... 640 ��Ϊ��Ҫ������ֵ��ȣ��ʴ��ڿ�
		//��	m_x = rand()%64*10; ��֤ʳ��������10��������
		m_x = rand() % 64 * 10;
		m_y = rand() % 48 * 10;
	}
};
//��Ϸ����
class GameScene {
public:
	GameScene() {
	
	};
	void run() {
		BeginBatchDraw();//˫�����ͼ
		cleardevice();
		snake.draw();
		food.draw();
		EndBatchDraw();

		//���ƶ����ı�����
		snake.bodyMove();

		snakeEatFood();
		//��ȡ��Ϣ
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EX_KEY)) {
			onMsg(msg);
		}
	}
	//��Ӧ��Ϣ�������Ϣ ������Ϣ
	void onMsg(const ExMessage& msg) {
	//����а�����Ϣ����û�а������£�
	//�ı���ķ��� ��ȡ���̰��� _getch()
		if (msg.message == WM_KEYDOWN) {
		//�жϾ�����ǰ��µ��Ǹ����� virtual key coe �������
			
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
	//�ж����ܷ�Ե�ʳ��
	void snakeEatFood() {
		if (snake.collision(food)) {//����ߺ�ʳ�﷢����ײ
		//�ߵĽ�������
			snake.incrment();
		//ʳ�����²����µ�λ��
			food.changPos();
		}
	}
private:
	Snake snake;
	Food food;
};

int main() {

	initgraph(640,480,EX_SHOWCONSOLE);//����easyx��������
	//�������������
	srand(time(nullptr));
	
	GameScene scene;
	while (true) {
		scene.run();
		Sleep(100);
	}
	


	getchar();
	return 0;
}