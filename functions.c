#ifndef FUNCTIONS
#define FUNCTIONS


int slack_communication(struct Slack slack ){
    if(strlen(slack.icon) < 3){
        slack.icon = ":dossier_ouvert:";
    }

    char  *out;
    cJSON *root;
    root  = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "channel", cJSON_CreateString(slack.channel));
    cJSON_AddItemToObject(root, "icon", cJSON_CreateString(slack.icon));
    cJSON_AddItemToObject(root, "username", cJSON_CreateString(slack.username));
    cJSON_AddItemToObject(root, "text", cJSON_CreateString(slack.text));
    out = cJSON_Print(root);

    curl_to_slack(slack.url, out);

    return 0;
}


int curl_to_slack(char * url, char * json){

  CURL *curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();

  if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
      res = curl_easy_perform(curl);

    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    printf("ok\n");
  }

  curl_global_cleanup();
  return 0;
}


void viderBuffer(){
    int c = 0;
    while (c != '\n' && c != EOF){
        c = getchar();
    }
}

int custom_read(char *chaine, int longueur){
    char *positionEntree = NULL;

    if (fgets(chaine, longueur, stdin) != NULL){
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL){
            *positionEntree = '\0';
        }
        else{
            viderBuffer();
        }
        return 1;
    }
    else{
        viderBuffer();
        return 0;
    }
}

void get_and_test_path(char * path){

  int   confirmation = 0;

  while(confirmation == 0){
    printf("Quel est est le chemin du dossier à surveiller ? : \n ");
    custom_read(path, 400);
    if(strlen(path) > 1){
          if(strlen(path) < 399){
            if (strstr(path, ";") != NULL) {
              printf("Erreur, caractère interdit\n");
           }
           else if(strstr(path, "#") != NULL){
             printf("Erreur, caractère interdit\n");
           }
           else{
             DIR* dir = opendir(path);
            if (dir) {
                confirmation = 1;
                closedir(dir);
             }else if (ENOENT == errno) {
               printf("Erreur, le chemin ne semble pas exister\n");
             }else {
               printf("Erreur, le chemin soulève une erreur inconnue\n");
             }
           }
         }else{
           printf("Erreur, le chemin est trop long pour le programme\n");
         }
    }else{
          printf("Erreur pas de chemin spécifié\n");
    }
  }
}

void get_and_test_url(char * urlSlack){
  int   confirmation = 0;

  while(confirmation == 0){
    printf("Quel est est l'url de l'API Slack ? : \n ");
    custom_read(urlSlack, 300);
    if(strlen(urlSlack) > 1){
          if(strlen(urlSlack) < 299){
            int boolean = check_url(urlSlack);
            if(boolean == 1){
              confirmation =  1;
            }else{
              printf("L'Url fournie ne semble pas être fonctionnelle\n");
            }
         }else{
           printf("Erreur, l'url est trop longue pour le programme\n");
         }
    }else{
           printf("Erreur pas d'url valide entrée\n");
    }
  }
}



void get_and_test_icon(char *icon){
  int   confirmation = 0;

  while(confirmation == 0){
  printf("Quel icône sera utilisé sur l'API Slack ? : \n ");
  printf("exemple : :collision: :anger: :exclamation: :question: :grey_exclamation: \n");
  custom_read(icon, 100);
  if(strlen(icon) > 1){
        if(strlen(icon) < 99){
          confirmation = 1;
       }else{
         printf("Erreur, la taille de l'icon est trop longue pour le programme\n");
       }
  }else{
         printf("Erreur pas d'icon valide entré\n");
    }
  }
}


void get_and_test_channel(char *channel){
  int   confirmation = 0;

  while(confirmation == 0){
  printf("Sur quel Channel sera envoyé les notifications ? : \n ");
  custom_read(channel, 100);
  if(strlen(channel) > 1){
        if(strlen(channel) < 99){
          confirmation = 1;
       }else{
         printf("Erreur, la taille du channel  est trop longue pour le programme\n");
       }
  }else{
         printf("Erreur pas de nom de channel valide entré\n");
    }
  }
}

void get_and_test_user_name(char *username){

  int   confirmation = 0;

  while(confirmation == 0){
  printf("Sur quel username sera envoyé les notifications ? : \n ");
  custom_read(username, 50);
  if(strlen(username) > 1){
        if(strlen(username) < 49){
          confirmation = 1;
       }else{
         printf("Erreur, la taille de l'username  est trop longue pour le programme\n");
       }
  }else{
         printf("Erreur pas de nom d'username valide entré\n");
    }
  }
}



int check_url(char *url){
    CURL *curl;
    CURLcode response;
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
        response = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return (response == CURLE_OK) ? 1 : 0;
}

#endif
