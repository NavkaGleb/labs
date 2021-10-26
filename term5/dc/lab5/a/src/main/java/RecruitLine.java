import java.util.Arrays;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.atomic.AtomicBoolean;

public class RecruitLine implements Runnable {

    private Recruit[]           recruits;
    private Recruit.Direction   left = null;
    private Recruit.Direction   right = null;
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

    public void setLeft(Recruit.Direction left) {
        this.left = left;
    }

    public void setRight(Recruit.Direction right) {
        this.right = right;
    }

    public Recruit.Direction getLeft() {
        return recruits[0].getDirection();
    }

    public Recruit.Direction getRight() {
        return recruits[recruits.length - 1].getDirection();
    }

    @Override
    public String toString() {
        var result = new StringBuilder();

        for (var recruit : recruits) {
//            result.append(recruit);
            result.append(recruit.getDirection());
        }

        return result.toString();
    }

    @Override
    public synchronized void run() {
        try {
            while (!isDone.get()) {
                System.out.println("....");

                var next = Arrays.copyOf(recruits, recruits.length);

                isModified = false;

//                for (int i = 1; i < recruits.length - 1; ++i) {
//                    if ((recruits[i].isGoingLeft() && recruits[i - 1].isGoingRight()) || (recruits[i].isGoingRight() && recruits[i + 1].isGoingLeft())) {
//                        next[i] = recruits[i].turn();
////                        next[i].setDirection(recruits[i].getDirection().turn());
////                        next[i].direction = recruits[i].direction.turn();
//
//                        isModified = true;
//                    }
//                }
//
//                switch (recruits[0]) {
//                    case LEFT:
//                        if (left != null && left == Recruit.Direction.RIGHT) {
//                            next[0] = recruits[0].turn();
////                            next[0].setDirection(recruits[0].getDirection().turn());
////                            next[0].direction = recruits[0].direction.turn();
//
//                            isModified = true;
//                        }
//                        break;
//                    case RIGHT:
//                        if (recruits[1].isGoingLeft()) {
//                            next[0] = recruits[0].turn();
////                            next[0].setDirection(recruits[0].getDirection().turn());
////                            next[0].direction = recruits[0].direction.turn();
//
//                            isModified = true;
//                        }
//                        break;
//                }
//
//                switch (recruits[recruits.length - 1]) {
//                    case LEFT:
//                        if (recruits[recruits.length - 2].isGoingRight()) {
//                            next[next.length - 1] = recruits[recruits.length - 1].turn();
////                            next[next.length - 1].setDirection(recruits[recruits.length - 1].getDirection().turn());
////                            next[next.length - 1].direction = recruits[recruits.length - 1].direction.turn();
//
//                            isModified = true;
//                        }
//                        break;
//                    case RIGHT:
//                        if (right != null && right == Recruit.Direction.LEFT) {
//                            next[next.length - 1] = recruits[recruits.length - 1].turn();
////                            next[next.length - 1].setDirection(recruits[recruits.length - 1].getDirection().turn());
////                            next[next.length - 1].direction = recruits[recruits.length - 1].direction.turn();
//
//                            isModified = true;
//                        }
//                        break;
//                }






////////////////////////////////////////////////////////////////////////////




                for (int i = 1; i < recruits.length - 1; ++i) {
                    if ((recruits[i].isGoingLeft() && recruits[i - 1].isGoingRight()) || (recruits[i].isGoingRight() && recruits[i + 1].isGoingLeft())) {
//                        next[i] = recruits[i].turn();
//                        next[i].setDirection(recruits[i].getDirection().turn());
//                        next[i].direction = recruits[i].direction.turn();
                        next[i].turn();

                        isModified = true;
                    }
                }

                switch (recruits[0].direction) {
                    case LEFT:
                        if (left != null && left == Recruit.Direction.RIGHT) {
//                            next[0] = recruits[0].turn();
//                            next[0].setDirection(recruits[0].getDirection().turn());
//                            next[0].direction = recruits[0].direction.turn();
                            next[0].turn();

                            isModified = true;
                        }
                        break;
                    case RIGHT:
                        if (recruits[1].isGoingLeft()) {
//                            next[0] = recruits[0].turn();
//                            next[0].setDirection(recruits[0].getDirection().turn());
//                            next[0].direction = recruits[0].direction.turn();
                            next[0].turn();


                            isModified = true;
                        }
                        break;
                }

                switch (recruits[recruits.length - 1].direction) {
                    case LEFT:
                        if (recruits[recruits.length - 2].isGoingRight()) {
//                            next[next.length - 1] = recruits[recruits.length - 1].turn();
//                            next[next.length - 1].setDirection(recruits[recruits.length - 1].getDirection().turn());
//                            next[next.length - 1].direction = recruits[recruits.length - 1].direction.turn();
                            next[next.length - 1].turn();

                            isModified = true;
                        }
                        break;
                    case RIGHT:
                        if (right != null && right == Recruit.Direction.LEFT) {
//                            next[next.length - 1] = recruits[recruits.length - 1].turn();
//                            next[next.length - 1].setDirection(recruits[recruits.length - 1].getDirection().turn());
//                            next[next.length - 1].direction = recruits[recruits.length - 1].direction.turn();
                            next[next.length - 1].turn();


                            isModified = true;
                        }
                        break;
                }





                recruits = next;

                barrier.await();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
