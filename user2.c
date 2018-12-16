#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define user_to_jud 6666
#define jud_to_user 5454

typedef struct {
	long data_type;
	int pid;
	int value;
} data_t;

int main(){

	while(1){
	int msqid, msqid2;
	int index, i,r, n, score =0, losescore =0;
	int a=0, b=0, c=0;
	int arr_value[10]={1,2,3,4,5,6,7,8,9,10};
	data_t data, data2;



	if( -1 == (msqid = msgget ((key_t)user_to_jud, IPC_CREAT | 0666)))
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
	printf("*****************new game******************\n");

	for(i=0;i<10;i++){
		printf("============================================\n");
		printf("보낼수 있는 값 : ");

		for(n=0;n<10;n++){
			if(arr_value[n] !=0){
				printf("%d  ",arr_value[n]);
			}	
		}	
		printf("\n============================================\n");

		printf("\n보낼값을 입력해주세요 : ");
		scanf("%d", &index);

		if(index == 0){
			printf("0을 입력하셨네요.... 다른값을 입력해주세요~ 1~10범위 내 \n");
			i--;
		}
		else if(0 != arr_value[index-1] && index<11){
			data.pid = 2;
			data.value = arr_value[index-1];
			data.data_type=1;

			arr_value[index-1] = 0;

			r= msgsnd (msqid, &data, sizeof(data_t)-sizeof(long), 0);
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

			if(data2.pid == 2){
				printf("[%d round]\t승리하였습니다. \n", i+1);
				score++;
				a++;
			}
			else if(data2.pid == 0){
				printf("[%d round]\t비겼습니다. \n", i+1);
				score++;
				losescore++;
				b++;
			}
			else{
				printf("[%d round]\t패배하였습니다. \n", i+1);
				losescore++; 
				c++;
			}
		}
		else if (index >= 11){
			printf("너무 큰 숫자를 입력하셨습니다. 1~10사이 값을 입력해 주세요 \n");
			i--;
		}
		else if(arr_value[index-1] == 0){
			printf("**********이미 입력한 값입니다. 다시 입력해 주세요********** \n");
			i--;
		}
		printf("-------------  현재 내점수는 ? 승 : %d  무 : %d  패 : %d\n", a, b, c);
	}

	printf("최종 결과 발표 \n");
	if(score > losescore)
		printf("축하합니다!!!!!!! 최종 승자입니다. \n");
    else if(score < losescore)
		printf(" 아쉽네요... 패자입니다. \n");
	else
         printf("무승부 입니다.\n");
}
	return 0;
}


