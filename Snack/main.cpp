
#include<iostream>
#include<graphics.h>
#include<easyx.h>
#include<vector>
#include<ctime>

//������
class Sprite
{
public:
	Sprite() :Sprite(0, 0) {};
	Sprite(int x, int y) :m_x(x), m_y(y), m_color(RED) {};
	//����
	virtual void draw()
	{
		setfillcolor(m_color);//���������ɫ
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);//���ƾ���
	}
	void moveBy(int dx, int dy)
	{
		m_x += dx;
		m_y += dy;
	}

	//��ײ���
	bool collision(const Sprite& other)
	{
		//std::cout << "(x1:" << m_x << "y1:" << m_y << ")" << "(x2:" << other.m_x << "y2:" << other.m_y << ")" << std::endl;
		return m_x == other.m_x && m_y == other.m_y;
	}
protected:
	int m_x;
	int m_y;
	COLORREF m_color;
};

//����
class Snake:public Sprite
{
public:
	Snake():Snake(0,0){}
	Snake(int x, int y) :Sprite(x, y) ,dir(VK_RIGHT)
	{
		//��ʼ��������
		nodes.push_back(Sprite(20, 0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	};
	void draw()override
	{
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i].draw();
		}
	}

	//�ߵ������ƶ�
	void BodyMove()
	{
		//���������ͷ�ƶ�
		for (size_t i = nodes.size() - 1; i > 0; i--) {
			nodes[i] = nodes[i - 1];
		}
		//��ͷ�ƶ�
		switch (dir)
		{
		case VK_UP:
			nodes[0].moveBy(0, -10);
			break;
		case VK_DOWN:
			nodes[0].moveBy(0, 10);
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0);
			break;
		case VK_RIGHT:
			nodes[0].moveBy(10, 0);
			break;
		}
	}

	bool collision(const Sprite& other)
	{
		return nodes[0].collision(other);
	}

	//������һ��
	void incrment()
	{
		nodes.push_back(Sprite());
	}
private:
	std::vector<Sprite>nodes;//�ߵ����нڵ�
public:
	int dir;//�ߵķ���
};

//ʳ��
class Food:public Sprite
{
public:
	Food():Sprite(0,0)
	{
		changePos();

	}

	void draw()override
	{
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}

	//�ı�ʳ������
	void changePos()
	{
		//�����������
		m_x = rand() % 64 * 10;
		m_y = rand() % 48 * 10;
	}

private:

};

//��Ϸ����
class GameScene
{
public:
	GameScene() 
	{
	
	};
	void run()
	{
		BeginBatchDraw();//˫�����ͼ
		cleardevice();
		snake.draw();
		food.draw();
		EndBatchDraw();

		//�ƶ��ߣ��ı��ߵ�����
		snake.BodyMove();
		
		snakeEatFood();
		//��ȡ��Ϣ
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EM_KEY)) {
			onMsg(msg);
		}
	}

	//��Ӧ��Ϣ�������Ϣ ������Ϣ
	void onMsg(const ExMessage& msg)
	{
		//����м�����Ϣ����û�а������£�
		if (msg.message == WM_KEYDOWN) {
			//�ı��ߵ��ƶ����� ��ȡ���̰���
			//�жϾ�������ĸ��������� virtual key code �������
			/*if (msg.vkcode == VK_UP || msg.vkcode == VK_DOWN || msg.vkcode == VK_LEFT || msg.vkcode == VK_RIGHT) {
				snake.dir = msg.vkcode;
			}*/
			//std::cout << std::dec << (int)msg.vkcode << std::endl;
			switch (msg.vkcode)
			{
			case VK_UP:
				if (snake.dir != VK_DOWN) {
					snake.dir = msg.vkcode;
				}
				break;
			case VK_DOWN:
				if (snake.dir != VK_UP) {
					snake.dir = msg.vkcode;
				}
				break;
			case VK_LEFT:
				if (snake.dir != VK_RIGHT) {
					snake.dir = msg.vkcode;
				}
				break;
			case VK_RIGHT:
				if (snake.dir != VK_LEFT) {
					snake.dir = msg.vkcode;
				}
				break;
			}
		}
	}

	//�ж����ܷ�Ե�ʳ��
	void snakeEatFood()
	{
		if (snake.collision(food)) {//����ߺ�ʳ���������ײ
			//�ߵĽ�������
			snake.incrment();
			//ʳ�����²����ڱ��λ��
			food.changePos();
		}
	}
private:
	Snake snake;
	Food food;
};

int main()
{
	initgraph(640, 480, EX_SHOWCONSOLE);//��������
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