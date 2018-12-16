#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define user1_to_jud 4545
#define user2_to_jud 6666
#define jud_to_user 5454

typedef struct {   // 메시지 큐를 통해 주고받는 구조체
	long data_type;
	int pid;
	int value;
} data_t;

int num;
int msqid, msqid2, msqid3;  // msqid = user1->judg  msqid2 = judg->user1,2  msqid3 = user2->judg

int main(){                        // 변수 선언
	int i, j, r, ri;
	data_t data, data2, data3;
	data_t rcv_data[2];          // user1 값과 user2 값 비교
while(1){
	printf("┌──────────────────────────┐\n");
	printf("│♤♠재미있는 심리포커 게임!!│\n");
	printf("└──────────────────────────┘\n");


	if( -1 == (msqid = msgget ((key_t)user1_to_jud, IPC_CREAT | 0666)))  // 메시지 큐 생성	
	{
		perror("msgget() 실패");
		exit(1);
	}
				
	if( -1 == (msqid2 = msgget ((key_t)jud_to_user, IPC_CREAT | 0666)))
	{
		perror("msgger() 실패");
		exit(1);
	}
	if( -1 == (msqid3 = msgget ((key_t)user2_to_jud, IPC_CREAT | 0666)))
	{
		perror("msgget() 실패");
		exit(1);
	}

	
	for(i=0;i<10;i++){      // 10라운드까지라서 10번 반복
		num = 1;
		while(num){
			ri= msgrcv(msqid, &data, sizeof(data_t)-sizeof(long),0, 0); //  user1이 보낸 값을 읽어들임
			if(ri == -1)
			{
				perror("msgrcv() 실패");
				exit(1);
			}
			if(ri != -1){
				rcv_data[0].pid = data.pid;     // 읽어들인 피드값을 다른 구조체 배열에 저장
				rcv_data[0].value = data.value; // 읽어들인 카드값을 다른 구조체 배열에 저장
				printf("[ User%d로부터 %d 값을 받음 ] \n", rcv_data[0].pid, rcv_data[0].value);
				ri = 0;  // 하나의 값을 받으면 초기화 , 메시지 큐 값을 받지 않기 위해
			}

			r= msgrcv(msqid3, &data3, sizeof(data_t)-sizeof(long), 0, 0); 
			if(r == -1)
			{
				perror("msgrcv() 실패");
				exit(1);
			}
			if (r != -1){
				rcv_data[1].pid = data3.pid;
				rcv_data[1].value = data3.value;
				printf("\t                User%d로부터 %d 값을 받음 \n", rcv_data[1].pid, rcv_data[1].value);
				r = 0;
			}
			if(r==0 && ri == 0){  // user1, user2에게 값을 하나씩 받았을 때  num=0으로 초기화해서 무한루프 탈출
				num = 0;
			}
		}




		if( rcv_data[0].value > rcv_data[1].value )          // user1 값이 클 떄
		{
			data2.value = rcv_data[0].value;
			data2.pid = rcv_data[0].pid;
			data2.data_type = 1;
		}
		else if (rcv_data[0].value < rcv_data[1].value )     // user2 값이 클 떄
		{
			data2.value = rcv_data[1].value;
			data2.pid = rcv_data[1].pid;
			data2.data_type = 1;
		}
		else                                                 // 무승부
		{
			data2.value = 0;
			data2.pid = 0;
			data2.data_type = 1;
		}
		for(j=0;j<2;j++)                           //  user들에게 송신하는 메시지큐에 데이터 전송
		{
			if( -1 == msgsnd(msqid2, &data2, sizeof(data_t)-sizeof(long), 0))
			{
				perror("msgsnd() error ");
				exit(1);
			}
		}
	}
}
	return 0;
}




