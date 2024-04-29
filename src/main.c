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

typedef enum GameScreen
{
	TITLE = 0,
	GAME_AI,
	GAME_2P,
	GAMEPAUSED,
	GAMEOVER
} GameScreen;

int main(void) {
	// ----------------------------------------------------------
	// Screen size constants
	const int screenWidth = 800;
	const int screenHeight = 400;

	// Create window, set target FPS
	InitWindow(screenWidth, screenHeight, "Pong");
	SetTargetFPS(60);

	// Set game screen
	GameScreen currentScreen = TITLE;

	// Use default system time to create a random seed
	// Random values below will not work otherwise
	SetRandomSeed(time(NULL));

	// Rectangles
	Rectangle player = {20.0f, screenHeight / 2, 20.0f, 80.0f};
	Rectangle enemy = { screenWidth - 50.0f, screenHeight / 2, 20.0f, 80.0f};
	Rectangle ball = {screenWidth / 2, screenHeight / 2, 10.0f, 10.0f};
	Rectangle playerScoreArea = {-20.0f, 0.0f, 30.0f, screenWidth};
	Rectangle enemyScoreArea = {screenWidth - 10.0f, 0.0f, 30.0f, screenHeight};

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
	if (randomStartDirection.y <= 0) {
		ballDirection.y = -1.0f;
	} else {
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

	// ----------------------------------------------------------
	// Main game loop
	while(!WindowShouldClose())
	{
		// ----------------------------------------------------------
		// Update
		switch(currentScreen) {
			case TITLE: {
				if (IsKeyDown(KEY_ONE) || IsKeyDown(KEY_KP_1)) {
					currentScreen = GAME_AI;
				}

				if (IsKeyDown(KEY_TWO) || IsKeyDown(KEY_KP_2)) {
					currentScreen = GAME_2P;
				}

				if (IsKeyDown(KEY_Q) || IsKeyDown(KEY_THREE) || IsKeyDown(KEY_KP_3)) {
					CloseWindow();
				}
			} break;

			case GAME_AI: {
				// Delta time
				float dt = GetFrameTime();

				// Move player
				if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) player.y += (paddleSpeed * dt);
				if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) player.y -= (paddleSpeed * dt);

				// Move the enemy paddle
				if (ball.x >= screenWidth / 2)
				{
					if (ball.y > enemy.y) {
						enemy.y += (paddleSpeed * dt);
					}
					if (ball.y < enemy.y) {
						enemy.y -= (paddleSpeed * dt);
					}
				}

				// Keep the paddles in the screen bounds
				player.y = Clamp(player.y, 0, screenHeight - player.height);
				enemy.y = Clamp(enemy.y, 0, screenHeight - enemy.height);

				// Move the ball
				ball.x += ballSpeed * ballDirection.x * dt;
				ball.y += ballSpeed * ballDirection.y * dt;

				// Keep the ball in the screen bounds
				ball.y = Clamp(ball.y, 0, screenHeight - ball.height);
				if (ball.y == 0 || ball.y == screenHeight - ball.height) ballDirection.y *= -1;

				// Clamp the ball's speed so that it doesn't go bananas
				ballSpeed = Clamp(ballSpeed, -2500.0f, 2500.0f);

				// Collision
				if (CheckCollisionRecs(ball, player)) ballSpeed *= -ballSpeedMultiplier;
				if (CheckCollisionRecs(ball, enemy)) ballSpeed *= -ballSpeedMultiplier;

				// Collision with scoring
				if (CheckCollisionRecs(ball, playerScoreArea)) {
					enemyScore += 1;
					ResetBallPosition(&ball, ballOGPos.x, ballOGPos.y);
					ballSpeed = initialBallSpeed;
				}

				if (CheckCollisionRecs(ball, enemyScoreArea)) {
					playerScore += 1;
					ResetBallPosition(&ball, ballOGPos.x, ballOGPos.y);
					ballSpeed = initialBallSpeed;
				}

				if (playerScore >= 5 || enemyScore >= 5) currentScreen = GAMEOVER;

			} break;

			case GAME_2P: {
				// Delta time
				float dt = GetFrameTime();

				// Move player
				if (IsKeyDown(KEY_W)) player.y -= (paddleSpeed * dt);
				if (IsKeyDown(KEY_S)) player.y += (paddleSpeed * dt);

				// Move 2P
				if (IsKeyDown(KEY_DOWN)) enemy.y += (paddleSpeed * dt);
				if (IsKeyDown(KEY_UP)) enemy.y -= (paddleSpeed * dt);

				// Keep the paddles in the screen bounds
				player.y = Clamp(player.y, 0, screenHeight - player.height);
				enemy.y = Clamp(enemy.y, 0, screenHeight - enemy.height);

				// Move the ball
				ball.x += ballSpeed * ballDirection.x * dt;
				ball.y += ballSpeed * ballDirection.y * dt;

				// Keep the ball in the screen bounds
				ball.y = Clamp(ball.y, 0, screenHeight - ball.height);
				if (ball.y == 0 || ball.y == screenHeight - ball.height) ballDirection.y *= -1;

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
				if (playerScore >= 5 || enemyScore >= 5) currentScreen = GAMEOVER;

			} break;

			case GAMEOVER: {
					if (IsKeyDown(KEY_M)) {
						playerScore = 0;
						enemyScore = 0;
						currentScreen = TITLE;
					}
			}

			default: break;
		}

		// ----------------------------------------------------------
		// Draw loop
		BeginDrawing();
		ClearBackground(RAYWHITE);

		switch (currentScreen) {
			case TITLE: {
				DrawText("1. Game vs A.I.", screenWidth / 2 - MeasureText("Game vs AI", 20), screenHeight / 2, 20, BLACK);
				DrawText("2. Game vs 2P", screenWidth / 2 - MeasureText("Game vs 2P", 20) ,screenHeight / 2 + 40, 20, BLACK);
				DrawText("3. Quit Game", screenWidth / 2 - MeasureText("3. Quit Game", 20) + 6, screenHeight / 2 + 80, 20, BLACK);
			} break;

			case GAME_AI: {
				DrawRectangleRec(player, MAROON);
				DrawRectangleRec(enemy, BLUE);
				DrawRectangleRec(ball, RED);
				DrawRectangleRec(playerScoreArea, LIGHTGRAY);
				DrawRectangleRec(enemyScoreArea, LIGHTGRAY);
				DrawText(TextFormat("Player: %i", playerScore), 100, 10, 20, BLACK);
				DrawText(TextFormat("Enemy: %i", enemyScore), 600, 10, 20, BLACK);
			} break;

			case GAME_2P: {
				DrawRectangleRec(player, MAROON);
				DrawRectangleRec(enemy, BLUE);
				DrawRectangleRec(ball, RED);
				DrawRectangleRec(playerScoreArea, LIGHTGRAY);
				DrawRectangleRec(enemyScoreArea, LIGHTGRAY);
				DrawText(TextFormat("Player: %i", playerScore), 100, 10, 20, BLACK);
				DrawText(TextFormat("Enemy: %i", enemyScore), 600, 10, 20, BLACK);
			} break;

			case GAMEPAUSED: {

			} break;

			case GAMEOVER: {
				DrawText("Game Over", screenWidth / 2 - MeasureText("Game Over", 20) / 2, screenHeight / 2 - 10, 20, BLACK);
				DrawText("Press 'M' to go to the Main Menu", screenWidth /2 - MeasureText("Press 'M' to go to the Main Menu", 20) / 2, screenHeight / 2 + 20, 20, BLACK);
			} break;

			default: break;
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