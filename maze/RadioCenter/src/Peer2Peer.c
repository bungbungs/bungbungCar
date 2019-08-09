#include "asf.h"
#include "phy.h"
#include "nwk.h"
#include "config.h"
#include "sys.h"
#include "sio2host.h"

//----------------------------------------------------------------
static NWK_DataReq_t appDataReq;
static uint8_t data[]= "qkqh";
static bool sendBusy = false;

static SYS_Timer_t sendT;

static void sendDonePKT(NWK_DataReq_t *req)
{
	sendBusy = false;
}

static void sendPKT(void)
{
	if(sendBusy)
		return;
		
	appDataReq.dstAddr = 16;
	appDataReq.dstEndpoint = APP_ENDPOINT;
	appDataReq.srcEndpoint = APP_ENDPOINT;
	appDataReq.data = data;
	appDataReq.size = 40;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	LED_Toggle(LED0);
	sendBusy = true;
}

//----------------------------------------------------------------
static bool receivePKT(NWK_DataInd_t* ind)
{
	printf("%s\n", ind->data);
	LED_Toggle(LED0);
	
	return true;
}

void radioInit()
{
	NWK_SetAddr(15);
	NWK_SetPanId(APP_PANID);
	PHY_SetChannel(APP_CHANNEL);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(APP_ENDPOINT, receivePKT);
}

//----------------------------------------------------------------

int main()
{
	irq_initialize_vectors();
	system_init();
	delay_init();
	SYS_Init();
	sio2host_init();
	cpu_irq_enable();
	
	radioInit();
	sendPKT();
	
	while(1){
		SYS_TaskHandler();
	}
	
}