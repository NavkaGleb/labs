package main

import (
	"fmt"
	"math"
	"math/rand"
	"sync"
	"time"
)

type IntVector []int
type IntMatrix []IntVector

func (matrix IntMatrix) getRows() int {
	return len(matrix)
}

func (matrix IntMatrix) getCols() int {
	return len(matrix[0])
}

func (matrix IntMatrix) isSquare() bool {
	return matrix.getRows() == matrix.getCols()
}

func newMatrix(rows, cols int) IntMatrix {
	matrix := make(IntMatrix, rows)

	for i := range matrix {
		matrix[i] = make(IntVector, cols)
	}

	return matrix
}

func generateRandomMatrix(rows, cols, min, max int) IntMatrix {
	matrix := newMatrix(rows, cols)
	random := rand.New(rand.NewSource(time.Now().UnixNano()))

	for i := 0; i < matrix.getRows(); i++ {
		for j := 0; j < matrix.getCols(); j++ {
			matrix[i][j] = random.Intn(max - min) + min
		}
	}

	return matrix
}

func logMatrix(matrix IntMatrix) {
	for i := 0; i < matrix.getRows(); i++ {
		for j := 0; j < matrix.getCols(); j++ {
			fmt.Printf("%v\t", matrix[i][j])
		}

		fmt.Println()
	}
}

func tapeMethod(lhs, rhs IntMatrix, workerCount int) IntMatrix {
	if !lhs.isSquare() || !rhs.isSquare() || lhs.getRows() != rhs.getRows() {
		panic("Bad size!")
	}

	size := lhs.getRows()

	result := newMatrix(lhs.getRows(), rhs.getCols())

	group := sync.WaitGroup{}
	group.Add(workerCount)

	for worker := 0; worker < workerCount; worker++ {
		go func(worker int) {
			pivot := int(math.Ceil(float64(size) / float64(workerCount)))
			index := worker

			for row := index * pivot; row < (index + 1) * pivot	&& row < size; row++ {
				for i := 0; i < size; i++ {
					for j := 0; j < size; j++ {
						result[row][index] += lhs[row][j] * rhs[j][index]
					}

					index = (index + 1) % size
				}
			}

			group.Done()
		}(worker)
	}

	group.Wait()

	return result
}

func getElapsedTime(function func()) float64 {
	start := time.Now()

	function()

	return float64(time.Since(start)) * 1e-6
}

func runMultiplication(matrixSize, runCount, workerCount int) float64 {
 	var sum float64 = 0

	for i := 0; i < runCount; i++ {
		lhs := generateRandomMatrix(matrixSize, matrixSize, -100, 100)
		rhs := generateRandomMatrix(matrixSize, matrixSize, -100, 100)

		elapsedTime := getElapsedTime(func() {
			tapeMethod(lhs, rhs, workerCount)
		})

		sum += elapsedTime
	}

	return sum / float64(runCount)
}

func main() {
	const runCount = 50

	logResult := func(size int) {
		worker1Result := runMultiplication(size, runCount, 1)
		worker2Result := runMultiplication(size, runCount, 2)
		worker4Result := runMultiplication(size, runCount, 4)

		fmt.Printf(
			"%3dx%-3d:\t%.10f\t%.10f\t%.10f\n",
			size,
			size,
			worker1Result,
			worker2Result,
			worker4Result,
		)
	}

	logResult(8)
	logResult(24)
	logResult(80)
	logResult(160)
	logResult(240)
	logResult(320)
	logResult(400)
}
