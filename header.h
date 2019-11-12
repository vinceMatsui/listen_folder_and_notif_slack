#ifndef HEADER
#define HEADER
#define MAX 150

struct Slack{
    char * url;
    char * channel;
    char * icon;
    char * username;
    char * text;
} SlackStruct;


int slack_communication(struct Slack slack );
int curl_to_slack(char * url, char * json);
void viderBuffer();
int custom_read(char *chaine, int longueur);
void get_and_test_path(char * path);
void get_and_test_icon(char *icon);
void get_and_test_channel(char *channel);
void get_and_test_url(char * urlSlack);
void get_and_test_user_name(char *username);
int check_url(char *url);


#endif
