#include "libvghubpd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

// 使うポートの数
#define N_PORT 7
#define TIME 15
#define THRESHOLD_VOLTAGE 500
#define MAX_QUEUE_SIZE 100

void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents);
int SetPowerPole(PORT_ID portID, uint8_t Role);
int SetPortConfig(PORT_ID portID, uint8_t Role, uint32_t SourceMax, uint32_t SinkMax);

int flag_finish = 0;

FILE *fp1, *fp2;
char *fname1 = "./python/csv/vghub_15s_data.csv";
char *pdnego_fname = "./python/csv/vghub_pdnego_data.csv";

typedef struct
{
	float voltage;
} VoltageData;

typedef struct
{
	VoltageData data[MAX_QUEUE_SIZE];
	int front;
	int rear;
	int size;
} VoltageQueue;

void initQueue(VoltageQueue *queue);
int isQueueEmpty(VoltageQueue *queue);
int isQueueFull(VoltageQueue *queue);
void enqueue(VoltageQueue *queue, float voltage);
VoltageData dequeue(VoltageQueue *queue);
int calculateSlope(VoltageQueue *queue);
void displayQueue(VoltageQueue *queue);

void initQueue(VoltageQueue *queue)
{
	queue->front = 0;
	queue->rear = -1;
	queue->size = 0;
}

int isQueueEmpty(VoltageQueue *queue)
{
	return (queue->size == 0);
}

int isQueueFull(VoltageQueue *queue)
{
	return (queue->size == MAX_QUEUE_SIZE);
}

void enqueue(VoltageQueue *queue, float voltage)
{
	if (isQueueFull(queue))
	{
		dequeue(queue); // キューが満杯の場合、最も古い値を削除する
	}

	queue->rear = (queue->rear + 1) % MAX_QUEUE_SIZE;
	queue->data[queue->rear].voltage = voltage;
	queue->size++;
}

VoltageData dequeue(VoltageQueue *queue)
{
	VoltageData item;
	if (isQueueEmpty(queue))
	{
		printf("Queue is empty. Cannot dequeue voltage.\n");
		item.voltage = 0.0; // 適当なデフォルト値を返す
	}
	else
	{
		item = queue->data[queue->front];
		queue->front = (queue->front + 1) % MAX_QUEUE_SIZE;
		queue->size--;
	}
	return item;
}

