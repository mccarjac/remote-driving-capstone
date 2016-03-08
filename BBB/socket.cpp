#include "socket.h"

mySocket::mySocket() {
	wheelInputs ins;
	ins.buttons = new bool[23];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) 
		error("ERROR opening socket");
	bzero((char*) &serv_addr, sizeof(serv_addr));
	portno = 27015;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("Error on binding");
	listen(sockfd, 1);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if(newsockfd < 0)
		error("ERROR on accept");
	bzero(buffer, 256);
	std::fstream fs;
	while(true) {
		bzero(buffer, 256);
		n = read(newsockfd, buffer, 256);
		if(buffer[0] == 'q') 
			break;
		for(int i = 0;i < 23;i++) {
			if(buffer[i] == '0')
				ins.buttons[i] = false;
			else
				ins.buttons[i] = true;
		}
		string temp = buffer;
		stringstream iss(temp);
		string word;
		iss >> word; // Pass buttons input

		iss >> word; // Get gas input
		ins.gas = atoi(word.c_str());
		
		iss >> word; // Get wheel input
		ins.wheel = atoi(word.c_str());
		
		iss >> word; // Get brakes
		ins.brakes = atoi(word.c_str());

		iss >> word; // Get dpad
		ins.dpad = atoi(word.c_str());
		
		//DO STUFF WITH INS
		ins.print();
	}

	delete ins.buttons;
	close(newsockfd);
	close(sockfd);	
}

void mySocket::error(const char* msg) {
	perror(msg);
	exit(1);
}
