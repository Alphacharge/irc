#include <iostream>
#include <vector>
#include <cstring>
#include <poll.h> // Include the poll header
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int port = 12345; // Replace with your desired port number

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket.\n";
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error binding socket.\n";
        return 1;
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Error listening on socket.\n";
        return 1;
    }

    std::cout << "Server started and listening on port " << port << ".\n";
	// poll part
	std::vector<pollfd> fds;
    pollfd serverPollfd;
	serverPollfd.fd = serverSocket;
	serverPollfd.events = POLLIN;
	fds.push_back(serverPollfd);


    while (true) {
        int numReady = poll(&fds[0], fds.size(), -1); // Wait indefinitely for events

        if (numReady < 0) {
            std::cerr << "Error in poll().\n";
            return 1;
        }

        // Check for events on the server socket
        if (fds[0].revents & POLLIN) {
            // Incoming connection request
            socklen_t clientAddrLen = sizeof(clientAddr);
            clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

            if (clientSocket < 0) {
                std::cerr << "Error accepting connection.\n";
                return 1;
            }

            std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "\n";

            // Add the new client socket to the poll set
            pollfd serverPollfd;
			serverPollfd.fd = serverSocket;
			serverPollfd.events = POLLIN;
			fds.push_back(serverPollfd);

        }

        // Check for events on client sockets
        for (size_t i = 1; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                // Handle data from the client
                // Replace this with your custom IR chat message handling code
                // For example, read data from the client and broadcast it to other clients.
            }
        }
    }

    return 0;
}



