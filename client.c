#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

#define MAXLEN 100000

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;
    char **cookies = malloc(sizeof(char*) * MAXLEN);
    for (int i = 0; i < 10; i++) {
        cookies[i] = malloc(MAXLEN * sizeof(char));
    }
    int cookies_count = 0;
    char* strgHeader;   
    char auxCookies[MAXLEN];
    memset(auxCookies, 0, MAXLEN);
    char auxTokens[MAXLEN];
    char username[20];
    char password[20];
    char idBook[20];
    char title[20];
    char author[20];
    char genre[20];
    int page_count;
    char publisher[20];
    int i;
    char **body_data = malloc(MAXLEN * sizeof(char*)); 
    for (i = 0; i < 10; i++) {
        body_data[i] = malloc(MAXLEN * sizeof(char));
    }
    int j = 1;
    char reg[20];
    char *aux2;
  
    while (j != 0) {
        sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
        scanf("%s", reg);
        if (strcmp(reg, "register") == 0) {
            printf("username=");
            scanf("%s", username);
            printf("password=");
            scanf("%s", password);
            sprintf(body_data[0], "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
            message = compute_post_request("34.118.48.238", "/api/v1/tema/auth/register", "application/json",
                body_data , 1, cookies, cookies_count, NULL);
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

        }
    

        if (strcmp(reg, "login") == 0) {
            printf("username=");
            scanf("%s", username); 
            printf("password=");
            scanf("%s", password);
            sprintf(body_data[0], "{\"username\":\"%s\",\"password\":\"%s\"}", username, password);
            message = compute_post_request("34.118.48.238", "/api/v1/tema/auth/login", "application/json",
                body_data , 1, cookies, cookies_count, NULL);
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);

            char *format_cookie;
            char *aux = strstr(response, "connect");
            
            if(aux == NULL) {
                continue;
            }
            char *garbage = strstr(aux, "Date");
            char *cookie = malloc((strlen(aux) - strlen(garbage) + 1) * sizeof(char));
            memcpy(cookie, aux, strlen(aux) - (strlen(garbage) + 1));
            cookie[strlen(cookie) - 1] = '\0';
            
            format_cookie = malloc((8 + strlen(cookie) + 1) * sizeof(char));
            strcpy(format_cookie, "Cookie: ");
            strcat(format_cookie, cookie);

            printf("%s\n", format_cookie);
            cookies[cookies_count] = strdup(format_cookie);
            cookies_count++;
           
        }
        if(strcmp(reg, "enter_library") == 0) {
          

            message = compute_get_request("34.118.48.238", "/api/v1/tema/library/access", NULL, NULL, cookies, cookies_count);
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            char *copyResponse = strdup(response);
            copyResponse = strtok(copyResponse, "{");
            copyResponse[strlen(copyResponse) - 1] = '\0';
            puts(copyResponse);


            char *aux1 = strstr(response, "token");
            if(aux1 == NULL) {
                continue;
            }
            aux1 += 8;
           
            aux2 = strtok(aux1, "\"");
            
            strgHeader = malloc(MAXLEN * sizeof(char));
            strcpy(strgHeader, "Authorization: Bearer ");
            strcat(strgHeader, aux2);
            puts(strgHeader);
        }

        if(strcmp(reg,"get_books") == 0) {
            message = compute_get_request("34.118.48.238", "/api/v1/tema/library/books", NULL, strgHeader, NULL, 0);
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        }

        if(strcmp(reg, "get_book") == 0) {
            printf("id=");
            scanf("%s", idBook);
            char vari[MAXLEN];
            strcpy(vari, "/api/v1/tema/library/books/");
            strcat(vari, idBook);
            message = compute_get_request("34.118.48.238", vari, NULL, strgHeader, NULL, 0);
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        }

        if(strcmp(reg, "add_book") == 0) {
            printf("title=");
            scanf("%s", title);
            printf("author=");
            scanf("%s", author);
            printf("genre=");
            scanf("%s", genre);
            printf("page_count=");
            scanf("%d", &page_count); 
            printf("publisher=");
            scanf("%s", publisher);
            sprintf(body_data[0],
             "{\"title\":\"%s\",\"author\":\"%s\",\"genre\":\"%s\",\"page_count\":\"%d\",\"publisher\":\"%s\"}",
             title, author, genre, page_count, publisher);
            message = compute_post_request("34.118.48.238", "/api/v1/tema/library/books", "application/json",
                body_data , 1, cookies, cookies_count, strgHeader);
        
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        }

        if(strcmp(reg, "delete_book") == 0) {
            printf("id=");
            scanf("%s", idBook);
            char vari[MAXLEN];
            strcpy(vari, "/api/v1/tema/library/books/");
            strcat(vari, idBook);
            message = compute_delete_request("34.118.48.238", vari, NULL, cookies, cookies_count, strgHeader);
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
        } 

        if(strcmp(reg, "logout") == 0) {
            message = compute_get_request("34.118.48.238", "/api/v1/tema/auth/logout", NULL, NULL,cookies, cookies_count);
            puts(message);
            send_to_server(sockfd, message);
            response = receive_from_server(sockfd);
            puts(response);
            close_connection(sockfd);
            for (i = 0 ; i < cookies_count ;i++) {
                cookies[i] = NULL;
            }
            cookies_count = 0;
        }
       
        if(strcmp(reg, "exit") == 0) {
            break;
        }        

    }

    close_connection(sockfd);
    for(i = 0; i < 10 ; i++) {
        free(cookies[i]);
        free(body_data[i]);
    }
    free(cookies);
    free(body_data);
    free(strgHeader);
    return 0;
}
