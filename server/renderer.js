// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering

// process.
const PERSON_DETECTION = 0x81
const ACC_GRY = 0x82
const SEND_IMG = 0x83
const GET_IP = 0x91
const PERSON_FIND = 0x92
const GET_IMG = 0x93
const CONTINUE_FIND = 0x94

const IMG_HEIGHT = 240
const IMG_WIDTH = 320

window.onload = () => {
    const trajectory = trajectoryGraph(d3.select("#trajectory"))
    
}

const showAlert = () => {
    Swal.fire({
        title: "偵測到受難者",
        icon: "warning",
        html: `
        <div class="relative pt-1">
            <div class="flex mb-2 items-center justify-between">
            <div>
                <span class="text-xs font-semibold inline-block py-1 px-2 rounded-full text-orange-600 bg-orange-200">
                Receive Image...
                </span>
            </div>
            <div class="text-right">
                <span id="progress-percentage" class="text-xs font-semibold inline-block text-orange-600">
                0%
                </span>
            </div>
            </div>
            <div class="overflow-hidden h-2 mb-4 text-xs flex rounded bg-orange-200">
            <div id="progress-bar" class="shadow-none flex flex-col text-center whitespace-nowrap text-white justify-center bg-orange-500"></div>
            </div>
        </div>
        `,
        showConfirmButton: false
    })
}
api.findPersonRecall(showAlert)

const showImage = imageData => {
    Swal.fire({
        html: `
        <canvas width="${IMG_WIDTH}" height="${IMG_HEIGHT}" id="img-canvas" class="mx-auto"></canvas>
        `
    })
    window.canvas = document.getElementById("img-canvas")
    window.ctx = canvas.getContext("2d")
    let img = ctx.createImageData(IMG_WIDTH, IMG_HEIGHT);
    let data = img.data
    for(let i = 0; i < IMG_WIDTH * IMG_HEIGHT; i++) {
        data[i * 4] = data[i * 4 + 1] = data[i * 4 + 2] = imageData[i] ^ 128
        data[i * 4 + 3] = 255
    }
    ctx.putImageData(img, 0, 0)
}
api.showImageRecall(showImage)