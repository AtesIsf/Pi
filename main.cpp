#include <raylib.h>
#include <random>
#include <tuple>

typedef struct Global
{
	unsigned int n_points = 1000000;
	unsigned int n_circle_points = 0;
	double approx_pi = 0.0f;
	bool running = false;
	std::tuple<int, int> points[100];

	std::random_device d;
} global_t;

void Update(global_t *g)
{
	static unsigned int counter = 0;

	if (!g->running && IsKeyPressed(KEY_ENTER))
		g->running = true;

	if (!g->running)
		return;
	

	if (counter != 0)
		g->approx_pi = 4/((100.0f * (double)counter)/g->n_circle_points);

	if (!g->running)
		return;

	std::mt19937 rng(g->d());
	std::uniform_int_distribution<int> uni(0, 1000);

	for (int i = 0; i < 100; i++)
	{
		int x = uni(rng) + 460;
		int y = uni(rng) + 140;

		if (CheckCollisionCircles(Vector2 {960, 640}, 500.0f, Vector2 {(float) x, (float) y}, 1.0f))
			g->n_circle_points++;

		g->points[i] = std::tuple<int, int>(x, y);
	}

	counter++;

	if (counter == g->n_points/100)
	{
		counter = 0;
		g->running = false;
	}
}

void Draw(global_t *g)
{
	static bool first_frame = true;
	BeginDrawing();
	
	if (first_frame)
	{
		ClearBackground(BLACK);
		first_frame = false;
	}

	// The illusion of ClearBackground
	DrawRectangleRec(Rectangle {1600, 0, 320, 1280}, BLACK);

	// Draw Coordinate System
	DrawRectangleLinesEx(Rectangle {460, 140, 1000, 1000}, 1.0f, WHITE);
	DrawCircleLinesV(Vector2 {960, 640}, 500.0f, WHITE);

	DrawLineEx(Vector2 {360, 640}, Vector2 {1560, 640}, 1.1f, RAYWHITE);
	DrawLineEx(Vector2 {960, 40}, Vector2 {960, 1240}, 1.1f, RAYWHITE);

	DrawTriangle(Vector2 {360, 660}, Vector2 {360, 620}, Vector2 {340, 640}, RAYWHITE);
	DrawTriangle(Vector2 {1580, 640}, Vector2 {1560, 620}, Vector2 {1560, 660}, RAYWHITE);
	DrawTriangle(Vector2 {960, 20}, Vector2 {940, 40}, Vector2 {980, 40}, RAYWHITE);
	DrawTriangle(Vector2 {960, 1260}, Vector2 {980, 1240}, Vector2 {940, 1240}, RAYWHITE);

	// Draw Text
	DrawText(TextFormat("Number of Data Points: %u", g->n_points), 40, 40, 50, RAYWHITE);
	DrawText(TextFormat("Approximated PI: %lf", g->approx_pi), 1220, 40, 50, RAYWHITE);

	// Draw FPS
	DrawFPS(1820, 1240);

	// Draw Points
	for (auto p : g->points)
		DrawCircleV(Vector2 {(float) std::get<0>(p), (float) std::get<1>(p)}, 1.0f, LIME);

	EndDrawing();
}

global_t * Init()
{
	InitWindow(1920, 1280, "Pi Approximation");
	SetTargetFPS(60);

	global_t *g = new global_t;

	return g;
}

void Deinit(global_t *g)
{
	CloseWindow();

	delete g;
}

int main()
{
	global_t *g = Init();

	while (!WindowShouldClose()) 
	{
		Update(g);
		Draw(g);
	}

	Deinit(g);
	return 0;
}

