#include "libvghubpd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

//使うポートの数
#define N_PORT 7
#define TIME 15
#define THRESHOLD_VOLTAGE 500
#define MAX_QUEUE_SIZE 120

void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents);
int SetPowerPole(PORT_ID portID, uint8_t Role);
int SetPortConfig(PORT_ID portID, uint8_t Role, uint32_t SourceMax, uint32_t SinkMax);

FILE *fp;
char *fname = "./python/csv/vghub_upbpd_data.csv";
char *fname_forpy = "vghub_upbpd_data";

typedef struct {
    float voltage;
} VoltageData;

typedef struct {
    VoltageData data[MAX_QUEUE_SIZE];
    int front;
    int rear;
    int size;
} VoltageQueue;

void initQueue(VoltageQueue* queue);
int isQueueEmpty(VoltageQueue* queue);
int isQueueFull(VoltageQueue* queue);
void enqueue(VoltageQueue* queue, float voltage);
VoltageData dequeue(VoltageQueue* queue);
float calculateSlope(VoltageQueue* queue);

void initQueue(VoltageQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

int isQueueEmpty(VoltageQueue* queue) {
    return (queue->size == 0);
}

int isQueueFull(VoltageQueue* queue) {
    return (queue->size == MAX_QUEUE_SIZE);
}

void enqueue(VoltageQueue* queue, float voltage) {
    if (isQueueFull(queue)) {
        dequeue(queue);  // キューが満杯の場合、最も古い値を削除する
    }

    queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
    queue->data[queue->rear].voltage = voltage;
    queue->size++;
}

VoltageData dequeue(VoltageQueue* queue) {
    VoltageData item;
    if (isQueueEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue voltage.\n");
        item.voltage = 0.0;  // 適当なデフォルト値を返す
    } else {
        item = queue->data[queue->front];
        queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
        queue->size--;
    }
    return item;
}

float calculateSlope(VoltageQueue* queue) {
    if (queue->size < 2) {
        printf("Insufficient data points. Cannot calculate slope.\n");
        return 0.0;
    }

    float sumX = 0.0;
    float sumY = 0.0;
    float sumXY = 0.0;
    float sumX2 = 0.0;
    int numValues = queue->size;

    VoltageQueue tempQueue = *queue; // 元のキューの状態を保持するためにテンポラリキューを作成
    while (!isQueueEmpty(&tempQueue)) {
        VoltageData voltageData = dequeue(&tempQueue);
        float x = numValues - tempQueue.size;
        float y = voltageData.voltage;
        sumX += x;
        sumY += y;
        sumXY += x * y;
        sumX2 += x * x;
    }

    float meanX = sumX / numValues;
    float meanY = sumY / numValues;

    float numerator = sumXY - numValues * meanX * meanY;
    float denominator = sumX2 - numValues * meanX * meanX;

    if (denominator == 0.0) {
        printf("Cannot calculate slope. Denominator is zero.\n");
        return 0.0;
    }

    float slope = numerator / denominator;
    return slope;
}

int write_csv_init(void)
{
	fp = fopen(fname, "w");
	if (fp == NULL)
	{
		printf("%sファイルが開けません¥n", fname);
		return -1;
	}
	//title
	fprintf(fp, "time[sec],count, PORT[0]C[mA], PORT[0]V[mV], PORT[1]C[mA], PORT[1]V[mV], PORT[2]C[mA], PORT[2]V[mV], PORT[3]C[mA], PORT[3]V[mV], PORT[4]C[mA], PORT[4]V[mV], PORT[5]C[mA], PORT[5]V[mV], PORT[6]C[mA], PORT[6]V[mV], slope\n");
	return 0;
}

int write_csv_body(double d_sec, float count, PORT_VBUS_INFO *v_information, float slope)
{
	//全部mV, mAの単位
	fprintf(fp, "%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f\n",
			d_sec,
			count,
			v_information[0].Current, v_information[0].Voltage,
			v_information[1].Current, v_information[1].Voltage,
			v_information[2].Current, v_information[2].Voltage,
			v_information[3].Current, v_information[3].Voltage,
			v_information[4].Current, v_information[4].Voltage,
			v_information[5].Current, v_information[5].Voltage,
			v_information[6].Current, v_information[6].Voltage,
			slope);
	return 0;
}

int write_csv_end(void)
{
	fclose(fp);
	printf("%sファイル書き込みが終わりました\n", fname);
	return 0;
}

int call_python_csv_split(void)
{
	printf("call py csv_split\n");
	//	char *a = "python3 python/csv_split.py"+" "+"vghub_upbpd_data";
	//	printf("%s",a);

	system("python3 python/csv_split.py vghub_upbpd_data");
	return 0;
}

int call_python(void){
	return 0;
}

int vghub_sampling()
{

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
	float current_slope = 0;
	float previous_slope = 0;
	int watt[N_PORT];
	int input, output;

	//CPU時間計測
	unsigned int sec;
	int nsec;
	double d_sec,point_5v_time,point_5v_voltage,negotiation_period=2.0;
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_REALTIME, &start_time);
	
	VoltageQueue voltageQueue;
	initQueue (&voltageQueue);

	while (count_time < TIME){
		PORT_VBUS_INFO v_information[N_PORT];
		VGHUBPD_STATUS getvinformation[N_PORT];
		input = 0;
		output = 0;
		//GetPortVbusInfoで取得した状態を表示　
		for (int i = 0; i < N_PORT; i++)
		{
			getvinformation[i] = VGHUBPD_GetPortVbusInfo(i, &v_information[i]);

			//printf("PortID: %d\n Voltage: %dV\n Current: %dmA\n Watt: %dmW\n",
			//		i, v_information[i].Voltage/1000,v_information[i].Current,  (v_information[i].Voltage*v_information[i].Current)/1000);
			watt[i] = (v_information[i].Voltage * v_information[i].Current) / 1000;
		}

		printf("\n");
		for (int i = 0; i < N_PORT; i++)
		{
			if (watt[i] < 0)
			{
				input += watt[i];
			}
			else if (watt[i] > 0)
			{
				output += watt[i];
			}
		}

		printf("Input:  %dmW\nOutput: %dmW\nEfficiency: %f%\n\n", -input, output, -(double)output / (double)input * 100);

		clock_gettime(CLOCK_REALTIME, &end_time);
		sec = end_time.tv_sec - start_time.tv_sec;
		nsec = end_time.tv_nsec - start_time.tv_nsec;

		d_sec = (double)sec + (double)nsec / (1000 * 1000 * 1000);

		printf("time:%f\n", d_sec);
		
		count_time = count_time + sampling_time;
		printf("経過時間 = %f\n", count_time);
		//usleep(sampling_time * 1000 * 1000);
		
		enqueue(&voltageQueue, v_information[3].Voltage);
		VoltageQueue tempQueue = voltageQueue; // テンポラリキューを作成し、元のキューの状態を保持
		while (!isQueueEmpty(&tempQueue)) {
		    VoltageData voltageData = dequeue(&tempQueue);
		}
		printf("\n");
		
		if (v_information[3].Voltage > THRESHOLD_VOLTAGE) {
			// 傾きの計算と表示
			current_slope = calculateSlope(&voltageQueue);
			printf("Slope: %.2f\n", current_slope);
			write_csv_body(d_sec, count_time, v_information, current_slope);
			if (current_slope > 0.0 && previous_slope < 0.0){
				break;
			}
			previous_slope = current_slope;
		}
	}
	
	write_csv_end();

	printf("\n");
	return 0;
}
int vghub_power_rule(){
	
	PORT_INFO information[N_PORT];		   //PORT_INFO型変数の定義
	VGHUBPD_STATUS getinformation[N_PORT]; //VGHUBPD_STATUS型の定義

	/*SWAPPORT →ロールスワップしたいポート番号　しない場合-1
	int SWAPPORT =-1;
	if(SWAPPORT != -1){
		VGHUBPD_STATUS SWAPC= VGHUBPD_SendPRSwap(SWAPPORT,1);
		printf("Swapped the Role of %d     STATUS = %d\n", SWAPPORT, SWAPC);
		sleep(1);
	}	
	*/

	//左から　ポートID、ロール（Source→0、Sink→1）、Source最大W、Sink最大W
	// SINKはVGHUBがもらう方　SOURCEは渡す方
	//VGHUBPD_STATUS sets0 = SetPortConfig(0, 1, 0, 60);
	//VGHUBPD_STATUS sets1 = SetPortConfig(1, 1, 0, 60);
	//VGHUBPD_STATUS sets2 = SetPortConfig(2, 1, 0, 60);
	VGHUBPD_STATUS sets3 = SetPortConfig(3, 0, 60, 0);
	//VGHUBPD_STATUS sets4 = SetPortConfig(4, 1, 0, 60);
	//VGHUBPD_STATUS sets5 = SetPortConfig(5, 1, 0, 60);
	//VGHUBPD_STATUS sets6 = SetPortConfig(6, 1, 0, 60);

	sleep(1);

	printf("#### Seted PortConfig ####\n");
	printf("---------------------\n");
	//ポート情報を取得
	for (int i = 0; i < N_PORT; i++)
	{
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
	}
	printf("---------------------\n");
	printf("getinfocheck:%d %d %d\n", getinformation[0], getinformation[1], getinformation[2]);
	printf("Power Role\n");

	//本来VGHubではなく向こうの立場に立ってのSink Sourceなんだけどわかりにくいので反転
	for (int i = 0; i < N_PORT; i++)
	{
		if (information[i].PowerRole == 1)
		{
			printf("Port %d: Source Role  %d %d Connect:%d  PDCPower%d SMBE%d UFp%d DFP%d PDC%d\n", i, information[i].SourceEnabled, information[i].SinkEnabled,
				   information[i].PortConnect, information[i].PDCPower, information[i].SMBusError, information[i].UFPEnabled, information[i].DFPEnabled, information[i].PDContract);
		}
		else if (information[i].PowerRole == 0)
		{
			printf("Port %d: Sink Role %d %d Connect:%d PDCPower%d SMBE%d UFp%d DFP%d PDC%d\n", i, information[i].SourceEnabled, information[i].SinkEnabled,
				   information[i].PortConnect, information[i].PDCPower, information[i].SMBusError, information[i].UFPEnabled, information[i].DFPEnabled, information[i].PDContract);
		}
	}

	printf("---------------------\n");

	sleep(1);

	//update
	for (int i = 0; i < N_PORT; i++)
	{
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
		printf("%d ", getinformation[i]);
	}

	//GetPortInfoで更新した最大能力を表示する
	printf("Max Power\n");
	for (int i = 0; i < N_PORT; i++)
	{
		printf("Port %d:Source>%dmW Sink>%dmW \n",
			   i, information[i].SourceMaxPower, information[i].SinkMaxPower);
	}

	printf("---------------------\n");

	return 0;

}
int main_vghub()
{

	VGHUBPD_STATUS VGHUBPD_Status_Val = VGHUBPD_Init(); // おまじない
	if (VGHUBPD_STATUS_SUCCESS == VGHUBPD_Status_Val)
	{
		printf("VGHub init success\n");
	}
	else
	{
		printf("VGHub init error\n");
	}
	VGHUBPD_SetEventNotificationCallback(FuncPortEventNotify);

	//pwer rule
	vghub_power_rule();
	//サンプリング
	vghub_sampling();


	VGHUBPD_STATUS VGHUBPD_Status_Val_Exit = VGHUBPD_Exit();
	if (VGHUBPD_Status_Val_Exit == VGHUBPD_STATUS_SUCCESS)
	{
		printf("VGHub exit success\n");
	}
	else
	{
		printf("VGHub exit error\n");
	}

	return 0;
}

