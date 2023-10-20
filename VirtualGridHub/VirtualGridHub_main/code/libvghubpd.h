/////////////////////////////////////////////////////////////////////////////
// VG-Hub USB PD 制御ライブラリヘッダファイル
/////////////////////////////////////////////////////////////////////////////

#include <sys/types.h>
#include <stdint.h>
#include <linux/i2c-dev.h>

/////////////////////////////////////////////////////////////////////////////
// USB-C ポートID定義
/////////////////////////////////////////////////////////////////////////////
typedef enum {
	PORT_ID_UP = 0,
	PORT_ID_DP1 = 1,
	PORT_ID_DP2 = 2,
	PORT_ID_DP3 = 3,
	PORT_ID_DP4 = 4,
	PORT_ID_DP5 = 5,
	PORT_ID_DP6 = 6,
	PORTS_COUNT = 7
} PORT_ID;

/////////////////////////////////////////////////////////////////////////////
// ステータス定義
/////////////////////////////////////////////////////////////////////////////
typedef enum {

	VGHUBPD_STATUS_SUCCESS 						= 0,
	VGHUBPD_STATUS_CMD_PENDING					= 1,

	VGHUBPD_STATUS_LIB_ALREADY_INITIALIZED		= -1,
	VGHUBPD_STATUS_GPIO_INIT_ERROR				= -2,
	VGHUBPD_STATUS_LIB_NOT_INITIALIZED			= -3,
	VGHUBPD_STATUS_LIB_INTERNAL_ERROR			= -4,
	VGHUBPD_STATUS_INVALID_PORT_ID				= -5,
	VGHUBPD_STATUS_INVALID_PARAMETER			= -6,
	VGHUBPD_STATUS_NO_VSYS_POWERED				= -7,
	VGHUBPD_STATUS_I2C_BUS_ERROR				= -8,
	VGHUBPD_STATUS_I2C_NOT_RESPOND				= -9,
	VGHUBPD_STATUS_NO_PLUG						= -10,
	VGHUBPD_STATUS_I2C_IN_PROGRESS				= -11,
	VGHUBPD_STATUS_NOT_PD_CONTRACTED			= -12,
	VGHUBPD_STATUS_NOT_DFP						= -13,
	VGHUBPD_STATUS_NOT_UFP						= -14,
	VGHUBPD_STATUS_NO_VCONN						= -15,
	VGHUBPD_STATUS_NOT_DRP						= -16,
	VGHUBPD_STATUS_PDC_INIT_ERROR				= -17,

	VGHUBPD_STATUS_CMD_IN_PROGRESS				= -100,
	VGHUBPD_STATUS_CMD_TIMEOUT					= -101,
	VGHUBPD_STATUS_CMD_RETRY_TIMEOUT			= -102,
	VGHUBPD_STATUS_CMD_ABORTED					= -103,
	VGHUBPD_STATUS_CMD_INVALID					= -104,
	VGHUBPD_STATUS_PDC_CMD_FAILURE				= -111,
	VGHUBPD_STATUS_PDC_CMD_INVALID				= -112,
	VGHUBPD_STATUS_PDC_CMD_REJECT				= -113,
	VGHUBPD_STATUS_PDC_CMD_BUSY					= -114,
	VGHUBPD_STATUS_PDC_CMD_NO_RESPONSE			= -115,
	VGHUBPD_STATUS_PDC_CMD_UNKNOWN_6			= -116,
	VGHUBPD_STATUS_PDC_CMD_CANCEL				= -117,

} VGHUBPD_STATUS;

