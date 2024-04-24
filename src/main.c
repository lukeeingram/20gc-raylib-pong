/*
 *	Written by Luke Ingram in C99 to learn game development.
 *	Created using raylib 5.0 in Clion.
 */

#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

#include "../cmake-build-debug/_deps/raylib-src/src/raylib.h"
#include "../cmake-build-debug/_deps/raylib-src/src/raymath.h"

void ResetBallPosition(Rectangle *ball, float initialBallX, float initialBallY);
void ResetGameState(int playerScore, int enemyScore);

typedef enum GameState {
	GAME_ACTIVE,
	GAME_OVER
} GameState;

int main(void) {
	// ----------------------------------------------------------
	// Screen size constants
	const int windowWidth = 800;
	const int windowHeight = 400;

	// Create window, set target FPS
	InitWindow(windowWidth, windowHeight, "Pong");
	SetTargetFPS(60);

	// Use default system time to create a random seed
	// Random values below will not work otherwise
	SetRandomSeed(time(NULL));

	// Rectangles
	Rectangle player = {20.0f, windowHeight / 2, 20.0f, 80.0f};
	Rectangle enemy = { windowWidth - 50.0f, windowHeight / 2, 20.0f, 80.0f};
	Rectangle ball = {windowWidth / 2, windowHeight / 2, 10.0f, 10.0f};
	Rectangle playerScoreArea = {-20.0f, 0.0f, 30.0f, windowHeight};
	Rectangle enemyScoreArea = {windowWidth - 10.0f, 0.0f, 30.0f, windowHeight};

	// Record initial positions of the paddles/ball for reset
	Vector2 ballOGPos = {ball.x, ball.y};

	// Give the ball a random direction for game start
	Vector2 randomStartDirection = {GetRandomValue(-1.0f, 1.0f), GetRandomValue(-1.0f, 1.0f)};
	Vector2 ballDirection = {};
	if (randomStartDirection.x <= 0) {
		ballDirection.x = -1.0f;
	} else {
		ballDirection.x = 1.0f;
	}

	if (randomStartDirection.y <= 0)
	{
		ballDirection.y = -1.0f;
	} else
	{
		ballDirection.y = 1.0f;
	}

	// Speed
	float paddleSpeed = 150.0f;
	float ballSpeed = 100.0f;
	float initialBallSpeed = ballSpeed;
	float ballSpeedMultiplier = 2.5f;

	// Scoring
	int playerScore = 0;
	int enemyScore = 0;

	// Set GameState
	GameState currentGameState = GAME_ACTIVE;

	// ----------------------------------------------------------
	// Main game loop
	while(!WindowShouldClose())
	{
		// ----------------------------------------------------------
		// Update


		if (GAME_ACTIVE == currentGameState)
		{
			// Delta time
			float dt = GetFrameTime();

			// Move player
			if (IsKeyDown(KEY_DOWN)) player.y += (paddleSpeed * dt);
			if (IsKeyDown(KEY_UP)) player.y -= (paddleSpeed * dt);

			// Move the enemy paddle
			if (ball.x >= windowWidth / 2)
			{
				if (ball.y > enemy.y) {
					enemy.y += (paddleSpeed * dt);
				}
				if (ball.y < enemy.y) {
					enemy.y -= (paddleSpeed * dt);
				}
			}

			// Keep the paddles in the screen bounds
			player.y = Clamp(player.y, 0, windowHeight - player.height);
			enemy.y = Clamp(enemy.y, 0, windowHeight - enemy.height);

			// Move the ball
			ball.x += ballSpeed * ballDirection.x * dt;
			ball.y += ballSpeed * ballDirection.y * dt;

			// Keep the ball in the screen bounds
			ball.y = Clamp(ball.y, 0, windowHeight - ball.height);
			if (ball.y == 0 || ball.y == windowHeight - ball.height) ballDirection.y *= -1;

			// Clamp the ball's speed so that it doesn't go bananas
			ballSpeed = Clamp(ballSpeed, -2500.0f, 2500.0f);

			// Collision
			if (CheckCollisionRecs(ball, player)) ballSpeed *= -ballSpeedMultiplier;
			if (CheckCollisionRecs(ball, enemy)) ballSpeed *= -ballSpeedMultiplier;

			// Collision with scoring
			if (CheckCollisionRecs(ball, playerScoreArea))
			{
				enemyScore += 1;
				ResetBallPosition(&ball, ballOGPos.x, ballOGPos.y);
				ballSpeed = initialBallSpeed;
			}

			if (CheckCollisionRecs(ball, enemyScoreArea))
			{
				playerScore += 1;
				ResetBallPosition(&ball, ballOGPos.x, ballOGPos.y);
				ballSpeed = initialBallSpeed;
			}

			if (playerScore >= 1 || enemyScore >= 1)
			{
				currentGameState = GAME_OVER;
			}


		}
		// Reset the game if state is game_over
		if (currentGameState == GAME_OVER) {
			printf("Current game state: %d", currentGameState);
			if (IsKeyPressed(KEY_R)) {
				printf("Reset key pressed!");
				ResetBallPosition(&ball, ballOGPos.x, ballOGPos.y);
				ResetGameState(playerScore, enemyScore);
				currentGameState = GAME_ACTIVE;
			}
		}

		// ----------------------------------------------------------
		// Draw loop
		BeginDrawing();
		ClearBackground(RAYWHITE);
		if (currentGameState == GAME_ACTIVE) {
			DrawRectangleRec(player, MAROON);
			DrawRectangleRec(enemy, BLUE);
			DrawRectangleRec(ball, RED);
			DrawRectangleRec(playerScoreArea, LIGHTGRAY);
			DrawRectangleRec(enemyScoreArea, LIGHTGRAY);
			DrawText(TextFormat("Player: %i", playerScore), 100, 10, 20, BLACK);
			DrawText(TextFormat("Enemy: %i", enemyScore), 600, 10, 20, BLACK);
		} else if (GAME_OVER == currentGameState) {
			DrawText("Game Over", windowWidth / 2 - MeasureText("Game Over", 20) / 2, windowHeight / 2 - 10, 20, BLACK);
			DrawText("Press 'R' to Restart", windowWidth /2 - MeasureText("Press 'R' to Restart", 20) / 2, windowHeight / 2 + 20, 20, BLACK);
		}
		EndDrawing();


	}
	// ----------------------------------------------------------
	// De-initialization
	CloseWindow();
	return 0;
}

void ResetBallPosition(Rectangle *ball, float initialBallX, float initialBallY)
{
	ball->x = initialBallX;
	ball->y = initialBallY;
}

void ResetGameState(int playerScore, int enemyScore) {
	playerScore = 0;
	enemyScore = 0;
}