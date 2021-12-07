package com.ngng.lab3;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends Activity {

    private GameView        gameView;
    private LinearLayout    boardLayout;
    private TextView        status;
    private Handler         updateHandler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN
        );

        updateHandler = new Handler();
        gameView = new GameView(this);

        // Get 'board_layout'
        boardLayout = findViewById(R.id.board_layout);
        boardLayout.addView(gameView);

        // Add listener to 'new_game_button'
        findViewById(R.id.new_game_button).setOnClickListener((View view) -> {
            if (view.getId() == R.id.new_game_button) {
                startNewGame();
            }
        });

        // Get 'status_text_view'
        status = findViewById(R.id.status_text_view);

        // Init update handler
        updateHandler.post(new Runnable() {

            @Override
            public void run() {
                if (gameView != null) {
                    status.setText(gameView.getStatus());
                }

                updateHandler.postDelayed(this, 200);
            }
        });

        startNewGame();
    }

    private void startNewGame() {
        if (gameView != null && gameView.getBotFuture() != null) {
            gameView.getBotFuture().cancel(true);
        }

        gameView = new GameView(this);

        boardLayout.removeAllViews();
        boardLayout.addView(gameView);

        gameView.updateView();
    }
}