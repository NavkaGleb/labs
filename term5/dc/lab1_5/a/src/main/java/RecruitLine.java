import java.util.concurrent.atomic.AtomicBoolean;

public class RecruitLine implements Runnable {

    private Recruit[]           recruits;
    private Recruit             left = null;
    private Recruit             right = null;
    private final CyclicBarrier barrier;
    private final AtomicBoolean isDone;
    private boolean             isModified = false;

    public RecruitLine(int recruitCount, AtomicBoolean isDone, CyclicBarrier barrier) {
        this.recruits = new Recruit[recruitCount];

        for (int i = 0; i < this.recruits.length; ++i) {
            this.recruits[i] = new Recruit();
        }

        this.isDone = isDone;
        this.barrier = barrier;
    }

    public boolean isModified() {
        return isModified;
    }

    public void setLeftRecruit(Recruit left) {
        this.left = left;
    }

    public void setRightRecruit(Recruit right) {
        this.right = right;
    }

    public Recruit getFirstRecruit() {
        return recruits[0];
    }

    public Recruit getLastRecruit() {
        return recruits[recruits.length - 1];
    }

    public String toColoredString() {
        var result = new StringBuilder();

        for (var recruit : recruits) {
            result.append(recruit.toColoredString());
        }

        return result.toString();
    }

    @Override
    public synchronized void run() {
        try {
            while (!isDone.get()) {
                var next = new Recruit[recruits.length];

                for (int i = 0; i < recruits.length; ++i) {
                    next[i] = new Recruit(recruits[i]);
                }

                isModified = false;

                // Update all recruits except first and last
                for (int i = 1; i < recruits.length - 1; ++i) {
                    if ((recruits[i].isGoingLeft() && recruits[i - 1].isGoingRight()) ||
                        (recruits[i].isGoingRight() && recruits[i + 1].isGoingLeft())) {
                        next[i].turn();
                        isModified = true;
                    }
                }

                // Update first recruit
                if (recruits[0].isGoingLeft()) {
                    if (left != null && left.isGoingRight()) {
                        next[0].turn();
                        isModified = true;
                    }
                }

                if (recruits[0].isGoingRight()) {
                    if (recruits[1].isGoingLeft()) {
                        next[0].turn();
                        isModified = true;
                    }
                }

                // Update last recruit
                if (recruits[recruits.length - 1].isGoingLeft()) {
                    if (recruits[recruits.length - 2].isGoingRight()) {
                        next[next.length - 1].turn();
                        isModified = true;
                    }
                }

                if (recruits[recruits.length - 1].isGoingRight()) {
                    if (right != null && right.isGoingLeft()) {
                        next[next.length - 1].turn();
                        isModified = true;
                    }
                }

                recruits = next;

                barrier.await();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
