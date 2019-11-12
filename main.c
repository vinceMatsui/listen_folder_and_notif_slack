#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <dirent.h>
#include <errno.h>
#include "header.h"
#include "functions.c"

int main(int argc, char *argv[] ){

    int count, i;
    count              = 360;
    int numOfFiles, oldNumOfFiles = 0;
    char * fistPart    = "find ";
    char * seconPart   =  " -type f | wc -l";
    char path[400];
    char urlSlack[300];
    char icon[100];
    char channel[100];
    char username[50];
    char passwordSQL[100];

    get_and_test_path(path);
    printf("\n");
    get_and_test_url(urlSlack);
    printf("\n");
    get_and_test_icon(icon);
    printf("\n");
    get_and_test_channel(channel);
    printf("\n");
    get_and_test_user_name(username);


    printf("\n\n\n-------Commande enregistrée-------\n\n");
    char * fullCommand;
    size_t fullSize       = strlen( fistPart ) + 1 +  strlen( path ) + 1 + strlen(seconPart) + 1;
    fullCommand = (char *) malloc( fullSize );
    strcpy( fullCommand, fistPart );
    strcat( fullCommand, path);
    strcat( fullCommand, seconPart );


    int pid = fork();
    if (pid < 0){
        perror("Echec lors du clonage.");
        exit(-1);
     }
      else if (pid > 0){
            printf("Service lancé sur le PID : %d \n", pid);
            exit(0);
        }
        while(i != count){
            i++;
            sleep(1);
            printf("%s", fullCommand);
            FILE *fp = popen(fullCommand, "r");
            fscanf(fp, "%d", &numOfFiles);
            pclose(fp);
            if((numOfFiles > oldNumOfFiles) && (oldNumOfFiles == 0)){

                struct Slack slack;
                slack.url      = urlSlack;
                slack.text      = malloc(MAX);
                slack.channel  = channel;
                slack.icon     = icon;
                slack.username = username;
                slack.text    = malloc(MAX * sizeof(char));
                char buffer[150];
                int   a = numOfFiles;
                sprintf(buffer, "il y a %d fichiers - première entrée", a);
                strcpy(slack.text, buffer);
                slack_communication(slack);
                oldNumOfFiles = numOfFiles;
                //lecture BDD
                sleep(1);
            }
            else if(numOfFiles < oldNumOfFiles){

                struct Slack slack;
                slack.url      = urlSlack;
                slack.text      = malloc(MAX);
                slack.channel  = channel;
                slack.icon     = icon;
                slack.username = username;
                slack.text    = malloc(MAX * sizeof(char));
                char buffer[150];
                int a =  numOfFiles;
                sprintf(buffer, "il y a %d fichiers -  Un fichier a été supprimé", a);
                strcpy(slack.text, buffer);
                slack_communication(slack);
                oldNumOfFiles = numOfFiles;
                sleep(1);
            }
            else if(numOfFiles > oldNumOfFiles){

                struct Slack slack;
                slack.url      = urlSlack;
                slack.text      = malloc(MAX);
                slack.channel  = channel;
                slack.icon     = icon;
                slack.username = username;
                slack.text    = malloc(MAX * sizeof(char));
                char buffer[150];
                int a =  numOfFiles;
                sprintf(buffer, "il y a %d fichiers -  Un fichier a été ajouté. ", a);
                strcpy(slack.text, buffer);
                slack_communication(slack);
                oldNumOfFiles = numOfFiles;
                //Lecture BDD pour AHB
                sleep(1);
            }
        }
    return 0;
}
