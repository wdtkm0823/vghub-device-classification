#include"libvghubpd.h"
#include<stdio.h>
#include<unistd.h>

//使うポートの数
#define N_PORT 6
void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents);
int SetPowerPole(PORT_ID portID,uint8_t Role);
int SetPortConfig(PORT_ID portID,uint8_t Role, uint32_t SourceMax, uint32_t SinkMax);
int PlugCheck(PORT_ID portID);
int DecideRole(PORT_ID portID);
int flag[N_PORT];
int prio;
int sourcetotal;
int sinktotal;
int OverCapa(PORT_ID portID);

void main(){
	printf("Hello world\n");
	
	int i;
	
	
	VGHUBPD_STATUS vghubpd_statusvg = VGHUBPD_Init();
   if(vghubpd_statusvg == VGHUBPD_STATUS_SUCCESS){
 		printf("finish init\n");

	}
	
	printf("\n");


	PORT_INFO information[N_PORT];
	VGHUBPD_STATUS getinformation[N_PORT];
	
	/*SWAPPORT →ロールスワップしたいポート番号　しない場合-1
	int SWAPPORT =-1;
	if(SWAPPORT != -1){
		VGHUBPD_STATUS SWAPC= VGHUBPD_SendPRSwap(SWAPPORT,1);
		printf("Swapped the Role of %d     STATUS = %d\n", SWAPPORT, SWAPC);
		sleep(1);
	}	
	*/
		//ポート情報を取得
	for(i=0;i<N_PORT;i++){
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
	}
	
		for(i=0; i<N_PORT;i++){
		if(information[i].PowerRole == 1){
			printf("Port %d: Source Role  %d %d Connect:%d  PDCPower%d SMBE%d UFp%d DFP%d PDC%d\n", i,information[i].SourceEnabled, information[i].SinkEnabled, 
				   information[i].PortConnect,information[i].PDCPower,information[i].SMBusError,information[i].UFPEnabled,information[i].DFPEnabled,information[i].PDContract);
		}
		else if(information[i].PowerRole == 0){
			printf("Port %d: Sink Role %d %d Connect:%d PDCPower%d SMBE%d UFp%d DFP%d PDC%d\n", i,information[i].SourceEnabled, information[i].SinkEnabled, 
				   information[i].PortConnect,information[i].PDCPower,information[i].SMBusError,information[i].UFPEnabled,information[i].DFPEnabled,information[i].PDContract);
		}
	}
	
	printf("\n");
	//左から　ポートID、ロール（Source→0、Sink→1）、Source最大W、Sink最大W
	//VGHUBPD_STATUS sets0 = SetPortConfig(0,0,18,9);
	//VGHUBPD_STATUS sets1 = SetPortConfig(1,0,18,18);
	//VGHUBPD_STATUS sets2 = SetPortConfig(2,1,20,60);
	//VGHUBPD_STATUS sets3 = SetPortConfig(3,0,34,36);
	//VGHUBPD_STATUS sets4 = SetPortConfig(4,0,15,15);
	//VGHUBPD_STATUS sets5 = SetPortConfig(5,0,32,60);
	VGHUBPD_STATUS sets;

	sleep(1);
	int bufrole,bufsource, bufsink;
	
	printf("Choose the priority port");
	scanf("%d",&prio);	
	bufrole = DecideRole(prio);
	if(bufrole == 1){
		printf("MAX Sink Power?\n");
		scanf("%d", &bufsink);
		sinktotal += bufsink;
		printf("Source:%dW\nSink:%dW\n\n",sourcetotal,sinktotal);
		sets = SetPortConfig(prio,bufrole,0,bufsink);
	}
		getinformation[i] = VGHUBPD_GetPortInfo(prio, &information[prio]);
		printf("Port %d:Source>%dmW Sink>%dmW \n",
			prio,information[prio].SourceMaxPower,information[prio].SinkMaxPower);
	
	
	for(i=0;i<N_PORT;i++){
		if(information[i].PortConnect == 1 && i!=prio){
			bufrole=DecideRole(i);
			
			if(bufrole == 0){
				printf("MAX Source Power?\n");
				scanf("%d", &bufsource);
				sourcetotal += bufsource;
				sets = SetPortConfig(i,bufrole,bufsource,0);
			}
			
			else if(bufrole == 1){
				printf("MAX Sink Power?\n");
				scanf("%d", &bufsink);
				sinktotal += bufsink;
				sets = SetPortConfig(i,bufrole,0,bufsink);
			}
			getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
			printf("Port %d:Source>%dmW Sink>%dmW \n",
				i,information[i].SourceMaxPower,information[i].SinkMaxPower);
		}
		else{
			printf("Port %d is not used\n",i);
		}
	}

	
	printf("\n");
	
	
	//ポート情報を取得
	for(i=0;i<N_PORT;i++){
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
	}
	
	

	printf("getinfocheck:%d %d %d\n",getinformation[0],getinformation[1],getinformation[2]);
	printf("Power Role\n");
	
	

	
	

	//本来VGHubではなく向こうの立場に立ってのSink Sourceなんだけどわかりにくいので反転
	for(i=0; i<N_PORT;i++){
		if(information[i].PowerRole == 1){
			printf("Port %d: Source Role  %d %d Connect:%d  PDCPower%d SMBE%d UFp%d DFP%d PDC%d\n", i,information[i].SourceEnabled, information[i].SinkEnabled, 
				   information[i].PortConnect,information[i].PDCPower,information[i].SMBusError,information[i].UFPEnabled,information[i].DFPEnabled,information[i].PDContract);
		}
		else if(information[i].PowerRole == 0){
			printf("Port %d: Sink Role %d %d Connect:%d PDCPower%d SMBE%d UFp%d DFP%d PDC%d\n", i,information[i].SourceEnabled, information[i].SinkEnabled, 
				   information[i].PortConnect,information[i].PDCPower,information[i].SMBusError,information[i].UFPEnabled,information[i].DFPEnabled,information[i].PDContract);
		}
	}
	
	printf("\n");

	
	sleep(1);
	
	

	
	//update
	for(i=0;i<N_PORT;i++){
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
		printf("%d ",getinformation[i]);
	}
	
	//GetPortInfoで更新した最大能力を表示する
	printf("Max Power\n");
	for(i=0;i<N_PORT;i++){
		printf("Port %d:Source>%dmW Sink>%dmW \n",
			   				i,information[i].SourceMaxPower,information[i].SinkMaxPower);
	}
	
	printf("\n");


	
	PORT_VBUS_INFO v_information[N_PORT];
	VGHUBPD_STATUS getvinformation[N_PORT];

	
	//GetPortVbusInfoで取得した状態を表示　
	for(i=0;i<N_PORT;i++){
		getvinformation[i] = VGHUBPD_GetPortVbusInfo(i, &v_information[i]);
	
		printf("PortID: %d\n Voltage: %dV\n Current: %dmA\n Watt: %dmW\n", 
				i, v_information[i].Voltage/1000,v_information[i].Current,  (v_information[i].Voltage*v_information[i].Current)/1000);
	}
	
	for(i=0;i<N_PORT;i++){
		if(information[i].PortConnect ==1){
			flag[i] = 1;
		}
	}
	
	VGHUBPD_SetEventNotificationCallback(FuncPortEventNotify);

	
	while(1){
	sleep(1);
	}
		/*
	MIDB_INFO m_infomation[N_PORT];
	VGHUBPD_STATUS getmanuinformation[N_PORT];
	
	for(i=0;i<N_PORT;i++){
		getmanuinformation[i]= VGHUBPD_GetFarEndPortManuInfo(i, &m_infomation[i]);
		if(getmanuinformation[i] == 0){
			printf("PORT %d:\nVID:%d \nPID:%d \nManufacture:%s\n%d\n\n",
								i, m_infomation[i].VID, m_infomation[i].PID , m_infomation[i].ManufacturerString,
								getmanuinformation[i]);
		}
		else{
			printf("faii to get the data of ManuInfo of %d\n",i);
		}
		
		sleep(2);
	}
				*/
		
			
	
	
	
	VGHUBPD_STATUS vghubpd_statusvg_exit = VGHUBPD_Exit();
   	if(vghubpd_statusvg_exit == VGHUBPD_STATUS_SUCCESS){
 		printf("\nfinish exit\n");

	}
	
}

