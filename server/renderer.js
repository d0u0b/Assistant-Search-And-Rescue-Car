// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering

// process.

const IMG = api.getImageSize();

window.onload = () => {
    const trajectory = trajectoryGraph(d3.select("#trajectory"))
    trajectory.rerender()
    api.trajectoryRecall(trajectory)
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
        <canvas width="${IMG.WIDTH}" height="${IMG.HEIGHT}" id="img-canvas" class="mx-auto"></canvas>
        `,
        width: IMG.WIDTH + 60
    })
    window.canvas = document.getElementById("img-canvas")
    window.ctx = canvas.getContext("2d")
    let img = ctx.createImageData(IMG.WIDTH, IMG.HEIGHT);
    let data = img.data
    for(let i = 0; i < IMG.WIDTH * IMG.HEIGHT; i++) {
        data[i * 4] = data[i * 4 + 1] = data[i * 4 + 2] = imageData[i]
        data[i * 4 + 3] = 255
    }
    ctx.putImageData(img, 0, 0)
}
api.showImageRecall(showImage)