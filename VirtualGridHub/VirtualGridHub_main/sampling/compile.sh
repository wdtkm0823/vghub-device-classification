SOURCE_FILE=${1}
Compile_FILE=${2}

echo "引数1は${SOURCE_FILE}です"
echo "引数2は${Compile_FILE}です"

gcc -o ${Compile_FILE} ${SOURCE_FILE} libVGHubPD.a -lpthread -lwiringPi -lrt
exit 0