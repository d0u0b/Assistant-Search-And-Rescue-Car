// All of the Node.js APIs are available in the preload process.
// It has the same sandbox as a Chrome extension.
const dgram = require('dgram')
const net = require('net')
const { IMG, attitudeOffset, CMD } = require('./config')

const { contextBridge, ipcRenderer } = require('electron')

const serverUdp = dgram.createSocket('udp4')
serverUdp.bind(5000)
serverUdp.on("message", (msg, rinfo) => {
    if (msg[0] == CMD.GET_IP) {
        serverUdp.send("1", 0, 1, rinfo.port, rinfo.address)
    }
})

const server = net.createServer()

server.listen(5000, "0.0.0.0", () => {
    console.log("Server listening")
})

let clientIP, clientPort;

let findPersonRecall;
let showImageRecall;
let trajectoryRecall;

const showVal = (k, v) => {
    outVal = (Math.round(v * 100) / 100).toFixed(2)
    return `${outVal >= 0 ? "+" : ""}${outVal} ${k[0] == 'a' ? 'g' : ''}`
}

let state = CMD.STOP_FIND

window.addEventListener('DOMContentLoaded', () => {
    const attitudeElement = {}
    Object.keys(attitudeOffset).forEach(id => {
        attitudeElement[id] = document.getElementById(id)
    })
    const direction = document.getElementById("direction")
    let imageBuffer
    let progressPercentage
    let progressBar

    // let yaw = 0
    // let pitch = 0
    // let roll = 0

    var socket;

    const sendCMD = cmd => socket.write(new Uint8Array([cmd]));

    // let lastTime = 0
    let lostData = null;
    server.on("connection", s => {
        socket = s
        socket.on("data", receiveData => {
            if (lostData) {
                tmp = receiveData;
                receiveData = new Uint8Array(lostData.length + tmp.length)
                receiveData.set(lostData, 0)
                receiveData.set(tmp, lostData.length)
                lostData = null;
            }
            while (receiveData.length) {
                switch (receiveData[0]) {
                    case CMD.GET_ATTITUDE:
                        if (receiveData.length < 29) {
                            lostData = receiveData;
                            receiveData = []
                            break;
                        }
                        data = receiveData.slice(1, 29)
                        receiveData = receiveData.slice(29)

                        recvVal = {}
                        if (!data.readUInt32LE) break;
                        time = data.readUInt32LE(0)

                        recvVal["acc-x"] = data.readInt16LE(4) / 16384;
                        recvVal["acc-y"] = data.readInt16LE(6) / 16384;
                        recvVal["acc-z"] = data.readInt16LE(8) / 16384;
                        recvVal["yaw"] = data.readFloatLE(10);
                        recvVal["pitch"] = data.readFloatLE(14) * 180 / Math.PI;
                        recvVal["roll"] = data.readFloatLE(18) * 180 / Math.PI;
                        recvVal["gyr-x"] = data.readInt16LE(22);
                        recvVal["gyr-y"] = data.readInt16LE(24);
                        recvVal["gyr-z"] = data.readInt16LE(26);

                        if (state == CMD.CONTINUE_FIND)
                            trajectoryRecall.update(time, recvVal);

                        Object.keys(attitudeOffset).forEach(key => {
                            attitudeElement[key].innerText = showVal(key, recvVal[key])
                        })

                        document.getElementById("yaw").innerText = recvVal["yaw"].toFixed(2)

                        // document.getElementById("ultrasonicL").innerText = data.readFloatLE(40).toFixed(2);
                        // document.getElementById("ultrasonicR").innerText = data.readFloatLE(44).toFixed(2);

                        // console.log(time-lastTime);
                        // lastTime = time
                        break;

                    case CMD.GET_ULTRASONIC:
                        if(receiveData.length < 9) {
                            lostData = receiveData;
                            receiveData = []
                            break;
                        }
                        data = receiveData.slice(1, 9)
                        receiveData = receiveData.slice(9)
                        document.getElementById("ultrasonicL").innerText = data.readFloatLE(0).toFixed(2);
                        document.getElementById("ultrasonicR").innerText = data.readFloatLE(4).toFixed(2);
                            
                        break;
                    case CMD.GET_MOTOR:
                            if(receiveData.length < 3) {
                                lostData = receiveData;
                                receiveData = []
                                break;
                            }
                            data = receiveData.slice(1, 3)
                            receiveData = receiveData.slice(3)
                            document.getElementById("motorL").innerText = data.readUint8(0);
                            document.getElementById("motorR").innerText = data.readUint8(1);
                            
                        break;
                    case CMD.PERSON_FINDED:
                        receiveData = receiveData.slice(1)
                        state = CMD.STOP_FIND;
                        trajectoryRecall.stop();
                        findPersonRecall()
                        imageBuffer = []
                        progressPercentage = document.getElementById("progress-percentage")
                        progressBar = document.getElementById("progress-bar")
                        socket.write(new Uint8Array([CMD.GET_IMG]));

                    case CMD.GET_IMG:
                        if (receiveData.length < 1921) {
                            lostData = receiveData;
                            receiveData = []
                            break;
                        }
                        data = receiveData.slice(1, 1921)
                        receiveData = receiveData.slice(1921)
                        imageBuffer.push(...[].slice.call(data, 0))
                        let percentage = Math.round(imageBuffer.length / (IMG.HEIGHT * IMG.WIDTH) * 100)
                        progressPercentage.innerText = `${percentage}%`
                        progressBar.style.width = `${percentage}%`
                        if (imageBuffer.length >= IMG.HEIGHT * IMG.WIDTH) {
                            showImageRecall(imageBuffer)
                        }
                        break;
                    default:
                        receiveData = []
                        break;
                }
            }
        })
    })
    document.getElementById("start-btn")
        .addEventListener("click", () => {
            sendCMD(CMD.CONTINUE_FIND);
            state = CMD.CONTINUE_FIND;
        })

    document.getElementById("stop-btn")
        .addEventListener("click", () => {
            state = CMD.STOP_FIND;
            trajectoryRecall.stop();
            sendCMD(CMD.STOP_FIND);
        })

    document.getElementById("restart-btn")
        .addEventListener("click", () => {
            sendCMD(CMD.RESTART);
            state = CMD.STOP_FIND;
            trajectoryRecall.reset();
        })

    document.getElementById("reset-btn")
        .addEventListener("click", () => {
            if(state == CMD.STOP_FIND) {
                trajectoryRecall.reset();
            }
        })
})

window.sendData = (cmd, data) => {
    console.assert(!(clientIP && clientPort), "client not connect");
    let sendData = String.fromCharCode(cmd) + (data || "");
    server.send(sendData, 0, sendData.length, clientPort, clientIP);
}

contextBridge.exposeInMainWorld("api", {
    findPersonRecall: func => {
        findPersonRecall = func;
    },
    showImageRecall: func => {
        showImageRecall = func;
    },
    trajectoryRecall: funcs => {
        trajectoryRecall = funcs;
    },
    sendCMD: (cmd, data = new Uint8Array(0)) => {
        console.assert((clientIP && clientPort), "client not connect");
        let sendData = new Uint8Array([cmd, ...data])
        server.send(sendData, 0, sendData.length, clientPort, clientIP);
    },
    getImageSize: () => IMG,
    continueFind: () => server.send(new Uint8Array([CMD.CONTINUE_FIND]), 0, 1, clientPort, clientIP),

})