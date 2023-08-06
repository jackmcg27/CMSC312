#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

 
#define PORT 3456
 
int main()
{
	int sock1,sock2, clength;
	sock1 =  socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serv,cli;
 
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PORT);
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	bind(sock1,(struct sockaddr *)&serv, sizeof(serv));
	listen(sock1,5);
	clength = sizeof(cli);
	int i=0;
	char buf[50];
    while(1){
	    sock2 = accept(sock1,(struct sockaddr *)&cli, (socklen_t *)&clength);
        cout << "\n CLIENT CONNECTED" << endl;
        char hold[10];
        int numElements;
        double elementsList[100];
        double temp;
        read(sock2, hold, sizeof(int));
        numElements = stoi(hold);
        
        for (int i = 0; i < numElements; i++){
            read(sock2, hold, sizeof(double));
            elementsList[i] = stod(hold);
            cout << "Got element " << elementsList[i] << endl;
        }

        for (int i = 0; i < numElements; i++) {
            for (int j = i+1; j < numElements; j++) {
                if (elementsList[i] > elementsList[j] ) {
                    temp = elementsList[i];
                    elementsList[i] = elementsList[j];
                    elementsList[j] = temp;
                }
            }
        }
        char holdString[10];
        for (int i = 0; i < numElements; i++) {
            sprintf(holdString,"%f", elementsList[i]);
            write(sock2, holdString, sizeof(double));
        }
        cout << "\nReturned sorted list" << endl;
    }
    close(sock2);
    close(sock1);
	return 0;
}
 
