/*
 *	Written by Luke Ingram in C99 to learn game development.
 *	Created using raylib 5.0 in Clion.
 */

#include <stdio.h>
#include <sys/stat.h>

#include "../cmake-build-debug/_deps/raylib-src/src/raylib.h"
#include "../cmake-build-debug/_deps/raylib-src/src/raymath.h"

Vector2 getRandomStartDirection() {
	Vector2 startDir = {GetRandomValue(-1, 1), GetRandomValue(-1, 1)};
	return startDir;
}

int main(void) {
	// ----------------------------------------------------------
	// Screen size constants
	const int windowWidth = 800;
	const int windowHeight = 400;

	// Rectangles
	Rectangle player = {20.0f, windowHeight / 2, 25.0f, 100.0f};
	Rectangle enemy = { windowWidth - 50.0f, windowHeight / 2, 25.0f, 100.0f};
	Rectangle ball = {windowWidth / 2, windowHeight / 2, 20.0f, 20.0f};
	Rectangle playerScoreArea = {-20.0f, 0.0f, 30.0f, windowHeight};
	Rectangle enemyScoreArea = {windowWidth - 10.0f, 0.0f, 30.0f, windowHeight};

	// Record initial positions of the paddles/ball for reset
	Vector2 playerOGPos = {player.x, player.y};
	Vector2 enemyOGPos = {enemy.x, enemy.y};
	Vector2 ballOGPos = {ball.x, ball.y};

	// Give the ball a random direction for game start
	Vector2 ballDirection = {1.0f, 1.0f};
	ballDirection = getRandomStartDirection();

	// Speed
	float paddleSpeed = 150.0f;
	float ballSpeed = 100.0f;
	float ballSpeedMulitiplier = 25.0f;

	// Scoring
	int playerScore = 0;
	int enemyScore = 0;

	// Create window, set target FPS
	InitWindow(windowWidth, windowHeight, "Pong");
	SetTargetFPS(60);

	// ----------------------------------------------------------
	// Main game loop
	while(!WindowShouldClose()) {
		// ----------------------------------------------------------
		// Update

		// Delta time
		float dt = GetFrameTime();

		// Move player
		if (IsKeyDown(KEY_DOWN)) player.y += (paddleSpeed * dt);
		if (IsKeyDown(KEY_UP)) player.y -= (paddleSpeed * dt);

		// Keep the paddles in the screen bounds
		player.y = Clamp(player.y, 0, windowHeight - player.height);
		enemy.y = Clamp(enemy.y, 0, windowHeight - enemy.height);

		// Move the ball
		ball.x += ballSpeed * ballDirection.x * dt;
		ball.y += ballSpeed * ballDirection.y * dt;

		// printf("X: %1.2f\n", ball.x);
		// printf("Y: %1.2f\n", ball.y);

		// Keep the ball in the screen bounds
		ball.y = Clamp(ball.y, 0, windowHeight - ball.height);
		if (ball.y == 0 || ball.y == windowHeight - ball.height) ballDirection.y *= -1;

		// ----------------------------------------------------------
		// Draw loop
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawRectangleRec(player, MAROON);
			DrawRectangleRec(enemy, BLUE);
			DrawRectangleRec(ball, RED);
			DrawRectangleRec(playerScoreArea, LIGHTGRAY);
			DrawRectangleRec(enemyScoreArea, LIGHTGRAY);
		EndDrawing();
	}
	// ----------------------------------------------------------
	// De-initialization
	CloseWindow();
	return 0;
}
