package com.ngng.lab3;

import android.graphics.Canvas;

import java.util.ArrayList;
import java.util.List;

public class GameManager {

    private static final int            BOT_SLEEP_TIME = 300;
    private static final Checker.Color  playerSide = Checker.Color.BLACK;

    private final GameView              gameView;
    private final Board                 board;
    private final BoardManager          boardManager;
    private final int                   fieldSize;

    private Checker                     selectedChecker;
    private Checker.Color               gameState = Checker.Color.BLACK;
    private List<Coordinates>           availableCoordinates = new ArrayList<>();
    private boolean                     playerBeatRow = false;

    public GameManager(int size, GameView caller) {
        this.fieldSize = size;
        this.gameView = caller;

        board = new Board(size);
        boardManager = new BoardManager(size);
    }

    public void draw(Canvas canvas) {
        board.draw(canvas);
        boardManager.draw(canvas);
    }

    public void startBotCycle() {
        while (!Thread.currentThread().isInterrupted()) {
            if (gameState == otherColor(playerSide)) {
                if (!playerBeatRow) {
                    startBotTurn();
                    reverseState();
                    updateQueens();
                }
            }
        }
    }

    public void activatePlayer(float x, float y) {
        if (gameState == playerSide) {
            Coordinates tapCoordinates = Coordinates.toCoordinates(x, y, fieldSize, board.getCellSize());

            if (selectedChecker == null) {
                trySelect(tapCoordinates);
                updateView();
            } else {
                if (coordsInAvailableCells(tapCoordinates)) {
                    Coordinates lastPosition = boardManager.find(selectedChecker);

                    playerBeatRow = doPlayerStep(tapCoordinates);
                    
                    updateQueens();

                    if (playerBeatRow && canBeatMore(tapCoordinates)) {
                        playerBeatRow = true;
                        trySelectBeatable(lastPosition);
                    } else {
                        playerBeatRow = false;
                        unselectAll();
                    }
                } else {
                    unselectAll();
                    updateView();
                    return;
                }

                updateView();

                if (!playerBeatRow) {
                    unselectAll();
                    reverseState();
                }
            }
        }
    }

    public void startBotTurn() {
        boolean botBeatRow;

        Coordinates checkerCoordinates = Bot.chooseChecker(boardManager, otherColor(playerSide), BOT_SLEEP_TIME);

        if (checkerCoordinates == null) {
            return;
        }

        Checker checkerBot = boardManager.getChecker(checkerCoordinates);
        Coordinates moveCoordinates = Bot.chooseMove(boardManager, checkerBot, BOT_SLEEP_TIME);
        
        trySelect(checkerCoordinates);
        updateView();        

        if (moveCoordinates == null) {
            return;
        }

        botBeatRow = boardManager.move(checkerBot, moveCoordinates.x, moveCoordinates.y);
        unselectAll();
        
        updateView();

        while (botBeatRow && canBeatMore(moveCoordinates)) {
            unselectAll();
            trySelectBeatable(moveCoordinates);

            updateView();
            
            moveCoordinates = Bot.chooseMove(boardManager, checkerBot, BOT_SLEEP_TIME);
            botBeatRow = moveCoordinates != null && boardManager.move(checkerBot, moveCoordinates.x, moveCoordinates.y);

            updateView();
        }
        
        unselectAll();
    }

    public void updateView() {
        if (gameView != null) {
            gameView.invalidate();
        }
    }

    private boolean doPlayerStep(Coordinates tapCoordinates) {
        Coordinates oldCoordinates = boardManager.find(selectedChecker);

        if (oldCoordinates.equals(tapCoordinates)) {
            return false;
        }

        return boardManager.move(selectedChecker, tapCoordinates.x, tapCoordinates.y);
    }

    public boolean canBeatMore(Coordinates lastPosition) {
        if (lastPosition == null) {
            return false;
        }

        Checker checker = boardManager.getChecker(lastPosition);
        List<Coordinates> beatable = boardManager.canBeat(checker);

        return !beatable.isEmpty();
    }

    private boolean coordsInAvailableCells(Coordinates tapCoordinates) {
        if (tapCoordinates == null) {
            return false;
        }

        for (Coordinates coordinates : availableCoordinates) {
            if (tapCoordinates.equals(coordinates)) {
                return true;
            }
        }

        return false;
    }

    private void trySelect(Coordinates coordinates) {
        if (coordinates == null) {
            return;
        }

        unselectAll();
        Cell cell = board.getCell(coordinates);
        Checker checker = boardManager.getChecker(coordinates);

        if (cell != null && checker != null && cell.getColor() == Cell.Color.BLACK && checker.getColor().equals(gameState)) {
            cell.setState(Cell.State.ACTIVE);

            selectedChecker = checker;

            activateAvailable(checker);
        }
    }


    private void trySelectBeatable(Coordinates coordinates) {
        if (coordinates == null) {
            return;
        }

        Cell cell = board.getCell(coordinates);
        Checker checker = boardManager.getChecker(coordinates);

        if (cell != null && checker != null && cell.getColor() == Cell.Color.BLACK && checker.getColor().equals(gameState)) {
            cell.setState(Cell.State.ACTIVE);
            selectedChecker = checker;

            activateAvailableBeatable(checker);
        }
    }


    public void reverseState() {
        gameState = otherColor(gameState);
    }

    public static Checker.Color otherColor(Checker.Color color) {
        return color == Checker.Color.BLACK ? Checker.Color.WHITE : Checker.Color.BLACK;
    }

    private void unselectAll() {
        selectedChecker = null;
        availableCoordinates.clear();
        board.unselectAll();
    }

    private void activateAvailable(Checker checker) {
        if (activationPreparation(checker)) {
            return;
        }
        
        availableCoordinates = boardManager.buildAvailable(checker);
        setActiveToAvailable();
    }

    private void activateAvailableBeatable(Checker checker) {
        if (activationPreparation(checker)) {
            return;
        }

        availableCoordinates = boardManager.canBeat(checker);
        setActiveToAvailable();
    }

    private boolean activationPreparation(Checker checker) {
        if (checker == null) {
            return true;
        }

        availableCoordinates.clear();
        board.unselectAll();
        
        return false;
    }

    private void setActiveToAvailable() {
        for (Coordinates coordinates : availableCoordinates) {
            board.getCell(coordinates).setState(Cell.State.ACTIVE);
        }
    }

    private void updateQueens() {
        boardManager.updateQueens();
    }

    public String getStatus() {
        if (boardManager.countByColor(Checker.Color.BLACK) == 0) {
            return "White Win!";
        }

        if (boardManager.countByColor(Checker.Color.WHITE) == 0) {
            return "Black Win!";
        }

        if (boardManager.isDraw(gameState)) {
            return "Draw";
        }

        return gameState == Checker.Color.WHITE ? "White trun" : "Black turn";
    }
}
