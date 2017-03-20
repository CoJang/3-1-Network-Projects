#include "Main.h"

int num = 0;

vector<int> v_queue;	// 여기에 값을 저장한다.
recursive_mutex Q_mutex; // 큐 뮤텍스


void make_product()
{
	DWORD dwTime = GetTickCount(); // 현재시간을 저장 [시작 시점]
	int n; 

	while (true) // 항상
	{
		if (GetTickCount() >= dwTime) // dwTime이 현재시간과 같거나 작을 경우
		{
			n = num;				
			lock_guard<recursive_mutex> Q_lock(Q_mutex); // 보호 Lock을 건다.
			v_queue.push_back(n);	// 생산품을 큐에 삽입
			num++;					// 다음 생산품 생산
			printf_s("%d\n", n);	// 삽입된 생산품 출력

			dwTime = GetTickCount() + 100; // 다음 if문까지 대기시간 [100 = 0.1sec]
		}
	}


}

void consume_product()
{
	DWORD dwTime = GetTickCount(); // 현재시간을 저장 [시작 시점]
	int n = 0;
	while (true) // 항상
	{
		lock_guard<recursive_mutex> Q_lock(Q_mutex); // 보호 Lock을 건다.
		for (int i = 0; i < v_queue.size(); i++)     // 계속 n에 값을 더함과 동시에 제거한다.
		{
			n += v_queue[i];		// 더함
			v_queue.pop_back();		// 제거
		}

		if (GetTickCount() >= dwTime) // dwTime이 현재시간과 같거나 작을 경우
		{
			printf_s("thread 2 : %d\n", n);	// 소비된 생산품 출력
			dwTime = GetTickCount() + 1000; // 다음 if문까지 대기시간 [1000 = 1sec]
		}
	}
}

int main()
{
	thread t1(make_product);	// 1번 쓰레드 일 시킴 [ 일 : make_product ]
	thread t2(consume_product); // 2번 쓰레드 일 시킴 [ 일 : consume_product ]

	t1.join();					// 1번 쓰레드 일 끝날 때 까지 대기.
	t2.join();					// 2번 쓰레드 일 끝날 때 까지 대기.

	return 0;
}