const dgram = require('dgram')
const server = dgram.createSocket('udp4')
server.bind(5000)

server.on('listening', () => console.log("listening"))

server.on('message', (msg, rinfo) => {
    switch (msg[0]) {
        case 0x91:
            console.log("Get server IP.")
            server.send("1", 0, 1, rinfo.port, rinfo.address);
    }
});