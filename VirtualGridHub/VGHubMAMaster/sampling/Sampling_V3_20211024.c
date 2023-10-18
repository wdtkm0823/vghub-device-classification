#include"libvghubpd.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include <time.h>
#include <sys/time.h>

//使うポートの数
#define N_PORT 7
#define TIME 5
void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents);

FILE *fp;
char *fname = "./python/csv/vghub_upbpd_data.csv";
char *fname_forpy = "vghub_upbpd_data";

int write_csv_init(void){
	fp = fopen(fname, "w");
	if (fp == NULL)
	{
		printf("%sファイルが開けません¥n", fname);
		return -1;
	}
	//title
	fprintf(fp, "time[sec],count, PORT[0]C[mA], PORT[0]V[mV], PORT[1]C[mA], PORT[1]V[mV], PORT[2]C[mA], PORT[2]V[mV], PORT[3]C[mA], PORT[3]V[mV], PORT[4]C[mA], PORT[4]V[mV], PORT[5]C[mA], PORT[5]V[mV], PORT[6]C[mA], PORT[6]V[mV]\n");
	return 0;
}

int write_csv_body(double d_sec, float count, PORT_VBUS_INFO* v_information)
{
	//全部mV, mAの単位
	fprintf(fp, "%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", 
			d_sec,
			count,
			v_information[0].Current, v_information[0].Voltage,
			v_information[1].Current, v_information[1].Voltage,
			v_information[2].Current, v_information[2].Voltage,
			v_information[3].Current, v_information[3].Voltage,
			v_information[4].Current, v_information[4].Voltage,
			v_information[5].Current, v_information[5].Voltage,
			v_information[6].Current, v_information[6].Voltage);
	return 0;
}

int write_csv_end(void)
{
	fclose(fp);
	printf("%sファイル書き込みが終わりました\n", fname);
	return 0;
}

int call_python(void){
	printf("call py");
//	char *a = "python3 python/csv_split.py"+" "+"vghub_upbpd_data";
//	printf("%s",a);

	system("python3 python/csv_split.py vghub_upbpd_data");
	return 0;
}




int main_vghub(){

	VGHUBPD_STATUS VGHUBPD_Status_Val = VGHUBPD_Init(); // おまじない
	if (VGHUBPD_STATUS_SUCCESS == VGHUBPD_Status_Val){
		printf("VGHub init success\n");
	}else{
		printf("VGHub init error\n");
	}
	VGHUBPD_SetEventNotificationCallback(FuncPortEventNotify);

	/*
	char filename[100];
	char aaa[100] = "a";
	/*printf("file name?: ");
	scanf("%s", filename);
	if(strcmp(filename, aaa) ==0){
		strcpy(filename,"ooo.csv");
	}*/
	printf("\n");
	float sampling_time = 0.01;
	//printf("sampling time[s]?: ");
	//scanf("%d", &sampling_time);
	printf("-------------\n");

	write_csv_init();

	PORT_VBUS_INFO v_information[N_PORT];
	VGHUBPD_STATUS getvinformation[N_PORT];

	float count_time = 0;
	int watt[N_PORT];
	int input, output;

	//CPU時間計測
	unsigned int sec;
	int nsec;
	double d_sec;
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_REALTIME, &start_time);

	while (count_time < TIME){
		input = 0;
		output = 0;
		//GetPortVbusInfoで取得した状態を表示　

		for (int i = 0; i < N_PORT; i++){
			getvinformation[i] = VGHUBPD_GetPortVbusInfo(i, &v_information[i]);

			//printf("PortID: %d\n Voltage: %dV\n Current: %dmA\n Watt: %dmW\n",
			//		i, v_information[i].Voltage/1000,v_information[i].Current,  (v_information[i].Voltage*v_information[i].Current)/1000);
			watt[i] = (v_information[i].Voltage * v_information[i].Current) / 1000;
		}

		printf("\n");
		for (int i = 0; i < N_PORT; i++){
			if (watt[i] < 0){
				input += watt[i];
			}else if (watt[i] > 0){
				output += watt[i];
			}
		}

		printf("Input:  %dmW\nOutput: %dmW\nEfficiency: %f%\n\n", -input, output, -(double)output / (double)input * 100);

		clock_gettime(CLOCK_REALTIME, &end_time);
		sec = end_time.tv_sec - start_time.tv_sec;
		nsec = end_time.tv_nsec - start_time.tv_nsec;

		d_sec = (double)sec + (double)nsec / (1000 * 1000 * 1000);

		printf("time:%f\n", d_sec);
		write_csv_body(d_sec, count_time, v_information);
		count_time = count_time + sampling_time;
		printf("経過時間 = %f\n", count_time);
		//usleep(sampling_time * 1000 * 1000);
	}

	write_csv_end();

	printf("\n");

	VGHUBPD_STATUS VGHUBPD_Status_Val_Exit = VGHUBPD_Exit();
	if (VGHUBPD_Status_Val_Exit == VGHUBPD_STATUS_SUCCESS){
		printf("VGHub exit success\n");
	}else{
		printf("VGHub exit error\n");
	}
	
	return 0;
}


void main(){
	main_vghub();
	call_python();
}

void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents){
	printf("Callback %d %d\n", portID,bmPortEvents);
}

