'use strict'

const POINT_RADIUS = 5

class Rect {
    constructor(topLeft, bottomRight) {
        this.topLeft = topLeft;
        this.bottomRight = bottomRight;
    }
}

class State {
    static Preprocessing = new State("Preprocessing")
    static Run = new State("Run")
    static Restart = new State("Restart")

    #name

    constructor(name) {
        this.#name = name
    }

    toString() {
        return `${this.#name}`
    }
}

class LogSeverity {
    static Info = new LogSeverity("INFO")
    static Warning = new LogSeverity("WARNING")
    static Tip = new LogSeverity("TIP")
    
    #name

    constructor(name) {
        this.#name = name
    }

    toString() {
        return `${this.#name}`
    }
}

class Logger {
    static log(severity, message) {
        console.log(`%c [${severity.toString()}] ${message}`, `color: ${this.#getColor(severity)}`)
    }

    static #getColor(severity) {
        switch (severity) {
            case LogSeverity.Info: return "#84fa84"
            case LogSeverity.Warning: return "yellow"
            case LogSeverity.Tip: return "#24a0ed"
            default: return "red"
        }
    }
}

let state = State.Preprocessing;
let points = []
let rect = new Rect(null, null)
let tree

let mainCanvas;

addEventListener('load', () => {
    mainCanvas = document.getElementById('main-canvas');
    mainCanvas.width = mainCanvas.parentElement.clientWidth;
    mainCanvas.height = 500;
    mainCanvas.addEventListener('click', onCanvasClick);

    document.getElementById('proceed-button').addEventListener('click', proceed)
    document.getElementById('reset-button').addEventListener('click', () => {
        Logger.log(LogSeverity.Info, 'Canvas cleared')
        reset()
    });

    reset();
})

function reset() {
    points = [];
    rect = new Rect(null, null);
    state = State.Preprocessing;

    drawGraph(mainCanvas, points);

    let proceedBtn = document.getElementById('proceed-button');
    proceedBtn.innerText = state.toString();

    if (!proceedBtn.classList.contains('active-button')) {
        proceedBtn.classList.add('active-button');
    }
}

function drawPoint(context, color, point) {
    context.fillStyle = color;
    
    context.beginPath();
    context.arc(point.x, point.y, POINT_RADIUS, 0, 2 * Math.PI);
    context.fill();
}

function drawGrid(canvas) {
    let context = canvas.getContext('2d');
    context.strokeStyle = window.getComputedStyle(canvas).getPropertyValue('--grid-color');
    context.lineWidth = 0.5;

    for (let i = 10; i < canvas.width; i += 10) {
        context.beginPath();
        {
            context.moveTo(i, 10);
            context.lineTo(i, canvas.height - 10);
        }
        context.stroke();
    }

    for (let i = 10; i < canvas.height; i += 10) {
        context.beginPath();
        {
            context.moveTo(10, i);
            context.lineTo(canvas.width - 10, i);
        }
        context.stroke();
    }
}

function drawGraph(canvas, points, rect, tree) {
    let context = canvas.getContext('2d');
    context.clearRect(0, 0, canvas.width, canvas.height);

    drawGrid(canvas)

    if (rect != null) {
        if (rect.topLeft && rect.bottomRight) {
            context.fillStyle = window.getComputedStyle(canvas).getPropertyValue('--section-color')

            context.fillRect(rect.topLeft.x, rect.topLeft.y, rect.bottomRight.x - rect.topLeft.x, rect.bottomRight.y - rect.topLeft.y)
        } else if (rect.topLeft) {
            drawPoint(context, window.getComputedStyle(canvas).getPropertyValue('--section-color'), rect.topLeft)
        }
    }

    if (tree) {
        drawTree(mainCanvas, tree)
    }

    for (let point of points) {
        drawPoint(context, window.getComputedStyle(canvas).getPropertyValue('--point-color'), point)
    }
}

function drawTree(canvas, tree) {
    let context = canvas.getContext('2d')
    context.strokeStyle = window.getComputedStyle(canvas).getPropertyValue('--edge-color')
    context.lineWidth = 1

    const drawNode = (node, max_left, max_right, max_top, max_bottom) => {
        if (node == null) {
            return
        }

        context.beginPath()

        if (node.orientation === Orientation.Horizontal) {
            context.moveTo(node.point.x, max_top)
            context.lineTo(node.point.x, max_bottom)
        } else {
            context.moveTo(max_left, node.point.y)
            context.lineTo(max_right, node.point.y)
        }

        context.stroke()

        if (node.orientation === Orientation.Horizontal) {
            drawNode(node.left, max_left, node.point.x, max_top, max_bottom)
            drawNode(node.right, node.point.x, max_right, max_top, max_bottom)
        } else {
            drawNode(node.right, max_left, max_right, node.point.y, max_bottom)
            drawNode(node.left, max_left, max_right, max_top, node.point.y)
        }
    }

    drawNode(tree.root, 10, canvas.width - 10, 10, canvas.height - 10)
}

function onCanvasClick(e) {
    const x = e.pageX - mainCanvas.offsetLeft
    const y = e.pageY - mainCanvas.offsetTop

    let point = points.find(item => Math.abs(x - item.x) < POINT_RADIUS && Math.abs(y - item.y) < POINT_RADIUS)

    switch (state) {
        case State.Preprocessing: {
            if (point) {
                Logger.log(LogSeverity.Warning, "point already exists")
                return;
            }

            point = new Point(x, y);
            points.push(point);

            Logger.log(LogSeverity.Info, `Added point (${point.x}, ${point.y})`)
            drawGraph(mainCanvas, points);

            break;
        }
        case State.Run: {
            point = new Point(x, y);

            if (rect.topLeft == null) {
                rect.topLeft = point;

                Logger.log(LogSeverity.Info, `Top-left point of rect: (${point.x}, ${point.y})`)
                drawGraph(mainCanvas, points, rect, tree);
            } else if (rect.bottomRight == null) {
                rect.bottomRight = point;
                
                Logger.log(LogSeverity.Info, `Bottom-right point of rect: (${point.x}, ${point.y})`)
                drawGraph(mainCanvas, points, rect, tree);

                document.getElementById('proceed-button').classList.add('active-button')
            }

            break;
        }
        default: {
            break;
        }
    }
}

function proceed() {
    let proceedButton = document.getElementById('proceed-button');

    if (!proceedButton.classList.contains('active-button')) {
        return
    }

    switch (state) {
        case State.Preprocessing: {
            tree = new Tree(points)

            drawGraph(mainCanvas, points, rect, tree);

            state = State.Run;

            proceedButton.classList.remove('active-button');

            proceedButton.innerText = state.toString();
            
            Logger.log(LogSeverity.Tip, "Select rectangle and run")
            break;
        }
        case State.Run: {
            const res = tree.search(rect)

            for (let point of res) {
                drawPoint(mainCanvas.getContext('2d'), window.getComputedStyle(mainCanvas).getPropertyValue('--highlight-color'), point);
            }

            state = State.Restart;
            proceedButton.classList.remove('active-button');

            Logger.log(LogSeverity.Info, `Found ${res.length} points`);
            Logger.log(LogSeverity.Tip, "Reset to restart");

            break;
        }
        default: {
            break;
        }
    }
}