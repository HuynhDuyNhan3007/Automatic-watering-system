//192.168.43.242
#include <mysql.h>
#include <math.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <errno.h>
#include <string.h>
#include <wiringPiSPI.h>
#include <time.h>
#include <stdint.h>
#include "wiringPi.h"
#include "wiringPiI2C.h"
#include "HTU21D.h"


int dc1,dc2,chance,data1,data2,dl1,dl2;
float nhietdo,doam;
int dem=0;
int day1,month1,year1,hh1,mm1,ss1;

#define pwm1	1
#define pwm2	2
#define duong	4
#define am	5
#define channel	0

void kn()
{
    conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0); 
    mysql_query(conn, "select * from dongco");
    res = mysql_store_result(conn); 
    int num_column = mysql_num_fields(res); 
}
void kn1()
{
	conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0); 
    res = mysql_store_result(conn);

}
void kn2()
{
        conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0); 
    mysql_query(conn, "select * from hengio");
    res = mysql_store_result(conn); 
    int num_column = mysql_num_fields(res);  


}
void kn3()
{
        conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0); 
    mysql_query(conn, "select * from option");
    res = mysql_store_result(conn);
    int num_column = mysql_num_fields(res); 
    

}


void send_data(uint8_t address,uint8_t value)
{
uint8_t data[2];
data[0]=address;
data[1]=value;
wiringPiSPIDataRW(channel,data,2);
}
void Init_max7219(void)
{
send_data(0x09,0xFF);
send_data(0x0A,0x07);
send_data(0x0B,7);
send_data(0x0C,1);
send_data(0x0F,0);
}