/////////////////////////////////////////////////////////////////////////////
// ポートイベント定義
/////////////////////////////////////////////////////////////////////////////
typedef union _port_events{

	uint32_t	eventData;

	struct{
		uint32_t	VsysChanged						: 1;	// bit0
		uint32_t	SMBusError						: 1;	// bit1
		uint32_t	PlugChanged						: 1;	// bit2
		uint32_t	PowerRoleChanged				: 1;	// bit3
		uint32_t	DataRoleChanged					: 1;	// bit4
		uint32_t	PDContracted					: 1;	// bit5
		uint32_t	NonePDDetected					: 1;	// bit6
		uint32_t	HardResetHappened				: 1;	// bit7
		uint32_t	SoftResetHappened				: 1;	// bit8
		uint32_t	SourceCapsReceived				: 1;	// bit9
		uint32_t	SinkCapsReceived				: 1;	// bit10
		uint32_t	FarEndDiscoverIDReceived		: 1;	// bit11
		uint32_t	CablePlugDiscoverIDReceived		: 1;	// bit12
		uint32_t	PowerConfigCompleted			: 1;	// bit13
		uint32_t	SinkVsysOputputVoltageChanged	: 1;	// bit14
		uint32_t	PortEnabledDisabled				: 1;	// bit15
		uint32_t	_rsv_16_						: 1;	// bit16
		uint32_t	_rsv_17_						: 1;	// bit17
		uint32_t	_rsv_18_						: 1;	// bit18
		uint32_t	_rsv_19_						: 1;	// bit19
		uint32_t	_rsv_20_						: 1;	// bit20
		uint32_t	_rsv_21_						: 1;	// bit21
		uint32_t	_rsv_22_						: 1;	// bit22
		uint32_t	_rsv_23_						: 1;	// bit23
		uint32_t	_rsv_24_						: 1;	// bit24
		uint32_t	_rsv_25_						: 1;	// bit25
		uint32_t	_rsv_26_						: 1;	// bit26
		uint32_t	_rsv_27_						: 1;	// bit27
		uint32_t	_rsv_28_						: 1;	// bit28
		uint32_t	OTP								: 1;	// bit29
		uint32_t	OCP								: 1;	// bit30
		uint32_t	OVP								: 1;	// bit31
	}event;

}PORT_EVENT_BITMAP, *PPORT_EVENT_BITMAP;

#define PORT_EVENT_FLAG_VSYS_CHANGED						(1UL <<  0)
#define PORT_EVENT_FLAG_SMBUS_ERROR							(1UL <<  1)
#define PORT_EVENT_FLAG_PLUG_CHANGED						(1UL <<  2)
#define PORT_EVENT_FLAG_POWER_ROLE_CHANGED					(1UL <<  3)
#define PORT_EVENT_FLAG_DATA_ROLE_CHANGED					(1UL <<  4)
#define PORT_EVENT_FLAG_PD_CONTRACTED						(1UL <<  5)
#define PORT_EVENT_FLAG_NONE_PD_DEVICE						(1UL <<  6)
#define PORT_EVENT_FLAG_HARD_RESET_HAPPENED					(1UL <<  7)
#define PORT_EVENT_FLAG_SOFT_RESET_HAPPENED					(1UL <<  8)
#define PORT_EVENT_FLAG_SRC_CAPS_RECEIVED					(1UL <<  9)
#define PORT_EVENT_FLAG_SINK_CAPS_RECEIVED					(1UL << 10)
#define PORT_EVENT_FLAG_FAR_END_DISCOVER_ID_RECEIVED		(1UL << 11)
#define PORT_EVENT_FLAG_CABLE_PLUG_DISCOVER_ID_RECEIVED		(1UL << 12)
#define PORT_EVENT_FLAG_POWER_CONFIG_COMPLETED				(1UL << 13)
#define PORT_EVENT_FLAG_SINK_VSYS_OUTPUT_VOLTAGE_CHANGED	(1UL << 14)
#define PORT_EVENT_FLAG_PORT_ENABLED_DISABLED				(1UL << 15)
#define PORT_EVENT_FLAG_OTP									(1UL << 29)
#define PORT_EVENT_FLAG_OCP									(1UL << 30)
#define PORT_EVENT_FLAG_OVP									(1UL << 31)


/////////////////////////////////////////////////////////////////////////////
// コールバック関数定義
/////////////////////////////////////////////////////////////////////////////
typedef void (*PCALLBACK)(PORT_ID portID, PORT_EVENT_BITMAP bmPortEvents);

