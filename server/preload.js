// All of the Node.js APIs are available in the preload process.
// It has the same sandbox as a Chrome extension.
const dgram = require('dgram')
const server = dgram.createSocket('udp4')

const {contextBridge, ipcRenderer} = require('electron')

const PERSON_DETECTION = 0x81
const ACC_GRY = 0x82
const SEND_IMG = 0x83
const GET_IP = 0x91
const PERSON_FIND = 0x92
const GET_IMG = 0x93
const CONTINUE_FIND = 0x94

const IMG_WIDTH = 320
const IMG_HEIGHT = 240

server.bind(5000)

var clientIP, clientPort;

var findPersonRecall;
var showImageRecall;

const showVal = v => (Math.round(v * 100) / 100).toFixed(2)

window.addEventListener('DOMContentLoaded', () => {
    const accX = document.getElementById("acc-x")
    const accY = document.getElementById("acc-y")
    const accZ = document.getElementById("acc-z")
    const gryX = document.getElementById("gry-x")
    const gryY = document.getElementById("gry-y")
    const gryZ = document.getElementById("gry-z")
    const direction = document.getElementById("direction")
    let imageBuffer
    let progressPercentage;
    let progressBar;


	server.on('message', (msg, rinfo) => {
        clientIP = rinfo.address;
        clientPort = rinfo.port;
        switch (msg[0]) {
            case GET_IP:
                // console.log("Get server IP.")
                server.send("1", 0, 1, rinfo.port, rinfo.address)
                break;
            case ACC_GRY:
                // console.log("Get Acc Gry")
                let time = msg.readUInt32LE(1)
                
                let accXData = msg.readFloatLE(5)
                let accYData = msg.readFloatLE(9)
                let accZData = msg.readFloatLE(13)
                let gryXData = msg.readFloatLE(21)
                let gryYData = msg.readFloatLE(25)
                let gryZData = msg.readFloatLE(29)
                let directionData =  msg.readFloatLE(33)
                
                accX.innerText = `${showVal(accXData)[0] != '-' ? "+": ""}${showVal(accXData)} g`
                accY.innerText = `${showVal(accYData)[0] != '-' ? "+": ""}${showVal(accYData)} g`
                accZ.innerText = `${showVal(accZData)[0] != '-' ? "+": ""}${showVal(accZData)} g`
                gryX.innerText = `${showVal(gryXData)[0] != '-' ? "+": ""}${showVal(gryXData)}`
                gryY.innerText = `${showVal(gryYData)[0] != '-' ? "+": ""}${showVal(gryYData)}`
                gryZ.innerText = `${showVal(gryZData)[0] != '-' ? "+": ""}${showVal(gryZData)}`
                direction.innerText = showVal(directionData)
                break;
            case PERSON_FIND:
                findPersonRecall()
                imageBuffer = []
                progressPercentage = document.getElementById("progress-percentage")
                progressBar = document.getElementById("progress-bar")
                server.send(new Uint8Array([GET_IMG]), 0, 1, clientPort, clientIP);
                break;

            case SEND_IMG:
                imageBuffer.push(...[].slice.call(msg, 1))
                let percentage = Math.round(imageBuffer.length / (IMG_HEIGHT * IMG_WIDTH) * 100)
                progressPercentage.innerText = `${percentage}%`
                progressBar.style.width = `${percentage}%`
                console.log(percentage)
                if(imageBuffer.length >= IMG_HEIGHT * IMG_WIDTH) {
                    showImageRecall(imageBuffer)
                }
                break;
            default:
                console.log(msg)
                break;

                
        }
    })
});

window.sendData = (cmd, data) => {
    console.assert(!(clientIP && clientPort), "client not connect");
    let sendData = String.fromCharCode(cmd) + (data || "");
    server.send(sendData, 0, sendData.length, clientPort, clientIP);
}

contextBridge.exposeInMainWorld("api", {
    send: (channel, data) => {
        let validChannels = ["getImg"];
        if (validChannels.includes(channel)) {
            ipcRenderer.send(channel, data);
        }
    },
    findPersonRecall: func => {
        findPersonRecall = func;
    },
    showImageRecall: func => {
        showImageRecall = func;
    },
    sendCMD: (cmd, data = new Uint8Array(0)) => {
        console.assert((clientIP && clientPort), "client not connect");
        let sendData = new Uint8Array([cmd, ...data])
        server.send(sendData, 0, sendData.length, clientPort, clientIP);
    }
})