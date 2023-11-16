#include"libvghubpd.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

//使うポートの数
#define N_PORT 7
#define N 30
void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents);

void main(){
	
	int i;
	
	
	VGHUBPD_STATUS VGHUBPD_Status_Val = VGHUBPD_Init(); // おまじない
	if(VGHUBPD_STATUS_SUCCESS==VGHUBPD_Status_Val){
 		printf("VGHub init success\n");
	}else{
 		printf("VGHub init error\n");
 		//return 0;
	}
	VGHUBPD_SetEventNotificationCallback(FuncPortEventNotify);
	
	
	FILE *fp;
	
	char filename[100];
	char aaa[100] = "a";
	printf("file name?: ");
	scanf("%s", filename);
	if(strcmp(filename, aaa) ==0){
		strcpy(filename,"ooo.csv");
	}
	printf("\n");
	int sampling_time=1;
	printf("sampling time[s]?: ");
	scanf("%d", &sampling_time);
	printf("-------------\n");
	
	if((fp=fopen(filename,"a")) !=NULL){
		fprintf(fp,"time,PORT[0]C,PORT[0]V,PORT[1]C,PORT[1]V,PORT[2]C,PORT[2]V,PORT[3]C,PORT[3]V,PORT[4]C,PORT[4]V,PORT[5]C,PORT[5]V\n");
	}
	
	//fp = fopen(filename,"a");
	
	PORT_VBUS_INFO v_information[N_PORT];
	VGHUBPD_STATUS getvinformation[N_PORT];
	
	int count = 0;
	int watt[N_PORT];
		int input, output;
		
	while(count<N){
		input=0;
		output=0;
	//GetPortVbusInfoで取得した状態を表示　
		for(i=0;i<N_PORT;i++){
			getvinformation[i] = VGHUBPD_GetPortVbusInfo(i, &v_information[i]);

			printf("PortID: %d\n Voltage: %dV\n Current: %dmA\n Watt: %dmW\n", 
					i, v_information[i].Voltage/1000,v_information[i].Current,  (v_information[i].Voltage*v_information[i].Current)/1000);
			watt[i] = (v_information[i].Voltage*v_information[i].Current)/1000;
		}
		
		printf("\n");
	
		for(i=0;i<N_PORT; i++){
			if(watt[i] <0){
				input+= watt[i];
			}
			else if(watt[i] >0){
				output += watt[i];
			}
		}
		
		printf("Input:  %dmW\nOutput: %dmW\nEfficiency: %f%\n\n",-input,output,-(double)output/(double)input *100);	
		
		fprintf(fp,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", count,
				v_information[0].Current,v_information[0].Voltage,
				v_information[1].Current,v_information[1].Voltage,
				v_information[2].Current,v_information[2].Voltage,
			   	v_information[3].Current,v_information[3].Voltage,
				v_information[4].Current,v_information[4].Voltage,
			   	v_information[5].Current,v_information[5].Voltage,
			   	v_information[6].Current,v_information[6].Voltage);

		count= count+sampling_time;
		sleep(sampling_time);
	}

	fclose(fp);
	
	printf("\n");
	VGHUBPD_STATUS VGHUBPD_Status_Val_Exit = VGHUBPD_Exit();
   	if(VGHUBPD_Status_Val_Exit == VGHUBPD_STATUS_SUCCESS){
 		printf("VGHub exit success\n");
	}else{
		printf("VGHub exit error\n");
		
	}
	return;
	
}

void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents){
	printf("Callback %d %d\n", portID,bmPortEvents);
}
