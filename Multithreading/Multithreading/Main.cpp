#include "Main.h"

int num = 0;

vector<int> v_queue;	// ���⿡ ���� �����Ѵ�.
recursive_mutex Q_mutex; // ť ���ؽ�


void make_product()
{
	DWORD dwTime = GetTickCount(); // ����ð��� ���� [���� ����]
	int n; 

	while (true) // �׻�
	{
		if (GetTickCount() >= dwTime) // dwTime�� ����ð��� ���ų� ���� ���
		{
			n = num;				
			lock_guard<recursive_mutex> Q_lock(Q_mutex); // ��ȣ Lock�� �Ǵ�.
			v_queue.push_back(n);	// ����ǰ�� ť�� ����
			num++;					// ���� ����ǰ ����
			printf_s("%d\n", n);	// ���Ե� ����ǰ ���

			dwTime = GetTickCount() + 100; // ���� if������ ���ð� [100 = 0.1sec]
		}
	}


}

void consume_product()
{
	DWORD dwTime = GetTickCount(); // ����ð��� ���� [���� ����]
	int n = 0;
	while (true) // �׻�
	{
		lock_guard<recursive_mutex> Q_lock(Q_mutex); // ��ȣ Lock�� �Ǵ�.
		for (int i = 0; i < v_queue.size(); i++)     // ��� n�� ���� ���԰� ���ÿ� �����Ѵ�.
		{
			n += v_queue[i];		// ����
			v_queue.pop_back();		// ����
		}

		if (GetTickCount() >= dwTime) // dwTime�� ����ð��� ���ų� ���� ���
		{
			printf_s("thread 2 : %d\n", n);	// �Һ�� ����ǰ ���
			dwTime = GetTickCount() + 1000; // ���� if������ ���ð� [1000 = 1sec]
		}
	}
}

int main()
{
	thread t1(make_product);	// 1�� ������ �� ��Ŵ [ �� : make_product ]
	thread t2(consume_product); // 2�� ������ �� ��Ŵ [ �� : consume_product ]

	t1.join();					// 1�� ������ �� ���� �� ���� ���.
	t2.join();					// 2�� ������ �� ���� �� ���� ���.

	return 0;
}