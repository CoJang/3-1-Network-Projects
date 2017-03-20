#include "Main.h"

int num = 0;

//vector<int> v_queue;	// 여기에 값을 저장한다.
std::queue<int> v_queue;
recursive_mutex v_queue_mutex; // 큐 뮤텍스


void make_product()
{
	DWORD dwTime = 0; // 시간을 저장할 변수 [시작 시점]
	int i; 

	while (true) // 항상
	{
		if (GetTickCount() >= dwTime) // dwTime이 현재시간과 같거나 작을 경우
		{
			dwTime = GetTickCount() + 100; // 다음 if문까지 대기시간 [100 = 0.1sec]

			i = num;				
			lock_guard<recursive_mutex> v_queue_lock(v_queue_mutex); // 보호 Lock을 건다.
			v_queue.push(i);		// 생산품을 큐에 삽입
			num++;					// 다음 생산품 생산
		}
	}


}

void consume_product()
{
	DWORD dwTime = GetTickCount() + 1000; // 현재시간을 저장 [시작 시점] 시작을 1초 뒤부터 하기위해 1초를 더해준다
	int n = 0;

	while (true) // 항상
	{
		lock_guard<recursive_mutex> v_queue_lock(v_queue_mutex); // 보호 Lock을 건다.
		for (int i = 0; i < v_queue.size(); i++)     // 계속 n에 값을 더함과 동시에 제거한다.
		{
			n += v_queue.front();	// 더함
			v_queue.pop();			// 제거함
		}

		if (GetTickCount() >= dwTime) // dwTime이 현재시간과 같거나 작을 경우
		{
			dwTime = GetTickCount() + 1000; // 다음 if문까지 대기시간 [1000 = 1sec]
			printf_s("Consume Result : %d\n", n);	// 소비된 생산품 출력
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