void FuncPortEventNotify(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents){
	VGHUBPD_STATUS sets;
	printf("Callback %d %d\n", portID,bmPortEvents);
	//printf("che %d\n ",bmPortEvents.event.PDContracted	);
	int bufrole,bufsource, bufsink,i,capa,choose;
	PORT_INFO information[N_PORT];
	VGHUBPD_STATUS getinformation[N_PORT];
	char stable = 'a';
	//printf("4:%d 5:%d  13:%d\n",bmPortEvents.event.DataRoleChanged,bmPortEvents.event.PDContracted,bmPortEvents.event.PowerConfigCompleted);
	if(bmPortEvents.event.DataRoleChanged== 1 && bmPortEvents.event.PDContracted== 1 && bmPortEvents.event.PowerConfigCompleted == 0 ){//4 5 -13
		capa = OverCapa(portID);
	}
	
	if(bmPortEvents.event.PlugChanged== 1 &&  bmPortEvents.event.PortEnabledDisabled==0 && flag[portID] == 0 ){
		printf("Connected Port %d\n",portID);
		flag[portID] = 1;
	}
	
	else if(bmPortEvents.event.PlugChanged== 1 && bmPortEvents.event.PortEnabledDisabled==0 && flag[portID] == 1 ){
		printf("DisConnected Port %d\n",portID);
		
		
		///////////
		//ここに抜けて電力供給がなくなったときの処理関数を置きたい
		if(portID== prio){
			int chan = PlugCheck(portID);
		}
		
		flag[portID] = 0;
	}
	//printf("PlugChange%d PDCon%d\n",bmPortEvents.event.PlugChanged,bmPortEvents.event.PDContracted);
	//printf("13:%d 15:%d\n",bmPortEvents.event.PowerConfigCompleted,bmPortEvents.event.PortEnabledDisabled);

	/* Sourceがキャパオーバーしたときの条件がよくわからない
	
	int priosource;
	if(bmPortEvents.event.PlugChanged== 1  && bmPortEvents.event.PDContracted== 1){//36
		printf("Choose Priority Source Port\n");
		scanf("%d",priosource);
		for(i=0;i<N_PORT;i++){
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
		if(information[i].PDContract == 1 && information[i].SourceEnabled==1 && i != priosource){
			sets = SetPortConfig(i,1,0,60);
			printf("Emergency change the role of %d\n",i);
		}
	}
		
	}
	
	*/
	
	
	//	printf("Connected Port %d\n",portID);
	//}
	//if(flag[portID] == 1){
		//printf("DisConnected Port %d\n",portID);
		
		
	//}
	//if(bmPortEvents.event.PDContracted== 1){
	//			flag[portID] = 1;
		//printf("Disconnected Port %d\n",portID);
	//}
	
	//if(flag[portID] == 1 && bmPortEvents.event.PlugChanged== 1){
		//flag[portID] = 0;
	//}
	


	
	if(bmPortEvents.event.PDContracted== 1 && bmPortEvents.event.PowerConfigCompleted == 0){

		printf("Role of Port %d? Source>0 Sink >1\n",portID);
		scanf("%d", &bufrole);

		if(bufrole == 0){
			printf("MAX Source Power?\n");
			scanf("%d", &bufsource);
			sets = SetPortConfig(portID,bufrole,bufsource,0);
			sourcetotal += bufsource;

		}

		else if(bufrole == 1){
			printf("MAX Sink Power?\n");
			scanf("%d", &bufsink);
			sets = SetPortConfig(portID,bufrole,0,bufsink);
			sinktotal += bufsink;
		}
	
	
		sleep(3);
		while(1){
			while(1){ 
				printf("OK?  y or n>\n");
				scanf(" %c",&stable);
				printf("input %c\n",stable);
				if(stable == 'y' || stable =='n'){
					break;
				}
				else{
					printf("y or n!!!!!!\n");
				}
			}
			if(stable == 'y'){
				break;
			}
			else if(stable == 'n'){
				//不安定なときの処理
				while(1){ 
					printf("Choose\n1.Change Source Max Power\n2.Change or Add Sink Power\n");
					scanf("%d",&choose);
					if(choose == 1 || choose == 2){
						break;
					}
				}

				if(choose == 1){
					printf("MAX Source Power?\n");
					scanf("%d", &bufsource);
					sets = SetPortConfig(portID,bufrole,bufsource,0);
				}

				else if(choose == 2){
					sets = OverCapa(portID);
				}
			}
		}
	}
	//if( bmPortEvents.event.DataRoleChanged ==1){
		//printf("Changed the port connect of %d", portID);
	//}
}

