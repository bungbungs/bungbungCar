#include <asf.h>
#include "asf.h"
#include "phy.h"
#include "nwk.h"
#include "config.h"
#include "sys.h"
#include "sio2host.h"


static NWK_DataReq_t appDataReq;
static uint8_t data[5];
static bool sendBusy = false;

static void sendDonePKT(NWK_DataReq_t *req)
{
	sendBusy = false;
}

static void sendPKT(void)
{
	if(sendBusy)
		return;
	
	appDataReq.dstAddr = 0xffff;
	appDataReq.dstEndpoint = 1;
	appDataReq.srcEndpoint = 1;
	appDataReq.data = data;
	appDataReq.size = 40;
	appDataReq.confirm = sendDonePKT;
	NWK_DataReq(&appDataReq);
	
	//LED_Toggle(LED0);
	sendBusy = true;
	
}
static bool receivePKT(NWK_DataInd_t *ind)
{
	printf("%s\n", ind->data);
	//LED_Toggle(LED0);
	
	return true;
}

static void radioInit(void)
{
	NWK_SetAddr(1);
	NWK_SetPanId(0xffff);
	PHY_SetChannel(0x1234);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(1, receivePKT);
	
	//sendT.interval = variation;
	///sendT.mode = SYS_TIMER_INTERVAL_MODE;
	//sendT.handler = sendPKT;
	
}

int main(void)
{
	irq_initialize_vectors();
	system_init();
	delay_init();
	SYS_Init();
	sio2host_init();
	cpu_irq_enable();
	
	printf("Hi this is the center\n");
	
	data[0] = 0x00;
	data[1] = 0x01;
	
	data[2] = 0x00;
	data[3] = 0x5f;
	
	data[4] = 0x01;
	
	radioInit();

	while(1)
	{
		SYS_TaskHandler();
		sendPKT();
		data[4]++;
		
	}
}