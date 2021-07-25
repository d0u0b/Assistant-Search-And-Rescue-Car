const trajectoryGraph = div => {
    const containerHeight = div._groups[0][0].offsetHeight
    const containerWidth = div._groups[0][0].offsetWidth


    var margin = ({ top: 20, right: 25, bottom: 20, left: 25 })

    const spacing = 80
    const xTicks = Math.floor((containerWidth - margin.left - margin.right) / spacing)
    const yTicks = Math.floor((containerHeight - margin.top - margin.bottom) / spacing)

    var minX = -14, maxX = 14, minY = -8, maxY = 8;

    var showData = [
        { x: 0, y: 0 }
    ]

    const svg = div.append("svg")
    svg.attr("width", "100%")
    svg.attr("height", "100%")

    const gX = svg.append("g")
    const gY = svg.append("g")

    const line = svg.append("path")

    const rerender = () => {
        const x = d3.scaleLinear()
            .domain([minX, maxX])
            .nice()
            .range([margin.left, margin.left + spacing * xTicks])

        const y = d3.scaleLinear()
            .domain([minY, maxY])
            .nice()
            .range([margin.top + spacing * yTicks, margin.top])

        const xAxis = g => g
            .attr("transform", `translate(0, ${containerHeight - margin.bottom})`)
            .call(d3.axisBottom(x).ticks(xTicks))
            .call(g => g.select(".domain").remove())
            .call(g => g
                .selectAll(".tick line")
                .clone()
                .attr("y2", -containerHeight)
                .attr("stroke-opacity", 0.1)
            )
    
        const yAxis = g => g
            .attr("transform", `translate(${margin.left},0)`)
            .call(d3.axisLeft(y).ticks(yTicks))
            .call(g => g.select(".domain").remove())
            .call(g => g
                .selectAll(".tick line")
                .clone()
                .attr("x2", containerWidth)
                .attr("stroke-opacity", 0.1)
            )

        const calcLine = d3.line()
            .curve(d3.curveCatmullRom)
            .x(d => x(d.x))
            .y(d => y(d.y))

        gX.html(null).call(xAxis)
        gY.html(null).call(yAxis)

        line.datum(showData)
            .attr("fill", "none")
            .attr("stroke", "black")
            .attr("stroke-width", 2.5)
            .attr("stroke-linejoin", "round")
            .attr("stroke-linecap", "round")
            .attr("d", calcLine)
    }

    const updateChart = (x, y) => {
        showData.push({x, y})
        minX = Math.min(minX, x);
        maxX = Math.max(maxX, x);
        minY = Math.min(minY, y);
        maxY = Math.max(maxY, y);
        let spacing = Math.ceil(Math.max((maxX - minX) / xTicks, (maxY - minY) / yTicks))

        let xDiff = (spacing * xTicks + minX - maxX) / 2
        minX -= xDiff
        maxX += xDiff
        
        let yDiff = (spacing * yTicks + minY - maxY) / 2
        minY -= yDiff
        maxY += yDiff
        
        rerender();
    }

    var prevTime = 0;
    var prevAccX, prevAccY, prevAccZ;
    var speedX = 0, speedY = 0;
    var prevSpeedX = 0, prevSpeedY = 0;
    var X=0, Y=0;

    const getPitch = Vals => (Math.atan(Vals["acc-y"] / (Vals["acc-x"] ** 2 + Vals["acc-z"] ** 2) ** 0.5) * 180 / Math.PI)
    const getRoll = Vals => (Math.atan(-Vals["acc-x"] / (Vals["acc-y"] ** 2 + Vals["acc-z"] ** 2) ** 0.5) * 180 / Math.PI)
    const update = (time, Vals) => {

        if (prevTime) {
            prevTime = time - prevTime;
            
            //9.8 / 2
            speedX += (Vals["acc-x"] + prevAccX) * 0.49 * prevTime - 0.0004;
            speedY += (Vals["acc-y"] + prevAccY) * 0.49 * prevTime + 0.0001;
            // speedZ += (Vals["acc-z"] + prevAccZ) * 0.49 * prevTime;
            console.log(
                speedX,
                speedY
            );

            X += (speedX + prevSpeedX) * prevTime / 2000 
            Y += (speedY + prevSpeedY) * prevTime / 2000

            updateChart(X, Y);
        }

        prevAccX = Vals["acc-x"]
        prevAccY = Vals["acc-y"]
        prevAccZ = Vals["acc-z"]

        prevTime = time;
    }

    const reset = () => {
        prevTime = prevAccX = prevAccY = prevAccZ = 0;
        speedX = speedY = prevSpeedX = prevSpeedY = X = Y = 0;

        minX = -14, maxX = 14, minY = -8, maxY = 8;
        showData = [
            { x: 0, y: 0 }
        ];
        rerender();
    }

    const stop = () => {
        prevTime = prevAccX = prevAccY = prevAccZ = 0;
        speedX = speedY = prevSpeedX = prevSpeedY = X = Y = 0;
    }

    return {
        rerender,
        update,
        reset,
        stop
    };
}