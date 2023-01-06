
#include<iostream>
#include<graphics.h>
#include<easyx.h>
#include<vector>
#include<ctime>

//精灵类
class Sprite
{
public:
	Sprite() :Sprite(0, 0) {};
	Sprite(int x, int y) :m_x(x), m_y(y), m_color(RED) {};
	//绘制
	virtual void draw()
	{
		setfillcolor(m_color);//设置填充颜色
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);//绘制矩形
	}
	void moveBy(int dx, int dy)
	{
		m_x += dx;
		m_y += dy;
	}

	//碰撞检测
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

//蛇类
class Snake:public Sprite
{
public:
	Snake():Snake(0,0){}
	Snake(int x, int y) :Sprite(x, y) ,dir(VK_RIGHT)
	{
		//初始化三节蛇
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

	//蛇的身体移动
	void BodyMove()
	{
		//身体跟着蛇头移动
		for (size_t i = nodes.size() - 1; i > 0; i--) {
			nodes[i] = nodes[i - 1];
		}
		//蛇头移动
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

	//蛇增加一节
	void incrment()
	{
		nodes.push_back(Sprite());
	}
private:
	std::vector<Sprite>nodes;//蛇的所有节点
public:
	int dir;//蛇的方向
};

//食物
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

	//改变食物坐标
	void changePos()
	{
		//随机生成坐标
		m_x = rand() % 64 * 10;
		m_y = rand() % 48 * 10;
	}

private:

};

//游戏场景
class GameScene
{
public:
	GameScene() 
	{
	
	};
	void run()
	{
		BeginBatchDraw();//双缓冲绘图
		cleardevice();
		snake.draw();
		food.draw();
		EndBatchDraw();

		//移动蛇，改变蛇的坐标
		snake.BodyMove();
		
		snakeEatFood();
		//获取消息
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EM_KEY)) {
			onMsg(msg);
		}
	}

	//响应消息：鼠标消息 键盘消息
	void onMsg(const ExMessage& msg)
	{
		//如果有键盘消息（有没有按键按下）
		if (msg.message == WM_KEYDOWN) {
			//改变蛇的移动方向 获取键盘按键
			//判断具体的是哪个按键按下 virtual key code 虚拟键码
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

	//判断蛇能否吃到食物
	void snakeEatFood()
	{
		if (snake.collision(food)) {//如果蛇和食物产生了碰撞
			//蛇的节数增加
			snake.incrment();
			//食物重新产生在别的位置
			food.changePos();
		}
	}
private:
	Snake snake;
	Food food;
};

int main()
{
	initgraph(640, 480, EX_SHOWCONSOLE);//创建窗口
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