'use strict'

class Point {
    constructor(x, y) {
        this.x = x
        this.y = y
    }
}

class Orientation {
    static Horizontal = new Orientation(0)
    static Vertical = new Orientation(1)

    constructor(value) {
        this.value = value
    }

    static getOposite(orientation) {
        return orientation === this.Horizontal ? this.Vertical : this.Horizontal;
    }
}

class Node {
    orientation
    point
    left
    right

    constructor(orientation) {
        this.orientation = orientation;
    }
}

class Tree {
    root

    constructor(points) {
        this.root = this.#build(points, Orientation.Horizontal)
    }

    #build(points, orientation) {
        if (points.length === 0) {
            return
        }

        let node = new Node(orientation);

        const compareFunc = node.orientation === Orientation.Horizontal ? ((p1, p2) => p1.x - p2.x) : ((p1, p2) => p1.y - p2.y)
        points.sort(compareFunc)

        const middleIndex = Math.floor(points.length / 2)
        node.point = points[middleIndex]

        node.left = this.#build(points.filter((point, index) => index < middleIndex), Orientation.getOposite(orientation))
        node.right = this.#build(points.filter((point, index) => index > middleIndex), Orientation.getOposite(orientation))

        return node
    }

    search(rect) {
        let result = []
        this.#search(this.root, rect, result)

        return result
    }

    #search(node, rect, result) {
        if (node == null) {
            return
        }
    
        if (rect.topLeft.x < node.point.x && node.point.x < rect.bottomRight.x && rect.topLeft.y < node.point.y && node.point.y < rect.bottomRight.y) {
            result.push(node.point)
        }
    
        const middle = (node.orientation === Orientation.Horizontal) ? node.point.x : node.point.y
        const left = (node.orientation === Orientation.Horizontal) ? rect.topLeft.x : rect.topLeft.y
        const right = (node.orientation === Orientation.Horizontal) ? rect.bottomRight.x : rect.bottomRight.y
    
        if (left < middle) {
            this.#search(node.left, rect, result)
        }
    
        if (middle < right) {
            this.#search(node.right, rect, result)
        }
    }
}