#include <unity.h>

#include <cmath>

#include "config.h"
#include "input.h"
#include "map.h"
#include "movement.h"
#include "player.h"
#include "raycaster.h"
#include "renderer.h"

class FakeDisplay final : public Display {
 public:
    int clearCount = 0;
    int drawCount = 0;
    int presentCount = 0;
    std::uint8_t firstColumn = 255;
    std::uint8_t lastColumn = 255;

    /** Satisfy the display initialization contract. */
    bool begin() override { return true; }

    /** Record one frame-buffer clear. */
    void clear() override { ++clearCount; }

    /** Record one rendered wall column. */
    void drawVerticalLine(std::uint8_t x, std::uint8_t y,
                          std::uint8_t height) override {
        (void)y;
        (void)height;
        if (drawCount == 0) firstColumn = x;
        lastColumn = x;
        ++drawCount;
    }

    /** Record one completed frame presentation. */
    void present() override { ++presentCount; }
};

/** Provide Unity's per-test setup hook. */
void setUp() {}

/** Provide Unity's per-test cleanup hook. */
void tearDown() {}

/** Create the milestone player at its initial position and orientation. */
static Player initialPlayer() {
    return {
        {1.5f, 1.5f},
        {1.0f, 0.0f},
        {0.0f, config::CAMERA_PLANE_LENGTH},
    };
}

/** Verify bounds and fixed wall cells are always solid. */
static void testMapWalls() {
    const Map map;
    TEST_ASSERT_TRUE(map.isWall(-1, 1));
    TEST_ASSERT_TRUE(map.isWall(8, 1));
    TEST_ASSERT_TRUE(map.isWall(2, 2));
    TEST_ASSERT_FALSE(map.isWall(1, 1));
}

/** Verify an axis-aligned east ray reaches the outer wall. */
static void testAxisAlignedRay() {
    const Map map;
    Player player = initialPlayer();
    player.cameraPlane = {0.0f, 0.0f};
    const RayHit hit = castRay(map, player, 64);

    TEST_ASSERT_TRUE(hit.hit);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(HitSide::X),
                          static_cast<int>(hit.side));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 5.5f, hit.perpendicularDistance);
}

/** Verify an axis-aligned west ray reaches the nearby outer wall. */
static void testNegativeAxisRay() {
    const Map map;
    Player player = initialPlayer();
    player.direction = {-1.0f, 0.0f};
    player.cameraPlane = {0.0f, 0.0f};
    const RayHit hit = castRay(map, player, 64);

    TEST_ASSERT_TRUE(hit.hit);
    TEST_ASSERT_EQUAL_INT(static_cast<int>(HitSide::X),
                          static_cast<int>(hit.side));
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.5f, hit.perpendicularDistance);
}

/** Verify every OLED column produces a finite positive wall hit. */
static void testAllScreenRaysHit() {
    const Map map;
    const Player player = initialPlayer();

    for (std::uint8_t column = 0; column < config::SCREEN_WIDTH; ++column) {
        const RayHit hit = castRay(map, player, column);
        TEST_ASSERT_TRUE(hit.hit);
        TEST_ASSERT_TRUE(std::isfinite(hit.perpendicularDistance));
        TEST_ASSERT_TRUE(hit.perpendicularDistance > 0.0f);
    }
}

/** Verify rotation preserves camera vector lengths and orthogonality. */
static void testRotationPreservesCamera() {
    const Map map;
    Player player = initialPlayer();
    const InputState input = {false, false, true};

    for (int frame = 0; frame < 100; ++frame)
        updatePlayer(player, map, input, 0.01f);
    const float directionLength =
        std::sqrt(player.direction.x * player.direction.x +
                  player.direction.y * player.direction.y);
    const float planeLength =
        std::sqrt(player.cameraPlane.x * player.cameraPlane.x +
                  player.cameraPlane.y * player.cameraPlane.y);
    const float dot = player.direction.x * player.cameraPlane.x +
                      player.direction.y * player.cameraPlane.y;

    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, directionLength);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, config::CAMERA_PLANE_LENGTH, planeLength);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, dot);
}

