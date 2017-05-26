#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<time.h>
// funkcja pobierajaca czas systemowy
char* time_send(){

  time_t tmt = time(NULL);
  struct tm tm = *localtime(&tmt);
  int hour = tm.tm_hour;
  int min = tm.tm_min;
  int sec = tm.tm_sec;

  char hour_c[15];
  char min_c[15];
  char sec_c[15];
  sprintf(hour_c, "%d", hour);
  sprintf(min_c, "%d", min);
  sprintf(sec_c, "%d", sec);
  char time_all[80];
  strcpy(time_all, hour_c);
  strcat(time_all, ":");
  strcat(time_all, min_c);
  strcat(time_all, ":");
  strcat(time_all, sec_c);
  char *recv_time = malloc(80);
  strcpy(recv_time, time_all);
  return recv_time;
}

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    char nick[20];
    printf("Podaj swój nick: ");
    scanf("%s", nick);
    printf("%s", nick);
    //char *message_send = malloc(sizeof(char) * 1200);
    char message_send[2000];
    char *port =malloc(sizeof(char) *16);
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Nie mogę utworzyć socketa");
    }
    puts("Socket utworzony");
    if(argv[1] != NULL && argv[2] !=NULL){
      port = strcpy(port,argv[2]);
      int result_port = atoi(port);
      printf("%d", result_port);
      server.sin_addr.s_addr = inet_addr(argv[1]);
      server.sin_family = AF_INET;
      server.sin_port = htons( result_port );
      printf("Server IP: %s Port: %s\n", argv[1], argv[2]);
    }else if(argv[2] ==NULL){
      server.sin_addr.s_addr = inet_addr(argv[1]);
      server.sin_family = AF_INET;
      server.sin_port = htons( 8888 );
      printf("Server IP: %s Port 8888\n", argv[1]);
    }else{
      server.sin_addr.s_addr = inet_addr("127.0.0.1");
      server.sin_family = AF_INET;
      server.sin_port = htons( 8888 );
      printf("Server IP: 127.0.0.1 Port:8888\n");
    }


    //polaczenie z serwerem
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("conn. failed. Error");
        return 1;
    }

    puts("Połączony\n");

    //utrzymanie komunikacji z serwerem
    while(1)
    {
        //odbieranie danych
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        if (strcmp(server_reply, "quit")==0){
          close(sock);
          exit(0);
        }
        puts("Serwer :");
        puts(server_reply);
        memset(server_reply, 0, sizeof(server_reply));

        printf("napisz wiadomość: ");
        scanf("%s" , message);

        //wysylanie sformatowanych danych
        strcpy(message_send, nick);
        strcat(message_send, ",");
        strcat(message_send, time_send());
        strcat(message_send, ",");
        strcat(message_send,message);
        if( send(sock , message_send , strlen(message_send)+1 , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        fflush(stdout);
    }
    //zamykanie socketu, sprzatanie
    close(sock);
    free(port);
    free(message_send);
    return 0;
}
