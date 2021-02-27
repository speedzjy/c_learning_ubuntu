#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"

#define EC_TIMEOUTMON 500

char IOmap[4096];
// soem里定义：
// #define OSAL_THREAD_HANDLE pthread_t *
OSAL_THREAD_HANDLE thread1;
int expectedWKC;
// typedef uint8_t             boolean;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;

uint16 control;
int16 speed;
uint16 control_word;
uint16 state;
int32 curr_position;
int32 position;
int32 position2;
int add_position;
int i;
int ret;
char run = 1;


#define READ(slaveId, idx, sub, buf, comment)    \
    {   \
        buf=0;  \
        int __s = sizeof(buf);    \
        int __ret = ec_SDOread(slaveId, idx, sub, FALSE, &__s, &buf, EC_TIMEOUTRXM);   \
        printf("Slave: %d - Read at 0x%04x:%d => wkc: %d; data: 0x%.*x (%d)\t[%s]\n", slaveId, idx, sub, __ret, __s, (unsigned int)buf, (unsigned int)buf, comment);    \
     }

#define WRITE(slaveId, idx, sub, buf, value, comment) \
    {   \
        int __s = sizeof(buf);  \
        buf = value;    \
        int __ret = ec_SDOwrite(slaveId, idx, sub, FALSE, __s, &buf, EC_TIMEOUTRXM);  \
        printf("Slave: %d - Write at 0x%04x:%d => wkc: %d; data: 0x%.*x\t{%s}\n", slaveId, idx, sub, __ret, __s, (unsigned int)buf, comment);    \
    }

#define CHECKERROR(slaveId)   \
{   \
    ec_readstate();\
    printf("EC> \"%s\" %x - %x [%s] \n", (char*)ec_elist2string(), ec_slave[slaveId].state, ec_slave[slaveId].ALstatuscode, (char*)ec_ALstatuscode2string(ec_slave[slaveId].ALstatuscode));    \
}

//  函数声明
void slavetop(int i);
void simpletest(char *ifname);
OSAL_THREAD_FUNC ecatcheck();

int main(int argc, char *argv[])
{
    printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");

	if (argc > 1){   
        // osal : operation system abstract layer 操作系统抽象层   
		osal_thread_create(&thread1, 128000, &ecatcheck, (void*) &ctime);
		simpletest(argv[1]);
	}
	else{
		printf("Usage: simple_test ifname1\nifname = eth0 for example\n");
	}   

	printf("End program\n");
	return (0);
}

// 使从站进入op状态
void slavetop(int i)
{
	ec_slave[i].state = EC_STATE_OPERATIONAL;
	ec_send_processdata();
	ec_receive_processdata(EC_TIMEOUTRET);
	ec_writestate(0);
}


