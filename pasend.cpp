#include <iostream>
#include <stdlib.h>
#include <cstring>

using namespace std;

void start();
void stop();
int restore_vol();
int store_vol();
int get_vol(char *cmd);


int main(int argc, char const *argv[])
{
    if (argc!=2)
    {
        printf("Usage: %s start|stop\n",argv[0]);
        exit(0);
    }

    if (strcmp(argv[1],"start") == 0)
    {
        start();
    }
    else if(strcmp(argv[1],"stop") == 0)
    {
        stop();
    }
    else
    {
        printf("Invalid argument: %s", argv[1]);
    }

    return 0;
}

void start()
{
    char load[] = "pactl load-module module-simple-protocol-tcp rate=48000 format=s16le channels=2 source=alsa_output.pci-0000_00_1b.0.analog-stereo.monitor record=true  port=8000";
    char sink[] = "pactl set-sink-volume 0 1%";
    char srce[] = "pactl set-source-volume 0 125%";
    store_vol();
    system(load);
    system(sink);
    system(srce);
    printf("Started\n");
}

void stop()
{
    char uload[] = "pactl unload-module module-simple-protocol-tcp";
    system(uload);
    restore_vol();
    printf("stopped\n");
}


int restore_vol()
{
    FILE *fp;
    char sink[30];
    char srce[30];
    char reset[100];
    fp = fopen(".pa_data","rb");
    if(fp == NULL)
    {
        printf("Failed to open file for writing\n");
        exit(1);
    }
    char sr_vol = fgetc(fp);
    char sn_vol = fgetc(fp);
    printf("%d:%d\n", sr_vol,sn_vol);
    sprintf(reset,"pactl set-source-volume 0 %d%%;pactl set-sink-volume 0 %d%%;",sr_vol,sn_vol);
    system(reset);
    fclose(fp);
    system("rm .pa_data")
    return 0;
}


int get_vol(char *cmd)
{
    FILE *fp;
    int vol;
    char path[10];
    fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }
    fgets(path, sizeof(path), fp);
    vol = atoi(path);
    pclose(fp);
    return vol;
}

int store_vol()
{
    FILE *fp;
    char cmd1[] = "pactl list sinks | grep '^[[:space:]]Volume:' |     head -n $(( 0 + 1 )) | tail -n 1 | sed -e 's/.* \\([0-9][0-9]*\\)%.*/\\1/'";
    char cmd2[] = "pactl list sources | grep '^[[:space:]]Volume:' |     head -n $(( 0 + 1 )) | tail -n 1 | sed -e 's/.* \\([0-9][0-9]*\\)%.*/\\1/'";
    int sn_vol = get_vol(cmd1);
    int sr_vol = get_vol(cmd2);
    printf("%d:%d\n", sr_vol, sn_vol);
    fp = fopen(".pa_data","wb");
    if(fp == NULL)
    {
        printf("Failed to open file for writing\n");
        exit(1);
    }
    fputc(sr_vol,fp);
    fputc(sn_vol,fp);
    fclose(fp);
    return 0;
}
