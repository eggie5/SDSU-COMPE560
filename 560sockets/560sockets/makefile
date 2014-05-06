all:
	gcc main.c -o 560socketbin
clean:
	rm -rf *o 560socketbin client server
client:
	gcc -Wall client.c -o client
server:
	gcc -Wall main.c -o server
deploy_rohan:
	scp -r ./* egg@rohan.sdsu.edu:~/echo_server/
deploy_jason:
	scp -r ./* egg@jason.sdsu.edu:~/echo_server/