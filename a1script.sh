FILENAME_ARRAY=("wonderland.txt" "bee-movie.txt" "LoremIpsum.txt" "nudibranch.txt")
SERVER_IP="172.31.97.94"
SERVER_PORT="8080"

for i in ${FILENAME_ARRAY[@]}
do
    ./sendFile $i $SERVER_IP:$SERVER_PORT $BUFFERSIZE
done
