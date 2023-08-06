#include <stdlib.h>
#include <cstdlib>
//#include <glut.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>  
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
 
using namespace std;

#define PORT 3456
 
int main (int argc, char *argv[])
{
     if (argc < 2)     /* argc should be 2 for correct execution */
     {
      cout << "usage: <executable> <numbers to sort>\n\n" <<  argv[0] << endl;
      exit(-1); 
      }

    if (argc > 101) { // 101 because 100 numbers and then 1 for the exectuable
        cout << "too many input values\n\n";
        exit(-1);
    }

	int sock1;
	sock1 =  socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in serv;
 
	serv.sin_port = htons(PORT);
        cout << "PORT" << PORT << "   " << htons(PORT) << endl;
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
        cout << "CLIENT CONNECTING" << endl;
	connect(sock1, (struct sockaddr *)&serv,sizeof(serv));
        cout << "CLIENT CONNECTED" << endl;
        // send numbers to server one at a time
        char sizeString[sizeof(int)];

        sprintf(sizeString, "%d", argc-1);
        write(sock1, sizeString, sizeof(int));
        for (int i = 1; i <= argc - 1; i++) {
            write(sock1, argv[i], sizeof(double));
            usleep(100);
        }  	
    double sortedNumbers[100];
    char hold[10];
    cout << "Sorted Numbers = ";
    for (int i = 0; i < argc-1; i++) {
        read(sock1, hold, sizeof(double));
        if (strlen(hold) != 0) {

            sortedNumbers[i] = stod(hold);
            cout << hold << " "; 
        }
        
    }
    cout << "\n";
}
