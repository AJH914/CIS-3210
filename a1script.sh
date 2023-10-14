NUM_OF_CLIENTS=5
FILENAME="testfile.txt"
SERVER_IP=""
SERVER_PORT=""


for ((i = 1; i <= NUM_CLIENTS; i++)); do
    ./sendFile $FILENAME $SERVER_IP:$SERVER_PORT $BUFFERSIZE
    sleep 1
done
