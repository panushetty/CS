echo "Hi, this program will now display running processes"
sleep 3
ps aux
echo "What process do you want to kill? (Enter number)"
read pid
if [[ $pid =~ ^[0-9]+$ ]]; then
    echo "The PID you have mentioned is $pid"
    echo "Are you sure you want to kill this process? (y/n)"
    read confirm
    if [ "$confirm" = "y" ]; then
        if kill "$pid"; then
            echo "Process $pid killed successfully"
        else
            echo "Failed to kill process $pid"
        fi
    else
        echo "Task aborted"
    fi
else
    echo "Not a valid number"
fi