void simpletest(char *ifname)
{
	int chk;
	uint32 buf32;
    uint16 buf16;
    uint8 buf8;

     /* initialise SOEM, bind socket to ifname */  
     // ifname 接口
	if(ec_init(ifname))
	{
		printf("start ethernet at %s\n",ifname);
		if ( ec_config_init(FALSE) > 0 )
		{
			printf("found %d slave on the bus\n",ec_slavecount);
			ec_config_map(&IOmap);
			ec_configdc();
			for(i=0;i<ec_slavecount;i++)
			{
				printf("slave%d to op\n", i);
				slavetop(i);
			}
			// set operation mode 
			for (i=1; i<=ec_slavecount; i++) 
			{
				WRITE(i, 0x6060, 0, buf8, 3, "OpMode");
				READ(i, 0x6061, 0, buf8, "OpMode display");
				
				//READ(i, 0x6065, 0, buf32, "Following error window");
				//READ(i, 0x607F, 0, buf32, "Max profile velocity");
				
			}
			
			
			printf("Request operational state for all slaves\n");
            expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
            printf("Calculated workcounter %d\n", expectedWKC);
            
            /* request OP state for all slaves */
            ec_writestate(0);
            chk = 40;
            /* wait for all slaves to reach OP state */
            do
            {
                ec_send_processdata();
                ec_receive_processdata(EC_TIMEOUTRET);
                ec_statecheck(0, EC_STATE_OPERATIONAL, 50000);
            }
            while (chk-- && (ec_slave[0].state != EC_STATE_OPERATIONAL));
            
			if(ec_slave[0].state == EC_STATE_OPERATIONAL)
			{
				printf("Operational state reached for all slaves.\n");
                inOP = TRUE;
                /**
                 * Drive state machine transistions
                 *   0 -> 6 -> 15
                 */
                for (i=1; i<=ec_slavecount; i++) 
                {
                	WRITE(i, 0x6040, 0, buf16, 0x0080, "control word");
					usleep(100000);
                    READ(i, 0x6041, 0, buf16, "status word");
                    
                    WRITE(i, 0x6040, 0, buf16, 0x0006, "control word");
					usleep(100000);
                    READ(i, 0x6041, 0, buf16, "status word");
                    
                    WRITE(i, 0x6040, 0, buf16, 0x000F, "control word");
					usleep(100000);
                    READ(i, 0x6041, 0, buf16, "status word");
                    
                    WRITE(i, 0x60FF, 0, buf32, 500, "Target velocity");
					usleep(100000);
					
					WRITE(i, 0x6040, 0, buf16, 0x000F, "control word");
					usleep(100000);
                    READ(i, 0x6041, 0, buf16, "status word");
					READ(i, 0x1001, 0, buf8, "Error");
                    
                }
                
              
                
				for(i = 1; i <= 10000; i++) 
				{
					
					
					// PDO I/O refresh 
					/*WRITE(1, 0x60FF, 0, buf32, 5000, "Target velocity");
					usleep(100);

					WRITE(1, 0x6040, 0, buf16, 0x007F, "control word");
					usleep(100);
                    READ(1, 0x6041, 0, buf16, "status word");
					ec_send_processdata();
					wkc = ec_receive_processdata(EC_TIMEOUTRET);*/
					usleep(1000);// 周期大小
				}
				WRITE(1, 0x6040, 0, buf16, 0x0006, "control word");
			}
			else
			{
				printf("slave again to op\n");
			}
		}
		else
		{
			printf("no slave on the bus\n");
		}
	}
	else
	{
		printf("no ethernet card\n");
	}
}

OSAL_THREAD_FUNC ecatcheck()
{
    int slave;

    while(1)
    {
        if( inOP && ((wkc < expectedWKC) || ec_group[currentgroup].docheckstate))
        {
            if (needlf)
            {
               needlf = FALSE;
               printf("\n");
            }
            /* one ore more slaves are not responding */
            ec_group[currentgroup].docheckstate = FALSE;
            ec_readstate();
            for (slave = 1; slave <= ec_slavecount; slave++)
            {
               if ((ec_slave[slave].group == currentgroup) && (ec_slave[slave].state != EC_STATE_OPERATIONAL))
               {
                  ec_group[currentgroup].docheckstate = TRUE;
                  if (ec_slave[slave].state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
                  {
                     printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slave);
                     ec_slave[slave].state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                     ec_writestate(slave);
                  }
                  else if(ec_slave[slave].state == EC_STATE_SAFE_OP)
                  {
                     printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slave);
                     ec_slave[slave].state = EC_STATE_OPERATIONAL;
                     ec_writestate(slave);                              
                  }
                  else if(ec_slave[slave].state > 0)
                  {
                     if (ec_reconfig_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d reconfigured\n",slave);                           
                     }
                  } 
                  else if(!ec_slave[slave].islost)
                  {
                     /* re-check state */
                     ec_statecheck(slave, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                     if (!ec_slave[slave].state)
                     {
                        ec_slave[slave].islost = TRUE;
                        printf("ERROR : slave %d lost\n",slave);                           
                     }
                  }
               }
               if (ec_slave[slave].islost)
               {
                  if(!ec_slave[slave].state)
                  {
                     if (ec_recover_slave(slave, EC_TIMEOUTMON))
                     {
                        ec_slave[slave].islost = FALSE;
                        printf("MESSAGE : slave %d recovered\n",slave);                           
                     }
                  }
                  else
                  {
                     ec_slave[slave].islost = FALSE;
                     printf("MESSAGE : slave %d found\n",slave);                           
                  }
               }
            }
            if(!ec_group[currentgroup].docheckstate)
               printf("OK : all slaves resumed OPERATIONAL.\n");
        }
        usleep(10000);
    }   
}   