int SetPowerPole(PORT_ID portID,uint8_t Role){
	PORT_INFO information;
	VGHUBPD_STATUS getinformation;
	VGHUBPD_STATUS SWAPC=-1;
	getinformation = VGHUBPD_GetPortInfo(portID, &information);
	//printf("RoleCheck: %d %d %d\n",portID , information.PowerRole , Role);

	//informationは向こう側のRole SWAPはこちら側のRole
	if(information.PowerRole == Role){
		SWAPC = VGHUBPD_SendPRSwap(portID,1);
		printf("Try to Role Swap of %d   >%d",portID,SWAPC);
	}
	sleep(1);
	return SWAPC;
}


int SetPortConfig(PORT_ID portID,uint8_t Role, uint32_t SourceMax, uint32_t SinkMax){
	VGHUBPD_STATUS sets;
		
	VGHUBPD_STATUS enable1 = VGHUBPD_EnablePort(portID,0);
	printf("disenabled Port%d   %d\n",portID,enable1);
	sleep(2);
	if(Role == 0){
		sets = VGHUBPD_SetPortPowerConfig(portID,1,0,SourceMax*1000,0,1);
		printf("Set the config of port%d   %d \n",portID,sets);
	}
	else if(Role == 1){
		sets = VGHUBPD_SetPortPowerConfig(portID,0,1,0,SinkMax*1000,1);
		printf("Set the config of port%d   %d Sink\n ",portID,sets);

		
	}
	sleep(1);
	enable1 = VGHUBPD_EnablePort(portID,1);
	printf("enabled Port%d   %d\n",portID,enable1);
	sleep(1);	

 return enable1;
}