int calculateSlope(VoltageQueue *queue)
{
	if (queue->size < MAX_QUEUE_SIZE)
	{
		printf("Insufficient data points. Cannot calculate slope.\n");
		return 0; // キューが満たされていない場合、0を返す（エラー値）
	}

	float sumX = 0.0;
	float sumY = 0.0;
	float sumXY = 0.0;
	float sumX2 = 0.0;
	int numValues = queue->size;

	VoltageQueue tempQueue = *queue; // 元のキューの状態を保持するためにテンポラリキューを作成
	while (!isQueueEmpty(&tempQueue))
	{
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

	if (denominator == 0.0)
	{
		printf("Cannot calculate slope. Denominator is zero.\n");
		return 0; // 分母が0の場合、0を返す（エラー値）
	}

	// 傾きを整数に変換して返す
	int slope = (int)(numerator / denominator);
	return slope;
}

void displayQueue(VoltageQueue *queue)
{
	if (isQueueEmpty(queue))
	{
		printf("Queue is empty.\n");
	}
	else
	{
		int i = queue->front;
		int count = 0;
		while (count < queue->size)
		{
			i = (i + 1) % MAX_QUEUE_SIZE;
			count++;
		}
	}
}

int write_csv_init(void)
{
	fp1 = fopen(fname1, "w");

	if (fp1 == NULL)
	{
		printf("I can't open the %s file\n", fname1);
		return -1;
	}
	// title
	fprintf(fp1, "time[sec],count, PORT[0]C[mA], PORT[0]V[mV], PORT[1]C[mA], PORT[1]V[mV], PORT[2]C[mA], PORT[2]V[mV], PORT[3]C[mA], PORT[3]V[mV], PORT[4]C[mA], PORT[4]V[mV], PORT[5]C[mA], PORT[5]V[mV], PORT[6]C[mA], PORT[6]V[mV], slope\n");
	return 0;
}

int write_csv_body(double d_sec, float count, PORT_VBUS_INFO *v_information, float slope)
{
	// 全部mV, mAの単位
	fprintf(fp1, "%f,%f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f\n",
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
	fclose(fp1);
	printf("\n");
	printf("Finished writing file to %s\n", fname1);
	return 0;
}
int call_python_csv_split(int i, int soc, char *data, char *app, char *device)
{
	printf("-------------------\n");
	printf("#### call py csv_split ####\n");
	char command1[200];
	char command2[200];

	snprintf(command1, sizeof(command1), "python3 python/csv_split.py vghub_15s_data %d %d %s %s %s\n", i, soc, data, app, device);
	snprintf(command2, sizeof(command2), "python3 python/csv_split.py vghub_pdnego_data %d %d %s %s %s\n", i, soc, data, app, device);

	system(command1);
	system(command2);

	return 0;
}

int call_python(void)
{
	return 0;
}

int vghub_sampling()
{
	printf("\n");
	float sampling_time = 0.01;
	printf("--------------------\n");

	write_csv_init();

	PORT_VBUS_INFO v_information[N_PORT];
	VGHUBPD_STATUS getvinformation[N_PORT];

	float count_time = 0;
	double current_slope = 0;
	double previous_slope = 0;
	int watt[N_PORT];
	int input, output;
	int flag_finish;
	char array[1500];

	// CPU時間計測
	unsigned int sec;
	int nsec;
	double d_sec, point_5v_time, point_5v_voltage, negotiation_period = 2.0;
	struct timespec start_time, end_time;
	clock_gettime(CLOCK_REALTIME, &start_time);
	int index = 0;

	VoltageQueue voltageQueue;
	initQueue(&voltageQueue);

	VGHUBPD_EnablePort(3, 1);

	while (count_time < TIME)
	{

		PORT_VBUS_INFO v_information[N_PORT];
		VGHUBPD_STATUS getvinformation[N_PORT];
		input = 0;
		output = 0;

		// GetPortVbusInfoで取得した状態を表示　
		for (int i = 0; i < N_PORT; i++)
		{
			getvinformation[i] = VGHUBPD_GetPortVbusInfo(i, &v_information[i]);
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

		count_time = count_time + sampling_time;
		printf("Elapsed time = %f\n", count_time);

		write_csv_body(d_sec, count_time, v_information, current_slope);

		enqueue(&voltageQueue, v_information[3].Voltage);
		displayQueue(&voltageQueue);
		VoltageQueue tempQueue = voltageQueue;
		while (!isQueueEmpty(&tempQueue))
		{
			VoltageData voltageData = dequeue(&tempQueue);
		}
		printf("\n");

		if (v_information[3].Voltage > THRESHOLD_VOLTAGE)
		{
			current_slope = calculateSlope(&voltageQueue);
			printf("Current slope: %f\n", current_slope);
			if (previous_slope > 0 && current_slope <= 0)
			{
				flag_finish = index;
			}
			previous_slope = current_slope;
		}
		printf("Finish Flag: %d\n", flag_finish);
		index++;
	}

	write_csv_end();

	printf("\n");
	return flag_finish;
}

void copyCsvDataWithIndex(const char *inputFileName, const char *outputFileName, int endIndex, int startIndex)
{
	FILE *inputFile = fopen(inputFileName, "r");
	FILE *outputFile = fopen(outputFileName, "w");

	if (inputFile == NULL || outputFile == NULL)
	{
		printf("Cannot open file.\n");
		return;
	}

	char line[1024]; // CSVファイルの1行分のデータを格納するバッファ

	// ヘッダ行をコピー
	if (fgets(line, sizeof(line), inputFile) != NULL)
	{
		fputs(line, outputFile);
	}

	// データ行をコピー
	int lineIndex = 0;
	while (fgets(line, sizeof(line), inputFile) != NULL)
	{
		if (lineIndex >= startIndex && lineIndex <= endIndex)
		{
			fputs(line, outputFile);
		}
		lineIndex++;
	}

	// ファイルを閉じる
	fclose(inputFile);
	fclose(outputFile);

	printf("Data copied.\n");
}

int findIndexExceedingValue(const char *fileName, int columnIndex, int threshold)
{
	FILE *inputFile = fopen(fileName, "r");
	if (inputFile == NULL)
	{
		printf("Failed to open file: %s\n", fileName);
		exit(1);
	}

	int index = 0;
	char line[256]; // 適切なサイズを選択してください

	while (fgets(line, sizeof(line), inputFile) != NULL)
	{
		char *token = strtok(line, ",");
		int col = 0;

		while (token != NULL)
		{
			if (col == columnIndex)
			{
				int value = atoi(token);
				if (value > threshold)
				{
					fclose(inputFile);
					return index;
				}
			}
			token = strtok(NULL, ",");
			col++;
		}

		index++;
	}

	fclose(inputFile);
	return -1; // 閾値を超えるデータが見つからない場合
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

	// informationは向こう側のRole SWAPはこちら側のRole
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

int vghub_power_rule()
{

	PORT_INFO information[N_PORT];		   // PORT_INFO型変数の定義
	VGHUBPD_STATUS getinformation[N_PORT]; // VGHUBPD_STATUS型の定義

	// 左から　ポートID、ロール（Source→0、Sink→1）、Source最大W、Sink最大W
	//  SINKはVGHUBがもらう方　SOURCEは渡す方
	// VGHUBPD_STATUS sets0 = SetPortConfig(0, 1, 0, 60);
	// VGHUBPD_STATUS sets1 = SetPortConfig(1, 1, 0, 60);
	// VGHUBPD_STATUS sets2 = SetPortConfig(2, 1, 0, 60);
	VGHUBPD_STATUS sets3 = SetPortConfig(3, 0, 60, 0);
	// VGHUBPD_STATUS sets4 = SetPortConfig(4, 1, 0, 60);
	// VGHUBPD_STATUS sets5 = SetPortConfig(5, 1, 0, 60);
	// VGHUBPD_STATUS sets6 = SetPortConfig(6, 1, 0, 60);

	sleep(1);

	printf("#### Seted PortConfig ####\n");
	printf("--------------------\n");
	// ポート情報を取得
	for (int i = 0; i < N_PORT; i++)
	{
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
	}
	printf("--------------------\n");
	printf("getinfocheck:%d %d %d\n", getinformation[0], getinformation[1], getinformation[2]);
	printf("Power Role\n");

	// 本来VGHubではなく向こうの立場に立ってのSink Sourceなんだけどわかりにくいので反転
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

	printf("--------------------\n");

	sleep(1);

	for (int i = 0; i < N_PORT; i++)
	{
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
		printf("%d ", getinformation[i]);
	}

	// GetPortInfoで更新した最大能力を表示する
	printf("Max Power\n");
	for (int i = 0; i < N_PORT; i++)
	{
		printf("Port %d:Source>%dmW Sink>%dmW \n",
			   i, information[i].SourceMaxPower, information[i].SinkMaxPower);
	}

	printf("--------------------\n");

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

	// 各種イベント発生時にコールされるコールバック関数を登録
	VGHUBPD_SetEventNotificationCallback(FuncPortEventNotify);

	vghub_power_rule(); // power rule

	VGHUBPD_EnablePort(3, 0);

	flag_finish = vghub_sampling(); // サンプリング

	VGHUBPD_STATUS VGHUBPD_Status_Val_Exit = VGHUBPD_Exit();

	if (VGHUBPD_Status_Val_Exit == VGHUBPD_STATUS_SUCCESS)
	{
		printf("VGHub exit success\n");
	}
	else
	{
		printf("VGHub exit error\n");
	}

	return flag_finish;
}

void main()
{
	int loop_index, soc;
	char data[20], device[20], app[20];

	printf("How many pieces of data do you want to create?：");
	scanf("%d",&loop_index);

	printf("SOC : ");
	scanf("%d",&soc);

	printf("test or validation or train : ");
	scanf("%s",&data);

	printf("home or sns or game or video : ");
	scanf("%s",&app);	

	printf("cheeropowerplus5 or cheeropowermountain or omnichargeomni20+ or pixel3a or ipadair4th or xperiaxz2compact : ");
	scanf("%s",&device);

	for (int i = 0; i < loop_index; i++)
	{
		flag_finish = main_vghub();

		const char *inputFileName = "./python/csv/vghub_15s_data.csv";
		const char *outputFileName = "./python/csv/vghub_pdnego_data.csv";

		int columnIndex = 9; // 10列目のデータをチェック
		int threshold = 5000;
		int flag_start = findIndexExceedingValue(inputFileName, columnIndex, threshold);

		printf("flag_start: %d\nflag_finish: %d\n", flag_start, flag_finish);
		copyCsvDataWithIndex(inputFileName, outputFileName, flag_finish, flag_start);
		printf("\n");
		call_python_csv_split(i+1, soc, data, app, device);
		// call_python_csv_split(i+1, soc, app);
		call_python();
	}
}

