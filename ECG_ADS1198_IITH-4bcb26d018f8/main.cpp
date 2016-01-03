#include "mbed.h"
#include "ADSlib/ADSlib.h"
#include "SDFileSystem/SDFileSystem.h"

/***********************PROTOTYPING AND SETTINGS*********************/
DigitalOut CLKSEL(p8);
DigitalOut ADS1CS(p16);
DigitalOut ADS2CS(p17);
DigitalIn MULT_ADS(p18);
DigitalOut PWDN(p10);
DigitalOut RESETpin(p9);
DigitalOut STARTpin(p19);
DigitalOut waitled(LED1);
DigitalIn DRDY(p15);
SPI spi(p5,p6,p7);
SDFileSystem sd(p11,p12,p13,p14,"sd");
signed char Input_Data_Stream[];
uint16_t Input_modified[];
LocalFileSystem local("local");
/********************************************************************/

int main()                          // main call
{
    /********************POWER ON SEQUENCING *****************************/
    waitled =1;
    wait_ms(500);                   //boot up time

    ADS1CS=1;                       //deselect ADS1
    spi.format(8,3);                //spi setttings
    spi.frequency(1000000);         //spi settings

    CLKSEL = 1;                     //clock signal (internal) setup Frequency = 2.048 MHz
    ADS1CS= 0;
    spi.write(W_CONFIG1);
    spi.write(0x00);
    spi.write(0x64);
    ADS1CS =1;

    PWDN = 1;
    RESETpin = 1;
    wait(1);                        //Delay for Power-On Reset and Oscillator Start-Up

    ADS1CS= 0;                      // intial reset command for device reset and register setting clean up
    spi.write(RESET);
    pause_clk(18);


    spi.write(SDATAC);              // device boots up in RDATAC mode set it in SDATAC mode
    pause_clk(4);


    spi.write(W_CONFIG3);           // internal reference stup and enable
    spi.write(0x00);
    spi.write(0xC0);

    /***************POWER ON SEQUENCING COMPLETE**************************/


    /**************CHANNEL SETTINGS***************************************/
    spi.write(W_CHnSET);
    spi.write(0x07);
    spi.write((int)ADS_Default_Channel_Settings);
    ADS1CS = 1;
    /*********************************************************************/

    /*****************************CONVERSION BEGINS**********************/
    STARTpin= 1;

    //write main superloop and rdatac mode and enable dout conversion AND SETUP FILESTORAFGE
    Timer tensec;
    tensec.start();
    
    
    
    
    if (MULT_ADS = 0) {
        while(tensec.read()<=10) {
            spi.write(RDATAC);
            wait_ms(10);
            while(DRDY) {}
            ADS1CS= 0;
            for (int i=0; i<=18; i++) {
                Input_Data_Stream[i] = spi.write(0x00);
            }
            int k= 0;
            int j= 4;
            for(int i=3; i<=17; i++) {
                Input_modified[k]=Input_Data_Stream[i]<<8 + Input_Data_Stream[j];
                j++;
                k++;
            }
            Input_modified[8]=Input_modified[7]-Input_modified[6];
            Input_modified[9]=-(Input_modified[7]+Input_modified[6]/2);
            Input_modified[10]=((2*Input_modified[6])-Input_modified[7])/2;
            Input_modified[11]=((2*Input_modified[7])-Input_modified[6])/2;
            spi.write(SDATAC);

            /***********************FILE IO PROCEDURE ON LOCAL STORAGE******************/
            FILE* file = fopen("local/logfile.txt","w");
            for (int k =0; k<12; k++) {
                fputc(Input_modified[k],file);
            }
            //dont forget to
            //fclose(file);
            /***************************************************************************/


///////////////////////////OR///////////////////////////////////////////////////////////////////////


            /****************************SD I/O OPERATIONS *****************************/
            mkdir("sd/logfiledir",0777);
            FILE* file1 = fopen("sd/logfiledir/logfile.txt","w");
            for (int k =0; k<12; k++) {
                fputc(Input_modified[k],file1);
            }
            //dont forget to
            //fclose(file1);
            /***************************************************************************/
        }
    }
    
    
     if (MULT_ADS = 0) {
        while(tensec.read()<=10) {
            spi.write(RDATAC);
            wait_ms(10);
            while(DRDY) {}
            ADS1CS= 0;
            for (int i=0; i<=18; i++) {
                Input_Data_Stream[i] = spi.write(0x00);
            }
            int k= 0;
            int j= 4;
            for(int i=3; i<=17; i++) {
                Input_modified[k]=Input_Data_Stream[i]<<8 + Input_Data_Stream[j];
                j++;
                k++;
            }
            Input_modified[8]=Input_modified[7]-Input_modified[6];
            Input_modified[9]=-(Input_modified[7]+Input_modified[6]/2);
            Input_modified[10]=((2*Input_modified[6])-Input_modified[7])/2;
            Input_modified[11]=((2*Input_modified[7])-Input_modified[6])/2;
            spi.write(SDATAC);

            /***********************FILE IO PROCEDURE ON LOCAL STORAGE******************/
            FILE* file = fopen("local/logfile.txt","w");
            for (int k =0; k<12; k++) {
                fputc(Input_modified[k],file);
            }
            //dont forget to
            //fclose(file);
            /***************************************************************************/


///////////////////////////OR///////////////////////////////////////////////////////////////////////


            /****************************SD I/O OPERATIONS *****************************/
            mkdir("sd/logfiledir",0777);
            FILE* file1 = fopen("sd/logfiledir/logfile.txt","w");
            for (int k =0; k<12; k++) {
                fputc(Input_modified[k],file1);
            }
            //dont forget to
            //fclose(file1);
            /***************************************************************************/
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    tensec.stop();
    waitled =0;
}