/** Verify opposite rotation inputs cancel each other. */
static void testOppositeRotationsCancel() {
    const Map map;
    Player player = initialPlayer();
    const InputState input = {false, true, true};

    updatePlayer(player, map, input, config::MAX_FRAME_SECONDS);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.0f, player.direction.x);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 0.0f, player.direction.y);
}

/** Verify forward movement uses the configured maximum frame delta. */
static void testMovementClampsLargeDelta() {
    const Map map;
    Player player = initialPlayer();
    const InputState input = {true, false, false};

    updatePlayer(player, map, input, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.6f, player.position.x);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.5f, player.position.y);
}

/** Verify the collision radius stops movement into the outer wall. */
static void testCollisionStopsAtWall() {
    const Map map;
    Player player = initialPlayer();
    player.position = {1.25f, 1.5f};
    player.direction = {-1.0f, 0.0f};
    const InputState input = {true, false, false};

    updatePlayer(player, map, input, config::MAX_FRAME_SECONDS);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.25f, player.position.x);
}

/** Verify axis separation permits movement along a blocked wall. */
static void testCollisionSlidesAlongWall() {
    const Map map;
    Player player = initialPlayer();
    player.position = {1.75f, 2.5f};
    player.direction = {0.70710678f, 0.70710678f};
    const InputState input = {true, false, false};

    updatePlayer(player, map, input, config::MAX_FRAME_SECONDS);
    TEST_ASSERT_FLOAT_WITHIN(0.0001f, 1.75f, player.position.x);
    TEST_ASSERT_TRUE(player.position.y > 2.5f);
}

/** Verify wall projection centres and clips near and far slices. */
static void testWallSliceProjection() {
    WallSlice slice = calculateWallSlice(2.0f);
    TEST_ASSERT_EQUAL_UINT8(16, slice.startY);
    TEST_ASSERT_EQUAL_UINT8(32, slice.height);

    slice = calculateWallSlice(0.1f);
    TEST_ASSERT_EQUAL_UINT8(0, slice.startY);
    TEST_ASSERT_EQUAL_UINT8(64, slice.height);

    slice = calculateWallSlice(100.0f);
    TEST_ASSERT_EQUAL_UINT8(31, slice.startY);
    TEST_ASSERT_EQUAL_UINT8(1, slice.height);
}

/** Verify one complete frame renders all 128 wall columns. */
static void testRendererCompletesFrame() {
    const Map map;
    const Player player = initialPlayer();
    FakeDisplay display;

    renderFrame(map, player, display);
    TEST_ASSERT_EQUAL_INT(1, display.clearCount);
    TEST_ASSERT_EQUAL_INT(config::SCREEN_WIDTH, display.drawCount);
    TEST_ASSERT_EQUAL_UINT8(0, display.firstColumn);
    TEST_ASSERT_EQUAL_UINT8(config::SCREEN_WIDTH - 1, display.lastColumn);
    TEST_ASSERT_EQUAL_INT(1, display.presentCount);
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;
    UNITY_BEGIN();
    RUN_TEST(testMapWalls);
    RUN_TEST(testAxisAlignedRay);
    RUN_TEST(testNegativeAxisRay);
    RUN_TEST(testAllScreenRaysHit);
    RUN_TEST(testRotationPreservesCamera);
    RUN_TEST(testOppositeRotationsCancel);
    RUN_TEST(testMovementClampsLargeDelta);
    RUN_TEST(testCollisionStopsAtWall);
    RUN_TEST(testCollisionSlidesAlongWall);
    RUN_TEST(testWallSliceProjection);
    RUN_TEST(testRendererCompletesFrame);
    return UNITY_END();
}