/////////////////////////////////////////////////////////////////////////////
// USB-C ポート情報定義
/////////////////////////////////////////////////////////////////////////////

#define PORT_CONNECT_UNPLUG		0
#define PORT_CONNECT_PLUG		1

#define POWER_ROLE_SINK			0
#define POWER_ROLE_SOURCE		1

#define DATA_ROLE_UFP			0
#define DATA_ROLE_DFP			1

#define PD_CONTRACT_NONE		0
#define PD_CONTRACT_DONE		1

#define NONE_PD					1

#define MAX_PDO		7	// 最大PDO数
#define MAX_VDO		7	// 最大VDO数

#define vRd_30		2	// vRd-30 (Type-C Current 3.0[A])
#define vRd_15		1	// vRd-15 (Type-C Current 1.5[A])
#define vRd_USB		0	// vRd-USB (Type-C Current 0.5[A])

#pragma pack(push)
#pragma pack(1)
typedef struct _port_info{

	// ポート番号
	uint8_t		PortID;

	// PDCコントローラの状態
	uint8_t		PDCPower;
	uint8_t		SMBusError;

	// ポートの設定状態
	uint16_t	SinkVsysOutVoltage;
	uint8_t		PortEnabled;
	uint8_t		SourceEnabled;
	uint8_t		SinkEnabled;
	uint8_t		UFPEnabled;
	uint8_t		DFPEnabled;
	uint32_t	SourceMaxPower;
	uint32_t	SinkMaxPower;

	// ポートの現在の状態
	uint8_t		PortConnect;
	uint8_t		PowerRole;
	uint8_t		DataRole;
	uint8_t		VCONNState;
	uint8_t		PDContract;
	uint8_t		NonePD;
	uint8_t		vRd;

	// USB PDネゴシエーション情報
	uint32_t	CurrentPDO;
	uint32_t	CurrentRDO;

	// Far End Port の 情報
	uint8_t		FarEndSourcePDOLength;
	uint32_t	FarEndSourcePDOs[MAX_PDO];

	uint8_t		FarEndSinkPDOLength;
	uint32_t	FarEndSinkPDOs[MAX_PDO];

	uint8_t		FarEndDiscoverIDLength;
	uint32_t	FarEndDiscoverID[MAX_VDO];

	// Cable Plug の 情報
	uint8_t		CablePlugDiscoverIDLength;
	uint32_t	CablePlugDiscoverID[MAX_VDO];

}PORT_INFO, *PPORT_INFO;
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// USB-C ポート VBUS 電力情報定義
/////////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
typedef struct _port_vbus_info{

	uint16_t	Voltage;
	int16_t		Current;

}PORT_VBUS_INFO, *PPORT_VBUS_INFO;
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// Source/Sink Capabilities 情報
/////////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
typedef struct _pdos{

	uint8_t		Length;

	uint32_t	PDO[MAX_PDO];

}PDO_INFO, *PPDO_INFO;
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// Discover Identity 情報
/////////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
typedef struct _discover_id{

	uint8_t			Length;

	struct{
		uint32_t	VDMHeader;
		uint32_t	IDHeaderVDO;
		uint32_t	CertStatVDO;
		uint32_t	ProductVDO;
		uint32_t	ProductTypeVDO[3];
	}VDO;

}DISCOVER_ID_INFO, *PDISCOVER_ID_INFO;
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// Manufacturer Info Data Block 情報
/////////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
typedef struct _manu_info_db{

	uint8_t		Length;

	uint16_t	VID;
	uint16_t	PID;
	uint8_t		ManufacturerString[22];

}MIDB_INFO, *PMIDB_INFO;
#pragma pack(pop)

/////////////////////////////////////////////////////////////////////////////
// DC-DC 内部出力電圧(Vsys)定義
/////////////////////////////////////////////////////////////////////////////
#define MIN_VSYS_VOLTAGE		18672U		// 最低Vsys電圧[mV]
#define MAX_VSYS_VOLTAGE		19344U		// 最高Vsys電圧[mV]
#define DEFAULT_VSYS_VOLTAGE	19008U		// デフォルトVsys電圧[mV]
#define VSYS_VOLTAGE_MIN_UNIT	12U			// Vsys電圧設定最小単位[mV]