int main(void)
{
wiringPiSetup();
wiringPiSPISetup(channel,500000);
Init_max7219();
pinMode(duong,OUTPUT);
pinMode(am,OUTPUT);
softPwmCreate(pwm1,0,100);
softPwmCreate(pwm2,0,100);
digitalWrite(duong,HIGH);
digitalWrite(am,LOW);

while(1)
{
    kn();
    mysql_close(conn);
    row = mysql_fetch_row(res);
    chance=atoi(row[5]);
	int fd=wiringPiI2CSetup(HTU21D_I2C_ADDR);
	nhietdo=getTemperature(fd);
	doam=getHumidity(fd);
    dl1=nhietdo*100;
	dl2=doam*100;
	time_t T = time(NULL);
    struct tm tm = *localtime(&T);
	day1=tm.tm_mday;
	month1=tm.tm_mon+1;
	year1=tm.tm_year + 1900;
	hh1=tm.tm_hour;
	mm1=tm.tm_min;
	ss1=tm.tm_sec;
	conn = mysql_init(NULL);
    	mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    	char sql5[200];
    	mysql_query(conn, sql5);
   	mysql_close(conn);
	send_data(1,((dl2%1000)%100)%10);
	send_data(2,((dl2%1000)%100)/10);
	send_data(3,0x80|(dl2%1000)/100);
	send_data(4,dl2/1000);
	send_data(5,((dl1%1000)%100)%10);
    send_data(6,((dl1%1000)%100)/10);
    send_data(7,0x80|(dl1%1000)/100);
    send_data(8,dl1/1000);
    conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    char sql[200];
    mysql_query(conn, sql);
    char sql2[200];
    mysql_query(conn, sql2);
    mysql_close(conn);

if(chance ==1)
    {
        kn();
	mysql_close(conn);
        row=mysql_fetch_row(res);
	dc1 = atoi(row[1]);
    	dc2 = atoi(row[2]);
        chance=atoi(row[5]);
	softPwmWrite(pwm1,dc1);
        softPwmWrite(pwm2,dc2);
    }

if(chance ==0)
   {
	if(nhietdo>=0 && nhietdo<=25)
	{
		softPwmWrite(pwm1,15);
		data1=15;
	}
	
	if(doam>=80 && doam<=100)
	{
		softPwmWrite(pwm2,0);
		data2=0;
	}
	
	if(nhietdo>=26 && nhietdo<=35)
	{
		softPwmWrite(pwm1,60);
		data1=60;
	}
	
	if(doam>=60 && doam<80)
	{
		softPwmWrite(pwm2,50);
		data2=50;
	}
	
	if(nhietdo>35 && nhietdo<=40)
	{
		softPwmWrite(pwm1,75);
		data1=75;
	}
	
	if(doam>=50 && doam<60)
	{
		softPwmWrite(pwm2,75);
		data2=75;
	}
	
	if(nhietdo>40)
	{
		softPwmWrite(pwm1,100);
		data1=100;
	}
	
	if(doam<50)
	{
		softPwmWrite(pwm2,100);
		data2=100;
	}
		conn = mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    char sql1[200];
    mysql_query(conn, sql1);
    mysql_close(conn);
    }
if(chance==2)
{
	int nd,da,tm;
	printf("chance==2");
	kn1();
        mysql_close(conn);
        row=mysql_fetch_row(res);
        nd = atoi(row[1]);
        da = atoi(row[2]);
        tm = atoi(row[3]);
	if(dl1>=nd*100)
	{
	for (int i=tm;i>=0;i--)
	{
	softPwmWrite(pwm1,100);
	data1=100;
	if(i==0)
	{
	softPwmWrite(pwm1,0);
	}
	conn = mysql_init(NULL);
    	mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    	char sql1[200];
    	mysql_query(conn, sql1);
	char sql2[200];
    	mysql_query(conn, sql2);
    	mysql_close(conn);
	delay(1000);
	kn();
        mysql_close(conn);
        row=mysql_fetch_row(res);
        chance=atoi(row[5]);
	if(chance==0||chance==1||chance==3)
	{
		break;
	}
	}
	}
	if(dl2<=da*100)
        {
	for(int j=tm;j>=0;j--)
	{
        softPwmWrite(pwm2,100);
        data2=100;
	if(j==0)
	{
	softPwmWrite(pwm2,0);
	}
	kn();
        mysql_close(conn);
        row=mysql_fetch_row(res);
        chance=atoi(row[5]);
        if(chance==0||chance==1||chance==3)
        { 
                break;
        }
	conn = mysql_init(NULL);
    	mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    	char sql1[200];
    	mysql_query(conn, sql1);
	char sql2[200];
    	mysql_query(conn, sql2);
    	mysql_close(conn);
	delay(500);
	}
        }
}
if(chance==3)
{
	int day,month,year,hh,mm,ss,tm;
	kn2();
    	mysql_close(conn);
    	row = mysql_fetch_row(res);
    	day=atoi(row[1]);
	month=atoi(row[2]);
	year=atoi(row[3]);
	hh=atoi(row[4]);
	mm=atoi(row[5]);
	ss=atoi(row[6]);
	tm=atoi(row[7]);

	if(day==day1 && month==month1 && year==year1 && hh==hh1 && mm==mm1 && ss==ss1)
	{ 
	for(int i=tm;i>=0;i--)
	{
	softPwmWrite(pwm2,100);
        data2=100;
	if(i==0)
	{
	softPwmWrite(pwm2,0);
	}
	kn();
        mysql_close(conn);
        row=mysql_fetch_row(res);
        chance=atoi(row[5]);
	conn = mysql_init(NULL);
        mysql_real_connect(conn,server,user,password,database,0,NULL,0);
        char sql1[200];
        mysql_query(conn, sql1);
	mysql_close(conn);
        if(chance==0||chance==1||chance==2)
        {
                break;
        }

	delay(1000);
	}
	}
}
if(chance==4)
{
	kn3();
    	mysql_close(conn);
    	row = mysql_fetch_row(res);
	int bien=atoi(row[1]);
    	int thoigian=atoi(row[2]);
	int thoigiancopy=atoi(row[3]);
	if(bien==1)
	{
	if(hh1==6 && mm1==0 && ss1==0)
		{
		for(int i=thoigian;i>=0;i--)
			{
			softPwmWrite(pwm2,100);
        		data2=100;
			conn = mysql_init(NULL);
		        mysql_real_connect(conn,server,user,password,database,0,NULL,0);
        		char sql1[200];
        		mysql_query(conn, sql1);
        		mysql_close(conn);
			kn();
                        mysql_close(conn);
                        row = mysql_fetch_row(res);
                        chance=atoi(row[5]);
                        kn3();
                        mysql_close(conn);
                        row = mysql_fetch_row(res);
                        int bien=atoi(row[1]);
                        if(chance==0 || chance==1 || chance==2 || chance==3 || bien==3 || bien==2)
                        {
                                break;
                        }

			delay(1000);
			if(i==0)
				{
				conn = mysql_init(NULL);
                        	mysql_real_connect(conn,server,user,password,database,0,NULL,0);
                        	char sql1[200];
                        	mysql_query(conn, sql1);
                        	mysql_close(conn);
				break;
				}
			}
		}
	}
	if(bien==2)
        { 
        if(hh1==17 && mm1==0 && ss1==0)
                { 
                for(int i=thoigian;i>=0;i--)
                        { 
                        softPwmWrite(pwm2,100);
                        data2=100;
                        conn = mysql_init(NULL);
                        mysql_real_connect(conn,server,user,password,database,0,NULL,0);
                        char sql1[200];
                        mysql_query(conn, sql1);
                        mysql_close(conn);
                        delay(1000);
			kn();
                        mysql_close(conn);
                        row = mysql_fetch_row(res);
                        chance=atoi(row[5]);
                        kn3();
                        mysql_close(conn);
                        row = mysql_fetch_row(res);
                        int bien=atoi(row[1]);
                        if(chance==0 || chance==1 || chance==2 || chance==3 || bien==1 || bien==3)
                        {
                                break;
                        }

                        if(i==0)
                                { 
                                conn = mysql_init(NULL);
                                mysql_real_connect(conn,server,user,password,database,0,NULL,0);
                                char sql1[200];
                                mysql_query(conn, sql1);
                                mysql_close(conn);
                                break;
                                }
                        }
                }
        }
	if(bien==3)
        {
        if((hh1==6 && mm1==0 && ss1==0) || (hh1==17 && mm1==0 && ss1==0))
                { 
                for(int i=thoigian;i>=0;i--)
                        { 
                        softPwmWrite(pwm2,100);
                        data2=100;
                        conn = mysql_init(NULL);
                        mysql_real_connect(conn,server,user,password,database,0,NULL,0);
                        char sql1[200];
                        mysql_query(conn, sql1);
                        mysql_close(conn);
			kn();
    			mysql_close(conn);
    			row = mysql_fetch_row(res);
    			chance=atoi(row[5]);
			kn3();
        		mysql_close(conn);
        		row = mysql_fetch_row(res);
        		int bien=atoi(row[1]);
			if(chance==0 || chance==1 || chance==2 || chance==3 || bien==1 || bien==2)
			{
				break;
			}
                        delay(1000);
                        if(i==0)
                                { 
                                conn = mysql_init(NULL);
                                mysql_real_connect(conn,server,user,password,database,0,NULL,0);
                                char sql1[200];
                                mysql_query(conn, sql1);
                                mysql_close(conn);
                                break;
                                }
                        }
                }
        }
}
}
    return 0;
}  
