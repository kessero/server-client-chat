#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <net/if.h>
#include <netdb.h>
#include<unistd.h>
#include<pthread.h>


//pobieranie czasu z systemu
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
//wyswietlanie wiadomosci i odsylanie odp(moznaby to rozdzielic)
const char * printData(char *client_m){
  char message[1999];
  strcpy(message, client_m);
  char *rec_msg =(char *) malloc(sizeof(char) *(200+1));
  char *odpow = "Message receive";
    rec_msg = strcpy(rec_msg, odpow);
    //czas odbioru wiadomosci
    char *t;
    t = time_send();
    printf("Receiving time: %s, Message: %s \n", t, message);
    memset(message, 0, sizeof message);
  return rec_msg;

}
//funkcja zarzadza polaczeniami
void *connection_handler(void *socket_desc){
	int sock = *(int*)socket_desc;
	int read_size;
	char *message , client_message[2000];
  char *odp;
  //wiadomość dla klienta
	message = "Witaj!\n";
	write(sock, message, strlen(message));
	//Odpowiedź klienta
	while( ( read_size = recv(sock , client_message , 2000 ,0))>0 )	{
    for (int i=0; client_message[i]; i++){
     client_message[i] = tolower(client_message[i]);
   }
     odp = printData(client_message);
     write(sock, odp, strlen(odp)+1);
  }
	if(read_size == 0)
	{
		puts("Klient rozłączył się");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("odpowiedź błędna");
	}
	//zwolnić gniazdo
  free(socket_desc);
  free(odp);
  fflush(stdout);
	return 0;
}

int main(int argc, char *argv[]){

	int socket_desc, client_sock, c , *new_sock;
	struct sockaddr_in server , client;
	//tworzymy gniazdo
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Nie mogę utworzyć gniazda");
	}
	puts("Gniazdo utworzone");

	//Struktura socket_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8888);

	//bind
	if( bind(socket_desc,(struct sockaddr *)&server, sizeof(server)) <0)
	{
		perror("Bind fail");
		return 1;
	}
	puts("Bind OK");

	//nasłuch
	listen(socket_desc , 1);
	puts("Czekam na połączenie...");
	c = sizeof(struct sockaddr_in);

	//akceptacja lub odrzucenie połączenia
	while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
		puts("Połączenie zaakceptowane");
    pthread_t sniffer_thread;
		new_sock = malloc(1);
		*new_sock = client_sock;

		if(pthread_create( &sniffer_thread , NULL , connection_handler , (void*) new_sock) < 0)
		{
			perror("Nie udało się utworzyć wątku");
			return 1;
		}
		puts("Utworzono wątek");
    }
	if (client_sock < 0)
	{
		perror("Błąd połączenia");
		return 1;
	}

	return 0;
}
