#include <asf.h>

#include "phy.h"
#include "nwk.h"
#include "config.h"
#include "sys.h"
#include "sio2host.h"
#include "master_i2c.h"

static NWK_DataReq_t appDataReq;
static uint8_t data[]= "Hello Center";
static bool sendBusy = false;


static void radioInit(void);
static bool receivePKT(NWK_DataInd_t *ind);
static void sendDonePKT(NWK_DataReq_t *req);

int main(void)
{
	irq_initialize_vectors();
	system_init();
	delay_init();
	SYS_Init();
	sio2host_init();
	cpu_irq_enable();
	
	printf("Hi this is a station\n");
	
	radioInit();
//	sendPKT();
	
	configure_i2c_master();
	
	while(1)
	{
		SYS_TaskHandler();
	}
}

static void radioInit(void)
{
	NWK_SetAddr(13);
	NWK_SetPanId(APP_PANID);
	PHY_SetChannel(0x1234);
	PHY_SetRxState(true);
	NWK_OpenEndpoint(1, receivePKT);
	
	//sendT.interval = variation;
	//sendT.mode = SYS_TIMER_INTERVAL_MODE;
	//sendT.handler = sendPKT;
	
}

static bool receivePKT(NWK_DataInd_t *ind)
{
	uint16_t sfd = 0;
	uint16_t addr = 0;
	uint8_t act = 0;
	
	uint16_t timeout = 0;
	
	struct i2c_master_packet packet = {
		.address     = SLAVE_ADDRESS,
		.data_length = BUF_LENGTH,
		.data        = &act,
		.ten_bit_address = false,
		.high_speed      = false,
		.hs_master_code  = 0x0,
	};
	
	sfd = ((uint16_t)ind->data[0] << 8) + ind->data[1];
	addr = ((uint16_t)ind->data[2] << 8) + ind->data[3];
	act = ind->data[4];
	printf("sfd: %.2x, addr: %.2x, act: %x\n", sfd, addr, act);
	LED_Toggle(LED0);
	
	if(i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
		printf("data: %x\n", packet.data[0]);
		//if (timeout++ == TIMEOUT) {
			//printf("inside timeout\n");
			//break;
		//}
	}
	
	return true;
}

static void sendDonePKT(NWK_DataReq_t *req)
{
	sendBusy = false;
}
//static void sendPKT(void)
//{
//if(sendBusy)
//return;
//
//appDataReq.dstAddr = 1;
//appDataReq.dstEndpoint = 1;
//appDataReq.srcEndpoint = 1;
//appDataReq.data = data;
//appDataReq.size = 40;
//appDataReq.confirm = sendDonePKT;
//NWK_DataReq(&appDataReq);
//
//LED_Toggle(LED0);
//sendBusy = true;
//
//}
