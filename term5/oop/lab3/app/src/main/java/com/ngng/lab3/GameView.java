package com.ngng.lab3;

import android.content.Context;
import android.graphics.*;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import java.util.Arrays;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;

import static java.util.concurrent.Executors.newFixedThreadPool;

class OnTouchTask implements Runnable {

    private final MotionEvent event;
    private final GameManager gameManager;

    OnTouchTask(MotionEvent event, GameManager gameManager) {
        this.event = event;
        this.gameManager = gameManager;
    }

    @Override
    public void run() {
        if (event != null && event.getAction() == MotionEvent.ACTION_UP) {
            gameManager.activatePlayer(event.getX(), event.getY());
        }
    }
}

public class GameView extends View {

    private static final int        FIELD_SIZE = 8;

    private final ExecutorService   executor = newFixedThreadPool(4);
    private final GameManager       gameManager;

    private Future                  botFuture;

    public GameView(Context context) {
        super(context);

        gameManager = new GameManager(FIELD_SIZE, this);

        assignTouchListener();
        startBotThread();
    }

    private void startBotThread() {
        botFuture = executor.submit(() -> {
            try {
                gameManager.startBotCycle();
            } catch (Exception e) {
                Thread.currentThread().interrupt();
            }
        });
    }

    private void assignTouchListener() {
        setOnTouchListener(new OnTouchListener() {

            @Override
            public boolean onTouch(View view, MotionEvent event) {
                Future touchFuture = executor.submit(new OnTouchTask(event, gameManager));

                try {
                    touchFuture.get();
                } catch (Exception e) {
                    Log.e("Touch exception", Arrays.toString(e.getStackTrace()));
                    Thread.currentThread().interrupt();
                }

                return true;
            }
        });
    }

    public void updateView() {
        invalidate();
    }

    public String getStatus() {
        return gameManager.getStatus();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        gameManager.draw(canvas);
    }

    public Future getBotFuture() {
        return botFuture;
    }
}
