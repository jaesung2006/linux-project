#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define user_to_jud 4545
#define jud_to_user 5454

typedef struct {              //메시지 큐를 통해 주고받는 구조체 선언
	long data_type;
	int pid;
	int value;
} data_t;


int main(){

	while(1){
	int msqid, msqid2;
	int index, i,r, notzero, score = 0, losescore=0;
	int a=0, b=0, c=0;
	int arr_value[10]={1,2,3,4,5,6,7,8,9,10};
	data_t data, data2;


	if( -1 == (msqid = msgget ((key_t)user_to_jud, IPC_CREAT | 0666)))  // 메시지큐 생성
	{
		perror("msgget() 실패");
		exit(1);
	}

	if( -1 == (msqid2 = msgget ((key_t)jud_to_user, IPC_CREAT | 0666)))
	{
		perror("msgger() 실패");
		exit(1);
	}

	printf("\n\n");
	printf("****************new game************** \n");

	for(i=0;i<10;i++){
		printf("=============================================\n");
		printf("보낼수 있는 값: ");
			
		for(notzero=0;notzero<10;notzero++){    // 자신이 지니고 있는 패 출력
			if(arr_value[notzero] != 0){        // 인덱스에있는 값이 0이 아닌 것들만 출력
				printf("%d  ",arr_value[notzero]);
			}
		}	
		printf("\n===========================================\n");
	
		printf("\n보낼값을 입력해주세요 : ");
		scanf("%d", &index);

		if( index == 0)  // 0 입력시 경고문 출력
		{
			printf("0을 입력하셨네요... 다른값을 입력해주세요~ 1~10범위내 \n");
				
			i--;
		}
		else if(0 != arr_value[index-1]&&index<11){   // 1~10까지의 값 입력시
			data.pid = 1;                             // 입력한 값을 구조체에 쓴다
			data.value = arr_value[index-1];
			data.data_type=1;

			arr_value[index-1] = 0;

			r= msgsnd (msqid, &data, sizeof(data_t)-sizeof(long), IPC_NOWAIT);
			if(r == -1)
			{
				perror("msgsnd() error");
				exit(1);
			}

			if ( -1 == msgrcv( msqid2, &data2, sizeof(data_t)-sizeof(long), 0, 0))
			{
				perror("msgrcv() 실패");
				exit(1);
			}

			if(data2.pid == 1){
				printf("[%d round]\t승리하였습니다.\n",i+1);
				score++;
				a++;
			}
			else if (data2.pid == 0){
				printf("[%d round]\t비겼습니다.\n",i+1);
				score++;
				losescore++;
				b++;
			}
			else{
				printf("[%d round]\t패배하였습니다.\n",i+1);
				losescore++;
				c++;
			}
		
		}
		else if(index >= 11)
		{
			printf(" 너무 큰 숫자를 입력하셨습니다 1~10 사이 값을 입력해 주세요 \n");
			i--;
		}
	
		else if(arr_value[index-1] == 0)
		{
			printf("\n**********이미 입력한 값입니다. 다시 입력해 주세요**********\n");
			i--;
		}
		printf("-------------  현재 내점수는 ? 승 : %d  무 : %d  패 : %d\n", a, b, c);
	}

	printf("최종 결과 발표 \n");
    // 최종 승자 패자 결정
	if(score > losescore)
		printf("축하합니다!!!!!! 최종 승자입니다. \n");
	else if(score < losescore)
		printf(" 아쉽네요... 패자입니다. \n");
	else
		printf("무승부 입니다.\n");
}
	return 0;
}


