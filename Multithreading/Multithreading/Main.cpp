#include "Main.h"

int num = 0;

//vector<int> v_queue;	// ���⿡ ���� �����Ѵ�.
std::queue<int> v_queue;
recursive_mutex v_queue_mutex; // ť ���ؽ�


void make_product()
{
	DWORD dwTime = 0; // �ð��� ������ ���� [���� ����]
	int i; 

	while (true) // �׻�
	{
		if (GetTickCount() >= dwTime) // dwTime�� ����ð��� ���ų� ���� ���
		{
			dwTime = GetTickCount() + 100; // ���� if������ ���ð� [100 = 0.1sec]

			i = num;				
			lock_guard<recursive_mutex> v_queue_lock(v_queue_mutex); // ��ȣ Lock�� �Ǵ�.
			v_queue.push(i);		// ����ǰ�� ť�� ����
			num++;					// ���� ����ǰ ����
		}
	}


}

void consume_product()
{
	DWORD dwTime = GetTickCount() + 1000; // ����ð��� ���� [���� ����] ������ 1�� �ں��� �ϱ����� 1�ʸ� �����ش�
	int n = 0;

	while (true) // �׻�
	{
		lock_guard<recursive_mutex> v_queue_lock(v_queue_mutex); // ��ȣ Lock�� �Ǵ�.
		for (int i = 0; i < v_queue.size(); i++)     // ��� n�� ���� ���԰� ���ÿ� �����Ѵ�.
		{
			n += v_queue.front();	// ����
			v_queue.pop();			// ������
		}

		if (GetTickCount() >= dwTime) // dwTime�� ����ð��� ���ų� ���� ���
		{
			dwTime = GetTickCount() + 1000; // ���� if������ ���ð� [1000 = 1sec]
			printf_s("Consume Result : %d\n", n);	// �Һ�� ����ǰ ���
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