#include <stdio.h>  
#include <winsock.h>
#include <string>
#include <iostream>

using namespace std;

int    const PORT      = 80;
string const USERAGENT = "MinGW Socket Client 1.0";

// build_request() will help us send the right data to the website
char  *build_request (string, string);

// Get the web page and return it in the last argument
int getWebPage (const string url, string &html)
{
	string host;
	string page;
	int    client_socket;
	struct hostent *hent;
	int    iplen = 15; //ip addr format length XXX.XXX.XXX.XXX
	struct sockaddr_in *remote;
	int    tmpres;
	char   *get;
	char   *ip;
	char   buf[BUFSIZ+1];
	string tempString;
	int    returnCode = 0;
	size_t pos;
	
	html = "";
	host = url;
	page = "";
	
	// Get rid of any leading "http://" or "https://"
	if (host.length() > 7) {
		if (host.substr(0,7) == "http://")	
			host.erase(0,7);
	}
	
	if (host.length() > 8) {
		if (host.substr(0,8) == "https://")	
			host.erase(0,8);
	}
	
	// See if there is a web page specified
	pos = host.find('/');
	
	// Set the web page
	if (pos == string::npos) {
		page = "/";
	}
	else {
		// take all the characters from the slash to the end
		page = host.substr(pos);
		
		// remove everything from the slash to the end
		host.erase(pos);         
	}
	
	// Create the socket
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (client_socket < 0) {
		printf("client_socket = %d\n",client_socket);
		perror("Can't create TCP socket\n");
		returnCode = 1;
	}  	
	
	// Using the host website name, look up the IP address
	if (returnCode == 0) {
		ip = (char *)malloc(iplen+1);
		memset(ip, 0, iplen+1);
		if((hent = gethostbyname(host.c_str())) == NULL)
		{
			perror("Can't get IP");
			returnCode = 2;
		}
		else {
			// Save the IP address
			ip = inet_ntoa(*(struct in_addr *)*hent->h_addr_list);
		}
	}
	
	// Setup and connect the socket
	if (returnCode == 0) {
		
		// Setup the socket
		remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
		remote->sin_family = AF_INET;
		remote->sin_addr.s_addr = inet_addr(ip);
		remote->sin_port = htons(PORT);
		
		// Connect the socket
		if(connect(client_socket, (struct sockaddr *)remote, sizeof(struct sockaddr)) == SOCKET_ERROR){
			perror("Could not connect");
			
			returnCode = 3;
		}
	}

	// Prepare the request
	if (returnCode == 0) {
		get = build_request(host,page);
	}
	
	//Send the request to the server
	int sent = 0;
	if (returnCode == 0) {
		while(returnCode == 0 & sent < strlen(get)) { 
			tmpres = send(client_socket, get+sent, strlen(get)-sent, 0);
			if(tmpres == -1) {
				perror("Can't send query");
				returnCode = 4;
			}
			sent += tmpres;
		} // end of while loop
	} // end of if statement
	
	//Now it is time to receive the page
	if (returnCode == 0) {
		memset(buf, 0, sizeof(buf));
		int htmlstart = 0;
		char * htmlcontent;
		while((tmpres = recv(client_socket, buf, BUFSIZ, 0)) > 0) {
		
			if(htmlstart == 0) {
				htmlcontent = strstr(buf, "\r\n\r\n");
				if(htmlcontent != NULL){
					htmlstart = 1;
					htmlcontent += 4;
				}
			} else {
				htmlcontent = buf;
			}
		
		
			if(htmlstart) {
				tempString = string(htmlcontent);
				html = html + tempString;
			}

			memset(buf, 0, tmpres);
		} // end of while loop
	
		if(tmpres < 0) {
			perror("Error receiving data");
			returnCode = 5;
		}
	}
	
	// Close the socket and free any memory we allocated
	free(get);
	free(remote);
	closesocket(client_socket);
	WSACleanup();
	
	return returnCode;
}

char *build_request(string host, string page)
{
	char *request;
	string getpage = page;
	
	string tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	
	//////////////////////////////////
	// Remove leading '/' characters
	//////////////////////////////////
	if (getpage.length() > 0) {
	
		do {
			if (getpage.at(0) != '/')
				break;
		
			getpage.erase(0,1);
		
		} while (getpage.length() > 0);	
	}
	
	// -5 is to consider the %s %s %s in tpl and the ending \0
	request = (char *)malloc(host.length()+getpage.length()+USERAGENT.length()+tpl.length()-5);
	
	sprintf(request, "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n", 
	               getpage.c_str(), 
				   host.c_str(), 
				   USERAGENT.c_str());
	
	return request;
}