import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveAction;
import java.util.concurrent.ThreadLocalRandom;
import java.lang.Math;

public class Main {

    public static int[][] generateMatrix(final int size, final int min, final int max) {
        var matrix = new int[size][size];

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                matrix[i][j] = ThreadLocalRandom.current().nextInt(max - min) + min;
            }
        }

        return matrix;
    }

    public static void logMatrix(final int[][] matrix) {
        for (var row : matrix) {
            for (var coefficient : row) {
                System.out.print(coefficient + "\t");
            }

            System.out.println();
        }
    }

    public static int[][] multiplyByTapeMethod(final int[][] lhs, final int[][] rhs, final int workerCount) {
        assert lhs.length == rhs.length;

        final int size = lhs.length;
        final var pool = new ForkJoinPool(workerCount);

        var result = new int[size][size];

        for (int worker = 0; worker < workerCount; ++worker) {
            int finalWorker = worker;

            pool.invoke(new RecursiveAction() {

                @Override
                protected void compute() {
                    final int pivot = (int)Math.ceil((float)size / (float)workerCount);

                    int index = finalWorker;

                    for (int row = index * pivot; row < (index + 1) * pivot	&& row < size; ++row) {
                        for (int i = 0; i < size; ++i) {
                            for (int j = 0; j < size; ++j) {
                                result[row][index] += lhs[row][j] * rhs[j][index];
                            }

                            index = (index + 1) % size;
                        }
                    }
                }
            });
        }

        return result;
    }

    public static double run(final int size, final int runCount, final int workerCount) {
        double time = 0.0;

        for (int i = 0; i < runCount; ++i) {
            final var lhs = generateMatrix(size, -100, 100);
            final var rhs = generateMatrix(size, -100, 100);

            final var begin = System.currentTimeMillis();
            {
                multiplyByTapeMethod(lhs, rhs, workerCount);
            }
            final var end = System.currentTimeMillis();

            time += end - begin;
        }

        return time / (double)runCount;
    }

    public static void logResult(final int size, final int runCount) {
        var worker1Time = run(size, runCount, 1);
        var worker2Time = run(size, runCount, 2);
        var worker4Time = run(size, runCount, 4);

        System.out.printf(
            "%3dx%-3d:\t%.10f\t%.10f\t%.10f\n",
            size,
            size,
            worker1Time,
            worker2Time,
            worker4Time
        );
    }

    public static void main(String[] args) {
        final int runCount = 50;

        logResult(8, runCount);
        logResult(24, runCount);
        logResult(80, runCount);
        logResult(160, runCount);
        logResult(240, runCount);
        logResult(160, runCount);
        logResult(320, runCount);
        logResult(400, runCount);
    }
}