int PlugCheck(PORT_ID portID){
	PORT_INFO information[N_PORT];
	VGHUBPD_STATUS getinformation[N_PORT];
	int i,sets=0;

	//ポート情報を取得
	for(i=0;i<N_PORT;i++){
		getinformation[i] = VGHUBPD_GetPortInfo(i, &information[i]);
		if(information[i].PDContract == 1 && information[i].SourceEnabled==1){
			sets = SetPortConfig(i,1,0,60);
			printf("Emergency change the role of %d\n",i);
		}
	}
	return sets;
}


int DecideRole(PORT_ID portID){
	if(portID == prio){
		printf("Port %d is decided Sink",portID);
		return 1;
	}
	else if(sourcetotal< sinktotal *0.8){
		printf("Port %d is decided Source",portID);
		return 0;
	}
	else{
		printf("Port %d is decided Sink",portID);
		return 1;
	}

}

int OverCapa(PORT_ID portID){
	VGHUBPD_STATUS sets;
	PORT_INFO information[N_PORT];
	VGHUBPD_STATUS getinformation[N_PORT];
	int bufsink,newsinkport;
	getinformation[portID] = VGHUBPD_GetPortInfo(portID, &information[portID]);
	
		int nowsource= information[portID].SourceMaxPower/1000;

	
	//printf("Total Source:%dW\nTotal Sink:%dW\nSource of  %d:%dW\n",sourcetotal,sinktotal,portID,nowsource);

	printf("Short of Sink! Plug or Choose New Sink Port\n");
	scanf("%d",&newsinkport);
	if(information[newsinkport].PowerRole==0){
		printf("Change the role of  Port%d\n",newsinkport);
		//sourcetotal -= information[portID].SourceMaxPower;
	}

	//if(0>=newsinkport && newsinkport<N_PORT){
		printf("MAX Sink Power of Port %d?\n",newsinkport);
		scanf("%d", &bufsink);
		//sinktotal += bufsink;
		sets = SetPortConfig(newsinkport,1,0,bufsink);
	//}
	return sets;

}