/////////////////////////////////////////////////////////////////////////////
// LED番号定義
/////////////////////////////////////////////////////////////////////////////
typedef enum {
	LED_INDEX_UP = 0,
	LED_INDEX_DP1 = 1,
	LED_INDEX_DP2 = 2,
	LED_INDEX_DP3 = 3,
	LED_INDEX_DP4 = 4,
	LED_INDEX_DP5 = 5,
	LED_INDEX_DP6 = 6,
	LED_INDEX_DBG = 7,
	LED_COUNT = 8
} LED_INDEX;

// LED表示色定義
typedef enum{
	LED_COLOR_NONE		= 0x7,	// 111b (消灯)
	LED_COLOR_RED		= 0x6,	// 110b
	LED_COLOR_GREEN		= 0x5,	// 101b
	LED_COLOR_BLUE		= 0x3,	// 011b
	LED_COLOR_YELLOW	= 0x4,	// 100b
	LED_COLOR_MAGENDA	= 0x2,	// 010b (ピンク)
	LED_COLOR_CYAN		= 0x1,	// 001b (水色)
	LED_COLOR_WHITE		= 0x0	// 000b
} LED_COLOR;

/////////////////////////////////////////////////////////////////////////////
// 各API定義
/////////////////////////////////////////////////////////////////////////////
extern uint16_t			VGHUBPD_GetLibVersion(void);

extern VGHUBPD_STATUS	VGHUBPD_Init(void);
extern VGHUBPD_STATUS	VGHUBPD_Exit(void);

extern VGHUBPD_STATUS	VGHUBPD_SetEventNotificationCallback(PCALLBACK pFuncEventNotify);

extern VGHUBPD_STATUS	VGHUBPD_EnablePort(PORT_ID portID, uint8_t bEnable);

extern VGHUBPD_STATUS	VGHUBPD_GetPortInfo(PORT_ID portID, PPORT_INFO pPortInfo);
extern VGHUBPD_STATUS	VGHUBPD_GetPortVbusInfo(PORT_ID portID, PPORT_VBUS_INFO pPortVbusInfo);

extern VGHUBPD_STATUS	VGHUBPD_SetPortPowerConfig(PORT_ID portID, uint8_t bSourceEnable, uint8_t bSinkEnable, uint32_t SourceMaxPower, uint32_t SinkMaxPower, uint8_t bWithPDContractWaiting);
extern VGHUBPD_STATUS	VGHUBPD_SetSinkVsysOutputVoltage(PORT_ID portID, uint16_t Voltage);

extern VGHUBPD_STATUS	VGHUBPD_SendVconnSwap(PORT_ID portID);
extern VGHUBPD_STATUS	VGHUBPD_SendPRSwap(PORT_ID portID, uint8_t bWithPDContractWaiting);

extern VGHUBPD_STATUS	VGHUBPD_SendGetSourceCapabilities(PORT_ID portID, PPDO_INFO pSourcePDOs);
extern VGHUBPD_STATUS	VGHUBPD_SendGetSinkCapabilities(PORT_ID portID, PPDO_INFO pSinkPDOs);
extern VGHUBPD_STATUS	VGHUBPD_GetFarEndPortDiscoverID(PORT_ID portID, PDISCOVER_ID_INFO pDiscoverID);
extern VGHUBPD_STATUS	VGHUBPD_GetCablePlugDiscoverID(PORT_ID portID, PDISCOVER_ID_INFO pDiscoverID);
extern VGHUBPD_STATUS	VGHUBPD_GetFarEndPortManuInfo(PORT_ID portID, PMIDB_INFO pMIDB);

extern VGHUBPD_STATUS	LEDCtrl_SetColor(LED_INDEX ledIndex, LED_COLOR color);