void main()
{
	main_vghub();
	call_python_csv_split();
	call_python();
}

void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents)
{
	printf("Callback %d %d\n", portID, bmPortEvents);
}

int SetPowerPole(PORT_ID portID, uint8_t Role)
{
	PORT_INFO information;
	VGHUBPD_STATUS getinformation;
	VGHUBPD_STATUS SWAPC = -1;
	getinformation = VGHUBPD_GetPortInfo(portID, &information);
	//printf("RoleCheck: %d %d %d\n",portID , information.PowerRole , Role);

	//informationは向こう側のRole SWAPはこちら側のRole
	if (information.PowerRole == Role)
	{
		SWAPC = VGHUBPD_SendPRSwap(portID, 1);
		printf("Try to Role Swap of %d   >%d", portID, SWAPC);
	}
	sleep(1);
	return SWAPC;
}

int SetPortConfig(PORT_ID portID, uint8_t Role, uint32_t SourceMax, uint32_t SinkMax)
{
	VGHUBPD_STATUS sets;

	VGHUBPD_STATUS enable1 = VGHUBPD_EnablePort(portID, 0);
	printf("disenabled Port%d   %d\n", portID, enable1);
	sleep(1);
	if (Role == 0)
	{
		sets = VGHUBPD_SetPortPowerConfig(portID, 1, 0, SourceMax * 1000, 0, 1);
		printf("Set the config of port%d   %d Source\n ", portID, sets);
	}
	else if (Role == 1)
	{
		sets = VGHUBPD_SetPortPowerConfig(portID, 0, 1, 0, SinkMax * 1000, 1);
		printf("Set the config of port%d   %d Sink\n ", portID, sets);
	}
	sleep(1);
	enable1 = VGHUBPD_EnablePort(portID, 1);
	printf("enabled Port%d   %d\n", portID, enable1);
	sleep(1);

	return enable1;
}
