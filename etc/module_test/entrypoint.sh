#!/bin/bash
set -eux

MODULE_PORT=8080

sudo viam-server -version

echo "Starting module..."
envsubst < etc/module_test/config.json.in > config.json
viam-server -allow-insecure-creds --config config.json &
MODULE_PID=$!

if ! ps -p $MODULE_PID; then
    echo "Module never started"
    exit 1
fi

cleanup() {
    echo "Cleaning up..."
    kill $MODULE_PID
    wait $MODULE_PID 2>/dev/null
    echo "Running module process $MODULE_PID closed"
}

trap cleanup EXIT

for i in {1..5}; do
    if lsof -i ":$MODULE_PORT" | grep -q 'LISTEN'; then
        CONNECTED=1
        break
    fi
    sleep 5
done

if [[  -z "${CONNECTED-}" ]]; then
    echo "Reached timeout waiting for module to start"
    exit 1
fi

echo "Module is now running on port $MODULE_PORT."

./build/install/bin/simple_module_client

exit $?

